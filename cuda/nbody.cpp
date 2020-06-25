//
// Created by LCX on 2020/5/24.
// 并行版本——成功
// 32 32 351

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "timer.h"
#include "check.h"

const float SOFTENING = 1e-9;
const int BLOCK_SIZE = 32;
const int TILE_SIZE = 32;

typedef struct {float x, y, z, vx, vy, vz;} Body;
typedef struct {float x,y,z;} Accelerate;

void randomizeBodies(float *data, int n)
{
    for (int i = 0; i < n; i++)
    {
        data[i] = 2.0f * (rand() / (float)RAND_MAX) - 1.0f;
    }
}

__device__ void accCal(Body major, Body minor, Accelerate *acc){
    float dx = minor.x - major.x;
    float dy = minor.y - major.y;
    float dz = minor.z - major.z;
    float distSqr = dx*dx + dy*dy + dz*dz + SOFTENING;
    float invDist = rsqrtf(distSqr);
    float invDist3 = invDist * invDist * invDist;

    acc->x += dx * invDist3; acc->y += dy * invDist3; acc->z += dz * invDist3;
}

__global__ void bodyForce(Body *globalPos, float dt, int n){
    int majorId = threadIdx.x + blockIdx.y * blockDim.x;
    float tmp[3] = {0.0f, 0.0f, 0.0f};
    Accelerate *acc = (Accelerate *)tmp;
    Body major = globalPos[majorId];
    __shared__ Body sharedPos[BLOCK_SIZE];

    for (int i = blockIdx.x; i < n / BLOCK_SIZE; i += TILE_SIZE){
        sharedPos[threadIdx.x] = globalPos[i * BLOCK_SIZE + threadIdx.x];
        __syncthreads();
#pragma unroll
        for (int j = 0; j < BLOCK_SIZE; j++){
            accCal(major, sharedPos[j], acc);
        }
        __syncthreads();
    }

    atomicAdd(&globalPos[majorId].vx, dt * acc->x);
    atomicAdd(&globalPos[majorId].vy, dt * acc->y);
    atomicAdd(&globalPos[majorId].vz, dt * acc->z);
}


__global__ void update(Body *globalPos, float dt, int n){
    int bodyId = threadIdx.x + blockIdx.x * blockDim.x;
    globalPos[bodyId].x += globalPos[bodyId].vx * dt;
    globalPos[bodyId].y += globalPos[bodyId].vy * dt;
    globalPos[bodyId].z += globalPos[bodyId].vz * dt;
}

int main(const int argc, const char **argv)
{
    int nBodies = 2 << 11;
    int salt = 0;
    if (argc > 1)
        nBodies = 2 << atoi(argv[1]);
    if (argc > 2)
        salt = atoi(argv[2]);

    const float dt = 0.01f;
    const int nIters = 10;
    int bytes = nBodies * sizeof(Body);
    double totalTime = 0.0;
    dim3 threadSize2d(BLOCK_SIZE,1, 1 );
    dim3 blockSize2d(TILE_SIZE, nBodies/TILE_SIZE, 1);

    float *buf;
    cudaHostAlloc((void**)&buf, bytes, cudaHostAllocDefault);
    randomizeBodies(buf, 6 * nBodies);

    float *bufDev;
    cudaMalloc(&bufDev, bytes);
    Body *pDev = (Body *)bufDev;

    cudaMemcpy(bufDev, buf, bytes, cudaMemcpyHostToDevice);

    /*******************************************************************/
    // Do not modify these 2 lines of code.
    for (int iter = 0; iter < nIters; iter++) {
        StartTimer();
        /*******************************************************************/

        /*
         * You will likely wish to refactor the work being done in `bodyForce`,
         * as well as the work to integrate the positions.
         */
        bodyForce<<<blockSize2d, threadSize2d>>>(pDev, dt, nBodies);
        update<<<nBodies/BLOCK_SIZE, BLOCK_SIZE>>>(pDev, dt, nBodies);

        if(iter == nIters - 1){
            cudaMemcpy(buf, bufDev, bytes, cudaMemcpyDeviceToHost);
        }

        /*
         * This position integration cannot occur until this round of `bodyForce` has completed.
         * Also, the next round of `bodyForce` cannot begin until the integration is complete.
         */


        /*******************************************************************/
        // Do not modify the code in this section.
        const double tElapsed = GetTimer() / 1000.0;
        totalTime += tElapsed;
    }

    double avgTime = totalTime / (double)(nIters);
    float billionsOfOpsPerSecond = 1e-9 * nBodies * nBodies / avgTime;

#ifdef ASSESS
    checkPerformance(buf, billionsOfOpsPerSecond, salt);
#else
    checkAccuracy(buf, nBodies);
    printf("%d Bodies: average %0.3f Billion Interactions / second\n", nBodies, billionsOfOpsPerSecond);
    salt += 1;
#endif
    /*******************************************************************/

    /*
     * Feel free to modify code below.
     */

    cudaFreeHost(buf);
    cudaFree(bufDev);
}

#include "mpi.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#define lowValue(ms,id) (2*((2*ms*id - 2*ms +id)/2) +2*ms + 3)
#define highValue(ms,id) (2*(((2*ms + 1)*id +1)/2) + 2*ms + 1)
#define valid(n) ((n + 1)/2 -1)
#define minSize(v,p) (v/p)
#define size(lv,hv) ((hv - lv)/2 + 1)

int main (int argc, char *argv[])
{
    long long cnt = 0;
    double elapsedTime;
    long long first;
    long long globalCnt = 0;
    long long highValue;
    long long lowValue;
    long long i,j,k;
    int id;
    char *marked;
    long long n;
    int p;
    long long p0Size;
    long long prime;
    long long size;
    long long sizeMin;
    long long numValid;

    /* Start the timer */
    MPI_Init (&argc, &argv);
    MPI_Comm_rank (MPI_COMM_WORLD, &id);
    MPI_Comm_size (MPI_COMM_WORLD, &p);
    MPI_Barrier(MPI_COMM_WORLD);
    elapsedTime = -MPI_Wtime();

    if (argc != 2) {
        if (!id) printf ("Command line: %s <m>\n", argv[0]);
        MPI_Finalize();
        exit (1);
    }
    n = atoll(argv[1]);
    long long total = 33554432;
//    long long total = atoll(argv[2]);
    long long base = 4;

    /* Figure out this process's share of the array, as
       well as the integers represented by the first and
       last array elements */
    numValid = valid(n);
    sizeMin = minSize(numValid, (long long)p);
    lowValue = lowValue(sizeMin, (long long)id);
    highValue = highValue(sizeMin, (long long)id);
    if(id == p - 1){
        if(n % 2 == 0)  highValue = n-1;
        else highValue = n;
    }
    if(highValue > n){
        if(n % 2 == 0)  highValue = n-1;
        else highValue = n;
    }
    size = size(lowValue, highValue);

    /* Bail out if all the primes used for sieving are
       not all held by process 0 */
    p0Size = (n-1)/(long long)p;
    long long cpN =sqrt((long double)n);
    if ((2 + p0Size) < cpN) {
        if (!id) printf ("Too many processes\n");
        MPI_Finalize();
        exit (1);
    }

    //用于消除广播的数组
    char *copy;
//    int cpN =sqrt((long double)n);
    int cpValidNUm = valid(cpN);
    int cpMinSize = minSize(cpValidNUm, 1);
    int cpLowValue = lowValue(cpMinSize, 0);
    int cpPrime = 3;
    int cpFirst;
    int cpIndex = 0;

    copy = (char *) malloc (cpMinSize);
    if (copy == NULL) {
        printf ("Cannot allocate enough memory\n");
        MPI_Finalize();
        exit (1);
    }
    for(i = 0; i < cpMinSize; i++)  copy[i] = 0;
    do{
        cpFirst = (cpPrime * cpPrime - cpLowValue) / 2;
        for(i =cpFirst; i < cpMinSize; i += cpPrime)
            copy[i] = 1;
        while (copy[++cpIndex]);
        cpPrime = 2*cpIndex + 3;
    }while (cpPrime * cpPrime <= cpN);

    //分块
    int cacheSize = total / p - cpMinSize*base*sizeof(char);
    int cacheSizeChar = cacheSize/(base*sizeof(char));
    long long blockNum = (size + (long long)cacheSizeChar -1) / (long long)cacheSizeChar;
    long long  blockID = 0;
    long long  blockSizeMin = minSize(size, blockNum);
    long long blockLowValue = lowValue((long long)blockSizeMin, (long long)blockID) + lowValue - 3;
    long long blockHighValue = highValue((long long)blockSizeMin, (long long)blockID) + lowValue -3;
    if(blockHighValue > highValue){
        if(highValue % 2 == 0)  blockHighValue = highValue-1;
        else blockHighValue = highValue;
    }
    long long blockSize = size(blockLowValue, blockHighValue);

    //对每个块进行遍历
    for(i = 0; i < blockNum; i++){
        marked = (char *) malloc (blockSize);
        if (marked == NULL) {
            printf ("Cannot allocate enough memory\n");
            printf("%d-%lld\n",id, blockSize);
        }

        for(j = 0; j < blockSize; j++)  marked[j] = 0;

        for(j = 0; j < cpMinSize; j++){
            if(copy[j] == 1) continue;

            prime = 2*j + 3;
            if (prime * prime >= blockLowValue)
                first = (prime * prime - blockLowValue) / 2;
            else {
                if(blockLowValue % prime == 0)  first = 0;
                else{
                    first = ((((blockLowValue / prime) % 2) + 1)* prime - (blockLowValue % prime))/2;
                }
            }

            for(k = first; k < blockSize; k+=prime)
                marked[k] = 1;
        }

        for(j = 0; j < blockSize; j++)
            if(marked[j] == 0){
//                printf("%d\n", blockLowValue + 2*j);
                cnt++;
            }


        blockID++;
        blockLowValue = lowValue((long long)blockSizeMin, (long long)blockID) + lowValue -3;
        blockHighValue = highValue((long long)blockSizeMin, (long long)blockID) + lowValue -3;
        if(blockID == blockNum-1)   blockHighValue = highValue;
        if(blockHighValue > highValue){
            if(highValue % 2 == 0)  blockHighValue = highValue-1;
            else blockHighValue = highValue;
        }
        blockSize = size(blockLowValue, blockHighValue);
    }

    if (p > 1) MPI_Reduce (&cnt, &globalCnt, 1, MPI_LONG_LONG_INT, MPI_SUM,
                           0, MPI_COMM_WORLD);

    /* Stop the timer */
    elapsedTime += MPI_Wtime();

    /* Print the results */
    if (!id) {
        printf ("There are %lld primes less than or equal to %lld\n",globalCnt + 1, n);//把2也算上
        printf ("SIEVE (%d) %10.6f\n", p, elapsedTime);
    }
    MPI_Finalize ();
    return 0;
}

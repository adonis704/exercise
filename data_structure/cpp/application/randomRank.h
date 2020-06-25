#include<iostream>
#include<stdlib.h>
#include<time.h>

using namespace std;
extern int RandInt(int i, int j);

void RandomPermutation1(int n) {
	if (!(n > 0) || n % 1 != 0) {
		cout << "error";
		return;
	}

	int *res = new int[n], *resTemp = new int[n]();
	int count = 0;

	while (count != n) {
		int temp = RandInt(1, n);
		if (!resTemp[temp - 1]) {
			resTemp[temp - 1] = temp;
			res[count++] = temp;
		}
	}

	for (int i = 0; i < n; i++)
		cout << res[i] << ",";
	cout << 0;

}

void RandomPermutation2(int n) {
	if (!(n > 0) || n % 1 != 0) {
		cout << "error";
		return;
	}
	int *res = new int[n], *resTemp = new int[n]();
	int count = 0;

	while (count != n) {
		int temp = RandInt(1, n);
		if (!resTemp[temp - 1]) {
			resTemp[temp - 1] = temp;
			res[count++] = temp;
		}
	}

	for (int i = 0; i < n; i++)
		cout << res[i] << ",";
	cout << 0;


}

void swap(int &a, int &b)
{
	int tmp = a;
	a = b;
	b = tmp;
}

void RandomPermutation3(int n) {
	if (!(n > 0) || n % 1 != 0) {
		cout << "error";
		return;
	}
	int *res = new int[n];
	for (int i = 0; i < n; i++)
		res[i] = i + 1;
	for (int i = 1; i < n; i++)
		swap(res[i], res[RandInt(0, i)]);

	for (int i = 0; i < n; i++)
		cout << res[i] << ",";
	cout << 0;
		
}
int RandInt(int i, int j) {
	if (i == 0)
		return rand() % (j + 1);
	else
		return rand() % (j - i + 1) + i;
}
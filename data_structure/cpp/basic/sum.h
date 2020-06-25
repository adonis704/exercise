int get_sum(int arr[], int len) {
	int sum = 0;
	for (int i = 0; i < len; i++)
		sum += arr[i];
	return sum;
}

double get_sum(double arr[], int len) {
	double sum = 0;
	for (int i = 0; i < len; i++)
		sum += arr[i];
	return sum;
}
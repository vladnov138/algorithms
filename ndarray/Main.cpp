#include <iostream>

#include "NDArray.h"

int main() {
	int size[] = { 3, 2 };
	NDArray<int> ndarr = NDArray<int>(size);
	int k = 0;
	for (int i = 0; i < size[0]; i++) {
		for (int j = 0; j < size[1]; j++) {
			k++;
			int idx[] = { i, j };
			ndarr.set_item(idx, k);
		}
	}
	double* min_axis0 = new double[3];
	min_axis0 = ndarr.mean(1);
	for (int i = 0; i < 3; i++)
		std::cout << min_axis0[i] << " ";
	std::cout << std::endl << ndarr.mean();
}
#include "covexhullwindow.h"

int RNG(int a, int b) {
	int range = b - a;
	int rn = (rand() % range) + a;
	return rn;
};

void GenerateInitialPoints(D2D1_POINT_2F arr[], int size) {
	for (int i = 0; i < size; i++) {
		arr[i] = D2D1::Point2F((float)RNG(LOWRAND, UPPRAND), (float)RNG(LOWRAND, UPPRAND));
	}
	return;
};


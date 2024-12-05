#pragma warning(disable:4996)
#define _USE_MATH_DEFINES // for C
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define FILTER_SIZE 11
#define SIZE 512
#define PATH_INPUT "lena.img"
#define PATH_OUTPUT "output_1.bmp"

int main() {

	FILE* fp_in;
	FILE* fp_out;

	// 2D image -> 1D array
	unsigned char* image = (unsigned char*)malloc(sizeof(unsigned char) * SIZE * SIZE);

	// open bitmap file
	fp_in = fopen(PATH_INPUT, "rb");
	if (fp_in == NULL) {
		printf("fopen failed : %s\n", PATH_INPUT);
		exit(1);
	}

	fp_out = fopen(PATH_OUTPUT, "wb");
	if (fp_out == NULL) {
		printf("fopen failed : %s\n", PATH_OUTPUT);
		fclose(fp_in);
		exit(1);
	}	

	// read image
	fread(image, sizeof(unsigned char), SIZE * SIZE, fp_in);

	// add noise
	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			int index = i * SIZE + j; // 1D array index
			int n = j;

			// noise 
			// cos 함수의 반환형이 double 형, 따라서 unsigned char 형으로 noise 선언하면
			// 소수점 이하 값이 손실됨 -> 결과 bmp 보면 noise 간격이 다른 구간 존재
			double noise = 128 * cos((2 * M_PI * n) / FILTER_SIZE) + 128; 

			int noisy_pixel = image[index] + noise;

			// overflow 방지 (0 ~ 255) 
			if (noisy_pixel > 255) noisy_pixel = 255;
			if (noisy_pixel < 0) noisy_pixel = 0;

			// 변경된 픽셀 값을 저장
			image[index] = (unsigned char)noisy_pixel;
		}
	}

	fwrite(image, sizeof(unsigned char), SIZE * SIZE, fp_out);

	fclose(fp_in);
	fclose(fp_out);

	/*for (int i = 0; i < SIZE; i++) {
		free(image[i]);
	}*/
	free(image);

	return 0;
}
/***********************************************************************
 * (c) Copyright 2018 Microsemi SoC Products Group. All rights reserved.
 * version 1.0 - 21/06/2018 by anton.krug@microsemi.com
 ***********************************************************************/
#include <stdio.h>
#include <float.h>
#include <math.h>

#include "data.h"

#define PI_SIZE 3.14159f / SIZE
#define C(x) ( (x==0)? 1.0f / sqrtf(2.0f) : 1.0f )
//#define VERBOSE 1

#ifndef ITERATIONS
#define ITERATIONS 200
#endif

// Discrete cosine transform
void dct(float (*input)[SIZE], float (*output)[SIZE]) {
  // this could be optimized and storing whole row to separate local variable
  // and then only then multiplying the cosf(PI_SIZE * (j+0.5f) * y) with the
  // result, but this way it loads the a cpu bit more.

  for (int y = 0; y < SIZE; y++) {
    for (int x = 0; x < SIZE; x++) {
      float frequency = 0.0f;

      for (int j = 0; j < SIZE; j++) {
        for (int i = 0; i < SIZE; i++) {
          frequency += input[j][i] * cosf(PI_SIZE * (i + 0.5f) * x) *
                                     cosf(PI_SIZE * (j + 0.5f) * y);
        }
      }
      output[y][x] = (2.0f / SIZE) * C(y) * C(x) * frequency;

    }
  }
}


// Inverse discrete cosine transform
void idct(float (*input)[SIZE], float (*output)[SIZE]) {
  for (int j = 0; j < SIZE; j++) {
    for (int i = 0; i < SIZE; i++) {
      float pixel = 0.0f;

      for (int y = 0; y < SIZE; y++) {
        for (int x = 0; x < SIZE; x++) {
           pixel += C(y) * C(x) * input[y][x] * cosf(PI_SIZE * (i + 0.5f) * x) *
                                                cosf(PI_SIZE * (j + 0.5f) * y);
        }
      }
      output[j][i] = (2.0f / SIZE) * pixel;

    }
  }
}


void displayMatrix(float (*input)[SIZE]) {
  for (int y = 0; y < SIZE; y++) {
    for (int x = 0; x < SIZE; x++) {
      printf("%10f", input[y][x]);
    }
    printf("\n");
  }
  printf("\n");
}


float diffActualExpected(float (*expected)[SIZE], float (*actual)[SIZE]) {
  float difference = 0.0f;
  for (int y = 0; y < SIZE; y++) {
    for (int x = 0; x < SIZE; x++) {
      difference += fabsf(expected[y][x] - actual[y][x]);
    }
  }
  return difference;
}


unsigned int checksumMatrix(float (*input)[SIZE]) {
  unsigned int sum = 0;
  for (int y = 0; y < SIZE; y++) {
    for (int x = 0; x < SIZE; x++) {
      sum += *(unsigned int*)&input[y][x];
    }
  }
  return sum;
}


float test(float (*input)[SIZE], unsigned int* totalChecksum) {
  float frequencySpectrum[SIZE][SIZE];
  float actual[SIZE][SIZE];

  dct(input, frequencySpectrum);   // convert input data to spectrum
  idct(frequencySpectrum, actual); // then convert spectrum back to data
  *totalChecksum = *totalChecksum + checksumMatrix(actual);

#ifdef VERBOSE
  displayMatrix(input);
  displayMatrix(frequencySpectrum);
  displayMatrix(actual);

  printf("difference %f\n", diffActualExpected(input, actual));
#endif

  // compare how much calculated and original differ
  return diffActualExpected(input, actual);
}


void display_total_sum(unsigned int sum, unsigned int iterations) {
	// If you are doing automated gdb testing, this is the place where
	// you want to set your breakpoint to.
	// for 200 iteration ->  checksum = d2be7e68
	// for 400 iteration ->  checksum = a57cfcd0
	// for 600 iteration ->  checksum = 783b7b38

    printf("total_checksum=0x%08x iterations=%d \n", sum, iterations);
    exit(0);
}


int main() {
  unsigned int total_sum = 0;
  unsigned int iteration = 1;

  while (1) {
    float        total     = 0.0f;
    unsigned int sum = 0;

    total += test(data_wave,            &sum);
    total += test(data_A,               &sum);
    total += test(data_checker,         &sum);
    total += test(data_checker_inverse, &sum);
    total += test(data_empty,           &sum);
    total += test(data_full,            &sum);
    total += test(data_stripe,          &sum);
    total += test(data_stripe_inverse,  &sum);
    total += test(data_random,          &sum);

    total_sum += sum;

    if ( (iteration % ITERATIONS) == 0) {
    	display_total_sum(total_sum, iteration);
    }
    printf("checksum=0x%08x diff=%f \n", sum, total);
    iteration++;
  }

  return 0;
}

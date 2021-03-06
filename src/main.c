/***********************************************************************
 * (c) Copyright 2018 Microsemi SoC Products Group. All rights reserved.
 * version 1.0 - 21/06/2018 by anton.krug@microsemi.com
 ***********************************************************************/
#include <stdio.h>
#include <float.h>
#include <math.h>

#include "data.h"
#include "test-utils.h"

#define PI_SIZE 3.14159f / SIZE
#define C(x) ( (x==0)? 1.0f / sqrtf(2.0f) : 1.0f )
//#define VERBOSE 1

#ifndef ITERATIONS
#define ITERATIONS 10
#endif

unsigned int current_configuration;


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


void __attribute__((optimize("O0"))) testValidateBreak(unsigned int sum, unsigned int current_iteration) {
	// If you are doing automated gdb testing, this is the place where
	// you want to set your breakpoint to.
    printf("total_checksum=0x%08x current_iteration=%d \n", sum, current_iteration);
}


int main() {
  unsigned int total_checksum = 0;
  unsigned int iteration      = 1;
  current_configuration       = get_configuration_state();

  while (1) {
    float        totalDifference = 0.0f;
    unsigned int checksum        = 0;

    totalDifference += test(data_wave,            &checksum);
    totalDifference += test(data_A,               &checksum);
    totalDifference += test(data_checker,         &checksum);
    totalDifference += test(data_checker_inverse, &checksum);
    totalDifference += test(data_empty,           &checksum);
    totalDifference += test(data_full,            &checksum);
    totalDifference += test(data_stripe,          &checksum);
    totalDifference += test(data_stripe_inverse,  &checksum);
    totalDifference += test(data_random,          &checksum);

    total_checksum += checksum;

    if ( (iteration % ITERATIONS) == 0) {
      testValidateBreak(total_checksum, iteration);
    }
    printf("checksum=0x%08x diff=%f iteration=%d\n", checksum, totalDifference, iteration);
    iteration++;
  }

  return 0;
}

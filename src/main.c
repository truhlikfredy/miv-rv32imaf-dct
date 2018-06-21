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


float test(float (*input)[SIZE]) {
  float frequencySpectrum[SIZE][SIZE];
  float actual[SIZE][SIZE];

  dct(input, frequencySpectrum);   // convert input data to spectrum
  idct(frequencySpectrum, actual); // then convert spectrum back to data

#ifdef VERBOSE
  displayMatrix(input);
  displayMatrix(frequencySpectrum);
  displayMatrix(actual);

  printf("difference %f\n", diffActualExpected(input, actual));
#endif

  // compare how much calculated and original differ
  return diffActualExpected(input, actual);
}


int main() {

  while (1) {
    float total = 0.0f;

    total += test(data_wave);
    total += test(data_A);
    total += test(data_checker);
    total += test(data_empty);

    printf("%f \n", total);
  }

  return 0;
}

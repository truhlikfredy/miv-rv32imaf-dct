/***********************************************************************
 * (c) Copyright 2018 Microsemi SoC Products Group. All rights reserved.
 * version 1.0 - 21/06/2018 by anton.krug@microsemi.com
 ***********************************************************************/
#include <stdio.h>
#include <float.h>
#include <math.h>

#define SIZE 8
#define PI_SIZE 3.14159f / SIZE
#define C(x) ( (x==0)? 1.0f / sqrtf(2.0f) : 1.0f )

//const float input[SIZE][SIZE] =
//  { { 0.2f, 0.1f, 0.3f, 1.0f},
//		{ 1.0f, 1.0f, 1.0f, 0.9f},
//	  { 0.0f, 0.0f, 0.0f, 0.9f},
//		{ 0.1f, 0.2f, 0.3f, 0.4f} };


const float input[SIZE][SIZE] = {
    { 0.2f, 0.3f, 0.4f, 0.5f, 0.6f, 0.7f, 0.8f, 0.9f},
    { 0.2f, 0.3f, 0.4f, 0.5f, 0.6f, 0.7f, 0.8f, 0.9f},
    { 0.2f, 0.9f, 0.4f, 0.5f, 0.6f, 0.7f, 0.8f, 0.9f},
    { 0.2f, 0.3f, 0.4f, 0.5f, 0.6f, 0.7f, 0.8f, 0.9f},
    { 0.2f, 0.3f, 0.4f, 0.5f, 0.6f, 0.7f, 0.8f, 0.9f},
    { 0.2f, 0.3f, 0.4f, 0.5f, 0.6f, 0.7f, 0.8f, 0.9f},
    { 0.2f, 0.3f, 0.4f, 0.5f, 0.6f, 0.7f, 0.8f, 0.9f},
    { 0.2f, 0.3f, 0.4f, 0.5f, 0.6f, 0.7f, 0.8f, 0.9f}
  };

float result[SIZE][SIZE];
float result2[SIZE][SIZE];

int main() {
  for (int y = 0; y < SIZE; y++) {
    for (int x = 0; x < SIZE; x++) {
      float frequency = 0.0f;
      for (int j = 0; j < SIZE; j++) {
        for (int i = 0; i < SIZE; i++) {
          frequency += input[j][i] * cosf(PI_SIZE * (i+0.5f) * x) *
                                     cosf(PI_SIZE * (j+0.5f) * y);
        }
      }
      result[y][x] = 2.0f/SIZE * C(y) * C(x) * frequency;
    }
  }

//  for (int j = 0; j < SIZE; j++) {
//    for (int i = 0; i < SIZE; i++) {
//       if (i==0 ) {
//         result[j][i] *= 1.0f / sqrtf(2.0f);
////         result[j][i] *= 1.0f / sqrtf(2.0f);
//       }
//       if (j==0) {
//         result[j][i] *= 1.0f / sqrtf(2.0f);
////         result[j][i] *= 1.0f / sqrtf(2.0f);
//       }
//       {
//         result[j][i] *= 2.0f/SIZE;
////         result[j][i] *= sqrtf(2.0f/SIZE);
//       }
//    }
//  }

  for (int j = 0; j < SIZE; j++) {
    for (int i = 0; i < SIZE; i++) {
      float pixel = 0.0f;
      for (int y = 0; y < SIZE; y++) {
        for (int x = 0; x < SIZE; x++) {
           pixel += C(x) * C(y) * result[y][x] * cosf(PI_SIZE * (i+0.5f) * x) *
                                                 cosf(PI_SIZE * (j+0.5f) * y);
        }
      }
      result2[j][i] = pixel * 2.0f/SIZE;
    }
  }

//  for (int j = 0; j < SIZE; j++) {
//    for (int i = 0; i < SIZE; i++) {
//       {
//         result2[j][i] *= 2.0f/SIZE;
////         result2[j][i] *= 2.0f/SIZE;
//       }
//    }
//  }


  for (int y = 0; y < SIZE; y++) {
    for (int x = 0; x < SIZE; x++) {
      printf("%10f", result[y][x]);
    }
    printf("\n");
  }
  printf("\n");

  float error = 0.0f;
  for (int y = 0; y < SIZE; y++) {
    for (int x = 0; x < SIZE; x++) {
      printf("%10f", result2[y][x]);
      error += fabsf(result2[y][x]-input[y][x]);
    }
    printf("\n");
  }
  printf("error %f\n", error);

  return 0;
}

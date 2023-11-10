#include <inttypes.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "vo_time.h"
#include "vo_logger.h"

#define N 1000000000

uint8_t *a;

int main()
{
  printf("Building prime number calculator\n");
  uint64_t time_s;
  uint64_t time_e;

  time_s = vo_time_get();

  uint64_t i, j;
  uint64_t num = sqrt(N - 1);
  a = (uint8_t *)malloc(N);
  for (i = 0; i < N; i++)
  {
    a[i] = 1;
  }
  for (i = 2; i <= num; i++)
  {
    if (a[i] == 1)
    {
      for (j = i * i; j < N; j += i)
        a[j] = 0;
    }
  }
  time_e = vo_time_get();
  
  LOGI("Time spent: %llu", time_e - time_s);

  // printf("Prime numbers from 2 to %d are ...\n", N - 1);
  // for (i = 2; i < N; i++)
  // {
  //   if (a[i])
  //     printf("%llu\t", i);
  // }

  return 0;
}

#include "vo_logger.h"
#include "vo_thread.h"
#include "vo_time.h"
#include <inttypes.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define D_NUM_PRIMES  1000000000
#define D_NUM_THREADS (4)
#define D_NUM_LOOPS   (5)

volatile int dodgy_mutex;
uint32_t     num_threads;
uint32_t     num_loops;
uint32_t     num_primes;

float *time_res;

float prime_calc(uint32_t *a, uint32_t thread_num) {
  uint64_t time_s;
  uint64_t time_e;

  time_s = vo_time_get();

  uint64_t i, j;
  uint64_t num = sqrt(num_primes - 1);
  for (i = 0; i < num_primes; i += 32) {
    a[i >> 5] = 0xffffffff;
  }
  for (i = 2; i <= num; i++) {
    // if (a[i] == 1) {
    if (a[i >> 5] & (1 << (i & 0x1f))) {
      for (j = i * i; j < num_primes; j += i)
        // a[j] = 0;
        a[j >> 5] &= ~(1 << (j & 0x1f));
    }
  }
  time_e           = vo_time_get();
  float time_spent = time_e - time_s;
  time_spent /= 1000000000.f;
  // time_res(thread_num * num_threads)
  // LOGI("T%d Time spent: %.3f", thread_num, time_spent);
  return time_spent;
}

void *loop_prime(void *data) {
  // LOGI("* START *");
  dodgy_mutex++;
  int       thread_num = (int)(size_t)data;
  uint32_t *buf        = (uint32_t *)malloc(num_primes / 4 + 1);
  int       i;
  for (i = 0; i < num_loops; i++) {
    time_res[thread_num * num_loops + i] = prime_calc(buf, thread_num);
  }
  free(buf);
  // LOGI("* END *");
  dodgy_mutex--;
  return NULL;
}

int main(int argc, char **argv) {
  // LOGI("%d", argc);
  if (argc > 1)
    num_threads = atoi(argv[1]);
  else
    num_threads = D_NUM_THREADS;

  if (argc > 2)
    num_loops = atoi(argv[2]);
  else
    num_loops = D_NUM_LOOPS;

  if (argc > 3)
    num_primes = atoi(argv[3]);
  else
    num_primes = D_NUM_PRIMES;

  LOGI("PRIMER executing with Threads: %d Loops: %d Primes: %d", num_threads, num_loops, num_primes);

  time_res = malloc(sizeof(float) * num_threads * num_loops);

  dodgy_mutex = 0;
  vo_thread vt[num_threads];
  int       i;
  for (i = 0; i < num_threads; i++) {
    vt[i].func     = loop_prime;
    vt[i].data     = (void *)(size_t)i;
    vt[i].req_core = i;
    vo_thread_create(&vt[i]);
    vo_thread_set_core(&vt[i]);

    // loop_prime(NULL);
  }
  sleep(1);
  while (dodgy_mutex) {
    sleep(1);
  }
  float max_time;
  max_time = 0.f; 
  for(i = 0 ; i < num_threads * num_loops ; i++) {
    if (time_res[i] > max_time)
      max_time = time_res[i];
    // printf("%.3f ", time_res[i]);
  }
  LOGI("Fastest loop: %.3fs", max_time);
  // printf("Prime numbers from 2 to %d are ...\n", N - 1);
  // for (i = 2; i < N; i++)
  // {
  //   if (a[i])
  //     printf("%llu\t", i);
  // }

  return 0;
}

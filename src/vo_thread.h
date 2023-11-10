#pragma once
#include "app_config.h"

#include <pthread.h>

typedef void *(*vo_thread_func)(void *);

typedef struct {
  pthread_t      private_pthread;
  vo_thread_func func;
  uint8_t        req_core;
  uint8_t        cur_core;
  void          *data;
} vo_thread;

vo_thread *vo_thread_create(vo_thread *thread);
int        vo_thread_stick_to_core(int core_id);
int        vo_thread_set_core(vo_thread *thread);
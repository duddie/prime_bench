#define _GNU_SOURCE
#include "vo_thread.h"
#include "vo_logger.h"
#include <errno.h>
#include <sched.h>
#include <stdio.h>
#include <unistd.h>
// #include <sys/cpuset.h>

vo_thread *vo_thread_create(vo_thread *thread) {
  if (thread == NULL) {
    thread = malloc(sizeof(vo_thread));
  }
  pthread_attr_t     attr;
  struct sched_param sch_params;
  pthread_attr_init(&attr);
  // int status = pthread_attr_getschedparam(&attr, &sch_params);
  // LOGI("getschedparam status: %d", status);
  sch_params.sched_priority = 50;
  // status                    = pthread_attr_setschedpolicy(&attr, SCHED_RR);
  // LOGI("setschedpolicy status: %d", status);
  // status = pthread_attr_setschedparam(&attr, &sch_params);
  // LOGI("setschedparam status: %d", status);

  // sem_init(&se_sem, 0, 0);
  pthread_create(&thread->private_pthread, &attr, thread->func, thread->data);
  return thread;
}

#if defined(MacOS)

/*
this is implementation of thread affinity - assigning thread to a core for MacOS

http://www.hybridkernel.com/2015/01/18/binding_threads_to_cores_osx.html

*/

#include <mach/mach.h>
#include <mach/mach_time.h>
#include <mach/mach_types.h>
#include <mach/thread_policy.h>
#include <mach/thread_switch.h>
#include <sys/param.h>
#include <sys/sysctl.h>
#define SYSCTL_CORE_COUNT "machdep.cpu.core_count"

typedef struct cpu_set {
  uint32_t count;
} cpu_set_t;

static inline void
CPU_ZERO(cpu_set_t *cs) { cs->count = 0; }

static inline void
CPU_SET(int num, cpu_set_t *cs) { cs->count |= (1 << num); }

static inline int
CPU_ISSET(int num, cpu_set_t *cs) { return (cs->count & (1 << num)); }

int sched_getaffinity(pid_t pid, size_t cpu_size, cpu_set_t *cpu_set) {
  int32_t core_count = 0;
  size_t  len        = sizeof(core_count);
  int     ret        = sysctlbyname(SYSCTL_CORE_COUNT, &core_count, &len, 0, 0);
  if (ret) {
    printf("error while get core count %d\n", ret);
    return -1;
  }
  cpu_set->count = 0;
  for (int i = 0; i < core_count; i++) {
    cpu_set->count |= (1 << i);
  }

  return 0;
}

int pthread_setaffinity_np(pthread_t thread, size_t cpu_size,
                           cpu_set_t *cpu_set) {
  thread_port_t mach_thread;
  int           core = 0;

  for (core = 0; core < 8 * cpu_size; core++) {
    if (CPU_ISSET(core, cpu_set))
      break;
  }
  // printf("binding to core %d\n", core);
  thread_affinity_policy_data_t policy = {core};
  mach_thread                          = pthread_mach_thread_np(thread);
  thread_policy_set(mach_thread, THREAD_AFFINITY_POLICY,
                    (thread_policy_t)&policy, 1);
  return 0;
}
#endif

int vo_thread_stick_to_core(int core_id) {
  int res;
  int num_cores = sysconf(_SC_NPROCESSORS_ONLN);
  if (core_id < 0 || core_id >= num_cores)
    return EINVAL;

  cpu_set_t cpuset;
  CPU_ZERO(&cpuset);
  CPU_SET(core_id, &cpuset);

  pthread_t current_thread = pthread_self();
  res                      = pthread_setaffinity_np(current_thread, sizeof(cpu_set_t), &cpuset);

  /* Check the actual affinity mask assigned to the thread */

  return res;
}

int vo_thread_set_core(vo_thread *thread) {
  int     res;
  uint8_t core_id;
  core_id       = thread->req_core;

  int num_cores = sysconf(_SC_NPROCESSORS_ONLN);
  if (core_id < 0 || core_id >= num_cores) {
    LOGE("Core outside available cores");
    return EINVAL;
  }

  cpu_set_t cpuset;
  CPU_ZERO(&cpuset);
  CPU_SET(core_id, &cpuset);

  // pthread_t current_thread = pthread_self();
  res = pthread_setaffinity_np(thread->private_pthread, sizeof(cpu_set_t), &cpuset);
#if !defined(__APPLE__)
  res = pthread_getaffinity_np(thread->private_pthread, sizeof(cpu_set_t), &cpuset);
  if (res != 0)
    LOGE("getaffinity failed");

  LOGI("Set returned by pthread_getaffinity_np() contained:\n");
  int j;
  for (j = 0; j < num_cores; j++)
    if (CPU_ISSET(j, &cpuset))
      LOGI("    CPU %d\n", j);
#endif
  return res;
}
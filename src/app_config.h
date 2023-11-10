#pragma once
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define MCU_TYPE_ARM32       1
#define MCU_TYPE_ARM64       2
#define MCU_TYPE_STM32       3

#define TARGET_MCU           MCU_TYPE_ARM64

#define WITH_VO_DSP          (1)
#define WITH_VO_FFT          (0)
#define WITH_VO_JSON         (0)
#define WITH_VO_LOGGER       (1)
#define WITH_VO_MATH         (1)
#define WITH_VO_MATH_COMPLEX (0)
#define WITH_VO_MEMORY       (1)
#define WITH_VO_STRING       (1)
#define WITH_VO_TIME         (1)
// #include "vo_common.h"

#define TDAW_ENGINE          (1)

#define ENG_THECENTRE        (0)
#define ENG_TDAW             (1)

#define SOUND_ENGINE         (ENG_TDAW)
#define FS_FSFAT             (0)
#define FS_POSIX             (1)
#define USED_FILESYSTEM      (FS_FSFAT)

#if defined(__APPLE__)
#define WITH_GFX_FRAMEBUFFER (1)
#define WITH_MINIFB          (1)
#else
#define WITH_GFX_FRAMEBUFFER (0)
#define WITH_LINUX_FB        (1)
#endif

// vo_logger
#define VO_LOG_ENABLED                        (1)
#define VO_LOG_USE_SYSTEM_PRINTF              (1)
// #define VO_LOG_PRINTF_FUNC       printf

// vo_memory
#define VO_MEM_DEBUG                          0
#define VO_MEM_USE_NATIVE                     1

// sound definition
#define DAW_NUM_VOICES                        (16)
#define DAW_NUM_TRACKS                        (16)
#define DAW_NUM_PATTERNS                      (64)

#define BLOCK_SIZE                            (128)
#define SAMPLE_RATE                           (48000)
#define SAMPLE_RATE_F                         (48000.f)
#define SAMPLE_RATE_INV                       (1.0f / 48000.f)
#define BASE_TUNING_FREQ                      (440.f)
#define BASE_TUNING_NOTE                      (69.f)
#define BLOCKS_PER_SECOND                     (SAMPLE_RATE_F / BLOCK_SIZE)

#define ALLOC_STRUCT(STRUCT_NAME_, VAR_NAME_) __A__ *__B__ = malloc(sizeof(__A__))

#define UNUSED(_A_)                           (void)_A_

#define D3_DATA
#define SECTION_SDCARD

#define UNDER_DEVELOPMENT (0)
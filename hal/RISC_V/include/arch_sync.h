#pragma once

#define smp_mb_acquire() __asm__ volatile("fence r, rw" ::: "memory")
#define smp_mb_release() __asm__ volatile("fence rw, w" ::: "memory")

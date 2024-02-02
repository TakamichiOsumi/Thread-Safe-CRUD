#ifndef __REF_COUNT__
#define __REF_COUNT__

#include <pthread.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct ref_count {
    uint32_t ref_count;
    pthread_mutex_t mutex;
} ref_count;

void ref_count_init(ref_count *rc);
void ref_count_inc(ref_count *rc);
/*
 * Returns true if the value when ref_count
 * is zero after decrement
 */
bool ref_count_dec_and_iszero(ref_count *rc);
void ref_count_destory(ref_count *rc);
void thread_using_object(ref_count *rc);
bool thread_using_object_done(ref_count *rc);

#endif

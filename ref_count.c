#include <assert.h>
#include <pthread.h>
#include <stdint.h>
#include <stdbool.h>
#include "ref_count.h"

void
ref_count_init(ref_count *rc){
    rc->ref_count = 0;
    pthread_mutex_init(&rc->mutex, NULL);
}

void
ref_count_inc(ref_count *rc){
    pthread_mutex_lock(&rc->mutex);
    rc->ref_count++;
    pthread_mutex_unlock(&rc->mutex);
}

/*
 * Returns true if the value when ref_count
 * is zero after decrement
 */
bool
ref_count_dec_and_iszero(ref_count *rc){
    bool is_zero;

    pthread_mutex_lock(&rc->mutex);
    rc->ref_count--;
    assert(rc->ref_count >= 0);
    is_zero = rc->ref_count == 0 ? true : false;
    pthread_mutex_unlock(&rc->mutex);

    return is_zero;
}

void
ref_count_destory(ref_count *rc){
    assert(rc->ref_count == 0);
    pthread_mutex_destroy(&rc->mutex);
}

void
thread_using_object(ref_count *rc){
    ref_count_inc(rc);
}

bool
thread_using_object_done(ref_count *rc){
    return ref_count_dec_and_iszero(rc);
}

#include <pthread.h>
#include <stdint.h>
#include <stdbool.h>
#include "ref_count.h"

void
ref_count_init(ref_count *rc){
    rc->ref_count = 0;
    pthread_spin_init(&rc->spinlock,
		      PTHREAD_PROCESS_PRIVATE);
}

void
ref_count_inc(ref_count *rc){
    pthread_spin_lock(&rc->spinlock);
    rc->ref_count++;
    pthread_spin_unlock(&ref_count->spinlock);
}

/*
 * Returns true if the value when ref_count
 * is zero after decrement
 */
bool
ref_count_dec_and_is_zero(ref_count *rc){
    bool is_zero;

    pthread_spin_lock(&rc->spinlock);
    rc->ref_count--;
    assert(rc->ref_count >= 0);
    is_zero = rc->ref_count == 0 ? true : false;
    pthread_spin_unlock(&rc->spinlock);

    return is_zero;
}

void
ref_count_destory(ref_count *rc){
    assert(rc->ref_count == 0);
    pthread_spin_destroy(&rc->spinlock);
}

void
thread_using_object(ref_count *rc){
    ref_count_inc(rc);
}

void
thread_using_object_done(ref_count *rc){
    return rf_count_dec_and_iszero(rc);
}

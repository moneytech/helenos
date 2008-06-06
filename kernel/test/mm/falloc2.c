/*
 * Copyright (c) 2006 Sergey Bondari
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * - Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 * - Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in the
 *   documentation and/or other materials provided with the distribution.
 * - The name of the author may not be used to endorse or promote products
 *   derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <print.h>
#include <test.h>
#include <mm/page.h>
#include <mm/frame.h>
#include <mm/slab.h>
#include <arch/mm/page.h>
#include <arch/types.h>
#include <atomic.h>
#include <debug.h>
#include <proc/thread.h>
#include <memstr.h>
#include <arch.h>

#define MAX_FRAMES 256
#define MAX_ORDER 8

#define THREAD_RUNS 1
#define THREADS 8

static atomic_t thread_count;
static atomic_t thread_fail;
static bool sh_quiet;

static void falloc(void * arg)
{
	int order, run, allocated, i;
	uint8_t val = THREAD->tid % THREADS;
	index_t k;
	
	void **frames =  (void **) malloc(MAX_FRAMES * sizeof(void *), FRAME_ATOMIC);
	if (frames == NULL) {
		if (!sh_quiet)
			printf("Thread #%" PRIu64 " (cpu%u): Unable to allocate frames\n", THREAD->tid, CPU->id);
		atomic_inc(&thread_fail);
		atomic_dec(&thread_count);
		return;
	}
	
	thread_detach(THREAD);

	for (run = 0; run < THREAD_RUNS; run++) {
		for (order = 0; order <= MAX_ORDER; order++) {
			if (!sh_quiet)
				printf("Thread #%" PRIu64 " (cpu%u): Allocating %d frames blocks ... \n", THREAD->tid, CPU->id, 1 << order);
			
			allocated = 0;
			for (i = 0; i < (MAX_FRAMES >> order); i++) {
				frames[allocated] = frame_alloc(order, FRAME_ATOMIC | FRAME_KA);
				if (frames[allocated]) {
					memsetb(frames[allocated], FRAME_SIZE << order, val);
					allocated++;
				} else
					break;
			}
			
			if (!sh_quiet)
				printf("Thread #%" PRIu64 " (cpu%u): %d blocks allocated.\n", THREAD->tid, CPU->id, allocated);
			
			if (!sh_quiet)
				printf("Thread #%" PRIu64 " (cpu%u): Deallocating ... \n", THREAD->tid, CPU->id);
			
			for (i = 0; i < allocated; i++) {
				for (k = 0; k <= (((index_t) FRAME_SIZE << order) - 1); k++) {
					if (((uint8_t *) frames[i])[k] != val) {
						if (!sh_quiet)
							printf("Thread #%" PRIu64 " (cpu%u): Unexpected data (%c) in block %p offset %#" PRIi "\n", THREAD->tid, CPU->id, ((char *) frames[i])[k], frames[i], k);
						atomic_inc(&thread_fail);
						goto cleanup;
					}
				}
				frame_free(KA2PA(frames[i]));
			}
			
			if (!sh_quiet)
				printf("Thread #%" PRIu64 " (cpu%u): Finished run.\n", THREAD->tid, CPU->id);
		}
	}

cleanup:	
	free(frames);
	
	if (!sh_quiet)
		printf("Thread #%" PRIu64 " (cpu%u): Exiting\n", THREAD->tid, CPU->id);
	atomic_dec(&thread_count);
}

char * test_falloc2(bool quiet)
{
	unsigned int i;
	sh_quiet = quiet;

	atomic_set(&thread_count, THREADS);
	atomic_set(&thread_fail, 0);
		
	for (i = 0; i < THREADS; i++) {
		thread_t * thrd = thread_create(falloc, NULL, TASK, 0, "falloc", false);
		if (!thrd) {
			if (!quiet)
				printf("Could not create thread %u\n", i);
			break;
		}
		thread_ready(thrd);
	}
	
	while (atomic_get(&thread_count) > 0) {
		if (!quiet)
			printf("Threads left: %ld\n", atomic_get(&thread_count));
		thread_sleep(1);
	}
	
	if (atomic_get(&thread_fail) == 0)
		return NULL;
	
	return "Test failed";
}

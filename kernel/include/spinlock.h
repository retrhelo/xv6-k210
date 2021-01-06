/* 
 * Description: Spinlock
 */

#ifndef __SPINLOCK_H
#define __SPINLOCK_H 

#include "types.h"

struct spinlock {
	uint locked;		// is the lock held

	// For debugging 
	char *name;			// Name of lock 
	struct cpu *cpu;	// The cpu holding the lock 
};

void initlock(struct spinlock *lk, char *name);

void acquire(struct spinlock *lk);
void release(struct spinlock *lk);

int holding(struct spinlock *lk);

void push_off(void);
void pop_off(void);

#endif 

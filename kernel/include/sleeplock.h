/* 
 * Description: Sleep lock for user process.
 */

#ifndef __SLEEPLOCK_H
#define __SLEEPLOCK_H 

#include "spinlock.h"

// long-term locks for processes 
struct sleeplock {
	uint locked;		// is the lock held? 
	struct spinlock lk;	// spinlock protecting this sleep lock 

	// For debugging 
	char *name;
	int pid;
};

/* 
 * Acquire a sleep lock 
 */
void acquiresleep(struct sleeplock*);

/* 
 * Release the lock 
 */
void releasesleep(struct sleeplock*);

int holdingsleep(struct sleeplock*);

void initsleeplock(struct sleeplock*, char*);

#endif 

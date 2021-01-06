#ifndef __BIO_H
#define __BIO_H

#define BSIZE 512

#include "sleeplock.h"

struct buf {
	int valid;
	int disk;		// does disk "own" buf? 
	uint dev;
	uint sectorno;	// sector number 
	struct sleeplock lock;
	uint refcnt;
	struct buf *prev;
	struct buf *next;
	uchar data[BSIZE];
};

/* 
 * init buffer pool 
 */
void binit(void);

/*
 * get buffer of given device and sector 
 */
struct buf *bread(uint dev, uint sector);

/* 
 * release a locked buffer
 * move it to the head of the most-recently-used list. 
 */
void brelse(struct buf *b);

/* 
 * write buffer content back to disk 
 */
void bwrite(struct buf *b);

void bpin(struct buf *b);

void bunpin(struct buf *b);

#endif 

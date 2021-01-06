/* 
 * Description: Disk related functions, for init, 
 * 		read from and write to certain disk. 
 * 		Has a strong relation with bio.c 
 */

#ifndef __DISK_H
#define __DISK_H 

#include "bio.h"

/* 
 * Initialize the disk 
 */
void disk_init(void);

/* 
 * Read from a disk sector, write it into 
 * a give buffer data field. 
 */
void disk_read(struct buf *b);

/* 
 * Write data field of a buffer into the 
 * disk sector. 
 */
void disk_write(struct buf *b);

#endif 

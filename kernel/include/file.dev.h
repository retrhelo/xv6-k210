#ifndef __FILE_H
#define __FILE_H 

#include "types.h"

// currently no pipeline would be supported 
struct file {
	enum {FD_NONE, FD_PIPE, FD_ENTRY, FD_DEVICE} type;
	int ref;
	char readable;
	char writable;
	struct entry *ent; 	// FD_ENTRY 
	int offset;			// offset from start of file 
	uint32 cur_cluster;	// current cluster been accessed 
};

/* 
 * Initialize file structure pool 
 */
void file_init(void);

/* 
 * allocate a unused file structure 
 */
struct file *file_alloc(void);

/* 
 * close a file structure 
 */
void file_close(struct file *fp);

/* 
 * Read from a file 
 */
int file_read(struct file *fp, uint8 *buf, int len);

/* 
 * Write to file 
 */
int file_write(struct file *fp, uint8 *buf, int len);

/* 
 * Relocate file's r/w head 
 */
int file_reloc(struct file *fp, int offset, int start);

#endif 

#ifndef _FAT32_H
#define _FAT32_H

#include "sleeplock.h"

#define ATTR_READ_ONLY      0x01
#define ATTR_HIDDEN         0x02
#define ATTR_SYSTEM         0x04
#define ATTR_VOLUME_ID      0x08
#define ATTR_DIRECTORY      0x10
#define ATTR_ARCHIVE        0x20
#define ATTR_LONG_NAME      0x0F

#define LAST_LONG_ENTRY     0x40
#define FAT32_EOC           0x0ffffff8
#define EMPTY_ENTRY         0xe5
#define END_OF_ENTRY        0x00
#define CHAR_LONG_NAME      13

#define FAT32_MAX_FILENAME  255
#define ENTRY_CACHE_NUM     50

struct dir_entry {
    // uchar   path[260];
    ushort  filename[FAT32_MAX_FILENAME + 1];
    uint8   attribute;
    uint8   create_time_tenth;
    uint16  create_time;
    uint16  create_date;
    uint16  last_access_date;
    uint32  first_clus;
    uint16  last_write_time;
    uint16  last_write_date;
    uint32  file_size;

    /* for OS */
    int     ref;
    // struct sleeplock    lock;
};

/**
 * Read the Boot Parameter Block.
 * @return  0       if success
 *          -1      if fail
 */
int fat32_init(void);

// retrhelo: whether these functions should be in the header file 
// 		remains unknown. maybe upper level should only use get_entry() 
// 		and get_parent() to get a dir_entry object. 
struct dir_entry *ealoc(void);

struct dir_entry *edup(struct dir_entry *entry);

void eput(struct dir_entry *entry);

void elock(struct dir_entry *entry);
void eunlock(struct dir_entry *entry);

/* 
 * Read filename from directory entry. 
 * @param filename 		pointer to the array that stores the name 
 * @param raw_entry 	pointer to the entry in a sector buffer 
 * @param longcnt 		if non-zero, read as long-name-entry, otherwise 
 * 						short-name-entry 
 */
void read_entry_name(ushort *filename, uint8 *raw_entry, int longcnt);

/*
 * Read entry_info from directory entry. 
 * @param entry 		pointer to the structure that stores the entry info 
 * @param raw_entry 	pointer to the entry in a sector buffer 
 */
void read_entry_info(struct dir_entry *entry, uint8 *raw_entry);

/* 
 * Get an entry based on path. 
 * @param path 			the absolute path of target entry 
 */
struct dir_entry *get_entry(ushort *path);

/* 
 * Get the parent entry. 
 */
struct dir_entry *get_parent(ushort *path, ushort *name);

/* 
 * Read data from file 
 * @param cluster 		the cluster to access 
 * @param offset 		the offset from beginning of the file 
 * @param buf 			target data buf, MUST be physical address 
 * @param len 			length of data to be read from 
 * @return 				the actual number of bytes been read 
 */
int entry_read_data(uint32 cluster, int offset, uint8 *buf, int len);

/* 
 * Write data into file(actually the data buffer)
 * @param cluster 		the cluster to access 
 * @param offset 		offset in a cluster 
 * @param buf 			buf stores the data to write, MUST be physical address 
 * @param len 			length of data to write 
 * @return 				the actual number of bytes written 
 */
int entry_write_data(uint32 cluster, int offset, uint8 *buf, int len);

/* 
 * Get next cluster based on current cluster, by checking FAT 
 * @param cluster 		current cluster 
 * @return 				the next cluster number. 
 */
uint32 read_fat(uint32 cluster);

#endif

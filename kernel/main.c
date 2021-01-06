// A test file modified from old main.c(now as main.back.c) 

#include "include/bio.h"
#include "include/disk.h"

#include "include/fat32.h"

static inline void inithartid(unsigned long hartid) {
	asm volatile("mv tp, %0" : : "r"(hartid & 0x01));
}

void printfinit(void);
void printf(char*, ...);
void print_logo(void);

void plicinit(void);
void plicinithart(void);

void trapinit(void);
void trapinithart(void);

void timerinit(void);

void panic(char*) __attribute__((noreturn));

void disk_init(void);

void *memset(void*, int, uint);

void 
test_disk(void) {
	struct buf *b = bread(0, 0);

	memset(b, 0, sizeof(struct buf));

	memset(b->data, 'X', BSIZE);
	disk_write(b);
	printf("disk write finish\n");

	memset(b->data, 0, BSIZE);
	disk_read(b);

	for (int i = 0; i < BSIZE; i ++) {
		if (i % 16 == 0) printf("\n");
		printf("%c", b->data[i]);
	}

	printf("test_disk");
}

void 
main(unsigned long hartid, unsigned long dtb_pa) {
	inithartid(hartid);

	if (0 == hartid) {
		printfinit();	// init a lock for printf 
		print_logo();
		printf("hart %d enter main()...\n", hartid);

		trapinit();			// trap vectors 
		trapinithart();		// install kernel vector 
		timerinit();		// set up timer interrupt handler 

		#ifndef QEMU 
		panic("sd card no yet support");
		#endif 

		disk_init();			// init disk, could be virtio_disk or a real sd card 
		fat32_init();			// init fat32 fs 

		extern void fat32_test(char *path);
		fat32_test("/README");

		while (1);
	}
	else {	// disable hart 1
		while (1) 
			;
	}
}

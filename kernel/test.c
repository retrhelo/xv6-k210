// test implemetation

#include "include/types.h"
#include "include/param.h"
#include "include/memlayout.h"
#include "include/riscv.h"
#include "include/spinlock.h"
#include "include/proc.h"
#include "include/defs.h"
#include "include/sbi.h"
#include "include/sdcard.h"

#include "include/string.h"

extern char etext[];
extern struct proc *initproc;
void test_kalloc() {
    char *mem = kalloc();
    memset(mem, 0, PGSIZE);
    strncpy(mem, "Hello, xv6-k210", 16);
    printf("[test_kalloc]mem: %s\n", mem);
    kfree(mem);
}

void test_vm(unsigned long hart_id) {
  #ifndef QEMU
  printf("[test_vm]UARTHS:\n");
  printf("[test_vm](kvmpa) va: %p, pa: %p\n", UARTHS, kvmpa(UARTHS));
  #else
  printf("virto mmio:\n");
  printf("[test_vm](kvmpa) va: %p, pa: %p\n", VIRTIO0, kvmpa(VIRTIO0));
  #endif
  printf("[test_vm]CLINT:\n");
  printf("[test_vm](kvmpa) va: %p, pa: %p\n", CLINT, kvmpa(CLINT));
//   printf("[test_vm](kvmpa) va: %p, pa: %p\n", CLINT_MTIMECMP(hart_id), kvmpa(CLINT_MTIMECMP(hart_id)));
//   printf("[test_vm](kvmpa) va: %p, pa: %p\n", CLINT_MTIME, kvmpa(CLINT_MTIME));
  printf("[test_vm]PLIC\n");
  printf("[test_vm](kvmpa) va: %p, pa: %p\n", PLIC, kvmpa(PLIC));
//   printf("[test_vm](kvmpa) va: %p, pa: %p\n", PLIC_PRIORITY, kvmpa(PLIC_PRIORITY));
//   printf("[test_vm](kvmpa) va: %p, pa: %p\n", PLIC_PENDING, kvmpa(PLIC_PENDING));
//   printf("[test_vm](kvmpa) va: %p, pa: %p\n", PLIC_MENABLE(hart_id), kvmpa(PLIC_MENABLE(hart_id)));
//   printf("[test_vm](kvmpa) va: %p, pa: %p\n", PLIC_SENABLE(hart_id), kvmpa(PLIC_SENABLE(hart_id)));
//   printf("[test_vm](kvmpa) va: %p, pa: %p\n", PLIC_MPRIORITY(hart_id), kvmpa(PLIC_MPRIORITY(hart_id)));
//   printf("[test_vm](kvmpa) va: %p, pa: %p\n", PLIC_SPRIORITY(hart_id), kvmpa(PLIC_SPRIORITY(hart_id)));
//   printf("[test_vm](kvmpa) va: %p, pa: %p\n", PLIC_MCLAIM(hart_id), kvmpa(PLIC_MCLAIM(hart_id)));
//   printf("[test_vm](kvmpa) va: %p, pa: %p\n", PLIC_SCLAIM(hart_id), kvmpa(PLIC_SCLAIM(hart_id)));
  
  printf("[test_vm]rustsbi:\n");
  printf("[test_vm](kvmpa) va: %p, pa: %p\n", RUSTSBI_BASE, kvmpa(RUSTSBI_BASE));
  printf("[test_vm](kvmpa) va: %p, pa: %p\n", RUSTSBI_BASE + 0x1000, kvmpa(RUSTSBI_BASE + 0x1000));
  printf("[test_vm](kvmpa) va: %p, pa: %p\n", RUSTSBI_BASE + 0x2000, kvmpa(RUSTSBI_BASE + 0x2000));
  printf("[test_vm](kvmpa) va: %p, pa: %p\n", RUSTSBI_BASE + 0x3000, kvmpa(RUSTSBI_BASE + 0x3000));
  printf("[test_vm]kernel base:\n");
  printf("[test_vm](kvmpa) va: %p, pa: %p\n", KERNBASE, kvmpa(KERNBASE));
  printf("[test_vm](kvmpa) va: %p, pa: %p\n", KERNBASE + 0x1000, kvmpa(KERNBASE + 0x1000));
  printf("[test_vm](kvmpa) va: %p, pa: %p\n", KERNBASE + 0x2000, kvmpa(KERNBASE + 0x2000));
  printf("[test_vm](kvmpa) va: %p, pa: %p\n", KERNBASE + 0x3000, kvmpa(KERNBASE + 0x3000));
  printf("[test_vm]etext:\n");
  printf("[test_vm](kvmpa) va: %p, pa: %p\n", (uint64)etext, kvmpa((uint64)etext));
  printf("[test_vm](kvmpa) va: %p, pa: %p\n", (uint64)etext + 0x1000, kvmpa((uint64)etext + 0x1000));
  printf("[test_vm](kvmpa) va: %p, pa: %p\n", (uint64)etext + 0x2000, kvmpa((uint64)etext + 0x2000));
  printf("[test_vm](kvmpa) va: %p, pa: %p\n", (uint64)etext + 0x3000, kvmpa((uint64)etext + 0x3000));
  printf("[test_vm]trampoline:\n");
  printf("[test_vm](kvmpa) va: %p, pa: %p\n", TRAMPOLINE, kvmpa(TRAMPOLINE));
  printf("[test_vm](kvmpa) va: %p, pa: %p\n", TRAMPOLINE + PGSIZE - 1, kvmpa(TRAMPOLINE + PGSIZE - 1));
  printf("[test_vm]create test pagetable\n");
  pagetable_t test_pagetable = uvmcreate();
  printf("[test_vm]test_pagetable: %p\n", test_pagetable);
  char *test_mem = kalloc();
  memset(test_mem, 0, PGSIZE);
  if(mappages(test_pagetable, 0, PGSIZE, (uint64)test_mem, PTE_R | PTE_W | PTE_U | PTE_X) != 0) {
    panic("[test_vm]mappages failed\n");
  }
  printf("[test_vm](walkaddr) va: %p, pa: %p\n", 0, walkaddr(test_pagetable, 0));
  printf("[test_vm](walkaddr) va: %p, pa: %p\n", PGSIZE - 1, walkaddr(test_pagetable, PGSIZE - 1) + (PGSIZE - 1) % PGSIZE);
}

#include "include/fat32.h"
#include "include/string.h"

void fat32_test(char *path) {
	wchar wpath[FAT32_MAX_FILENAME];

	wnstr(wpath, (uchar const*)path, FAT32_MAX_FILENAME);

	printf("path: %ls\n", wpath);
	
	// test above first 
	//while (1) ;

	struct dir_entry *entry = get_entry(wpath);
	if (entry) {
		printf("filename: %ls\n", wpath);
		printf("\tsize: %u\n", entry->file_size);
		printf("\tcluster: %u\n", entry->first_clus);

		printf("\t: %u\n", entry->create_date);
		printf("\t: %u\n", entry->create_time);
		printf("\t: %u\n", entry->create_time_tenth);
		printf("\t: %u\n", entry->last_access_date);
		printf("\t: %u\n", entry->last_write_date);
		printf("\t: %u\n", entry->last_write_time);
	}
	else {
		printf("file \"%ls\" not found!\n", wpath);
	}

	if (entry) {
		uint8 buf[1025];

		memset(buf, 0, 1025);
		int ret = entry_read_data(entry->first_clus, 0, (uint8*)buf, 1024);
		buf[1024] = 0;

		printf("data read:\n%s\n", buf);
		printf("data bytes: %d\n", ret);
	}
}

// Copyright (c) 2006-2019 Frans Kaashoek, Robert Morris, Russ Cox,
//                         Massachusetts Institute of Technology

#include "include/types.h"
#include "include/param.h"
#include "include/memlayout.h"
#include "include/riscv.h"
#include "include/defs.h"
#include "include/sbi.h"
#include "include/sdcard.h"
#include "include/fpioa.h"

static inline void inithartid(unsigned long hartid) {
  asm volatile("mv tp, %0" : : "r" (hartid & 0x1));
}

volatile static int started = 0;

void
main(unsigned long hartid, unsigned long dtb_pa)
{
  inithartid(hartid);
  
  if (hartid == 0) {
    printfinit();   // init a lock for printf 
    print_logo();
    printf("hart %d enter main()...\n", hartid);
    kinit();         // physical page allocator
    kvminit();       // create kernel page table
    kvminithart();   // turn on paging
    trapinit();      // trap vectors
    trapinithart();  // install kernel trap vector
    timerinit();     // set up timer interrupt handler
    procinit();
    #ifndef QEMU
    device_init(dtb_pa, hartid);
    fpioa_pin_init();
    sdcard_init();
    #endif
    //plicinit();      // set up interrupt controller
    //plicinithart();  // ask PLIC for device interrupts
    // binit();         // buffer cache
    // iinit();         // inode cache
    // fileinit();      // file table
    // userinit();      // first user process
    test_proc_init(8);   // test porc init

    test_kalloc();    // test kalloc
    test_vm(hartid);       // test kernel pagetable
    #ifndef QEMU
    test_sdcard();
    #endif

    printf("hart 0 init done\n");
    for(int i = 1; i < NCPU; i++) {
      unsigned long mask = 1 << i;
      sbi_send_ipi(&mask);
    }
    __sync_synchronize();
    started = 1;
  } else
  {
    // hart 1
    while (started == 0)
      ;
    __sync_synchronize();
    printfinit();   // init a lock for printf 
    printf("hart %d enter main()...\n", hartid);
    kvminithart();
    trapinithart();
    #ifndef QEMU
    device_init(dtb_pa, hartid);
    #endif
    printf("hart 1 init done\n");
  }
  scheduler();
}
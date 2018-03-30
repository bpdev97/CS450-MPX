/*
  ----- kmain.c -----

  Description..: Kernel main. The first function called after
      the bootloader. Initialization of hardware, system
      structures, devices, and initial processes happens here.
*/

#include <stdint.h>
#include <string.h>
#include <system.h>

#include <core/io.h>
#include <core/serial.h>
#include <core/tables.h>
#include <core/interrupts.h>
#include <mem/heap.h>
#include <mem/paging.h>
#include <modules/processcommand.h>


#include "modules/mpx_supt.h"
#include "modules/comhand.h"
#include "modules/polling.h"
#include "modules/queue.h"
#include "modules/pcb.h"
#include "modules/mcb.h"

void kmain(void)
{
  extern uint32_t magic;
  // Uncomment if you want to access the multiboot header
  // extern void *mbd;
  // char *boot_loader_name = (char*)((long*)mbd)[16];

  //idt_set_gate(60, (u32int)sys_call_isr, 0x08, 0x8e);
  //adding sys_call_isr to the interrupt table


  klogv("Starting MPX boot sequence...");
  // 0) Initialize Serial I/O and call mpx_init
  init_serial(COM1);    // init COM1
  set_serial_in(COM1); // set COM1 as input
  set_serial_out(COM1); // set COM1 as output
  mpx_init(MODULE_R2);  // memory management module

  klogv("Initialized serial I/O on COM1 device...");

  // 1) Initialize the support software by identifying the current
  //     MPX Module.  This will change with each module.

  // 2) Check that the boot was successful and correct when using grub
  // Comment this when booting the kernel directly using QEMU, etc.
  if ( magic != 0x2BADB002 ){
    //kpanic("Boot was not error free. Halting.");
  }
  
  // 3) Descriptor Tables
  klogv("Initializing descriptor tables...");
  init_gdt(); // init Global Descriptor Table
  init_idt(); // init Interupt Descriptor Table

  // 4) Virtual Memory
  klogv("Initializing virtual memory...");
  init_pic();    // init programmable interrupt controllers
  init_irq();    // install initial interupt handlers for first 32 irq lines 
  sti();         // turn on interrupts
  init_paging(); // init paging

  /*initializeHeap(50000); //intialize heap
  sys_set_malloc(&allocMem); //override the default kmalloc and kfree
  sys_set_free(&freeMem); //override the default kmalloc and kfree*/

  // 4.5) PCB Queues
  klogv("Initializing PCB queues...");
  ready = sys_alloc_mem(sizeof(QUEUE));
  readySuspended = sys_alloc_mem(sizeof(QUEUE));
  blocked = sys_alloc_mem(sizeof(QUEUE));
  blockedSuspended = sys_alloc_mem(sizeof(QUEUE));
  
  // 4.6) Create the idle process and add it to the ready queue
  klogv("Initializing the idle process");
  InsertPCB(SetupPCB("idle", 0, 9, &idle));

  // 5) Call YOUR command handler -  interface method
  klogv("Transferring control to commhand...");
  InsertPCB(SetupPCB("comhand", 0, 8, &comhand));
  sys_req(IDLE, DEFAULT_DEVICE, NULL, NULL);

  // 6) System Shutdown on return from your command handler
  klogv("Starting system shutdown procedure...");
  
  /* Shutdown Procedure */
  klogv("Shutdown complete. You may now turn off the machine. (QEMU: C-a x)");
  hlt();
}

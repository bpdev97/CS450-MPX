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

void kmain(void)
{
   extern uint32_t magic;
   // Uncomment if you want to access the multiboot header
   // extern void *mbd;
   // char *boot_loader_name = (char*)((long*)mbd)[16];

  
   klogv("Starting MPX boot sequence...");
   // 0) Initialize Serial I/O and call mpx_init
   init_serial(COM1);    // init COM1
   set_serial_in(COM1); // set COM1 as input
   set_serial_out(COM1); // set COM1 as output
   mpx_init(MODULE_R1);  // init module R1 
 
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

  // 4.5) PCB Queues
  klogv("Initializing PCB queues...");
  ready = sys_alloc_mem(sizeof(QUEUE));
  readySuspended = sys_alloc_mem(sizeof(QUEUE));
  blocked = sys_alloc_mem(sizeof(QUEUE));
  blockedSuspended = sys_alloc_mem(sizeof(QUEUE));

   // 5) Call YOUR command handler -  interface method
   klogv("Transferring control to commhand...");
   comhand(); // Call command handler

   // 6) System Shutdown on return from your command handler
   klogv("Starting system shutdown procedure...");
   
   /* Shutdown Procedure */
   klogv("Shutdown complete. You may now turn off the machine. (QEMU: C-a x)");
   hlt();

}

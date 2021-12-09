// Physical memory allocator, intended to allocate
// memory for user processes, kernel stacks, page table pages,
// and pipe buffers. Allocates 4096-byte pages.

#include "types.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "spinlock.h"

#define PAGE_BITS 12

void freerange(void *vstart, void *vend);
extern char end[]; // first address after kernel loaded from ELF file
                   // defined by the kernel linker script in kernel.ld

struct run {
  struct run *next;
};


// memlayout.h:4:#define PHYSTOP 0xE000000           // Top physical memory

// #define V2P(a) (((uint) (a)) - KERNBASE)				//Subtract KERNBASE, marked at 2GB

// #define KERNBASE 0x80000000         // First kernel virtual address (2GB)

struct {
  struct spinlock lock;
  int use_lock;
  struct run *freelist;
  int numFreePages;		// Added a number to keep a track of free pages. -Josiah
  uint ref_count[PHYSTOP >> PAGE_BITS]; // Offset by 12 bits (log_2(4096)) 
} kmem;

// Initialization happens in two phases.
// 1. main() calls kinit1() while still using entrypgdir to place just
// the pages mapped by entrypgdir on free list.
// 2. main() calls kinit2() with the rest of the physical pages
// after installing a full page table that maps them on all cores.
void
kinit1(void *vstart, void *vend)
{
  initlock(&kmem.lock, "kmem");
  kmem.use_lock = 0;
  kmem.numFreePages = 0;		// Initialize with no free pages. -Josiah
  freerange(vstart, vend);
}

void
kinit2(void *vstart, void *vend)
{
  freerange(vstart, vend);
  kmem.use_lock = 1;
}

void
freerange(void *vstart, void *vend)
{
  char *p;
  p = (char*)PGROUNDUP((uint)vstart);
  for(; p + PGSIZE <= (char*)vend; p += PGSIZE){
  	kmem.ref_count[V2P(p) >> PAGE_BITS] = 0;		//Initialize reference count to 0
    kfree(p);
  }
}
//PAGEBREAK: 21
// Free the page of physical memory pointed at by v,
// which normally should have been returned by a
// call to kalloc().  (The exception is when
// initializing the allocator; see kinit above.)
void
kfree(char *v)
{
  struct run *r;

  if((uint)v % PGSIZE || v < end || V2P(v) >= PHYSTOP)
    panic("kfree");

  if(kmem.use_lock)
    acquire(&kmem.lock);
  r = (struct run*)v;
  
  if(kmem.ref_count[V2P(v) >> PAGE_BITS] > 0)
  	kmem.ref_count[V2P(v) >> PAGE_BITS]--;
  	
  
  if(kmem.ref_count[V2P(v) >> PAGE_BITS] == 0) {
    // Fill with junk to catch dangling refs.
  	memset(v, 1, PGSIZE);
		r->next = kmem.freelist;
		kmem.freelist = r;
		kmem.numFreePages++;		//Add a free page, because it was freed. -Josiah
  }
  
  if(kmem.use_lock)
    release(&kmem.lock);
}

// Allocate one 4096-byte page of physical memory.
// Returns a pointer that the kernel can use.
// Returns 0 if the memory cannot be allocated.
char*
kalloc(void)
{
  struct run *r;

  if(kmem.use_lock)
    acquire(&kmem.lock);
    
  r = kmem.freelist;

  if(r){
    kmem.freelist = r->next;
    kmem.numFreePages--;			// Remove a free page, because it was allocated. -Josiah
    kmem.ref_count[V2P((char*) r) >> PAGE_BITS] = 1;
  }
  
  if(kmem.use_lock)
    release(&kmem.lock);
    
  return (char*)r;
}

// Added this function, returns number of free pages -Josiah
int
getNumFreePages(void)
{
	int free_pages;
	acquire(&kmem.lock);
	free_pages = kmem.numFreePages;
	release(&kmem.lock);
	
	return free_pages;
}

void
inc_ref_count(uint pa)		//pa is used for physical address in xv6
{
	acquire(&kmem.lock);
	kmem.ref_count[pa>>PAGE_BITS]++;
	release(&kmem.lock);
}

void
dec_ref_count(uint pa)		//pa is used for physical address in xv6
{
	acquire(&kmem.lock);
	kmem.ref_count[pa>>PAGE_BITS]--;
	release(&kmem.lock);
}

uint 
get_ref_count(uint pa)
{
	uint count;
	acquire(&kmem.lock);
	count = kmem.ref_count[pa>>PAGE_BITS];
	release(&kmem.lock);
	return count;
}


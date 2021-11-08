Notes for each file modified/created for this lab:

defs.h
	-- added typedef uint lock_t for synchronization locks
	-- added thread and synchronization API function declarations
	
proc.c
	-- added thread and synchronization API function definitions
	
syscall.c
	-- added system call function declarations and dictionary entries
	
syscall.h
	-- added system call function macros
	
sysproc.c
	-- added system call function definitions
	
user.h
	-- added user level declarations of threading and syncronization API
	
usys.S
	-- added SYSCALLS for each function

threads_nosync.c and threads_sync.c were added as testing programs for the new
threading API.

threads_nosync and threads_sync were added to the Makefile for compiling

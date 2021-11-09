defs.h
	- added struct lock_t 
	- added thread and lock function declarations

proc.c
	- added thread and lock function definitions

syscall.c
	- added system call function declarations

syscall.h
	- added system call function macros

sysproc.c
	- added system call function definitions

user.h
	- added thread and lock function declarations

usys.S
	- added SYSCALLS for each function

Makefile
	- added threads_nosync.c and threads_sync.c

threads_nosync.c and threads_sync.c were added to the xv6 system
to test the added functions.

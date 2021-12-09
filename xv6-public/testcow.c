#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

int
main(int argc, char *argv[])
{
	int numFreePages = getNumFreePages();
	
	printf(1, "Number of Free Pages: %d\n", numFreePages);
/*	int pid = fork();*/
	fork();
	
	numFreePages = getNumFreePages();
	printf(1, "Number of Free Pages: %d\n", numFreePages);
	
	exit();
}

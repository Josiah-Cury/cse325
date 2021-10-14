#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

int
main(int argc, char *argv[])
{
	int pid, priority;
	
	if(argc == 3) {	
		pid = atoi(argv[1]);
		priority = atoi(argv[2]);
		
		set_priority(pid, priority);
		get_priority(pid);
	} else {
		printf(2, "nice needs 2 arguments. \nExample usage: nice 1 12\n");
	}

  
  exit();
}
 

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int bar (int fd) 
{ 
	char buf[100];
	if (fd>=0) 
	{ 
		read (fd,buf,10); 
	} 
	else 
	{ 
		printf ("could not read file"); 
	}
}

int foo() 
{ 
	int fd; 
	fd = open ("a.txt", O_RDONLY); 
	getpid();
	bar (fd); 
	close (fd); 
} 



int main()
{
	foo();
}

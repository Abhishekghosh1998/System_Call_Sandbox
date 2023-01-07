#include <stdio.h>
#include <unistd.h>

void foo()
{
	getpid();
	open();
	getpid();
    
}
int main()
{
	int x;
	foo();
}

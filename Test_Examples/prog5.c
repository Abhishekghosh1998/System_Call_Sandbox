#include <stdio.h>
#include <unistd.h>

void foo(int x);

void foo(int x)
{
	
	if(x==0)
		return;
		
	__asm__ ("mov $39, %rax\n\t"       
         "mov $0, %rdi\n\t"          
         "syscall");	//getpid
    foo(x-1);
}


int main()
{
	foo(10);
}

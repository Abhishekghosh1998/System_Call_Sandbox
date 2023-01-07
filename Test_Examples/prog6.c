#include <stdio.h>
#include <unistd.h>

void foo(int x);

void foo(int x)
{
	
	for(int i=1;i<=x;i++){
		
	__asm__ ("mov $39, %rax\n\t"       
         "mov $0, %rdi\n\t"          
         "syscall");	//getpid
    }
}


int main()
{
	foo(10);
}

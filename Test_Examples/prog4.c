#include <stdio.h>
#include <unistd.h>

int foo();
int bar();

int bar(int x)
{
	foo(x);
}

int foo(int x)
{
	if(x){
	__asm__ ("mov $57, %rax\n\t"      
         "mov $0, %rdi\n\t"          
         "syscall");	//fork
	}else{
		__asm__ ("mov $39, %rax\n\t"       
         "mov $0, %rdi\n\t"          
         "syscall");	//getpid
	
	}
}


int main()
{
	foo(1);
}

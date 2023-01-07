#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int bar();
int foo();

int bar(){
	char buf[8];
	gets(buf);
	puts(buf);
}

int foo()
{
	getpid();
}

int main(){
	int x=0;
	if(x==1)
		foo();
	else
		bar();
}

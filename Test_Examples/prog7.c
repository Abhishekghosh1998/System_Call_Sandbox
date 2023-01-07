#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

int main(int argc, char** argv){

	char buf[10];
	gets(buf);
	puts(buf);
}

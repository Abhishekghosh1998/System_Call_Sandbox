#include<stdio.h>
#include<time.h>

int main(int argc, char **argv) {
	//sleep(10);
	FILE *fp;
	for(int i=0;i<argc;i++){
		puts(argv[0]);
	}

	fp = fopen("hello.c","w");
	printf("Welcome to hello program\n");
	fclose(fp);
	return 0;
}

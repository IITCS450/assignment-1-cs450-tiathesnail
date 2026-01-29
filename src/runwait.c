#include "common.h"
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
static void usage(const char *a){fprintf(stderr,"Usage: %s <cmd> [args]\n",a); exit(1);}
static double d(struct timespec a, struct timespec b){
 return (b.tv_sec-a.tv_sec)+(b.tv_nsec-a.tv_nsec)/1e9;}
int main(int c,char**v){
	if(c<2) usage(v[0]);
	struct timespec start;
	clock_gettime(CLOCK_MONOTONIC,&start);
	
	int pid = fork();
	if(pid==0){
		execvp(v[1],&v[1]);
		perror(v[1]);
		exit(1);
	}
	else{
		struct timespec end;
		int exit_status;
		waitpid(pid, &exit_status, 0);
		clock_gettime(CLOCK_MONOTONIC,&end);
		printf("PID=%d elapsed=%.3f ", pid, d(start,end));
		if(WIFEXITED(exit_status)) {
			printf("exit=%d\n",WEXITSTATUS(exit_status));
		}
		else if(WIFSIGNALED(exit_status)){
			printf("signal=%d\n",WTERMSIG(exit_status));
		}
		else{
			printf("status=???");
		}
	}
 return 0;
}

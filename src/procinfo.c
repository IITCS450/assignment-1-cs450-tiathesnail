#include "common.h"
#include <ctype.h>
#include <stdio.h>
#include <unistd.h>
static void usage(const char *a){fprintf(stderr,"Usage: %s <pid>\n",a); exit(1);}
static int isnum(const char*s){for(;*s;s++) if(!isdigit(*s)) return 0; return 1;}
int main(int c,char**v){
 if(c!=2||!isnum(v[1])) usage(v[0]);
 char filepath[100];
 sprintf(filepath,"/proc/%s/stat",v[1]);
 char statusFilepath[100];
 sprintf(statusFilepath,"/proc/%s/status",v[1]);
 char cmdlineFilepath[100];
 sprintf(cmdlineFilepath,"/proc/%s/cmdline",v[1]);

 char infoString[100];
 FILE *statusFile = fopen(statusFilepath,"r");
 if(statusFile == NULL){
 	printf("pid does not correspond to an existing process.\n");
	return 0;
 }else{
	 while(fgets(infoString,100,statusFile)) {
		 long p = 0;
		 p |= (long) strstr(infoString,"State");
		 p |= (long) strstr(infoString,"PPid");
		 p |= (long) strstr(infoString,"VmRSS");
		 if(p){
			 printf("%s",infoString);
		 }

	 } 
	 fclose(statusFile);
 }
 FILE *cmdlineFile = fopen(cmdlineFilepath,"r");
 if(cmdlineFile != NULL){
 	if(fgets(infoString,100,cmdlineFile)){
	printf("Cmd: %s\n", infoString);
	}
	fclose(cmdlineFile);
 }
 FILE *statFile = fopen(filepath, "r");
 if(statFile){
	 int cpu_time = 0;
	 long bleh =0;
	 fscanf(statFile, "%[^)]) %s ", infoString, infoString);
	 for(int i=4; i<16;i++){
	 	fscanf(statFile, "%s ", infoString);
		if( i==14 || i == 15){
			sscanf(infoString,"%ld", &bleh);
			cpu_time+=bleh;
		}
	 }
	 double timeSeconds = (double)cpu_time / sysconf(_SC_CLK_TCK);
	 printf("CPU time: %ld %.3f\n", cpu_time, timeSeconds);
	 
	 fclose(statFile);
 }
 return 0;
}

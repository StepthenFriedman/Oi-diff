#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <stdint.h>
#include <dirent.h>
#include <errno.h>

#define TIME_LIMIT 	100000000
#define BILLION 	1000000000

void ask(){
	printf("do you wish to continue [y/n]?\n");
	if (getchar()=='n') exit(0);
	getchar();
}
char file_1[3][200]={"mine.c","master.c","gen.c"},
	 file_2[3][200]={"mine.cpp","master.cpp","gen.cpp"},
	 cmd[3][200]={'\0','\0','\0'},

	*target_file[3];

#define compile_mine 	if (system(cmd[0])){\
							printf("\n[Compile Error]\n");return 0;}\
						
#define compile_master	if (system(cmd[1])){\
							printf("\nfail to compile master program\n");return 0;}\

#define compile_gen		if (system(cmd[2])){\
							printf("\nfail to compile data generator\n");return 0;}\

#define run_gen			if (system("run/gen.run >data.in")){\
							printf("\ndata generator generated runtime error at test %llu\n",n);ask();}\

#define run_mine		if (system("run/mine.run <data.in >run/mine.out")){\
							printf("\n[Runtime Error]\nat test %llu\n",n);ask();}\

#define run_master		if (system("run/master.run <data.in >run/master.out")){\
							printf("\nmaster program generated runtime error at test %llu\n",n);ask();}\

int main(int argc, char **argv){
	unsigned long long n=1;
	struct timespec start, end1, end2;
	uint64_t diff1,diff2;

	if (argc>=3) strcpy(file_1[0],argv[2]);
	if (argc>=4) strcpy(file_1[1],argv[3]);
	if (argc>=5) strcpy(file_1[2],argv[4]);

	for (int i=0;i<3;i++){
		if (access(file_1[i],F_OK)){
			if (access(file_2[i],F_OK)){
				printf("\ndirectory does not exits at %s nor %s\n!",file_1[i],file_2[i]);
				exit(1);
			}else target_file[i]=file_2[i];
		}else target_file[i]=file_1[i];

		if (target_file[i][strlen(target_file[i])-1]=='c') strcpy(cmd[i],"gcc ");
		else strcpy(cmd[i],"g++ ");
	}

	DIR* dir = opendir("run");
	if (dir) closedir(dir);
 	else if (ENOENT == errno) system("mkdir run");

	strcat(cmd[0],strcat(target_file[0]," -o run/mine.run -O2 -Wall -Wextra"));
	strcat(cmd[1],strcat(target_file[1]," -o run/master.run -O2 -w"));
	strcat(cmd[2],strcat(target_file[2]," -o run/gen.run -O2 -Wall -Wextra"));

	printf("%s\n%s\n%s\n",cmd[0],cmd[1],cmd[2]);

	if (argc>=2){
		if (!strcmp(argv[1],"check")){
			compile_mine;
			compile_master;
			compile_gen;
			while(1){
				n++;
				run_gen;
				clock_gettime(CLOCK_MONOTONIC, &start);
				run_mine;
				clock_gettime(CLOCK_MONOTONIC, &end1);
				diff1 = BILLION*(end1.tv_sec-start.tv_sec)+end1.tv_nsec-start.tv_nsec;
				if (diff1>TIME_LIMIT){
					printf("[Time Limit Exceeded] at test %llu\ntime cost:%.3fs\n",n,(double)diff1/BILLION);
					ask();
				}
				run_master;
				if(system("diff run/master.out run/mine.out")){
					printf("\n[Wrong Answer] at test %llu\n",n);
					ask();
				}
				else printf("[Acceppted]\n");
			}
		}
		else if (!strcmp(argv[1],"race")){
			compile_mine;
			compile_master;
			compile_gen;
			while(1){
				n++;
				run_gen;
				clock_gettime(CLOCK_MONOTONIC, &start);
				run_mine;
				clock_gettime(CLOCK_MONOTONIC, &end1);
				run_master;
				clock_gettime(CLOCK_MONOTONIC, &end2);

				diff1 = BILLION*(end1.tv_sec-start.tv_sec)+end1.tv_nsec-start.tv_nsec;
				diff2 = BILLION*(end2.tv_sec-end1.tv_sec)+end2.tv_nsec-end1.tv_nsec;

				if (diff1<diff2) printf("\t\t\t[faster] by %ld ns\n",diff2-diff1);
				else printf("[slower] by %ld ns\n",diff1-diff2);
			}
		}
		else if (!strcmp(argv[1],"single_data")){
			compile_mine;
			compile_master;
			clock_gettime(CLOCK_MONOTONIC, &start);
			run_mine;
			clock_gettime(CLOCK_MONOTONIC, &end1);
			diff1 = BILLION*(end1.tv_sec-start.tv_sec)+end1.tv_nsec-start.tv_nsec;
			if (diff1>TIME_LIMIT){
				printf("[Time Limit Exceeded] at test %llu with %.3fs\n",n,((double)diff1)/(double)BILLION);
			}
			run_master;
			if(system("diff run/master.out run/mine.out")){
				printf("\n[Wrong Answer] at test %llu\n",n);
			}
			else printf("[Acceppted]\n");
		}
	}
	return 0;
}

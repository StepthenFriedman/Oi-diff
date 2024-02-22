#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <stdint.h>
#include <dirent.h>
#include <errno.h>

#define BUILD 1
#define RUN 2
#define CHECK 3
#define RACE 4
#define ckmax(x,y) (((x)>(y))?0:((x)=(y)))
#define BILLION 	1000000000
#define MB_PER_BYTE	1048576
long int TIME_LIMIT=1200000000,
		  MEMORY_LIMIT=2516582;

long read_massif_log(){
	FILE* log=fopen("run/log.txt","r");
	char line[1001];
	long line_count=0,stack=0,heap=0,heap_extra=0,max_total=0;

	while (fgets(line,1000,log)!=NULL){
		if (strstr(line,"snapshot")!=NULL){
			ckmax(max_total,stack+heap+heap_extra);
			line_count++;
			stack=0,heap=0;
		}
		else if (strstr(line,"mem_heap_B")!=NULL){
			sscanf(line+11,"%ld",&heap);
		}
		else if (strstr(line,"mem_heap_extra_B")!=NULL){
			sscanf(line+17,"%ld",&heap_extra);
		}
		else if (strstr(line,"mem_stacks_B")!=NULL){
			sscanf(line+13,"%ld",&stack);
		}
	}
	fclose(log);
	return max_total;
}

void ask(){
	printf("do you wish to continue [y/n]?\n");
	if (getchar()=='n') exit(0);
	getchar();
}
char file_1[3][200]={"mine.c","master.c","gen.c"},
	 file_2[3][200]={"mine.cpp","master.cpp","gen.cpp"},
	 cmd[3][200]={"\0","\0","\0"},

	*target_file[3];

#define compile_mine 	if (system(cmd[0])){\
							printf("\n[Compile Error]\n");return 0;}
						
#define compile_master	if (system(cmd[1])){\
							printf("\nfail to compile master program\n");return 0;}

#define compile_gen		if (system(cmd[2])){\
							printf("\nfail to compile data generator\n");return 0;}

#define run_gen			if (system("run/gen.run >data/gen.in")){\
							printf("\ndata generator generated runtime error at test %llu\n",n);ask();}

#define run_mine		if (system("run/mine.run <data/gen.in >data/mine.out")){\
							printf("\n[Runtime Error]\nat test %llu\n",n);ask();}

#define run_master		if (system("run/master.run <data/gen.in >data/master.out")){\
							printf("\nmaster program generated runtime error at test %llu\n",n);ask();}

#define compare			if(system("diff data/master.out data/mine.out")) printf("\n[Wrong Answer] at test %llu\n",n),ask();\
						else printf("[Acceppted]\n");

#define tle				if (diff1>TIME_LIMIT){\
							printf("[Time Limit Exceeded] at test %llu\ntime cost:%.3fs\n",n,(double)diff1/BILLION);ask();}

#define run_with_vg		system("valgrind -q --tool=massif --stacks=yes --heap=yes --massif-out-file=\"run/log.txt\" run/mine.run <data/gen.in >data/mine.out");\
						long memory=read_massif_log();\
						if (memory>MEMORY_LIMIT){\
							printf("[Memory Limit Exceeded] at test %llu\nmemory cost:%.3fmb\n",n,(double)memory/1000000);ask();}\
						else printf("Memory cost:%.3fmb\n",(double)memory/1000000);

int main(int argc, char **argv){
	char specific_data=0,use_valgrind=0,detect_tle=1;
	unsigned long long n=0;
	struct timespec start, end1, end2;
	uint64_t diff1,diff2;

	char mod=0;

	if (argc>=2){
		if (!strcmp(argv[1],"update")){
			printf("downloading source code...\n");
			system("rm -rf Oi-diff");
			if (system("git clone https://github.com/StepthenFriedman/Oi-diff.git")){
				printf("cannot download source file, update failed.\n");
				return 1;
			}
			if (system("cd Oi-diff && chmod 777 ./install.sh && ./install.sh")) printf("cannot run build script.\nplease run manually.\n");
			printf("successfully updated offline-judge.\nremove source code [y/n]?\n");
			if (getchar()=='y' && system("rm -rf Oi-diff")){
				printf("cannot remove source code.\nplease do it manually.\n");
			}
			return 0;
		}
		else if (!strcmp(argv[1],"version")) {printf("offline judge v 0.1.0\n");return 0;}
		else if (!strcmp(argv[1],"build")) mod=BUILD;
		else if (!strcmp(argv[1],"run")) mod=RUN;
		else if (!strcmp(argv[1],"check")) mod=CHECK;
		else if (!strcmp(argv[1],"race")) mod=RACE;
	}

	for (int i=2,j=0;i<argc;i++){
		if (i<argc-1 && !strcmp(argv[i],"-i")) strcpy(file_1[j++],argv[i+1]);
		else if (i<argc-1 && !strcmp(argv[i],"-t")) sscanf(argv[i+1],"%ld",&TIME_LIMIT),TIME_LIMIT*=1000000;
		else if (i<argc-1 && !strcmp(argv[i],"-m")) sscanf(argv[i+1],"%ld",&MEMORY_LIMIT),MEMORY_LIMIT*=1000;
		else if (!strcmp(argv[i],"-sd")) specific_data=1;
		else if (!strcmp(argv[i],"-vg")) use_valgrind=1;
		else if (strstr(argv[i],"-tle=false")!=NULL) detect_tle=0;
	}

	if (use_valgrind){
		FILE* log=fopen("run/log.txt","w");fclose(log);
		if (system("valgrind --version")){
			printf("valgrind do not exists.\ninstall [y/n]?\n");
			if (getchar()=='n') exit(0);
			getchar();
			if (system("sudo apt update && sudo apt install valgrind")) printf("cannot install valgrind.\nplease install manually.\n");
		}
	}

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

	dir = opendir("data");
	if (dir) closedir(dir);
 	else if (ENOENT == errno) system("mkdir data");

	strcat(cmd[0],strcat(target_file[0]," -o run/mine.run -O2 -Wall -Wextra"));
	strcat(cmd[1],strcat(target_file[1]," -o run/master.run -O2 -w"));
	strcat(cmd[2],strcat(target_file[2]," -o run/gen.run -O2 -Wall -Wextra"));

	if (!mod){
		printf("offline judge, an OI program analyzer.\nversion: 0.1.0\nno command specific.\n");
		return 0;
	}

	compile_mine;
	if (mod==BUILD) return 0;
	if (mod>=CHECK){compile_master;}
	if (!specific_data) {compile_gen;}
	while(1){
		n++;
		if (mod>=RUN && !specific_data) {run_gen;}
		if (detect_tle) clock_gettime(CLOCK_MONOTONIC, &start);
		run_mine;
		if (detect_tle) clock_gettime(CLOCK_MONOTONIC, &end1);

		if (mod>=CHECK) {run_master;}
		
		if (mod>=RACE) clock_gettime(CLOCK_MONOTONIC, &end2);

		if (detect_tle) diff1 = BILLION*(end1.tv_sec-start.tv_sec)+end1.tv_nsec-start.tv_nsec;
		if (mod>=RACE) diff2 = BILLION*(end2.tv_sec-end1.tv_sec)+end2.tv_nsec-end1.tv_nsec;

		if (detect_tle) {tle;}
		if (use_valgrind){run_with_vg;}

		if (mod>=RACE){
			if (diff1<diff2) printf("\t\t\t\t\t\t[faster] by %ld ns\n",diff2-diff1);
			else printf("\b\t\t\t[slower] by %ld ns\n",diff1-diff2);
		}
		if (mod>=CHECK) {compare;}
		if (mod<=RUN) printf("running...\n");
		if (specific_data) break;
	}
	return 0;
}

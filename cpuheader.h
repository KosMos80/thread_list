// cpuheader

#pragma once

struct TaskMask
	{
	int pid;   //num of stat 1
	char name[64]; //2
	char state[2]; //3
	int ppid; //4
	long int utime; //14
	long int stime; //15
	long int last_utime;
	long int last_stime;
	int prior; //18
	int thread; //20
	long int vsize; //23
	};
struct TaskTemp
	{
		int pid;
		long int utime;
		long int stime;
	}

TaskTemp *procTemp = new TaskTemp[0];

TaskMask *procPID = new TaskMask[0];
TaskMask *procTID = new TaskMask[0];
int maxPID = 0;
int maxTID = 0;
int maxTemp = 0;

#include "my_string_fun.h"

char stat[] = "/stat";
char task[] = "/task";
int pusto;

void create_info_task(char *var, TaskMask *task)
	{
	sscanf(var,"%d%s%s%d%d%d%d%d%d%d%d%d%d%lu%ld%d%d%d%d%d%d%d%ld", 
	&task->pid, task->name, task->state, &task->ppid, //1-4
	&pusto, &pusto, &pusto, &pusto, &pusto, &pusto, &pusto, &pusto, &pusto, //5-13
	&task->utime, &task->stime, &pusto, &pusto, &task->prior, &pusto, &task->thread, //14-20
	&pusto, &pusto, &task->vsize); //21-23
	}

void find_out_pid(int pid = 0) // если пид равен нулю, то выбоp идет из проц иначе из такс пида
	{
	TaskMask proc;
	char var[64];
	char pt[64] = "ls /proc";
	char id[64] ="";
	FILE *fp;
	if(pid)	{
	  val(id, pid);
	  add_string(pt, "/");
	  add_string(pt, id);
	  add_string(pt, "/task");
	  }
	fp = popen(pt, "r");
	int index = 0;
	while(fgets(var, sizeof(var) - 1, fp) != 0)
		if((var[0] >= '0')&&(var[0]<='9')) index++;
	pclose(fp);
	if(pid){
		// перед удалением предідущего списка нужно сохранить предідущие показания utime & stime
	delete[] procTemp;
	procTemp = new TaskTemp[maxTID];
	maxTemp = maxTID;
	for(int i = 0; i < maxTID; i++)
		{
			procTemp[i].pid=procTID[i].pid;
			procTemp[i].utime = procTID[i].utime;
			procTemp[i].stime = procTID[i].stime;
		}
	delete[] procTID;
	procTID = new TaskMask[index];
	maxTID = index;
	}else{
	delete[] procTemp;
	procTemp = new TaskTemp[maxPID];
	maxTemp = maxPID;
	for(int i = 0; i < maxPID; i++)
		{
			procTemp[i].pid=procPID[i].pid;
			procTemp[i].utime = procPID[i].utime;
			procTemp[i].stime = procPID[i].stime;
		}		
	delete[] procPID;
	procPID = new TaskMask[index];
	maxPID = index;
	}
	index = 0;
	if((fp = popen(pt, "r")))
	   {
	   while(fgets(var, sizeof(var) - 1, fp) != 0)
		{
		if((var[0] >= '0')&&(var[0]<='9'))
			{
			// var строка 
			FILE *fs;
			char path[64]="/proc/";
			char dat[1024];
			if(pid == 0)
				{
				// запрос на стат из пид   /proc/<pid>/stat
				add_string(path, var);
				add_string(path, "/stat");
				}
				else
				{
				// запрос на стат из тид /proc/<pid>/task/<tid>/stat
				add_string(path, id);
				add_string(path, "/task/");
				add_string(path, var);
				add_string(path, "/stat");
				}
			if((fs = fopen(path,"r")))
				{
				fgets(dat, sizeof(dat)-1, fs);
				del_space(dat, sizeof(dat));
				create_info_task(dat, &proc);
				if(pid){
					proc.ppid = proc.pid;
					proc.pid = pid;
					procTID[index] = proc;
					for(int i = 0; i < maxTemp; i++)
						{
							if(procTID[index].pid == procTemp[i].pid)
								{
								procTID[index].last_utime = procTemp[i].utime;
								procTID[index].last_stime = procTemp[i];		
								i = maxTemp;
								}
								else
								{
								procTID[index].last_utime = 0;
								procTID[index].last_stime = 0;	
								}
						}
					} 
					else 
					{
					procPID[index] = proc;
					for(int i = 0; i < maxTemp; i++)
						{
							if(procPID[index].pid == procTemp[i].pid)
								{
								procPID[index].last_utime = procTemp[i].utime;
								procPID[index].last_stime = procTemp[i];		
								i = maxTemp;
								}
								else
								{
								procPID[index].last_utime = 0;
								procPID[index].last_stime = 0;	
								}
						}
					}
				index++;
				fclose(fs);
				}

			}		
		}
	   pclose(fp);
	   }
	}



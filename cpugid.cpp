#include <iostream>
#include <stdio.h>
#include "cpuheader.h"
#include <signal.h> 
#include <sys/time.h>

#define INTERVAL 100000  // интервал таймера  -- 100000 us - 100 ms
struct itimerval tout_val;
class 
MyClass {
    public : 
    void settimer(); 
    void registertimer(); 
    static void timerevent(void);
    private :
	};


ListBox listPID;
ListBox listTID;

int sec = 0;
int tim = 10;

int old_sizeW, old_sizeH, new_sizeW, new_sizeH;

int timer_count = 0;
 
void MyClass::settimer() { // настройка времени таймера
    tout_val.it_interval.tv_sec = 0; 
    tout_val.it_interval.tv_usec = 0; 
    tout_val.it_value.tv_sec = 0;  
    tout_val.it_value.tv_usec = INTERVAL; 
    setitimer(ITIMER_REAL, &tout_val,0);
    }

void MyClass::registertimer() { typedef 
    void(*fp)(void); fp ptr = &timerevent; 
    signal(SIGALRM,(void(*)(int))ptr); /* 
    set the Alarm signal capture */
     }
 
void MyClass::timerevent(void) {  //выполняемая процдера прерываний
     setitimer(ITIMER_REAL, &tout_val,0);
// тело 	

old_sizeW = new_sizeW;
old_sizeH = new_sizeH;
new_sizeW = getsizeW();
new_sizeH = getsizeH();
if((old_sizeW != new_sizeW)||(old_sizeH != new_sizeH))
	{
	listPID.Create(1,1,new_sizeW,new_sizeH/2);
	listTID.Create(1,new_sizeH/2+1,new_sizeW,new_sizeH/2);
	listPID.Draw();
	listTID.Draw();
	}
if(tim == 10)
	{
	find_out_pid();
	listPID.max = maxPID;
	find_out_pid(listPID.getPID());
	listTID.max = maxTID;
	listPID.Draw();
	listTID.Draw();
//	gotoXY(1,1); std::cout << "Time(s):" << sec<< std::endl;
	sec++;
	tim = 0;
	}
	tim++;
	gotoXY(1,1); std::cout << "Time(s):"<<sec<<std::endl;
    }
//////////////////////////////////////////////////////////////////////////////////////////

int pid;
volatile int oldkey, key;

char main_name1[] = "***-= Proc PID =-***";
char main_name2[] = "***-= Proc TID =-***";
char row_name1[] = " PID  S PPID";
char row_name2[] = " PID  S TID ";

int selectPT = 255;

int main(void)
{
system("clear");
std::cout << "\e[1;42m /proc/[pid]/stat \e[0m" << std::endl; 
//std::cout << "From GitHub"<<std::endl;
std::cout << "PID       The process ID. \n";
std::cout << "S         Indicating process state. \n";
std::cout << "PPID      The PID of the parent of this process. \n";
std::cout << "TR        Number of threads in this process. \n";
std::cout << "PR        The scheduling priority of the process. \n";
std::cout << "utime     Amount of time that this process has been scheduled in user mode. \n";
std::cout << "stime     Amount of time that this process has been scheduled in kernel mode. \n";
std::cout << "name proc The filename of the executable. \n";
std::cout << "vsize     Virtual memory size. \n";
std::cout << "=============================================================================\n";
std::cout << "  \n";
std::cout << "  \n";
std::cout << "  \n";
std::cout << "Key Up - move listing up  \n";
std::cout << "Key Down - move listing down  \n";
std::cout << "Key TAB - switch panel  \n";
std::cout << "Key q or Q- exit  \n";
std::cout << "  \n";
std::cout << "  \n";
std::cout << "  \n";
std::cout << "Press Space for start....\n";
while(getch()!= 32);

// настройка окон
listPID.Create(1,1,getsizeW(),getsizeH()/2);
listPID.main_name = main_name1;
listPID.main_name_lenght = sizeof(main_name1) - 1;
listPID.row_name = row_name1;
listPID.pid = 0;
listPID.num = 0;
listPID.max = 0;
listPID.activ = 1;

listTID.Create(1,getsizeH()/2+1,getsizeW(),getsizeH()/2);
listTID.main_name = main_name2;
listTID.main_name_lenght = sizeof(main_name2) - 1;
listTID.row_name = row_name2;
listTID.pid = 1;
listTID.num = 0;
listTID.max = 0;
listTID.activ = 0;

// настройка прерывания
	MyClass myclass; 
	myclass.settimer(); 
	myclass.registertimer(); 

while((key!='q')&&(key!='Q'))
	{
	oldkey = key;
	key = getch();
	if(key == 9) selectPT ^= 255;
	if(selectPT)
		{
		listPID.activ = 1;
		listTID.activ = 0;
		listPID.Draw();
		listTID.Draw();
		}
		else
		{
		listPID.activ = 0;
		listTID.activ = 1;
		listPID.Draw();
		listTID.Draw();
		}
	if((oldkey == 91)&&(key == 66)) 
		{if(selectPT) 
			{
			listPID.decnum();
			listPID.Draw();
			}
			else 
			{
			listTID.decnum();
			listTID.Draw();
			}
		} 
	if((oldkey == 91)&&(key == 65)) 
		{if(selectPT) 
			{
			listPID.incnum();
			listPID.Draw();
			}
			else 
			{
			listTID.incnum();
			listTID.Draw();
			}
		}
	}
delete[] procPID;
delete[] procTID;
system("clear");
return  0;
}

// Функции стринговые

#pragma once

#include <termios.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/ioctl.h>

///// Размер консоли
struct winsize screen_size;

inline int getsizeW(void)  // ширина
	{
	ioctl(0, TIOCGWINSZ, &screen_size);
	return screen_size.ws_col;
	}

inline int getsizeH(void)  // высота
	{
	ioctl(0, TIOCGWINSZ, &screen_size);
	return screen_size.ws_row;
	}

// gotoxy() function definition
void gotoXY(int x, int y)
{
    printf("%c[%d;%df", 0x1B, y, x);
}
// clear screen
void clrscr(void)
{
    system("clear");
}
// удаление пробелов внутри скобок
void del_space(char *var, int max)
	{
	bool f = false;
	for(int j = 0; j < max; j++)
		{
		if(var[j] == '(') f = true;
		if(var[j] == ')') f = false;
		if((var[j] == ' ')&&f) var[j] = '*';
		}
	}
// преобразование целочисленной перем. в символьный массив
void val(char *mas, int pid)
	{
	int index = 0;
	while(pid)
		{
		index++;
		for(int i = index; i > 0; i--)
			{
			mas[i] = mas[i-1];
			}
		mas[0] = (char)(pid%10) + '0';
		pid = pid / 10;
		}
	}
// добавление к строке  mas = mas + ad /// mas = "12"; ad="34"; -> mas = "1234"
void add_string(char *mas, const char *ad)
	{
	int i = 0;
	int j = 0;
	for(i = 0; mas[i] != 0; i++) {;}
	for(j = 0; ad[j] > 31;)
		{
		mas[i] = ad[j];
		i++;
		j++;
		}
	}	

void add_string(char *mas,char *ad)
	{
	int i = 0;
	int j = 0;
	for(i = 0; mas[i] != 0; i++) {;}
	for(j = 0; ad[j] > 31;)
		{
		mas[i] = ad[j];
		i++;
		j++;
		}
	}

// чтение кода нажатой кнопки на клавиатуре без эха
int getch(void)
{
    struct termios oldattr, newattr;
    int ch;
    tcgetattr( STDIN_FILENO, &oldattr );
    newattr = oldattr;
    newattr.c_lflag &= ~( ICANON | ECHO );
    tcsetattr( STDIN_FILENO, TCSANOW, &newattr );
    ch = getchar();
    tcsetattr( STDIN_FILENO, TCSANOW, &oldattr );
    return ch;
}

// чтение кода нажатой кнопки на клавиатуре c эхом
int getche(void)
{
    struct termios oldattr, newattr;
    int ch;
    tcgetattr( STDIN_FILENO, &oldattr );
    newattr = oldattr;
    newattr.c_lflag &= ~( ICANON );
    tcsetattr( STDIN_FILENO, TCSANOW, &newattr );
    ch = getchar();
    tcsetattr( STDIN_FILENO, TCSANOW, &oldattr );
    return ch;
}
////////////////////////////////////////////////////////////////


class ListBox
	{
	public:
	int x, y;  // координаты верхнего левого угла
	int dx, dy; // высота и ширина
	char *main_name; // наименование таблицы
	char *row_name; // перечень столбцов
	int main_name_lenght; // длина наименования таблицы
	volatile int num; // указатель на позицю в листинге
	int max; // максимальное количество списка
	TaskMask list; // ссылка на массив записей
	int pid; // pid = 0 PID, pid!=0 TID
	int activ;
	
	void Create(int xx, int yy, int dxx, int dyy);
	void Draw(void);
	void Update(void);
	void incnum(void);
	void decnum(void);
	int getPID(void);
	};

void ListBox::incnum(void)
	{
	if(num !=0) num--;
	}

void ListBox::decnum(void)
	{
	if(num >= max) num = max - 1;
	if(num < max - 1) num++;
	}



int ListBox::getPID(void)
	{
	return (this->pid)?(procTID[num].pid):(procPID[num].pid);
	}

void ListBox::Create(int xx = 1, int yy = 1, int dxx = 80, int dyy = 43)
	{
	x = xx;
	y = yy;
	dx = dxx;
	dy = dyy;
	}
void ListBox::Draw(void)
	{
	gotoXY(x,y); std::cout << "+"; for(int i = (x+1); i < (x + dx -1); i++) {std::cout << '-';} std::cout << '+'; 
	gotoXY(x,y+1);std::cout << '|'; 
		if(activ) std::cout << "\e[1;42m";
		for(int i = (x+1); i < (x + dx -1); i++) {std::cout << ' ';} 
		if(activ) std::cout << "\e[0m";
	std::cout << '|';
// заглавие таблицы
	gotoXY((x+dx/2-main_name_lenght/2) ,y+1); 
		if(activ) std::cout << "\e[1;42m";
		std::cout << main_name;
		if(activ) std::cout << "\e[0m";
//		printf("%s", main_name); 
	gotoXY(x,y+2); std::cout << '+'; for(int i = (x+1); i < (x + dx -1); i++) {std::cout << '-';} std::cout << '+';
// подпись столбцов
	gotoXY(x,y+3);std::cout << '|'; 
	std::cout << row_name << "  TR  PR  utime(s) stime(s) name proc";
	for(int i = 53; i < (x+dx-15); i++) std::cout <<' '; std::cout << "vsize(kB)       "; 
	std::cout << '|';
	gotoXY(x,y+4); std::cout << '+'; for(int i = (x+1); i < (x + dx -1); i++) {std::cout << '-';} std::cout << '+';
// вывод таблицы
	for(int j = y + 5; j < (y + dy - 1); j++)
	   {
  	   gotoXY(x,j);std::cout << '|'; 
	   if(( j == y+5)&&activ) std::cout << "\e[1;42m";
	   for(int i = (x+1); i < (x + dx -1); i++) {std::cout << ' ';} 
/////////// вывод строки в таблице
	   int index;
	   index = num+j-y-5;
	   if(index <= max-1) {
		list=(pid)?(procTID[index]):(procPID[index]);
		gotoXY(x+1, j);std::cout << list.pid;
		gotoXY(x+7, j);std::cout << list.state[0];
		gotoXY(x+9, j);std::cout << list.ppid;
		gotoXY(x+15, j); std::cout << list.thread;
		gotoXY(x+18, j); std::cout << list.prior;
		gotoXY(x+23, j); std::cout << (float)(list.utime-list.last_utime)<<'%';//100; // перевести в секунды
		gotoXY(x+32, j); std::cout << (float)(list.stime-list.last_stime)<<'%';//100;
		gotoXY(x+41, j); std::cout << list.name;
		gotoXY(x+dx-17, j); std::cout << list.vsize / 1024;
		}
	   if((j == y + 5)&&activ) std::cout << "\e[0m";
//////////////
	   gotoXY(x+dx-1, j); std::cout << '|';
	   }
	gotoXY(x,y+dy-1); std::cout << '+'; for(int i = (x+1); i < (x + dx -1); i++) {std::cout << '-';} std::cout << '+';
	}

void ListBox::Update(void)
	{
	
	}




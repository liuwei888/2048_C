/*************************************************************************
    > File Name: 2048.cpp
    > Author: liuwei
    > Mail: 271551048@qq.com 
    > Created Time: 2015年08月06日 星期四 14时15分55秒
 ************************************************************************/

#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<time.h>
#include<sys/time.h>
#include<curses.h>
#include<signal.h>

int a[4][4] = {0};  //4*4方格
int empty;  //方格中空格的数量
int old_x;
int old_y;

void draw();  //画方格线
void play();  //玩游戏
void init();  //初始化
void draw_one(int y, int x); //绘制单个数字
void cnt_value(int *new_y, int *new_x); //调整初步确定的新数字的坐标 
int game_over(); 
int cnt_one(int y, int x);  //坐标为(x, y)的数字周围的空格子的个数

int main()
{
	init();
	play();
	endwin();

	return 0;
}

void init()
{
	int x, y;

	initscr();  //将屏幕初始化，并进入CURSES模式
	cbreak();   //禁止行缓冲，让字符立即显示
	noecho();   //不让控制字符显示在屏幕
	curs_set(0); //设置光标不可见

	empty = 15;  //16个格子中，初始化其中的15个为空格
	srand(time(0));
	x = rand() % 4; 
	y = rand() % 4; 
	a[y][x] = 2;  //在16个空格中，随机找一个空格，将其初始化为2
	draw();   //开始画方框
}

//绘制方框
void draw()
{
	int x, y, m, n;

	clear();  //清屏

	//画横线
	for(n = 0; n < 9; n += 2)
		for(m = 0; m < 21; m++)
		{
			move(n, m);  //光标移动到坐标(m,n)，move()是先列后行
			addch('-');  //在光标处画-
			refresh();   //将缓冲区的内容输出到终端
		}

	//画竖线
	for(m = 0; m < 22; m += 5)
		for(n = 1; n < 8; n++)
		{
			move(n, m);  //光标移动到坐标(m,n)，move()是先列后行
			addch('|');  //在光标处画|
			refresh();   //将缓冲区的内容输出到终端
		}

	for(y = 0; y < 4; y++)
		for(x = 0; x < 4; x++)
		{
			draw_one(y, x);  //画坐标(x,y)的数字
		}
}

//绘制单个数字，将具体的数字填到我们用draw绘制的表格中
void draw_one(int y, int x)
{
	int i, m, k, j;
	char c[4] = {'0', '0', '0', '0'};

	i = a[y][x];
	m = 0;
	do
	{
		j = i % 10;
		c[m++] = j + '0';
		i = i / 10;
	}while(i > 0);
	m = 0;
	k = (x + 1) * 5 - 1;
	while(c[m] != '0')
	{
		move(2*y+1, k);
		addch(c[m++]);
		k--;
	}
}

void play()
{
	int x, y, i, new_x, new_y, tmp;
	int old_empty, move;
	char ch;

	while(1)
	{
		move = 0;
		old_empty = empty;
		ch = getch();
		switch(ch)
		{
			case 'A':
			case 'a':
				//从左向右消除相同的数字
				for(y = 0; y < 4; y++)
					for(x = 0; x < 4; )
					{
						if(a[y][x] == 0)
						{
							x++;
							continue;
						}
						else
						{
							for(i = x + 1; i < 4; i++)
							{
								if(a[y][i] == 0)
								{
									continue;
								}
								else
								{
									if(a[y][x] == a[y][i])
									{
										a[y][x] += a[y][i];
										a[y][i] = 0;
										x = i + 1;
										empty++;
										break;
									}
									else
									{
										x = i;
										break;
									}
								}
							}
							x = i;
						}
					}
				//向左移动方块
				for(y = 0; y < 4; y++)
					for(x = 0; x < 4; x++)
					{
						if(a[y][x] == 0)
						{
							continue;
						}
						else
						{
							for(i = x; (i>0)&&(a[y][i-1]==0); i--)
							{
								a[y][i-1] = a[y][i];
								a[y][i] = 0;
								move = 1;
							}
						}
					}
				break;
			case 'D':
			case 'd':
				//从右向左消除相同方块
				for(y = 0; y < 4; y++)
					for(x = 3; x >= 0; )
					{
						if(a[y][x] == 0)
						{
							x--;
							continue;
						}
						else
						{
							for(i = x - 1; i >= 0; i--)
							{
								if(a[y][i] == 0)
								{
									continue;
								}
								else if(a[y][x] == a[y][i])
								{
									a[y][x] += a[y][i];
									a[y][i] = 0;
									x = i - 1;
									empty++;
									break;
								}
								else
								{
									x = i;
									break;
								}
							}
							x = i;
						}
					}
				//向右移动方块
				for(y = 0; y < 4; y++)
					for(x = 3; x >= 0; x--)
					{
						if(a[y][x] == 0)
						{
							continue;
						}
						else
						{
							for(i = x; (i<3)&&(a[y][i+1]==0); i++)
							{
								a[y][i+1] = a[y][i];
								a[y][i] = 0;
								move = 1;
							}
						}
					}
				break;
			case 'W':
			case 'w':
				//从上向下消去相同方块
				for(x = 0; x < 4; x++)
					for(y = 0; y < 4; )
					{
						if(a[y][x] == 0)
						{
							y++;
							continue;
						}
						else
						{
							for(i = y + 1; i < 4; i++)
							{
								if(a[i][x] == 0)
								{
									continue;
								}
								else if(a[y][x] == a[i][x])
								{
									a[y][x] += a[i][x];
									a[i][x] = 0;
									y = i + 1;
									empty++;
									break;
								}
								else
								{
									y = i;
									break;
								}
							}
							y = i;
						}
					}
				//向上移动方块
				for(x = 0; x < 4; x++)
					for(y = 0; y < 4; y++)
					{
						if(a[y][x] == 0)
						{
							continue;
						}
						else
						{
							for(i = y; (i>0)&&(a[i-1][x]==0); i--)
							{
								a[i-1][x] = a[i][x];
								a[i][x] = 0;
								move = 1;
							}
						}
					}
				break;
			case 'S':
			case 's':
				//从下向上消去相同方块
				for(x = 0; x < 4; x++)
					for(y = 3; y >= 0; )
					{
						if(a[y][x] == 0)
						{
							y--;
							continue;
						}
						else
						{
							for(i = y - 1; i >= 0; i--)
							{
								if(a[i][x] == 0)
								{
									continue;
								}
								else if(a[y][x] == a[i][x])
								{
									a[y][x] += a[i][x];
									a[i][x] = 0;
									y = i - 1;
									empty++;
									break;
								}
								else
								{
									y = i;
									break;
								}
							}
							y = i;
						}
					}
				//向下移动方块
				for(x = 0; x < 4; x++)
					for(y = 3; y >= 0; y--)
					{
						if(a[y][x] == 0)
						{
							continue;
						}
						else
						{
							for(i = y; (i<3)&&(a[i+1][x]==0); i++)
							{
								a[i+1][x] = a[i][x];
								a[i][x] = 0;
								move = 1;
							}
						}
					}
				break;
			case 'Q':
			case 'q':
				game_over();
				break;
			default:
				continue;
				break;
		}
		
		if(empty <= 0) //游戏中空格数为0
			game_over();

		draw();  //重新绘制方格及数字

		//在移动一次之后，生成一个新方块
		if((empty != old_empty) || (move == 1))
		{
			//初步确定新生成的数字存放的位置
			do
			{
				new_x = rand() % 4;
				new_y = rand() % 4;
			}while(a[new_y][new_x] != 0);

			//对初步确定的新数字的位置进行调整，使其更合理
			cnt_value(&new_y, &new_x);

			//确定新生成的数字的值，只能为2或者4
			do
			{
				tmp = rand() % 4;
			}while(tmp == 0 || tmp == 2);
			a[new_y][new_x] = tmp + 1;
			empty--;

			//将生成的新数字放进坐标(new_x, new_y)所在的格子中
			draw_one(new_y, new_x);
		}
	}
}

//确定坐标为(x,y)表示的数字周围的空格子的个数
int cnt_one(int y, int x)
{
	int value = 1;

	if(y - 1 > 0)
		a[y-1][x]?0:value++;
	if(y + 1 < 4)
		a[y+1][x]?0:value++;
	if(x - 1 >= 0)
		a[y][x-1]?0:value++;
	if(x + 1 < 4)
		a[y][x+1]?0:value++;
	if(y - 1 >= 0 && x - 1 >= 0)
		a[y-1][x-1]?0:value++;
	if(y - 1 >= 0 && x + 1 <=4)
		a[y-1][x+1]?0:value++;
	if(y + 1 < 4 && x - 1 >= 0)
		a[y+1][x-1]?0:value++;
	if(y + 1 < 4 && x + 1 < 4)
		a[y+1][x+1]?0:value++;

	return value;
}

void cnt_value(int *new_y, int *new_x)
{
	int x, y, value;
	int max = 0;

	//max为新随机生成的新数字周围的空格子的个数
	max = cnt_one(*new_y, *new_x);
	for(y = 0; y < 4; y++)
		for(x = 0; x < 4; x++)
		{
			if(!a[y][x])  //a[y][x]为空
			{
				value = cnt_one(y, x); //value为坐标为(x,y)周围的空格子数
				if(value > max && old_y != y && old_x != x) //避免总在同一个地方生成新方块
				{
					*new_y = y;
					*new_x = x;
					old_x = x;
					old_y = y;
					break;
				}
			}
		}
}

int game_over()
{
	sleep(1);
	endwin();
	exit(0);
}

#include <graphics.h>      
#include <conio.h>
#include<stdio.h>
#include<windows.h>
#pragma comment(lib, "Winmm.lib")
#define Left_x 192
#define Up_y 384
#define Right_x 512
#define Down_y 448
#define CHANGE_MOUSE (m.y > Up_y && m.y < Down_y) || (m.y > Down_y + 10 && m.y < Down_y + 74) || (m.y > Down_y + 84 && m.y < Down_y + 148)

int Map[3][9][11] = {
	{
		{0,1,1,1,1,1,0,0,0,0,0},
		{0,1,0,0,0,1,1,0,0,0,0},
		{0,1,0,3,0,0,1,0,0,0,0},
		{0,1,0,0,3,0,1,1,1,1,0},
		{0,1,1,1,1,2,4,0,0,1,0},
		{0,1,0,1,0,0,4,1,0,1,0},
		{0,1,0,1,0,0,0,0,0,1,0},
		{0,1,0,1,1,1,1,1,1,1,0},
		{0,0,0,0,0,0,0,0,0,0,0},
	},
	{
		{0,1,1,1,1,1,1,1,1,1,0},
		{0,1,0,0,0,1,0,0,0,1,0},
		{0,1,0,0,3,0,0,3,0,1,0},
		{0,1,0,3,0,0,0,3,0,1,1},
		{0,1,0,0,0,2,0,0,3,0,1},
		{1,1,0,1,1,1,1,0,0,0,1},
		{1,0,4,4,0,0,0,1,0,0,1},
		{1,0,4,4,4,0,0,0,0,1,1},
		{1,1,1,1,1,1,1,1,1,1,0},
	},
	{
		{0,1,1,1,1,1,1,1,1,1,0},
		{0,1,0,0,0,1,0,0,0,1,0},
		{0,1,3,0,0,0,0,3,0,1,0},
		{0,1,0,3,0,1,0,3,0,1,1},
		{0,1,0,0,0,2,0,0,0,0,1},
		{1,1,0,1,1,1,1,0,0,0,1},
		{1,0,4,4,0,0,0,1,0,0,1},
		{1,0,4,4,0,0,0,0,0,1,1},
		{1,1,1,1,1,1,1,1,1,1,0},
	}
};//初始地图，9行11列，其中 0 代表着空方； 1 代表墙；2 代表人；
 //3 代表箱子；4 代表终点位置。

int WSAD = ' ';                                 //用户的操作
int Game_Flag = 1;								//标志变量
int Game_Level = 0;								//游戏的哪一关
int x_L1 = 64 * 3, x_L2 = 64 * 5, y1 = 64 * 10, y2 = 64 * 11;
int x_R1 = 64 * 6, x_R2 = 64 * 8;
int re_flag;									//进行撤回的标志变量

int Print_Map();
int Play_Game();
int Judge_Win();
IMAGE box, person, wall, space, end, person_end, box_end, caidan, help, Level, over;

//使鼠标指针移动到规定区域改变形状

void recall(char f, int row, int list, int re_flag);
void change_mouse_1();
void change_mouse_2();                               //鼠标移动到选项时，改变鼠标形状

//播放背景音乐
void Play_Music();
void Pass_Level();

//绘制初始界面并为用户提供选项
void Game_Start();

//用户在初始页面点击游戏帮助时显示
void Game_Help();


//加载游戏所需的图片资源
void loadResource();

void Menu_2();

int Menu_1();

void Game_over();

int main()
{
	int Flag = 1;
	initgraph(64 * 11, 64 * 12);
	Game_Start();			//关于鼠标
	Play_Music();
	loadResource();

	Menu_1();
	while (Game_Level != 3) {
		putimage(0, 64 * 8, &Level);
		Flag = Print_Map();
		while (Flag != 0) {
			Flag = Print_Map();
			if (Flag != 0)
				Play_Game();
		}
		if (Game_Level != 2)
		{
			Pass_Level();
			Menu_2();
		}
		else
		{
			Game_over();
			getch();
			exit(0);
		}
	}
	closegraph();
}

int Print_Map()
{
	int i, j;
	int x, y;
	LOGFONT f;
	gettextstyle(&f);                     // 获取当前字体设置
	f.lfHeight = 0;
	f.lfWidth = 0;
	settextstyle(&f);
	char s[] = "操作：使用WSAD键控制小人上下左右移动,R键退回上一步，按ESC键退出";
	outtextxy(40, 64 * 11.4, s);
	for (i = 0; i < 9; i++)
	{
		for (j = 0; j < 11; j++)
		{
			x = j * 64;
			y = i * 64;
			switch (Map[Game_Level][i][j])
			{
			case 0:
				putimage(x, y, &space); //空地
				break;
			case 1:
				putimage(x, y, &wall); //墙 
				break;
			case 2:
				putimage(x, y, &person); //人 
				break;
			case 3:
				putimage(x, y, &box); //箱子 
				break;
			case 4:
				putimage(x, y, &end); //终点
				break;
			case 6:
				putimage(x, y, &person_end);//人加终点
				break;
			case 7:
				putimage(x, y, &box_end);//箱子加终点
				break;
			}
		}
	}
	int Flag = Judge_Win();
	return Flag;
}

//进行小人的移动
int Play_Game()
{
	int row, list;				//行和列 
	for (int i = 0; i < 9; i++)//找到人的位置 
	{
		for (int j = 0; j < 11; j++)
		{
			if (Map[Game_Level][i][j] == 2 || Map[Game_Level][i][j] == 6)
			{
				row = i;
				list = j;
			}
		}
	}
	int f = WSAD;
	WSAD = getch();
	switch (WSAD)
	{								//上
	case 'W':
	case 'w':
		// 1.人的上面是空地；
		// 2.人的上面是终点位置；
								//3.人的上面是箱子
								//3.1.箱子的上面是空地；
								//3.2.箱子的上面是终点位置。
		if (Map[Game_Level][row - 1][list] == 0 || Map[Game_Level][row - 1][list] == 4)
		{
			re_flag = 1;
			Map[Game_Level][row][list] -= 2;
			Map[Game_Level][row - 1][list] += 2;
		}
		else if (Map[Game_Level][row - 1][list] == 3 || Map[Game_Level][row - 1][list] == 7)
		{
			if (Map[Game_Level][row - 2][list] == 0 || Map[Game_Level][row - 2][list] == 4)
			{
				re_flag = 2;
				Map[Game_Level][row][list] -= 2;
				Map[Game_Level][row - 1][list] -= 1;
				Map[Game_Level][row - 2][list] += 3;
			}
		}
		break;

			   //下 
	case 'S':
	case 's':
		if (Map[Game_Level][row + 1][list] == 0 || Map[Game_Level][row + 1][list] == 4)
		{
			re_flag = 1;
			Map[Game_Level][row][list] -= 2;
			Map[Game_Level][row + 1][list] += 2;
		}

		else if (Map[Game_Level][row + 2][list] == 0 || Map[Game_Level][row + 2][list] == 4)
		{
			if (Map[Game_Level][row + 1][list] == 3 || Map[Game_Level][row + 1][list] == 7)
			{
				re_flag = 2;
				Map[Game_Level][row][list] -= 2;
				Map[Game_Level][row + 1][list] -= 1;
				Map[Game_Level][row + 2][list] += 3;
			}
		}
		break;
		//左 
	case 'A':
	case 'a':
		if (Map[Game_Level][row][list - 1] == 0 || Map[Game_Level][row][list - 1] == 4)
		{
			re_flag = 1;
			Map[Game_Level][row][list] -= 2;
			Map[Game_Level][row][list - 1] += 2;
		}

		else if (Map[Game_Level][row][list - 2] == 0 || Map[Game_Level][row][list - 2] == 4)
		{
			if (Map[Game_Level][row][list - 1] == 3 || Map[Game_Level][row][list - 1] == 7)
			{
				re_flag = 2;
				Map[Game_Level][row][list] -= 2;
				Map[Game_Level][row][list - 1] -= 1;
				Map[Game_Level][row][list - 2] += 3;
			}
		}
		break;
		//右 
	case 'D':
	case 'd':
		if (Map[Game_Level][row][list + 1] == 0 || Map[Game_Level][row][list + 1] == 4)
		{
			re_flag = 1;
			Map[Game_Level][row][list] -= 2;
			Map[Game_Level][row][list + 1] += 2;
		}
		else if (Map[Game_Level][row][list + 2] == 0 || Map[Game_Level][row][list + 2] == 4)
		{
			if (Map[Game_Level][row][list + 1] == 3 || Map[Game_Level][row][list + 1] == 7)
			{
				re_flag = 2;
				Map[Game_Level][row][list] -= 2;
				Map[Game_Level][row][list + 1] -= 1;
				Map[Game_Level][row][list + 2] += 3;
			}
		}
		break;
	case 'R':
	case 'r':
		recall(f, row, list, re_flag);
		break;
	case 27:										//ESC键退出
		exit(0);
		break;
	}
	/*进行小人的上下左右的移动
	移动的情况：
		 1.人的下一个位置是空地；
		 2.人的下一个位置是终点位置；
		 3.人的下一个位置是箱子
			  3.1.箱子的下一个位置是空地；
			  3.2.箱子的下一个位置是终点位置。
	不移动的情况：
		 1.人的下一个位置是墙；
		 2.人的下一个位置是箱子；
			  2.1.箱子的下一个位置是墙 ；
			  2.2.箱子的下一个位置是箱子；
	*/
	//移动后，确定人的位置及胜利的条件
	return 0;
}

//判断游戏的输赢
int Judge_Win()
{
	char s[3][30] = { "恭喜你，通过第一关！","恭喜你，通过第二关！","恭喜你，通过第三关！" };
	int k = 0;
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 11; j++)
		{
			if (Map[Game_Level][i][j] == 3)
				k++;
		}
	}
	if (k == 0)
	{
		RECT r = { 0,64 * 9,64 * 11,64 * 10 };
		LOGFONT f;
		gettextstyle(&f);                     // 获取当前字体设置
		f.lfHeight = 0;
		f.lfWidth = 0;
		settextstyle(&f);
		drawtext(s[Game_Level], &r, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
	}
	return k;
}


void recall(char f, int row, int list,int re_flag)
{
	if (f == 'w' || f == 'W')
	{
		if (re_flag == 1)
		{
				Map[Game_Level][row][list] -= 2;
				Map[Game_Level][row + 1][list] += 2;
		}
		if (re_flag == 2)
		{
			Map[Game_Level][row][list] += 1;
			Map[Game_Level][row + 1][list] += 2;
			Map[Game_Level][row - 1][list] -= 3;
		}
	}
	if (f == 's' || f == 'S')
	{
		if (re_flag == 1)
		{
			Map[Game_Level][row][list] -= 2;
			Map[Game_Level][row - 1][list] += 2;
		}
		if (re_flag == 2)
		{
			Map[Game_Level][row][list] += 1;
			Map[Game_Level][row - 1][list] += 2;
			Map[Game_Level][row + 1][list] -= 3;
		}
	}
	if (f == 'a' || f == 'A')
	{
		if (re_flag == 1)
		{
			Map[Game_Level][row][list] -= 2;
			Map[Game_Level][row][list + 1] += 2;
		}
		if (re_flag == 2)
		{
			Map[Game_Level][row][list] += 1;
			Map[Game_Level][row][list + 1] += 2;
			Map[Game_Level][row][list - 1] -= 3;
		}
	}
	if (f == 'd' || f == 'D')
	{
		if (re_flag == 1)
		{
			Map[Game_Level][row][list] -= 2;
			Map[Game_Level][row][list - 1] += 2;
		}
		if (re_flag == 2)
		{
			Map[Game_Level][row][list] += 1;
			Map[Game_Level][row][list - 1] -= 3;
			Map[Game_Level][row][list + 1] += 2;
		}
	}
}

void change_mouse_1()
{
	HCURSOR hcur = LoadCursor(NULL, IDC_ARROW);		// 加载系统预置的鼠标样式
	HWND hwnd = GetHWnd();							// 获取绘图窗口句柄
	SetClassLong(hwnd, GCL_HCURSOR, (long)hcur);	// 设置窗口类的鼠标样式
}
void change_mouse_2()                               //鼠标移动到选项时，改变鼠标形状
{
	HCURSOR hcur = LoadCursor(NULL, IDC_CROSS);		// 加载系统预置的鼠标样式
	HWND hwnd = GetHWnd();							// 获取绘图窗口句柄
	SetClassLong(hwnd, GCL_HCURSOR, (long)hcur);	// 设置窗口类的鼠标样式
}
void Play_Music()
{
	mciSendString("play MP4.mp3",
		NULL,
		0,
		NULL);
}
void Pass_Level()
{
	roundrect(64 * 3, 64 * 10, 64 * 5, 64 * 11, 10, 10);
	roundrect(64 * 6, 64 * 10, 64 * 8, 64 * 11, 10, 10);
	RECT r4 = { 64 * 3,64 * 10,64 * 5,64 * 11 };
	RECT r5 = { 64 * 6,64 * 10,64 * 8,64 * 11 };
	LOGFONT f;
	gettextstyle(&f);                     // 获取当前字体设置
	f.lfHeight = 0;
	f.lfWidth = 0;
	settextstyle(&f);
	setbkmode(TRANSPARENT);
	settextcolor(BLACK);
	drawtext(_T("退出"), &r4, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
	drawtext(_T("下一关"), &r5, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
}
void Game_Start()
{
	loadimage(&caidan, "caidan.PNG", 64 * 11, 64 * 12);
	putimage(0, 0, &caidan);
	roundrect(Left_x, Up_y, Right_x, Down_y, 20, 20);
	roundrect(Left_x, Down_y + 10, Right_x, Down_y + 74, 20, 20);
	roundrect(Left_x, Down_y + 84, Right_x, Down_y + 148, 20, 20);
	RECT r1 = { Left_x, Up_y, Right_x, Down_y };
	RECT r2 = { Left_x,Down_y + 10,Right_x,Down_y + 74 };
	RECT r3 = { Left_x,Down_y + 84,Right_x,Down_y + 148 };
	LOGFONT f;
	gettextstyle(&f);                     // 获取当前字体设置
	f.lfHeight = 48;                      // 设置字体高度为 48
	strcpy(f.lfFaceName, _T("黑体"));    // 设置字体为“黑体”(高版本 VC 推荐使用 _tcscpy_s 函数)
	f.lfQuality = ANTIALIASED_QUALITY;    // 设置输出效果为抗锯齿  
	settextstyle(&f);                     // 设置字体样式
	setbkmode(TRANSPARENT);
	settextcolor(BLACK);
	drawtext(_T("开始游戏"), &r1, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
	drawtext(_T("退出游戏"), &r2, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
	drawtext(_T("游戏帮助"), &r3, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
}

void Game_Help()
{
	char Help[] = "在游戏主界面中，会出现一个小人、若干个箱子和箱子放置点。玩家需要利用WSAD(大写)键控制小人上下左右移动，并推动界面中的箱子到达指定的箱子放置点,按任意键继续............";
	loadimage(&help, "help.jpg", 64 * 11, 64 * 12);
	putimage(0, 0, &help);
	RECT R = { Left_x - 80, Up_y - 260, Right_x + 80, Down_y + 100 };
	LOGFONT f;
	gettextstyle(&f);                     // 获取当前字体设置
	f.lfHeight = 20;                      // 设置字体高度为 48
	settextcolor(BLACK);
	drawtext(Help, &R, DT_WORDBREAK | DT_LEFT);
}

//加载游戏图片
void loadResource()
{
	loadimage(&over, "over.jpg", 64 * 11, 64 * 12);
	loadimage(&Level, "Level.jpg", 64 * 11, 64 * 8);
	loadimage(&box, "box.PNG", 64, 64);
	loadimage(&person, "person.PNG", 64, 64);
	loadimage(&wall, "wall.PNG", 64, 64);
	loadimage(&space, "space.PNG", 64, 64);
	loadimage(&end, "end.PNG", 64, 64);
	loadimage(&person_end, "person_end.PNG", 64, 64);
	loadimage(&box_end, "box_end.PNG", 64, 64);
	loadimage(&caidan, "caidan.PNG", 64 * 11, 64 * 11);
}

void Menu_2()
{
	int z = 1;
	Game_Flag = 1;				//设置一个鼠标的标志变量
	MOUSEMSG m;
	m = GetMouseMsg();
	while (m.uMsg && z) {
		if (MouseHit)
		{
			m = GetMouseMsg();
			if (m.uMsg == WM_LBUTTONDOWN && Game_Flag)
			{
				if (m.x > x_L1 && m.x<x_L2 && m.y>y1 && m.y < y2)
					exit(0);
				if (m.x > x_R1 && m.x<x_R2 && m.y>y1 && m.y < y2)
				{
					z = 0;
					Game_Level++;
					Game_Flag = 0;
					change_mouse_1();
				}
			}
			if (m.uMsg == WM_MOUSEMOVE && Game_Flag)
			{
				if (m.y > y1 && m.y < y2 && ((m.x > x_L1 && m.x < x_L2) || (m.x > x_R1 && m.x < x_R2)))
				{
					change_mouse_2();
				}
				else change_mouse_1();
			}
		}
	}
}

int Menu_1()
{
	MOUSEMSG m;
	m = GetMouseMsg();
	while (m.uMsg) {
		if (MouseHit)
		{
			m = GetMouseMsg();
			if (m.uMsg == WM_MOUSEMOVE && Game_Flag)
			{
				if (m.x > Left_x && m.x < Right_x)
				{
					if (CHANGE_MOUSE)
						change_mouse_2();
					else change_mouse_1();
				}
				else if (m.x < Left_x || m.x > Right_x || m.y < Up_y || m.y > Down_y + 148)
					change_mouse_1();
			}
			if (m.uMsg == WM_LBUTTONDOWN && Game_Flag)
			{
				if (m.x > Left_x && m.x<Right_x && m.y>Up_y && m.y < Down_y)
				{
					Game_Flag = 0;
					change_mouse_1();
					return 1;
				}
				if (m.x > Left_x && m.x<Right_x && m.y>Down_y + 10 && m.y < Down_y + 74)
					exit(0);
				if (m.x > Left_x && m.x<Right_x && m.y>Down_y + 84 && m.y < Down_y + 148)
				{
					Game_Flag = 0;
					Game_Help();
					change_mouse_1();
					getch();
					FlushMouseMsgBuffer();
					Game_Start();
					Game_Flag = 1;
				}
			}
		}
	}
}
//通过三个关卡，游戏结束，输出结束词
void Game_over()
{
	putimage(0, 0, &over);
	RECT r = { 0,64 * 5,64 * 11,64 * 8 };
	drawtext("真棒，你已经通过所有关卡！欢迎再玩.按任意键结束.........", &r, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
}
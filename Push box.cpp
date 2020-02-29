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
};//��ʼ��ͼ��9��11�У����� 0 �����ſշ��� 1 ����ǽ��2 �����ˣ�
 //3 �������ӣ�4 �����յ�λ�á�

int WSAD = ' ';                                 //�û��Ĳ���
int Game_Flag = 1;								//��־����
int Game_Level = 0;								//��Ϸ����һ��
int x_L1 = 64 * 3, x_L2 = 64 * 5, y1 = 64 * 10, y2 = 64 * 11;
int x_R1 = 64 * 6, x_R2 = 64 * 8;
int re_flag;									//���г��صı�־����

int Print_Map();
int Play_Game();
int Judge_Win();
IMAGE box, person, wall, space, end, person_end, box_end, caidan, help, Level, over;

//ʹ���ָ���ƶ����涨����ı���״

void recall(char f, int row, int list, int re_flag);
void change_mouse_1();
void change_mouse_2();                               //����ƶ���ѡ��ʱ���ı������״

//���ű�������
void Play_Music();
void Pass_Level();

//���Ƴ�ʼ���沢Ϊ�û��ṩѡ��
void Game_Start();

//�û��ڳ�ʼҳ������Ϸ����ʱ��ʾ
void Game_Help();


//������Ϸ�����ͼƬ��Դ
void loadResource();

void Menu_2();

int Menu_1();

void Game_over();

int main()
{
	int Flag = 1;
	initgraph(64 * 11, 64 * 12);
	Game_Start();			//�������
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
	gettextstyle(&f);                     // ��ȡ��ǰ��������
	f.lfHeight = 0;
	f.lfWidth = 0;
	settextstyle(&f);
	char s[] = "������ʹ��WSAD������С�����������ƶ�,R���˻���һ������ESC���˳�";
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
				putimage(x, y, &space); //�յ�
				break;
			case 1:
				putimage(x, y, &wall); //ǽ 
				break;
			case 2:
				putimage(x, y, &person); //�� 
				break;
			case 3:
				putimage(x, y, &box); //���� 
				break;
			case 4:
				putimage(x, y, &end); //�յ�
				break;
			case 6:
				putimage(x, y, &person_end);//�˼��յ�
				break;
			case 7:
				putimage(x, y, &box_end);//���Ӽ��յ�
				break;
			}
		}
	}
	int Flag = Judge_Win();
	return Flag;
}

//����С�˵��ƶ�
int Play_Game()
{
	int row, list;				//�к��� 
	for (int i = 0; i < 9; i++)//�ҵ��˵�λ�� 
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
	{								//��
	case 'W':
	case 'w':
		// 1.�˵������ǿյأ�
		// 2.�˵��������յ�λ�ã�
								//3.�˵�����������
								//3.1.���ӵ������ǿյأ�
								//3.2.���ӵ��������յ�λ�á�
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

			   //�� 
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
		//�� 
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
		//�� 
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
	case 27:										//ESC���˳�
		exit(0);
		break;
	}
	/*����С�˵��������ҵ��ƶ�
	�ƶ��������
		 1.�˵���һ��λ���ǿյأ�
		 2.�˵���һ��λ�����յ�λ�ã�
		 3.�˵���һ��λ��������
			  3.1.���ӵ���һ��λ���ǿյأ�
			  3.2.���ӵ���һ��λ�����յ�λ�á�
	���ƶ��������
		 1.�˵���һ��λ����ǽ��
		 2.�˵���һ��λ�������ӣ�
			  2.1.���ӵ���һ��λ����ǽ ��
			  2.2.���ӵ���һ��λ�������ӣ�
	*/
	//�ƶ���ȷ���˵�λ�ü�ʤ��������
	return 0;
}

//�ж���Ϸ����Ӯ
int Judge_Win()
{
	char s[3][30] = { "��ϲ�㣬ͨ����һ�أ�","��ϲ�㣬ͨ���ڶ��أ�","��ϲ�㣬ͨ�������أ�" };
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
		gettextstyle(&f);                     // ��ȡ��ǰ��������
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
	HCURSOR hcur = LoadCursor(NULL, IDC_ARROW);		// ����ϵͳԤ�õ������ʽ
	HWND hwnd = GetHWnd();							// ��ȡ��ͼ���ھ��
	SetClassLong(hwnd, GCL_HCURSOR, (long)hcur);	// ���ô�����������ʽ
}
void change_mouse_2()                               //����ƶ���ѡ��ʱ���ı������״
{
	HCURSOR hcur = LoadCursor(NULL, IDC_CROSS);		// ����ϵͳԤ�õ������ʽ
	HWND hwnd = GetHWnd();							// ��ȡ��ͼ���ھ��
	SetClassLong(hwnd, GCL_HCURSOR, (long)hcur);	// ���ô�����������ʽ
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
	gettextstyle(&f);                     // ��ȡ��ǰ��������
	f.lfHeight = 0;
	f.lfWidth = 0;
	settextstyle(&f);
	setbkmode(TRANSPARENT);
	settextcolor(BLACK);
	drawtext(_T("�˳�"), &r4, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
	drawtext(_T("��һ��"), &r5, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
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
	gettextstyle(&f);                     // ��ȡ��ǰ��������
	f.lfHeight = 48;                      // ��������߶�Ϊ 48
	strcpy(f.lfFaceName, _T("����"));    // ��������Ϊ�����塱(�߰汾 VC �Ƽ�ʹ�� _tcscpy_s ����)
	f.lfQuality = ANTIALIASED_QUALITY;    // �������Ч��Ϊ�����  
	settextstyle(&f);                     // ����������ʽ
	setbkmode(TRANSPARENT);
	settextcolor(BLACK);
	drawtext(_T("��ʼ��Ϸ"), &r1, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
	drawtext(_T("�˳���Ϸ"), &r2, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
	drawtext(_T("��Ϸ����"), &r3, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
}

void Game_Help()
{
	char Help[] = "����Ϸ�������У������һ��С�ˡ����ɸ����Ӻ����ӷ��õ㡣�����Ҫ����WSAD(��д)������С�����������ƶ������ƶ������е����ӵ���ָ�������ӷ��õ�,�����������............";
	loadimage(&help, "help.jpg", 64 * 11, 64 * 12);
	putimage(0, 0, &help);
	RECT R = { Left_x - 80, Up_y - 260, Right_x + 80, Down_y + 100 };
	LOGFONT f;
	gettextstyle(&f);                     // ��ȡ��ǰ��������
	f.lfHeight = 20;                      // ��������߶�Ϊ 48
	settextcolor(BLACK);
	drawtext(Help, &R, DT_WORDBREAK | DT_LEFT);
}

//������ϷͼƬ
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
	Game_Flag = 1;				//����һ�����ı�־����
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
//ͨ�������ؿ�����Ϸ���������������
void Game_over()
{
	putimage(0, 0, &over);
	RECT r = { 0,64 * 5,64 * 11,64 * 8 };
	drawtext("��������Ѿ�ͨ�����йؿ�����ӭ����.�����������.........", &r, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
}
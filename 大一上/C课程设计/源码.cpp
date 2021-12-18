#include<stdio.h>
#include<easyx.h>
#include<conio.h>
#include<time.h>
#define ROW 46
#define COL 64
#define BLOCK 5
//枚举
enum game
{
	SPACE, WALL, SNAKE, FOOD, HEAD//空地  墙  蛇  食物
};
/************全局变量************/
int score = 0;			//分数
int level = 0;			//速度等级
DWORD t1, t2;			//定义两个时间控制移动速度
int map[ROW][COL];		//地图大小
COORD snake[1024];		//蛇			typedef struct _COORD {SHORT X;SHORT Y;} COORD, *PCOORD;
size_t SnakeSize;		//蛇的尺度		typedef unsigned int     size_t;
char SnakeDir;			//蛇移动方向
/************全局函数************/
void start();	//开始界面
void chose();	//选择是否开始
void DrawMap();	//画图
void init();	//初始化游戏数据
void addfood();	//添加食物
void ChangeDir();	//修改蛇的方向
void move();//蛇的移动
void addwall();//添加墙壁
int main()
{
	initgraph(640, 480);
	start();
	outtextxy(260, 150, L"->");		//初始默认模式一
	chose();
	init();
	while (1)
	{
		t2 = GetTickCount();
		DrawMap();
		if (_kbhit())
		{
			ChangeDir();
			move();

			t2 = GetTickCount();
			t1 = t2;
		}
		switch (level)
		{
		case 1:
			if (t2 - t1 > 200)
			{
				move();
				t1 = t2;
			}
			break;
		case 2:
			if (t2 - t1 > 100)
			{
				move();
				t1 = t2;
			}
			break;
		case 3:
			if (t2 - t1 > 50)
			{
				move();
				t1 = t2;
			}
			break;
		}
	}
	getchar();
	closegraph();
	return 0;
}
void init()
{
	srand((unsigned)time(NULL));	//随机种子
	setbkcolor(WHITE);				//设置背景颜色

	memset(map, SPACE, sizeof(map));//初始化map数组为0即SPACE
	//每一行的 第一个 和 最后一个 是墙
	for (int i = 0; i < ROW; i++)
	{
		map[i][0] = map[i][COL - 1] = WALL;
	}
	//每一列的 第二个 和 倒数第二 个是墙
	for (int j = 1; j < COL - 1; j++)
	{
		map[0][j] = map[ROW - 1][j] = WALL;
	}
	//定义蛇头和蛇的身体
	map[3][5] = HEAD;
	map[3][4] = map[3][3] = SNAKE;
	//初始化蛇
	SnakeSize = 3;	//蛇 长
	SnakeDir = 'D';	//蛇方向向右
	snake[0].X = 3;
	snake[0].Y = 5;
	snake[1].X = 3;
	snake[1].Y = 4;
	snake[2].X = 3;
	snake[2].Y = 3;
	addwall();
	addfood();
}
void start()
{
	setbkcolor(WHITE);		//设置窗口背景色为白色
	cleardevice();			//刷新屏幕

	setbkmode(TRANSPARENT);	//设置字体背景色为透明
	settextcolor(BLACK);		//设置字体颜色为黑色
	/*****************游戏规则*****************/
	outtextxy(290, 80, L"功能选择:");
	outtextxy(280, 150, L"1.低速模式");
	outtextxy(280, 200, L"2.中速模式");
	outtextxy(280, 250, L"3.高速模式");
	outtextxy(200, 280, L"数字键 1,2,3 选择模式，Enter键进入游戏");
	outtextxy(200, 300, L"字母键 W,S,A,D 方向键 上下左右 控制方向");
	outtextxy(130, 350, L"相关内容：");
	outtextxy(160, 380, L"作者：葛旭，黄弋斌");
}
void chose()
{
	switch (_getch())
	{
	case '1':
		start();
		outtextxy(260, 150, L"->");
		level = 1;
		break;
	case '2':
		start();
		outtextxy(260, 200, L"->");
		level = 2;
		break;
	case '3':
		start();
		outtextxy(260, 250, L"->");
		level = 3;
		break;
	case 13:
		return;
		break;
	}
}
void DrawMap()
{
	BeginBatchDraw();	//开始绘图
	setbkcolor(RGB(247, 238, 214));	//设置背景颜色为白色
	settextcolor(RGB(238, 0, 0));
	cleardevice();		//清屏
	WCHAR arr[10];		//保存成绩
	//WCHAR level[10];
	wsprintf(arr, L"总分：%d", score);	//将成绩格式化输出到字符串arr中 
	//wsprintf(level, L"速度等级：%d", level);
	outtextxy(0, 0, arr);				//显示成绩
	//outtextxy(150, 0, level);
	for (int y = 0; y < ROW; y++)		//y轴方向向下
	{
		for (int x = 0; x < COL; x++)	//x轴方向向右
		{
			switch (map[y][x])
			{
			case SPACE:
				break;
			case WALL:
				setlinecolor(BLACK);
				setfillcolor(RGB(238, 233, 233));	//灰色
				fillrectangle(x * 10, y * 10 + 20, x * 10 + 10, y * 10 + 30);
				break;
			case SNAKE:
				setlinecolor(RGB(255, 165, 0));		//绿色 
				setfillcolor(RGB(255, 165, 0));
				fillrectangle(x * 10, y * 10 + 20, x * 10 + 10, y * 10 + 30);
				break;
			case HEAD:
				//画蛇头
				setfillcolor(RGB(0, 0, 255));		//蓝  0 0 255
				solidrectangle(x * 10, y * 10 + 20, x * 10 + 10, y * 10 + 30);
				break;
			case FOOD:
				setfillcolor(RGB(255, 0, 0));			//红色
				solidrectangle(x * 10, y * 10 + 20, x * 10 + 10, y * 10 + 30);
				break;
			default:
				break;
			}
		}
	}
	EndBatchDraw();
}
void addfood()
{
	int row, col;
	do
	{
		row = rand() % (ROW - 1) + 1;
		col = rand() % (COL - 1) + 1;
	} while (map[row][col] != SPACE && map[row][col] != WALL);
	map[row][col] = FOOD;
}
void addwall()
{
	int row, col;
	int i = 0;
	for (i = 0; i < BLOCK; i++)
	{
		do
		{
			row = rand() % (ROW - 1) + 1;
			col = rand() % (COL - 1) + 1;
		} while (map[row][col] != SPACE && map[row][col] != WALL);
		map[row][col] = WALL;
	}
}
void ChangeDir()
{
	switch (_getch())
	{
	case'A':
	case'a':
	case 75:
		if (SnakeDir != 'D') SnakeDir = 'A';	//蛇不能后退
		break;
	case'D':
	case'd':
	case 77:
		if (SnakeDir != 'A') SnakeDir = 'D';
		break;
	case'W':
	case'w':
	case 72:
		if (SnakeDir != 'S') SnakeDir = 'W';
		break;
	case'S':
	case's':
	case 80:
		if (SnakeDir != 'W') SnakeDir = 'S';
		break;
	case 32:
		_getch();
		break;
	default:
		break;
	}
}
void move()
{
	COORD next;		//蛇头的下一个位置
	switch (SnakeDir)
	{
	case'A':
		next.X = snake[0].X;
		next.Y = snake[0].Y - 1;
		break;
	case'W':
		next.X = snake[0].X - 1;
		next.Y = snake[0].Y;
		break;
	case'D':
		next.X = snake[0].X;
		next.Y = snake[0].Y + 1;
		break;
	case'S':
		next.X = snake[0].X + 1;
		next.Y = snake[0].Y;
		break;
	default:
		break;
	}

	switch (map[next.X][next.Y])
	{
	case SPACE://直接移动
		map[snake[SnakeSize - 1].X][snake[SnakeSize - 1].Y] = SPACE;//地图蛇尾所在地置空
		for (int i = SnakeSize - 1; i > 0; i--)						//蛇尾到蛇头整体移动一位
		{
			snake[i] = snake[i - 1];
		}
		map[snake[0].X][snake[0].Y] = SNAKE;						//蛇头置 蛇
		snake[0] = next;											//将下一个位置赋值给蛇头
		map[snake[0].X][snake[0].Y] = HEAD;							//设置头
		break;
	case WALL:
		MessageBox(GetHWnd(), L"游戏结束", L"SORRY", MB_OK);
		outtextxy(320, 200, L"是否要结束？");
		outtextxy(320, 280, L"结束按N/n");
		if (_getch() == 'N' || _getch() == 'n')
		{
			exit(0);
		}
		break;
	case SNAKE:
		MessageBox(GetHWnd(), L"游戏结束", L"SORRY", MB_OK);
		outtextxy(320, 200, L"是否要结束？");
		outtextxy(320, 280, L"结束按N/n");
		if (_getch() == 'N' || _getch() == 'n')
		{
			exit(0);
		}
		break;
	case FOOD://食物				蛇尾不变
		for (int i = SnakeSize; i > 0; i--)							//蛇尾到蛇头整体移动一位
		{
			snake[i] = snake[i - 1];
		}
		map[snake[0].X][snake[0].Y] = SNAKE;						//蛇头 置 蛇
		snake[0] = next;											//将下一个位置赋值给蛇头
		score++;			//分数加一				
		(SnakeSize)++;		//蛇尺度加一
		map[snake[0].X][snake[0].Y] = HEAD;							//地图上重置蛇头
		addfood();
		break;
	default:break;
	}
}

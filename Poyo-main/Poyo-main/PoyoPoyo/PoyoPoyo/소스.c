/*헤더파일*/
#include <stdio.h>
#include <conio.h>
#include <Windows.h>
#include <stdlib.h>
#include <time.h>

/*상수*/
#define AREA_ROW 14 //0번째 줄 위 빈 공간, 13번째 줄 바닥 블록
#define AREA_COL 8 //양 끝 벽
//실제 보이는 게임 공간은 12*6 -> [1~12][1~6]

// 상하좌우 상수값 설정
#define UP 72
#define DOWN 80
#define LEFT 75
#define RIGHT 77

/*변수*/
int testArea[AREA_ROW][AREA_COL] = { 0, };
int nowArea[AREA_ROW][AREA_COL] = { 0, };
int poyo[2][2] = { 0 };
int testpoyo[2][2] = { 0 };
int chaincnt = 0;


enum {
	BLACK,
	DARK_BLUE,
	DARK_GREEN,
	DARK_SKYBLUE,
	DARK_RED,
	DARK_VOILET,
	DARK_YELLOW,
	GRAY,
	DARK_GRAY,
	BLUE,
	GREEN,
	SKYBLUE,
	RED,
	VIOLET,
	YELLOW,
	WHITE,
};

/*함수 원형 선언*/
void createArea();	//맵 생성?
void printArea();	//맵 출력
void spawnPoyo();
void downPoyo(int x,int y);
void putPoyo(int x, int y);
void clearArea(int x, int y);
int isCrash(int x, int y);
void saveArea();
void returnArea();
int setColor(int n);
int input();
int moveHorizontal(int ans,int x, int y);
void downAll();
void rotatePoyo(int x,int y);
void returnPoyo();
int checkPoyo(int x,int y,int cnt);
void clearPoyo(int x, int y,int v);
int chainPoyo();
int isGameOver();

int main()
{
	srand(time(NULL));
	createArea();
	printArea();
	spawnPoyo();
}
void createArea()
{
	for (int i = 0; i < AREA_COL; i++)
	{
		testArea[13][i] = 10;
	}
	for (int i = 0; i < AREA_ROW; i++)
	{
		testArea[i][0] = testArea[i][7] = 10;
	}
}
void printArea()
{
	system("cls");
	/*for (int i = 1; i < AREA_ROW-1; i++)
	{
		for (int j = 1; j < AREA_COL-1; j++)
		{
			printf("%d ", testArea[i][j]);
		}
		printf("\n");
	}*/
	for (int i = 1; i < AREA_ROW - 1; i++)
	{
		for (int j = 1; j < AREA_COL - 1; j++)
		{
			switch (nowArea[i][j])
			{
			case 0:
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE);
				printf("□");
				break;
			default:
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), setColor(nowArea[i][j]));
				printf("●");
				break;
			}
		}
		printf("\n");
	}
	if (chaincnt != 0)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), DARK_RED);
		printf("%d  chain!!", chaincnt);
	}
}
void spawnPoyo()
{
	downAll();
	saveArea();
	printArea();
	chaincnt = 0;
	while (1)
	{
		if (chainPoyo() == 0) {
			break;
		}
		Sleep(500);
		chaincnt++;

	}
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			poyo[i][j] = 0;
		}
	}
	poyo[0][0] = rand()%4+1;
	poyo[0][1] = rand()%4+1;
	downPoyo(3, 0);
}

void downPoyo(int x,int y)
{
	putPoyo(x, y);
	if (isCrash(x, y))
	{
		if (isGameOver())
		{
			return;
		}
		spawnPoyo();
	}
	printArea();
	clearArea(x, y);
	time_t start_time = time(NULL);
	while (1)
	{
		int ans = input();
		switch (ans)
		{
		case -1:
		case 1:
			clearArea(x,y);
			x+=moveHorizontal(ans,x,y);
			putPoyo(x, y);
			printArea();
			clearArea(x, y);
			break;
		case 2:
			clearArea(x, y);
			rotatePoyo(x,y);
			putPoyo(x, y);
			if (isCrash(x, y))
			{
				returnPoyo();
			}
			printArea();
			clearArea(x, y);
			break;
		case 3:
			return downPoyo(x, y + 1);
			

		}
		time_t now_time = time(NULL);
		if (now_time - start_time >= 1)     //1초동안
		{
			break;
		}
	}
	return downPoyo(x, y + 1);
}
void putPoyo(int x, int y)
{
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			testArea[i + y][j + x] += poyo[i][j];
		}
	}
}
void clearArea(int x, int y)
{
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			if (poyo[i][j] == 0)
				continue;
			testArea[i + y][j + x] = 0;
		}
	}
}
int isCrash(int x, int y)
{
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			if (poyo[i][j] == 0)
				continue;
			if (poyo[i][j] != testArea[i + y][j + x])	//충돌했을 때 1 반환
			{
				returnArea();
				return 1;
			}
		}
	}
	saveArea(); 
	return 0;
}
void saveArea()
{
	for (int i = 0; i < AREA_ROW; i++)
	{
		for (int j = 0; j < AREA_COL; j++)
		{
			nowArea[i][j] = testArea[i][j];
		}
	}
}
void returnArea()
{
	for (int i = 0; i < AREA_ROW; i++)
	{
		for (int j = 0; j < AREA_COL; j++)
		{
			testArea[i][j] = nowArea[i][j];
		}
	}
}

int setColor(int n)
{
	switch (n)
	{
	case 1:
		return RED;
	case 2:
		return GREEN;
	case 3:
		return YELLOW;
	case 4:
		return VIOLET;
	}
}

int input()
{
	if (_kbhit()) {        //키보드 입력 확인 (true / false)
		char c = _getch();      // 방향키 입력시 224 00이 들어오게 되기에 앞에 있는 값 224를 없앰
		if (c == -32) {    // -32로 입력되면
			c = _getch();  // 새로 입력값을 판별하여 상하좌우 출력
			switch (c) {
			case LEFT:
				return -1;
			case RIGHT:
				return 1;
			case UP:
				return 2;
			case DOWN:
				return 3;
			}
		}
	}
}

int moveHorizontal(int ans,int x,int y)
{
	putPoyo(x + ans, y);
	if (isCrash(x + ans, y))
	{
		clearArea(x, y);
		return 0;
	}
	clearArea(x + ans, y);
	printArea();
	return ans;
}

void downAll()
{
	for (int i = 1; i < AREA_ROW - 1; i++)
	{
		for (int j = 1; j < AREA_COL - 1; j++)
		{
			if (testArea[i + 1][j] == 0&&testArea[i][j]!=0)
			{
				int temp = testArea[i][j];
				testArea[i][j] = testArea[i + 1][j], testArea[i + 1][j] = temp;
			}
		}
	}
}

void rotatePoyo(int x,int y)
{
	int temp[2][2];
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			temp[j][2 - 1 - i]=testpoyo[i][j] = poyo[i][j];
		}
	}
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			poyo[i][j] = temp[i][j];
		}
	}
}

void returnPoyo()
{
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			poyo[i][j] = testpoyo[i][j];
		}
	}
}
int checkPoyo(int x,int y,int cnt)
{
	testArea[y][x] *= -1;
	cnt++;
	if (testArea[y][x]*-1 == testArea[y - 1][x])	//위쪽 칸 확인
	{
		cnt+=checkPoyo(x, y - 1,0);
	}
	if (testArea[y][x]*-1 == testArea[y + 1][x])	//아래쪽 칸 확인
	{
		cnt+= checkPoyo(x, y + 1, 0);
	}
	if (testArea[y][x]*-1 == testArea[y][x-1])	//왼쪽 칸 확인
	{
		cnt += checkPoyo(x-1, y, 0);
	}
	if (testArea[y][x]*-1 == testArea[y][x+1])	//오른쪽 칸 확인
	{
		cnt += checkPoyo(x+1, y, 0);
	}
	return cnt;

}

void clearPoyo(int x, int y,int v)
{
	testArea[y][x] = 0;
	if (v == testArea[y - 1][x])	//위쪽 칸 확인
	{
		clearPoyo(x, y - 1,v);
	}
	if (v == testArea[y + 1][x])	//아래쪽 칸 확인
	{
		clearPoyo(x, y + 1,v);
	}
	if (v == testArea[y][x-1])	//왼쪽 칸 확인
	{
		clearPoyo(x - 1, y,v);
	}
	if (v == testArea[y][x+1])	//오른쪽 칸 확인
	{;
		clearPoyo(x + 1, y,v);
	}
}

int chainPoyo()
{

	int chk = 0;
	for (int i = 1; i < AREA_ROW - 1; i++)
	{
		for (int j = 1; j < AREA_COL - 1; j++)
		{
			if (testArea[i][j] > 0)
			{
				if (checkPoyo(j, i, 0) >= 4)
				{
					chk = 1;
					clearPoyo(j, i, testArea[i][j]);
					printArea();
				}
			}
		}
	}
	printArea();
	for (int i = 1; i < AREA_ROW - 1; i++)
	{
		for (int j = 1; j < AREA_COL - 1; j++)
		{
			if (testArea[i][j] < 0)
				testArea[i][j] *= -1;
		}
	}
	for (int i = 0; i < AREA_ROW - 3; i++)
	{
		downAll();
	}
	saveArea();
	printArea();
	return chk;
}

int isGameOver()
{
	if (testArea[1][3] != 0 || testArea[1][4] != 0)
	{
		return 1;
	}
	return 0;
}
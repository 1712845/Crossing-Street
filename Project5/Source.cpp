#pragma warning(disable:4996)
#include<conio.h>
#include<iostream>
#include<Windows.h>
#include<thread>
#include<time.h>
#include<fstream>
using namespace std;
HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
void FixConsoleWidow()
{
	HWND consolewindow = GetConsoleWindow();
	LONG style = GetWindowLong(consolewindow, GWL_STYLE);
	style = style &~(WS_MAXIMIZEBOX)&~(WS_THICKFRAME);
	SetWindowLong(consolewindow, GWL_STYLE, style);
}
void GotoXY(int x, int y)
{
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}
#define MAX_CAR 17
#define MAX_CAR_LENGTH 5
#define MAX_SPEED 3
POINT **X; // Mang chua xe
POINT Y; // Nhan vat
int cnt = 0;  // bien ho tro tang toc xe
int MOVING;  // huong di chuyen cua nhan vat
int SPEED; // toc do xe chay (level)
int HEIGH_CONSOLE = 20, WIDTH_CONSOLE = 70; // chieu cao va chieu rong console
bool STATE; // trang thai song chet cua nhan vat
int score = 0; // điểm
int k[70]; // lưu tọa độ nhân vật trước đó
int t = 0; //Lưu điểm
void doc(char*t)
{
	fstream p(t, ios::in);
	if (p.fail())
	{
		cout << "Khong tin dc tep" << endl;
	}
	else
	{
		while (!p.eof())
		{
			char temp[255];
			p.getline(temp, 255);
			cout << temp << endl;
		}
	}
	p.close();
}
void resetdata()
{
	
	MOVING = 'D'; // ban dau cho nv di chuyen sang phai
	score = 0;
	SPEED = 1; // toc do luc dau
	Y = { 35, 19 }; // vi tri cua nguoi
	score = 0; // khởi tạo điểm
	k[70] = { 0 }; // Gán vị trí bằng không
	// tạo mảng xe 
	if (X == NULL)
	{
		X = (POINT**)malloc(MAX_CAR * sizeof(POINT*));
		for (int i = 0; i < MAX_CAR; i++)
		{
			*(X + i) = (POINT*)malloc(MAX_CAR_LENGTH * sizeof(POINT));

		}
		for (int i = 0; i < MAX_CAR; i++)
		{
			int temp = (rand() % (WIDTH_CONSOLE - MAX_CAR_LENGTH) + 1);
			for (int j = 0; j < MAX_CAR_LENGTH; j++)
			{
				X[i][j].x = temp + j;
				X[i][j].y = 2 + i;
			}
		}
	}
}
void resetdata1()
{
	SPEED = 1; // toc do luc dau
	Y = { 35, 19 }; // vi tri cua nguoi
	score = 0;
	k[70] = { 0 }; // Gán vị trí bằng không
				   // tạo mảng xe 
	if (X == NULL)
	{
		X = (POINT**)malloc(MAX_CAR * sizeof(POINT*));
		for (int i = 0; i < MAX_CAR; i++)
		{
			*(X + i) = (POINT*)malloc(MAX_CAR_LENGTH * sizeof(POINT));

		}
		for (int i = 0; i < MAX_CAR; i++)
		{
			int temp = (rand() % (WIDTH_CONSOLE - MAX_CAR_LENGTH) + 1);
			for (int j = 0; j < MAX_CAR_LENGTH; j++)
			{
				X[i][j].x = temp + j;
				X[i][j].y = 2 + i;
			}
		}
	}
}
void vekhung(int x, int y, int width, int height, int curposx = 0, int curposy = 0)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8);
	GotoXY(x, y); cout << char(219);
	for (int i = 1; i < width; i++) { cout << char(219); }
	cout << char(219);
	GotoXY(x, height + y); cout << char(219);
	for (int i = 1; i < width; i++) { cout << char(219); }
	cout << char(219);
	for (int i = y + 1; i < height + y; i++)
	{
		GotoXY(x, i); cout << char(219);
		GotoXY(x + width, i); cout << char(219);
	}
	GotoXY(curposx, curposy);
}
void menu()
{
	system("cls");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
	GotoXY(55, 12);
	cout << "MENU";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
	GotoXY(47, 15);
	cout << "ANY KEY TO START GAME";
	GotoXY(47, 18);
	cout << "PRESS TWICE T TO LOAD GAME";
	GotoXY(47, 21);
	cout << "PRESS G TO SEE GUIDES";
	GotoXY(47, 24);
	cout << "PRESS ESC TO EXIT";
	GotoXY(80, 30);
	cout << "17CTT7-1712845-NGUYEN NGOC TRUNG";
	srand(time(NULL));
	while (1)
	{
		if (kbhit()) break;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8 + rand() % 8);
		GotoXY(0, 0); doc("ascii.txt");
		Sleep(50);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8);
	}
}
void Startgame()
{
	system("cls");
	resetdata();
	vekhung(0, 0, WIDTH_CONSOLE, HEIGH_CONSOLE);
	GotoXY(30, 22); cout << "Score:" << score;
	STATE = true;
}
void Startgame1()
{
	system("cls");
	vekhung(0, 0, WIDTH_CONSOLE, HEIGH_CONSOLE);
	GotoXY(30, 22); cout << "Score:" << score;
	STATE = true;
}
void giaiphongxe() //xoa xe
{
	for (int i = 0; i < MAX_CAR; i++)
	{
		free(*(X + i));
	}
	free(X);
}
void exitgame(HANDLE t) //thoat game
{
	system("cls");
	TerminateThread(t, 0);
	giaiphongxe();
	exit(0);
}
void pausegame(HANDLE t)
{
	SuspendThread(t);
}
void processdead()
{
	STATE = 0;
	// tao hieu ung
	for (int i = 0; i <75; i++)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8 + rand() % 8);
		GotoXY(Y.x, Y.y);
		cout << "X";
		Sleep(i);
	}
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
	GotoXY(0, HEIGH_CONSOLE + 2);
	cout << "DEAD, TYLE Y TO CONTINUE OR ANYKEY TO EXIT";
	for (int i = 0; i < WIDTH_CONSOLE; i++)
	{
		k[i] = 0;
	}
}
void processfinish(POINT &p) // ham xu li khi bang qua duong thanh cong
{
	k[p.x] = 1;
	score++;
	GotoXY(30, 22); cout << "Score:" << score;
	SPEED == MAX_SPEED ? SPEED = 1 : SPEED++;
	p = { 35, 19 }; // vi tri van dau cua nv
	MOVING = 'D'; // ban dau cho nguoi choi di chuyen sang hai
}
void drawcars(char*s) // vẽ xe
{
	for (int i = 0; i < MAX_CAR; i++)
	{
		for (int j = 0; j < MAX_CAR_LENGTH; j++)
		{
			GotoXY(X[i][j].x, X[i][j].y);
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 13);
			cout << s;
		}
	}
}
void drawsticker(const POINT &p, char *s)
{
	GotoXY(p.x, p.y);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
	cout << s;
}
bool ktvacham(const POINT&p, int d)
{
	if(d == 19) return false;
	if (d == 1)
		if (k[p.x] == 1) return true; 
			else 
		return false;
	for (int i = 0; i < MAX_CAR_LENGTH; i++)
	{
		if (p.x == X[d - 2][i].x && p.y == X[d - 2][i].y)
			return true;
	}
	return false;
}
void lightleft(int color)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),color);
	for (int i = 0+1; i < HEIGH_CONSOLE+0; i++)
	{
		GotoXY(0, i); cout << char(219);
	}
}
void lightright(int color)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
	for (int i = 0 + 1; i < HEIGH_CONSOLE + 0; i++)
	{
		GotoXY(0 + WIDTH_CONSOLE, i); cout << char(219);
	}
}
void movecarsright()
{
	for (int i = 1; i < MAX_CAR; i += 2)
	{
		cnt = 0;
		do
		{
			cnt++;
			for (int j = 0; j < MAX_CAR_LENGTH - 1; j++)
			{
				X[i][j] = X[i][j + 1];
			}
			(X[i][MAX_CAR_LENGTH - 1].x + 1 == WIDTH_CONSOLE) ? X[i][MAX_CAR_LENGTH - 1].x = 1 : X[i][MAX_CAR_LENGTH - 1].x++;  // check xe có đụng màn hình không
		} while (cnt < SPEED);
	}
}
void movecarsleft()
{
	for (int i = 0; i < MAX_CAR; i += 2)
	{
		cnt = 0;
		do
		{
			cnt++;
			for (int j = MAX_CAR_LENGTH - 1; j > 0; j--)
			{
				X[i][j] = X[i][j - 1];
			}
			(X[i][0].x - 1 == 0) ? X[i][0].x = WIDTH_CONSOLE - 1 : X[i][0].x--; //kiểm tra xem xe có đụng màn hình không
		} while (cnt < SPEED);
	}
}
void erasecars()
{
	for (int i = 0; i < MAX_CAR; i += 2)
	{
		cnt = 0;
		do
		{
			GotoXY(X[i][MAX_CAR_LENGTH - 1 - cnt].x, X[i][MAX_CAR_LENGTH - 1 - cnt].y);
			cout << " ";
			cnt++;
		} while (cnt < SPEED);

	}
	for (int i = 1; i < MAX_CAR; i += 2)
	{
		cnt = 0;
		do
		{
			GotoXY(X[i][0 + cnt].x, X[i][0 + cnt].y);
			cout << " ";
			cnt++;
		} while (cnt < SPEED);
	}
}
void moveright()
{
	if (Y.x < WIDTH_CONSOLE - 1)
	{
		drawsticker(Y, " ");
		Y.x++;
		drawsticker(Y, "Y");
	}
}
void moveleft()
{
	if (Y.x > 1)
	{
		drawsticker(Y, " ");
		Y.x--;
		drawsticker(Y, "Y");
	}
}
void movedown()
{
	if (Y.y < HEIGH_CONSOLE - 1)
	{
		drawsticker(Y, " ");
		Y.y++;
		drawsticker(Y, "Y");
	}
}
void moveup()
{
	if (Y.y > 1)
	{
		drawsticker(Y, " ");
		Y.y--;
		drawsticker(Y, "Y");
	}
}
void movecars()
{
	srand(time(NULL));
	if (((int)(clock() / CLOCKS_PER_SEC) + 1) % (int)5)
	{
		lightleft(10);
		lightright(10);
		movecarsleft();
		movecarsright();
	}
	else if (((int)(clock() / CLOCKS_PER_SEC) + 1) % (int)10)
	{
		movecarsleft(); lightright(12);
	}
	else
	if (((int)(clock() / CLOCKS_PER_SEC) + 1) % (int)11)
	{
		movecarsright(); lightleft(12);
	}
}
void SubThread() //xu li di chuyen
{
	while (1)
	{
		if (STATE) //Nếu người vẫn còn sống
		{
			switch (MOVING) //Kiểm tra biến moving
			{
			case 'A':
				moveleft();
				break;
			case 'D':
				moveright();
				break;
			case 'W':
				moveup();
				break;
			case 'S':
				movedown();
				break;
			}
			MOVING = ' ';// Tạm khóa không cho di chuyển, chờ nhận phím từ hàm main
			erasecars();
			movecars();
			drawcars("0");
			if (ktvacham(Y, Y.y))
			{
				processdead(); // Kiểm tra xe có đụng không
			}
			else
				if (Y.y == 1)
				processfinish(Y); // Kiểm tra xem về đích chưa
			Sleep(150);//Hàm ngủ theo tốc độ SPEED
		}
	}
}
void guides()
{
		system("cls");
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
		doc("guides.txt");
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
		GotoXY(5, 12); cout << "Move Right : D ";
		GotoXY(5, 13); cout << "Move Left : A ";
		GotoXY(5, 14); cout << "Move Up : W ";
		GotoXY(5, 15); cout << "Turn Down : D";
		GotoXY(5, 16); cout << "Pause : P";
		GotoXY(5, 17); cout << "Load: T";
		GotoXY(5, 18); cout << "StartGame : Press Twice Y || Press Twice Anykey";
		GotoXY(5, 19); cout << "Save game : L";
		getch();
}
void savegame()
{
	char str[255];
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
	GotoXY(75, 0); cout << "Your name :";
	cin >> str;
	strcat(str, ".txt");
	fstream p(str,ios::out);
	for (int i = 0; i < WIDTH_CONSOLE; i++)
	{
		p << k[i]<<" ";   // Ghi lại vị trí y hiện tại
		if (k[i] == 1) score++;
	}
	p << endl;
	p << Y.x << " " << Y.y <<endl;
	for (int i = 0; i < MAX_CAR; i++)
		for (int j = 0; j < MAX_CAR_LENGTH; j++)
		{
			p << X[i][j].x << " " << X[i][j].y << endl;
		}
	p.close();
}
void loadread()
{
	char str[255];
	system("cls");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
	GotoXY(75, 2); cout << "Load :";
	cin >> str;
	strcat(str, ".txt");
	fstream p(str, ios::in);
	if (p.fail())
	{
		cout << "This file has not been saved" << endl;
	}
	else
	{
		X = (POINT**)malloc(MAX_CAR * sizeof(POINT*));
		for (int i = 0; i < MAX_CAR; i++)
		{
			*(X + i) = (POINT*)malloc(MAX_CAR_LENGTH * sizeof(POINT));

		}
		for (int i = 0; i < WIDTH_CONSOLE; i++)
		{
			p >> k[i];   // Ghi lại vị trí y hiện tại đã về đích
			if (k[i] == 1) { score++; t = score; }
		}
		p >> Y.x >> Y.y;
		for (int i = 0; i < MAX_CAR; i++)
			for (int j = 0; j < MAX_CAR_LENGTH; j++)
			{
				p >> X[i][j].x>> X[i][j].y;
			}
	}
	MOVING = ' ';
	STATE = 1;
	SPEED = t+1;
	p.close();

}
void loadwrite()
{
	drawsticker(Y, "Y");
	for (int i = 0; i < WIDTH_CONSOLE; i++)
	{
		if (k[i] == 1) {
			POINT temp = {i,1};
			drawsticker(temp, "Y");
		}
	}
}
void loadgame()
{
	loadread();
	Startgame1();
	loadwrite();
}
void main()
{
	int temp;
	FixConsoleWidow();
	srand(time(NULL));
	thread t1(SubThread);
	menu();
	int teem = toupper(getch());
	if (teem == 27) { exit(0); }
	else {
		if (teem == 'G') guides();
		else
		{
			if (teem != 'T')
			{
				Startgame();
			}
		}
	}
	while (1)
	{
		if (score != 3) {
			{
				temp = toupper(getch());
				if (temp == 'T')
				{
					resetdata1();
					pausegame(t1.native_handle());
					loadgame();
					pausegame(t1.native_handle());
					if (temp == 'D' || temp == 'A' || temp == 'W' || temp == 'S') MOVING = temp;
				}
				if (STATE == 1)
				{
					if (temp == 27)
					{
						exitgame(t1.native_handle());
					}
					else
						if (temp == 'L')
						{
							pausegame(t1.native_handle());
							savegame();
						}
						else if (temp == 'P') pausegame(t1.native_handle());
						else
						{
							ResumeThread((HANDLE)t1.native_handle());
							if (temp == 'D' || temp == 'A' || temp == 'W' || temp == 'S') MOVING = temp;
						}
				}
				else
				{
					if (temp == 'Y' || teem == 'G') Startgame();
					else
						if (temp == 'L')
						{
							pausegame(t1.native_handle());
							savegame();
						}
						else
						{
							exitgame(t1.native_handle());
						}
				}
			}
		}
		else
		{
			pausegame(t1.native_handle());
			GotoXY(0, 21);
			cout << "YOU WIN PRESS Y TO CONTINUE OR ANYKEY TO EXIT";
			STATE = 0;
			while (1)
			{
				temp = toupper(getch());
				if (temp == 'T')
				{
					resetdata1();
					pausegame(t1.native_handle());
					loadgame();
					pausegame(t1.native_handle());
					if (temp == 'D' || temp == 'A' || temp == 'W' || temp == 'S') MOVING = temp;
				}
				if (STATE == 1)
				{
					if (temp == 27)
					{
						exitgame(t1.native_handle());
					}
					else
						if (temp == 'L')
						{
							pausegame(t1.native_handle());
							savegame();
						}
						else if (temp == 'P') pausegame(t1.native_handle());
						else
						{
							ResumeThread((HANDLE)t1.native_handle());
							if (temp == 'D' || temp == 'A' || temp == 'W' || temp == 'S') MOVING = temp;
						}
				}
				else
				{
					if (temp == 'Y' || teem == 'G') Startgame();
					else
						if (temp == 'L')
						{
							pausegame(t1.native_handle());
							savegame();
						}
						else
						{
							exitgame(t1.native_handle());
						}
				}
			}
		}
	}
		getch();
}
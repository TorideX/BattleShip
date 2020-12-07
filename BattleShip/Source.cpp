#include <iostream>
#include <ctime>
#include <Windows.h>
#include <string>
#include <conio.h>
#include <bitset>

using namespace std;

struct CursorPosition {
	int X;
	int Y;
	bool back = false;
};
struct CleverCPU {

	int  x;
	int  y;
	char direction = '-';   //  l - left, r - right, u - up, d - down
};
struct CPU_Count_Helper_Struct {

	int rx = 1;
	int lx = 1;
	int ux = 1;
	int dx = 1;
};
struct Ship_Struct {

	short A_ship = 0;   // first 8 - Player,  Second 8 - CPU
	short B_ship = 0;   // ...
	short C_ship = 0;   // ...
	short D_ship = 0;   // ...
};
struct Variable_Struct {

	char      PlayerGame[8][8];
	char      OpponentGame[8][8];
	char      CPU[8][8];
	int	      PlayerScore = 0;
	int	      CPUScore = 0;
	int       WinningScore = 14;    // 14 Ship Coordinate
	char      EmptyChar = 249;
	bool      PreviousCPUAttack = false;  // if CPU found any ship in previous move
};

void SetShipCount(Ship_Struct& Ship)
{
	Ship.A_ship |= 5;
	Ship.A_ship <<= 8;
	Ship.A_ship |= 5;

	Ship.B_ship |= 4;
	Ship.B_ship <<= 8;
	Ship.B_ship |= 4;

	Ship.C_ship |= 3;
	Ship.C_ship <<= 8;
	Ship.C_ship |= 3;

	Ship.D_ship |= 2;
	Ship.D_ship <<= 8;
	Ship.D_ship |= 2;
}
void GetDirection(Variable_Struct& Variable, CleverCPU& LastAttack, int x, int y)
{
	short random;    //    0 - Left,  1 - Right,  2 - Up,  3 - Down
	srand(time(NULL));

Get_Rand_Again:
	random = rand() % 4;

	// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	if ((random == 0 && x == 0) || (random == 0 && Variable.PlayerGame[y][x - 1] == char(176)) || (random == 0 && Variable.PlayerGame[y][x - 1] == char(254)))
		goto Get_Rand_Again;
	else
		LastAttack.direction = 'l';
	// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	if ((random == 1 && x == 7) || (random == 1 && Variable.PlayerGame[y][x + 1] == char(176)) || (random == 1 && Variable.PlayerGame[y][x + 1] == char(254)))
		goto Get_Rand_Again;
	else
		LastAttack.direction = 'r';
	// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	if ((random == 2 && y == 0) || (random == 2 && Variable.PlayerGame[y - 1][x] == char(176)) || (random == 2 && Variable.PlayerGame[y - 1][x] == char(254)))
		goto Get_Rand_Again;
	else
		LastAttack.direction = 'u';
	// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	if ((random == 3 && y == 7) || (random == 3 && Variable.PlayerGame[y + 1][x] == char(176)) || (random == 3 && Variable.PlayerGame[y + 1][x] == char(254)))
		goto Get_Rand_Again;
	else
		LastAttack.direction = 'd';

}
void ChangeShipCount(Ship_Struct& Ship, char ShipName, bool Player_or_CPU /* True - Player, False - CPU */)
{
	int numCPU;
	int numPlayer;

	switch (ShipName)
	{
	case 'A':
	{
		if (Player_or_CPU == false)  // CPU
		{
			numCPU = Ship.A_ship >> 8;
			numPlayer = Ship.A_ship & 255;

			Ship.A_ship = numCPU - 1;
			Ship.A_ship <<= 8;
			Ship.A_ship |= numPlayer;
		}
		else   // Player
		{
			numCPU = Ship.A_ship >> 8;
			numPlayer = Ship.A_ship & 255;

			Ship.A_ship = numCPU;
			Ship.A_ship <<= 8;
			Ship.A_ship |= numPlayer - 1;
		}
		break;
	}
	case 'B':
	{
		if (Player_or_CPU == false)  // CPU
		{
			numCPU = Ship.B_ship >> 8;
			numPlayer = Ship.B_ship & 255;

			Ship.B_ship = numCPU - 1;
			Ship.B_ship <<= 8;
			Ship.B_ship |= numPlayer;
		}
		else   // Player
		{
			numCPU = Ship.B_ship >> 8;
			numPlayer = Ship.B_ship & 255;

			Ship.B_ship = numCPU;
			Ship.B_ship <<= 8;
			Ship.B_ship |= numPlayer - 1;
		}
		break;
	}
	case 'C':
	{
		if (Player_or_CPU == false)  // CPU
		{
			numCPU = Ship.C_ship >> 8;
			numPlayer = Ship.C_ship & 255;

			Ship.C_ship = numCPU - 1;
			Ship.C_ship <<= 8;
			Ship.C_ship |= numPlayer;
		}
		else   // Player
		{
			numCPU = Ship.C_ship >> 8;
			numPlayer = Ship.C_ship & 255;

			Ship.C_ship = numCPU;
			Ship.C_ship <<= 8;
			Ship.C_ship |= numPlayer - 1;
		}
		break;
	}
	case 'D':
	{
		if (Player_or_CPU == false)  // CPU
		{
			numCPU = Ship.D_ship >> 8;
			numPlayer = Ship.D_ship & 255;

			Ship.D_ship = numCPU - 1;
			Ship.D_ship <<= 8;
			Ship.D_ship |= numPlayer;
		}
		else   // Player
		{
			numCPU = Ship.D_ship >> 8;
			numPlayer = Ship.D_ship & 255;

			Ship.D_ship = numCPU;
			Ship.D_ship <<= 8;
			Ship.D_ship |= numPlayer - 1;
		}
		break;
	}
	}
}
void GotoXY(int x, int y)
{
	COORD c;
	c.X = x;
	c.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}
void Game_Refresh(Variable_Struct& Variable)
{
	for (int i = 0; i < 64; i++)
	{
		Variable.PlayerGame[0][i] = Variable.EmptyChar;
		Variable.OpponentGame[0][i] = Variable.EmptyChar;
		Variable.CPU[0][i] = Variable.EmptyChar;
	}
}
void AsciiBoat()
{
	cout << R"(

				      ~ ~				                                        ~ ~
		                  __/___            		                                           __/___            
		           _____/______|           		                                     _____/______|           
		   _______/_____\_______\_____     		                             _______/_____\_______\_____     
	  	   \              < < <       |    		                            \              < < <       |    
		 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~		                          ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ )" << endl;
}
void Draw(Variable_Struct& Variable)
{
	AsciiBoat();
	int Player_t = -1;
	int Opponent_t = -1;
	int CharLetters = 65;

	cout << "\n\n\n";
	cout << "\t\t1    2    3    4    5    6    7    8	\t\t\t\t1    2    3    4    5    6    7    8	" << endl;
	cout << "\t    _____________________________________________ \t\t\t    _____________________________________________     " << endl;
	cout << "\t   | ___________________________________________ |\t\t\t   | ___________________________________________ |    " << endl;
	cout << "\t   ||                                           ||\t\t\t   ||                                           ||    " << endl;

	for (int i = 0; i < 8; i++)
	{
		cout << "\t " << char(CharLetters + i) << " ||   ";
		for (int j = 0; j < 8; j++)
		{
			cout << Variable.PlayerGame[0][++Player_t] << "    ";
		}
		cout << "||\t\t\t " << char(CharLetters + i) << " ||   ";
		for (int j = 0; j < 8; j++)
		{

			cout << Variable.OpponentGame[0][++Opponent_t] << "    ";
		}
		cout << "||" << endl;
		cout << "\t   ||\t\t\t\t\t\t||\t\t\t   ||\t\t\t\t\t\t||" << endl;
	}
	cout << "\t   ||___________________________________________||\t\t\t   ||___________________________________________||         " << endl;
	cout << "\t   |_____________________________________________|\t\t\t   |_____________________________________________|       " << endl;
}
void Set_Boats(Variable_Struct& Variable, int num)    // EDIT NEEDED
{
	int key;
	key = _getch();

	Game_Refresh(Variable);
	for (int i = 0; i < 4; i++)
	{
		Variable.PlayerGame[4][i + 3] = 'A';
	}
	while (key != 27)  // ESC
	{
		GotoXY(0, 0);
		Draw(Variable);
		
		switch (key)
		{
		case 'w': case 'W':
		{
			for (int i = 0; i < 8; i++)
			{
				for (int j = 0; j < 8; j++)
				{
					if (Variable.PlayerGame[i][j] != Variable.EmptyChar)
					{
						Variable.PlayerGame[i - 1][j] = Variable.PlayerGame[i][j];
						Variable.PlayerGame[i][j] = Variable.EmptyChar;
					}
				}
			}
		}
		case 'a': case 'A':
		{
			for (int i = 0; i < 8; i++)
			{
				for (int j = 0; j < 8; j++)
				{
					if (Variable.PlayerGame[i][j] != Variable.EmptyChar)
					{
						Variable.PlayerGame[i][j - 1] = Variable.PlayerGame[i][j];
						Variable.PlayerGame[i][j] = Variable.EmptyChar;
					}
				}
			}
		}
		case 's': case 'S':
		{
			for (int i = 0; i < 8; i++)
			{
				for (int j = 0; j < 8; j++)
				{
					if (Variable.PlayerGame[i][j] != Variable.EmptyChar)
					{
						Variable.PlayerGame[i + 1][j] = Variable.PlayerGame[i][j];
						Variable.PlayerGame[i][j] = Variable.EmptyChar;
					}
				}
			}
		}
		case 'd': case 'D':
		{
			for (int i = 0; i < 8; i++)
			{
				for (int j = 0; j < 8; j++)
				{
					if (Variable.PlayerGame[i][j] != Variable.EmptyChar)
					{
						Variable.PlayerGame[i][j + 1] = Variable.PlayerGame[i][j];
						Variable.PlayerGame[i][j] = Variable.EmptyChar;
					}
				}
			}
		}
		}
		
		key = _getch();
	}
}
void Get_Boats(Variable_Struct& Variable)
{
	int choice;
	cout << "\n\n";
	cout << "\t1) Choose From DataBase:" << endl;
	cout << "\t2) Set Manually:" << endl;
	cout << "\tChoose: ";
	cin >> choice;
	
	if (cin.fail())   //  Cin Fail - Olarsa    ==> Duz islemir!
	{
		cin.clear();
		cin.ignore();
		cout << "\n\t Wrong Input!";
		Sleep(500);
		Get_Boats(Variable);
	}

	FILE *ft = nullptr;
	switch (choice)
	{
	case 2:
		MessageBox(NULL, TEXT("Sorry, We haven't Finished Manual Boat Set...\nBoats will be placed from Database"), TEXT(" Warning!"), MB_OK);  // Break Olunmur
	case 1:
		//XXXXXXXXXXXXXXXX   PLAYER BOATS DATABASE   XXXXXXXXXXXXXXXXXXXX

		srand(time(NULL));
		fopen_s(&ft, "BOATS.txt", "rb+");
		if (ft != nullptr)
		{
			char num[2];
			fread(&num, sizeof(char), 1, ft);

			//fseek(ft, 64 * (rand() % atoi(num)), SEEK_CUR);
			fseek(ft, 64 * 2, SEEK_CUR);
			char t;
			for (int i = 0; i < 64; i++)
			{
				fread(&t, sizeof(char), 1, ft);
				if (t != ' ')
					Variable.PlayerGame[0][i] = t;
			}
			fclose(ft);
			ft = nullptr;
		}

		//XXXXXXXXXXXXXXXX   CPU BOATS DATABASE   XXXXXXXXXXXXXXXXXXXX

		fopen_s(&ft, "CPU_BOATS.txt", "rb+");
		if (ft != nullptr)
		{
			char num[2];
			fread(&num, sizeof(char), 1, ft);

			fseek(ft, 64 * (rand() % atoi(num)), SEEK_CUR);

			char t;
			for (int i = 0; i < 64; i++)
			{
				fread(&t, sizeof(char), 1, ft);
				if (t != ' ')
					Variable.CPU[0][i] = t;
			}
			fclose(ft);
			ft = nullptr;
		}

		break;
	default:
		Get_Boats(Variable);
		break;
	}
}
void CPU_Wait()   //  67  21  - Oyunun orta yeri
{
	for (int i = 0; i < 4; i++)    //  Right
	{
		GotoXY(67 + i, 21);
		cout << "#";
		Sleep(200);
	}
	for (int i = 0; i < 2; i++)    // Down
	{
		GotoXY(70, 21 + i);
		cout << "#";
		Sleep(200);
	}
	for (int i = 0; i < 4; i++)    // Left
	{
		GotoXY(70 - i, 23);
		cout << "#";
		Sleep(200);
	}
	for (int i = 0; i < 2; i++)    // Up
	{
		GotoXY(67, 23 - i);
		cout << "#";
		Sleep(200);
	}
}
void Clear_Waiting_Move()
{
	for (int i = 0; i < 4; i++)    //  Right
	{
		GotoXY(67 + i, 21);
		cout << " ";
	}
	for (int i = 0; i < 2; i++)    // Down
	{
		GotoXY(70, 21 + i);
		cout << " ";
	}
	for (int i = 0; i < 4; i++)    // Left
	{
		GotoXY(70 - i, 23);
		cout << " ";
	}
	for (int i = 0; i < 2; i++)    // Up
	{
		GotoXY(67, 23 - i);
		cout << " ";
	}
}
void ChangeShipDirection(Variable_Struct& Variable, CleverCPU& LastAttack)
{
	short random;     //  2 Direction arasinda random secim etmek ucun..  ~~ Misal( 0-Up, 1-Down ) ~~
	srand(time(NULL));
	switch (LastAttack.direction)
	{
	case 'l':
	{
		if (LastAttack.x == 7 || Variable.PlayerGame[LastAttack.y][LastAttack.x + 1] == char(176) || Variable.PlayerGame[LastAttack.y][LastAttack.x + 1] == char(254) || Variable.PlayerGame[LastAttack.y][LastAttack.x + 2] == char(254))
		{
			random = rand() % 2;    //    0 - Up,  1 - Down
			if (random == 0)   // Up
			{
				if (Variable.PlayerGame[LastAttack.y - 1][LastAttack.x] == char(176) || LastAttack.y == 0)   //  Gemi-nin yuxarisi vurulubsa ve ya 'y' en ust setirdedirse...
					LastAttack.direction = 'd';
				else
					LastAttack.direction = 'u';
			}
			else    //  Down
			{
				if (Variable.PlayerGame[LastAttack.y + 1][LastAttack.x] == char(176) || LastAttack.y == 7)   //  Gemi-nin asagisi vurulubsa ve ya 'y' en alt setirdedirse...
					LastAttack.direction = 'u';
				else
					LastAttack.direction = 'd';
			}
		}
		else
		{
			LastAttack.direction = 'r';
		}
	}
	case 'r':
	{
		if (LastAttack.x == 0 || Variable.PlayerGame[LastAttack.y][LastAttack.x - 1] == char(176) || Variable.PlayerGame[LastAttack.y][LastAttack.x - 1] == char(254) || Variable.PlayerGame[LastAttack.y][LastAttack.x - 2] == char(254))
		{
			random = rand() % 2;    //    0 - Up,  1 - Down
			if (random == 0)     //  Up
			{
				if (Variable.PlayerGame[LastAttack.y - 1][LastAttack.x] == char(176) || LastAttack.y == 0)   //  Gemi-nin yuxarisi vurulubsa ve ya 'y' en ust setirdedirse...
					LastAttack.direction = 'd';
				else
					LastAttack.direction = 'u';
			}
			else	//   Down
			{
				if (Variable.PlayerGame[LastAttack.y + 1][LastAttack.x] == char(176) || LastAttack.y == 7)   //  Gemi-nin asagisi vurulubsa ve ya 'y' en alt setirdedirse...
					LastAttack.direction = 'u';
				else
					LastAttack.direction = 'd';
			}
		}
		else
		{
			LastAttack.direction = 'l';
		}
	}
	case 'u':
	{
		if (LastAttack.y == 7 || Variable.PlayerGame[LastAttack.y + 1][LastAttack.x] == char(176) || Variable.PlayerGame[LastAttack.y + 1][LastAttack.x] == char(254) || Variable.PlayerGame[LastAttack.y + 2][LastAttack.x] == char(254))
		{
			random = rand() % 2;    //    0 - Left,  1 - Right
			if (random == 0)   // Left
			{
				if (LastAttack.x == 0 || Variable.PlayerGame[LastAttack.y][LastAttack.x - 1] == char(176))   //  Gemi-nin solu vurulubsa ve ya 'x' en sol setirdedirse...
					LastAttack.direction = 'r';
				else
					LastAttack.direction = 'l';
			}
			else	 // Right
			{
				if (LastAttack.x == 7 || Variable.PlayerGame[LastAttack.y][LastAttack.x + 1] == char(176))   //  Gemi-nin sagi vurulubsa ve ya 'x' en sag setirdedirse...
					LastAttack.direction = 'l';
				else
					LastAttack.direction = 'r';
			}
		}
		else
		{
			LastAttack.direction = 'd';
		}
	}
	case 'd':
	{
		if (LastAttack.y == 0 || Variable.PlayerGame[LastAttack.y - 1][LastAttack.x] == char(176) || Variable.PlayerGame[LastAttack.y - 1][LastAttack.x] == char(254) || Variable.PlayerGame[LastAttack.y - 2][LastAttack.x] == char(254))
		{
			random = rand() % 2;    //    0 - Left,  1 - Right
			if (random == 0)   // Left
			{
				if (LastAttack.x == 0 || Variable.PlayerGame[LastAttack.y][LastAttack.x - 1] == char(176))   //  Gemi-nin solu vurulubsa ve ya 'x' en sol setirdedirse...
					LastAttack.direction = 'r';
				else
					LastAttack.direction = 'l';
			}
			else   //  Right
			{
				if (LastAttack.x == 7 || Variable.PlayerGame[LastAttack.y][LastAttack.x + 1] == char(176))   //  Gemi-nin sagi vurulubsa ve ya 'x' en sag setirdedirse...
					LastAttack.direction = 'l';
				else
					LastAttack.direction = 'r';
			}
		}
		else
		{
			LastAttack.direction = 'u';
		}
	}
	}
}
bool Ship_Count_is_Zero(Ship_Struct& Ship, char WhichShip, bool Player_or_CPU /* True - Player, False - CPU*/)
{
	int num;
	switch (WhichShip)
	{
	case 'A':
	{
		if (Player_or_CPU == true)   //Player
		{
			num = Ship.A_ship & 255;   //  Son 8 Bit
		}
		else   // CPU
		{
			num = (Ship.A_ship >> 8 & 255);   // Ilk 8 Bit
		}

		if (num == 0)
			return true;
		else
			return false;
	}
	case 'B':
	{
		if (Player_or_CPU == true)   //Player
		{
			num = Ship.B_ship & 255;   //  Son 8 Bit
		}
		else   // CPU
		{
			num = (Ship.B_ship >> 8 & 255);   // Ilk 8 Bit
		}

		if (num == 0)
			return true;
		else
			return false;
	}
	case 'C':
	{
		if (Player_or_CPU == true)   //Player
		{
			num = Ship.C_ship & 255;   //  Son 8 Bit
		}
		else   // CPU
		{
			num = (Ship.C_ship >> 8 & 255);   // Ilk 8 Bit
		}

		if (num == 0)
			return true;
		else
			return false;
	}
	case 'D':
	{
		if (Player_or_CPU == true)   //Player
		{
			num = Ship.D_ship & 255;   //  Son 8 Bit
		}
		else   // CPU
		{
			num = (Ship.D_ship >> 8 & 255);   // Ilk 8 Bit
		}

		if (num == 0)
			return true;
		else
			return false;
	}
	}
}
void Get_Coordinate_For_CPU(Variable_Struct& Variable, int& x, int& y)
{
	srand(time(NULL));

Get_Coordinate:    //  Bu olmasa Her defe srand() -i oxudugu ucun Random qiymetler deyismir!
	x = rand() % 8;
	y = rand() % 8;

	// Eger hemin Kordinata evvel vurubsa Yeni kordinat qiymetleri alsin
	if (Variable.PlayerGame[y][x] == char(176) || Variable.PlayerGame[y][x] == char(254))
		goto Get_Coordinate;

	//  Vurulan Geminin Yanina Vurmamasi ucun...
	if ((x != 0 && Variable.PlayerGame[y][x - 1] == char(254)) || (x != 7 && Variable.PlayerGame[y][x + 1] == char(254)) ||
		(y != 0 && Variable.PlayerGame[y - 1][x] == char(254)) || (y != 7 && Variable.PlayerGame[y + 1][x] == char(254)) ||
		(x != 0 && y != 0 && Variable.PlayerGame[y - 1][x - 1] == char(254)) || (x != 0 && y != 7 && Variable.PlayerGame[y + 1][x - 1] == char(254)) ||
		(x != 7 && y != 0 && Variable.PlayerGame[y - 1][x + 1] == char(254)) || (x != 7 && y != 7 && Variable.PlayerGame[y + 1][x + 1] == char(254)))
		goto Get_Coordinate;
}
void CPU_Attack(Variable_Struct& Variable, CleverCPU& LastAttack, Ship_Struct& Ship, CPU_Count_Helper_Struct& Count)           //  Duzelis Edilmelidi
{
	//XXXXXXXXXXXXXX   CPU TURN    XXXXXXXXXX
	GotoXY(29, 9);
	HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(out, 206);
	cout << "CPU Turn";
	SetConsoleTextAttribute(out, 15);
	//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX	

	CPU_Wait();
CPU_Attack_Starts:  //  CPU_Wait()-e girmeden Baslamasi ucun

	Clear_Waiting_Move();  //  Ortada cixan isareleri silir.. 

	static int count = 0; // Vurulan Gemilerin Sayi
	int x;
	int y;

	if (LastAttack.direction == '-')
	{
		Get_Coordinate_For_CPU(Variable, x, y);  // X ve Y - e Random Kordinat verir

		if (Variable.PlayerGame[y][x] == Variable.EmptyChar)
		{
			Variable.PlayerGame[y][x] = 176;
			GotoXY(0, 0);
			Draw(Variable);
			return;
		}
		else if (Variable.PlayerGame[y][x] == char(176))
		{
			goto CPU_Attack_Starts;   //  CPU_Wait -e girmeden yeniden random qiymet alsin
		}
		else	// Eger Hemin kordinatda Her hansisa Gemi Varsa:
		{

			LastAttack.x = x;
			LastAttack.y = y;
			GetDirection(Variable, LastAttack, x, y);  // "LastAttack.direction" -u Deyisir!

			Variable.CPUScore++;
			ChangeShipCount(Ship, Variable.PlayerGame[y][x]/* ShipName (A,B,C,D) */, false);

			Variable.PlayerGame[y][x] = 254;
			Count.rx = 1;
			Count.lx = 1;
			Count.ux = 1;
			Count.dx = 1;

			GotoXY(0, 0);
			Draw(Variable);
			CPU_Attack(Variable, LastAttack, Ship, Count);  //  ~~~ Gemiye Vurdugu ucun Yeniden Attack edir ~~~
		}
	}
	else //if (Variable.PreviousCPUAttack == true)    //   ~~ EDIT  NEEDED ~~
	{
	Switch_Starts:
		GotoXY(5, 40);

		switch (LastAttack.direction)
		{
		case 'l':
		{
			if (LastAttack.x - Count.lx < 0 || Variable.PlayerGame[LastAttack.y][LastAttack.x - Count.lx] == char(176) || Variable.PlayerGame[LastAttack.y][LastAttack.x - Count.lx] == char(254))
			{
				ChangeShipDirection(Variable, LastAttack);    // Yeni Direction Tapsin
				Count.lx = 1;
				goto Switch_Starts;        //  Yeni Direction-a uygun Switch-e yeniden daxil olsun... 
			}
			else if (Variable.PlayerGame[LastAttack.y][LastAttack.x - Count.lx] == Variable.EmptyChar)
			{
				Variable.PlayerGame[LastAttack.y][LastAttack.x - Count.lx] = 176;
				ChangeShipDirection(Variable, LastAttack);    // Yeni Direction Tapsin
				Count.lx = 1;

			}
			else      //  Gemiye Vurubsa 
			{
				ChangeShipCount(Ship, Variable.PlayerGame[LastAttack.y][LastAttack.x - Count.lx], false /* False = CPU */);   //  Uygun Gemi-nin Mumkun Vurula bilecek Kordinatlarinin sayi Azalir

				char Ship_Type = Variable.PlayerGame[LastAttack.y][LastAttack.x - Count.lx];   //  PlayerGame = 254 Oldugu ucun, evvelceden yadda saxlayiriq icindekini

				Variable.PlayerGame[LastAttack.y][LastAttack.x - Count.lx] = 254;
				Variable.CPUScore++;
				Count.lx++;
				GotoXY(0, 0);
				Draw(Variable);

				if (Ship_Count_is_Zero(Ship, Ship_Type, false))
				{

					HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
					SetConsoleTextAttribute(out, 91);

					for (int i = 0; i < count; i++)  //  Her Ship Type-ni alt setrden Yazmasi Ucun
						cout << "\n";
					cout << "\n\n\n\t\t  Ship Type " << Ship_Type << " - Has been DESTROYED! ";
					count++;
					Sleep(1000);
					system("color 0f");
					LastAttack.direction = '-';
				}

				if (Variable.CPUScore == Variable.WinningScore)  // Butun Gemiler Vurulubsa Main-e Donsun
					return;

				CPU_Attack(Variable, LastAttack, Ship, Count);  //  ~~~ Gemiye Vurdugu ucun Yeniden Attack edir ~~~
			}

			break;
		}
		case 'r':
		{
			if (LastAttack.x + Count.rx > 7 || Variable.PlayerGame[LastAttack.y][LastAttack.x + Count.rx] == char(176) || Variable.PlayerGame[LastAttack.y][LastAttack.x + Count.rx] == char(254))
			{
				ChangeShipDirection(Variable, LastAttack);    // Yeni Direction Tapsin
				Count.rx = 1;
				goto Switch_Starts;        //  Yeni Direction-a uygun Switch-e yeniden daxil olsun... 
			}
			else if (Variable.PlayerGame[LastAttack.y][LastAttack.x + Count.rx] == Variable.EmptyChar)   //  Bos Xanaya Vurubsa
			{
				Variable.PlayerGame[LastAttack.y][LastAttack.x + Count.rx] = 176;
				ChangeShipDirection(Variable, LastAttack);    // Yeni Direction Tapsin
				Count.rx = 1;

			}
			else      //  Gemiye Vurubsa 
			{
				ChangeShipCount(Ship, Variable.PlayerGame[LastAttack.y][LastAttack.x + Count.rx], false /* False = CPU */);   //  Uygun Gemi-nin Mumkun Vurula bilecek Kordinatlarinin sayi Azalir

				char Ship_Type = Variable.PlayerGame[LastAttack.y][LastAttack.x + Count.rx];   //  PlayerGame = 254 Oldugu ucun, evvelceden yadda saxlayiriq icindekini

				Variable.PlayerGame[LastAttack.y][LastAttack.x + Count.rx] = 254;
				Variable.CPUScore++;
				Count.rx++;
				GotoXY(0, 0);
				Draw(Variable);

				if (Ship_Count_is_Zero(Ship, Ship_Type, false))
				{
					HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
					SetConsoleTextAttribute(out, 91);

					for (int i = 0; i < count; i++)  //  Her Ship Type-ni alt setrden Yazmasi Ucun
						cout << "\n";
					cout << "\n\n\n\t\t  Ship Type " << Ship_Type << " - Has been DESTROYED! ";
					count++;
					Sleep(1000);
					system("color 0f");
					LastAttack.direction = '-';
				}

				if (Variable.CPUScore == Variable.WinningScore)  // Butun Gemiler Vurulubsa Main-e Donsun
					return;

				CPU_Attack(Variable, LastAttack, Ship, Count);  //  ~~~ Gemiye Vurdugu ucun Yeniden Attack edir ~~~
			}

			break;
		}
		case 'u':
		{
			if (LastAttack.y - Count.ux < 0 || Variable.PlayerGame[LastAttack.y - Count.ux][LastAttack.x] == char(176) || Variable.PlayerGame[LastAttack.y - Count.ux][LastAttack.x] == char(254))
			{
				ChangeShipDirection(Variable, LastAttack);    // Yeni Direction Tapsin
				Count.ux = 1;
				goto Switch_Starts;        //  Yeni Direction-a uygun Switch-e yeniden daxil olsun... 
			}
			else if (Variable.PlayerGame[LastAttack.y - Count.ux][LastAttack.x] == Variable.EmptyChar)
			{
				Variable.PlayerGame[LastAttack.y - Count.ux][LastAttack.x] = 176;
				ChangeShipDirection(Variable, LastAttack);    // Yeni Direction Tapsin
				Count.ux = 1;

			}
			else      //  Gemiye Vurubsa 
			{
				ChangeShipCount(Ship, Variable.PlayerGame[LastAttack.y - Count.ux][LastAttack.x], false /* False = CPU */);   //  Uygun Gemi-nin Mumkun Vurula bilecek Kordinatlarinin sayi Azalir

				char Ship_Type = Variable.PlayerGame[LastAttack.y - Count.ux][LastAttack.x];   //  PlayerGame = 254 Oldugu ucun, evvelceden yadda saxlayiriq icindekini

				Variable.PlayerGame[LastAttack.y - Count.ux][LastAttack.x] = 254;
				Variable.CPUScore++;
				Count.ux++;
				GotoXY(0, 0);
				Draw(Variable);

				if (Ship_Count_is_Zero(Ship, Ship_Type, false))
				{
					HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
					SetConsoleTextAttribute(out, 91);

					for (int i = 0; i < count; i++)  //  Her Ship Type-ni alt setrden Yazmasi Ucun
						cout << "\n";
					cout << "\n\n\n\t\t  Ship Type " << Ship_Type << " - Has been DESTROYED! ";
					count++;
					Sleep(1000);
					system("color 0f");
					LastAttack.direction = '-';
				}

				if (Variable.CPUScore == Variable.WinningScore)  // Butun Gemiler Vurulubsa Main-e Donsun
					return;

				CPU_Attack(Variable, LastAttack, Ship, Count);  //  ~~~ Gemiye Vurdugu ucun Yeniden Attack edir ~~~
			}

			break;
		}
		case 'd':
		{
			if (LastAttack.y + Count.dx > 7 || Variable.PlayerGame[LastAttack.y + Count.dx][LastAttack.x] == char(176) || Variable.PlayerGame[LastAttack.y + Count.dx][LastAttack.x] == char(254))
			{
				ChangeShipDirection(Variable, LastAttack);    // Yeni Direction Tapsin
				Count.dx = 1;
				goto Switch_Starts;        //  Yeni Direction-a uygun Switch-e yeniden daxil olsun... 
			}
			else if (Variable.PlayerGame[LastAttack.y + Count.dx][LastAttack.x] == Variable.EmptyChar)
			{
				Variable.PlayerGame[LastAttack.y + Count.dx][LastAttack.x] = 176;
				ChangeShipDirection(Variable, LastAttack);    // Yeni Direction Tapsin
				Count.dx = 1;

			}
			else      //  Gemiye Vurubsa 
			{
				ChangeShipCount(Ship, Variable.PlayerGame[LastAttack.y + Count.dx][LastAttack.x], false /* False = CPU */);   //  Uygun Gemi-nin Mumkun Vurula bilecek Kordinatlarinin sayi Azalir

				char Ship_Type = Variable.PlayerGame[LastAttack.y + Count.dx][LastAttack.x];   //  PlayerGame = 254 Oldugu ucun, evvelceden yadda saxlayiriq icindekini

				Variable.PlayerGame[LastAttack.y + Count.dx][LastAttack.x] = 254;
				Variable.CPUScore++;
				Count.dx++;
				GotoXY(0, 0);
				Draw(Variable);

				if (Ship_Count_is_Zero(Ship, Ship_Type, false))
				{
					HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
					SetConsoleTextAttribute(out, 91);

					for (int i = 0; i < count; i++)  //  Her Ship Type-ni alt setrden Yazmasi Ucun
						cout << "\n";
					cout << "\n\n\n\t\t  Ship Type " << Ship_Type << " - Has been DESTROYED! ";
					count++;
					Sleep(1000);
					system("color 0f");
					LastAttack.direction = '-';
				}

				if (Variable.CPUScore == Variable.WinningScore)  // Butun Gemiler Vurulubsa Main-e Donsun
					return;

				CPU_Attack(Variable, LastAttack, Ship, Count);  //  ~~~ Gemiye Vurdugu ucun Yeniden Attack edir ~~~
			}

			break;
		}
		}
	}

	GotoXY(0, 0);
	Draw(Variable);
}
CursorPosition GetCursorFunction()    //   [0][0] =  15, 88
{
	CursorPosition cursor;
	HANDLE cConsole = GetStdHandle(STD_INPUT_HANDLE);
	DWORD dwPreviousMode = 0;
	GetConsoleMode(cConsole, &dwPreviousMode);
	DWORD dwNewMode = dwPreviousMode | ENABLE_MOUSE_INPUT;
	SetConsoleMode(cConsole, dwPreviousMode);
	dwNewMode &= ~ENABLE_QUICK_EDIT_MODE;
	SetConsoleMode(cConsole, dwNewMode);
	HANDLE hConsoleIn = GetStdHandle(STD_INPUT_HANDLE);
	HANDLE hConsoleOut = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD coord = { 0, 0 };
	CONSOLE_CURSOR_INFO cursorInfo = {};
	INPUT_RECORD inputRecord = {};
	unsigned long nRecordRead = 0;
	cursorInfo.bVisible = false;
	cursorInfo.dwSize = 0;
	SetConsoleCursorInfo(hConsoleOut, &cursorInfo);
	SetConsoleMode(hConsoleIn, ENABLE_MOUSE_INPUT);
	SetConsoleMode(hConsoleIn, ENABLE_PROCESSED_INPUT | ENABLE_MOUSE_INPUT);
	while (true)
	{
		ReadConsoleInput(hConsoleIn, &inputRecord, 1, &nRecordRead);
		if (inputRecord.EventType == MOUSE_EVENT)
		{
			if (inputRecord.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)
			{
				cursor.X = inputRecord.Event.MouseEvent.dwMousePosition.X;
				cursor.Y = inputRecord.Event.MouseEvent.dwMousePosition.Y;

				//  X ve Y  OpponentGame-nin uygun xanalarindan birine dusurse SONSOZ dovrden cixsin
				if (cursor.X >= 88 && cursor.X <= 128 && (cursor.X - 88) % 5 == 0 &&
					cursor.Y >= 15 && cursor.Y <= 31 && (cursor.Y - 15) % 2 == 0)
				{
					return cursor;
				}
			}
		}
	}
	return cursor;
}
bool Player_Attack(Variable_Struct& Variable, Ship_Struct& Ship)
{
	//XXXXXXXXXXXXXX   YOUR TURN    XXXXXXXXXX
	GotoXY(101, 9);
	HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(out, 206);
	cout << "Your Turn";
	SetConsoleTextAttribute(out, 15);
	//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX	

	CursorPosition X_Y = GetCursorFunction();
	int x = (X_Y.X - 88) / 5;   // Baslangic Kordinati 88-dir ve Kordinatlar arasinda 5 xana ferq var
	int y = (X_Y.Y - 15) / 2;  // Baslangic Kordinati 15-dir ve Kordinatlar arasinda 2 xana ferq var

	if (Variable.CPU[y][x] == Variable.EmptyChar)   //   Gemi Olmayan Yere Vurubsa
	{
		Variable.CPU[y][x] = 'X';
		Variable.OpponentGame[y][x] = 177;
		GotoXY(0, 0);
		Draw(Variable);
		return 0;
	}
	else if (Variable.CPU[y][x] != 'X')        //  Evvelce Vurmadigi yerdirse :  (Demeli Burada Gemi Var cunki (else if)) 
	{
		ChangeShipCount(Ship, Variable.CPU[y][x], true);
		Variable.OpponentGame[y][x] = 254;
		Variable.PlayerScore++;
		GotoXY(0, 0);
		Draw(Variable);

		if (Ship_Count_is_Zero(Ship, Variable.CPU[y][x], true))    // Vurdugu Gemi Tam olaraq Vuruldusa
		{
			static int count = 0;
			HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
			SetConsoleTextAttribute(out, 91);

			for (int i = 0; i < count; i++)  //  Her Ship Type-ni alt setrden Yazmasi Ucun
				cout << "\n";
			cout << "\n\n\n\t\t\t\t\t\t\t\t\t\t\t  Ship Type " << Variable.CPU[y][x] << " - Has been DESTROYED! ";
			count++;

			Variable.CPU[y][x] = 'X';
			Sleep(1000);
			system("color 0f");
			return 1;
		}

		Variable.CPU[y][x] = 'X';
		return 1;
	}
	else return 1;    // 'X' olan yere vurulduguna gore dovrden cixmir
}
void ShowCursor(bool True_False)   // True - Visible,  False - Invisible
{
	HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO     cursorInfo;
	GetConsoleCursorInfo(out, &cursorInfo);
	cursorInfo.bVisible = True_False;
	SetConsoleCursorInfo(out, &cursorInfo);
}
void SetConsoleForMenu()
{
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);

	COORD a;
	a.X = 90;
	a.Y = 300;

	SMALL_RECT rect = { 0,0,30,10 };   //

	SetConsoleScreenBufferSize(handle, a);

	SetConsoleWindowInfo(handle, 1, &rect);

	//cout << "Making Console Ready..." << endl;

	int k = 0;
	for (int i = 0; i < 50; i++)
	{
		if (k == 5)
		{
			rect.Bottom++;
			k = 0;
		}
		k++;
		rect.Right++;
		int x = SetConsoleWindowInfo(handle, 1, &rect);
		if (x == 0)
		{
			cout << GetLastError() << endl;
		}
		Sleep(20);
	}
	system("cls");
}
void Draw_Rect_For_Menu()   //  Left - 15,   Right - 50,   Top - 4,  Bottom - 15
{
	int Height = 6;
	int Width = 35;

	for (int i = 1; i < Width; i++)
	{
		GotoXY(15 + i, 4);
		cout << "_";
		Sleep(20);
	}
	for (int i = 1; i <= Height; i++)
	{
		GotoXY(15 + Width, 4 + i);
		cout << "|";
		Sleep(50);
	}
	for (int i = Width - 1; i >= 0; i--)
	{
		GotoXY(15 + i, 4 + Height);
		cout << "_";
		Sleep(20);
	}
	for (int i = Height; i > 0; i--)
	{
		GotoXY(15, 4 + i);
		cout << "|";
		Sleep(50);
	}
}
void Show_Text(string text, int Time = 50)
{
	for (int i = 0; i < text.length(); i++)
	{
		cout << text[i];
		Sleep(Time);
	}
}
void GameRules()
{
	system("cls");
	GotoXY(0, 4);
	HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(out, 91);   // Set Color
	Show_Text("     Get ready, for a war has been declared. Its time for some naval warfare.\n\n\n\n", 40);

	SetConsoleTextAttribute(out, 143);   // Set Color
	Show_Text(" 1.  You have to defeat your opponent by destroying all his war ships\n     before he destroys yours.\n", 20);
	Show_Text(" 2.  They battlefield is a 8x8 grid where your ships will be placed\n", 20);
	Show_Text(" 3.  Controls are very easy. Just click where you want to attack.\n", 20);
	Show_Text(" 4.  If your attack hits the enemy ship, you get an extra turn\n", 20);
	Show_Text(" 5.  There are four Ship Types:\n     A Type -> 5x Ship\n     B Type -> 4x Ship\n     C Type -> 3x Ship\n     D Type -> 2x Ship", 20);
	cin.get();
	system("color B4");
}
void Menu()
{
	ShowCursor(false);
	system("color B4");
	SetConsoleForMenu();

	GotoXY(25, 9);
	Show_Text("Welcome To Battleship!");
	GotoXY(20, 10);
	Show_Text("Programmed by: Ibrahim Huseynzade");
	cin.get();
	system("cls");
	GotoXY(25, 7);
	Show_Text("1. Play Battleship");
	GotoXY(25, 8);
	Show_Text("2. Game Rules");

	Draw_Rect_For_Menu();

	int key;
	while (true)
	{

		key = int(_getch());

		if (key == 49)  // 1
		{
			int MB_Event = MessageBox(NULL, TEXT("Are You Sure?"), TEXT(" Menasiz Warning!"), MB_YESNO);
			if (MB_Event == IDYES)
				break;
		}
		else if (key == 50) // 2
		{
			int MB_Event = MessageBox(NULL, TEXT("   Do you want to Enter the Rules?..."), TEXT(" Menasiz Sual"), MB_YESNO);
			if (MB_Event == IDYES)
			{
				GameRules();
				system("cls");
				GotoXY(25, 7);
				Show_Text("1. Play Battleship");
				GotoXY(25, 8);
				Show_Text("2. Game Rules");
			}
		}
	}
	system("cls");
}
void Finish(Variable_Struct& Variable)
{
	system("cls");
	Draw(Variable);
	cout << "\n\n\n\t\t\t\t\t\t";
	if (Variable.CPUScore == Variable.WinningScore && Variable.PlayerScore == Variable.WinningScore)
	{
		HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(out, 91);   // Set Color
		Show_Text("       ~~~ Game Has Been DRAW! ~~~~     ", 50);
	}
	else if (Variable.CPUScore == Variable.WinningScore)
	{
		HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(out, 91);   // Set Color
		Show_Text("        ~~~ CPU Has Won The Game! ~~~~     ",50);
	}
	else
	{
		HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(out, 91);   // Set Color
		Show_Text("        ~~~ YOU Have Won The Game! ~~~~     ", 50);
	}
	cin.get();
}//.
int main()
{
	Menu();

	CleverCPU               LastAttack; 
	Variable_Struct         Variable;
	Ship_Struct             Ship;
	CPU_Count_Helper_Struct Count;

	system("color 0f");
	SetConsoleDisplayMode(GetStdHandle(STD_OUTPUT_HANDLE), CONSOLE_FULLSCREEN_MODE, 0);
	ShowCursor(false);  // Cursor InVisible

	Game_Refresh(Variable);
	GotoXY(0, 0);
	system("cls");
	Draw(Variable);

	Get_Boats(Variable);
	SetShipCount(Ship);
	system("cls");  //  Ekrandaki Trash-leri silir

	while (Variable.PlayerScore < Variable.WinningScore && Variable.CPUScore < Variable.WinningScore)
	{
		GotoXY(0, 0);
		Draw(Variable);

		while (Player_Attack(Variable, Ship))
		{
			if (Variable.PlayerScore == Variable.WinningScore)
				break;
		}
		GotoXY(100, 9);
		cout << "            ";   // Your Turn - yazisini silmek ucun

		CPU_Attack(Variable, LastAttack, Ship, Count);
		GotoXY(28, 9);
		cout << "            ";   //  CPU Turn - yazisini silmek Ucun
	}

	Finish(Variable);

	system("color 0f");
	cin.get();
	cin.get();
}
/*--------------------------------------------------------------------------*/ 
/* HW01_141044015_Ahmet_Mert_Gulbahce 										*/ 
/*--------------------------------------------------------------------------*/
/* hw2.cpp	 																*/
/*--------------------------------------------------------------------------*/
/* Created on 02/10/2017 by Ahmet Mert Gulbahce 							*/
/*--------------------------------------------------------------------------*/
#include <iostream>
#include <fstream>
#include <cstdlib>

using namespace std;

void initBoard(char board[][20], const int& boardSize);
void printBoard(const char board[][20], const int boardSize);
bool makeMove(char board[][20], const int boardSize, const char move, int turn = 1);
void playGame(char board[][20], int boardSize, char gameStyle, int& turn);
void getBestMove(char board[][20], const int boardSize, char& move);
void undoMove(char board[][20], const int boardSize, const char letter);
void changeTurn(int& turn);
void getMoveFromUser(char board[][20], int& boardSize, char& gameStyle, int& turn, char& move, int& gameLoaded);
int isFinished(char board[][20], const int boardSize, int changeCondition);
bool isFull(const char board[][20], const int boardSize);
int isFour(char board[][20], const int boardSize, int changeCondition);
int checkLine(string line);
bool saveGame(const char board[][20], const int boardSize, const char gameStyle, const int turn, const string line);
bool loadGame(char board[][20], int& boardSize, char& gameStyle, int& turn, string line);


int main(int argc, char const *argv[])
{
	auto turn = 1, gameLoaded = 0;
	int boardSize, error;
	char board[20][20], 
		 gameStyle;
	string line;
	
	cout << "Welcome to Connect Four!" << endl;

	/////////////////////// SIZE CHECK ///////////////////////////////
	do
	{
		error = 0;

		cout << endl;
		cout << "Board size: ";

		getline(cin,line);
		int lineCondition = checkLine(line); //checks what is in the line

		//if line is LOAD command
		if(lineCondition == 1)
		{
			if(loadGame(board,boardSize,gameStyle,turn,line))
			{
				gameLoaded = 1;
				playGame(board,boardSize,gameStyle,turn);
			}
			else
			{
				error = 1;
			}
		}
		//if line is SAVE command
		if(lineCondition == 2)
		{
			cerr << "Nothing to save! Try again!" << endl;
			error = 1;
		}
		//if line is NUMBER
		if(lineCondition == 3)
		{
			boardSize = stoi(line); 
			if( !(boardSize >= 4 && boardSize <= 20 && boardSize%2 == 0) )
			{
				cerr << "Invalid board size! Try again!" << endl;
				error = 1;
			}
		}
	}
	while( error == 1 );
	//////////////////////////////////////////////////////////////////


	//////////////////// GAME STYLE CHECK ////////////////////////////
	if( !gameLoaded )
	{
		do
		{
			error = 0;

			cout << endl;
			cout << "Choose game style:" << endl;
			cout << "   -Player vs Player (P)" << endl;
			cout << "   -Player vs Computer (C)" << endl;
			cout << "->";
			getline(cin,line);
			int lineCondition = checkLine(line); //checks what is in the line

			//if line is LOAD command
			if(lineCondition == 1)
			{
				if(loadGame(board,boardSize,gameStyle,turn,line))
				{
					gameLoaded = true;
					playGame(board,boardSize,gameStyle,turn);
				}
				else
				{
					cerr << "Game cannot be loaded! Try again!";
					error = 1;
				}
			}
			//if line is SAVE command
			if(lineCondition == 2)
			{
				cerr << "Nothing to save! Try again!" << endl;
				error = 1;
			}
			//if line is NUMBER
			if(lineCondition == 3)
			{
				gameStyle = line[0];
				if( !(gameStyle == 'P' || gameStyle == 'C' || gameStyle == 'p' || gameStyle == 'c') )
				{
					cerr << "Invalid game style! Try again!" << endl;
					error = 1;
				}
			}

		}
		while( error == 1 );
	}
	//////////////////////////////////////////////////////////////////


	////////////////////////// START A NEW GAME //////////////////////
	if( !gameLoaded ) 
	{
		// INITIALIZE BOARD
		initBoard(board,boardSize);
		playGame(board,boardSize,gameStyle,turn);
	}
	//////////////////////////////////////////////////////////////////
	
	return 0;
}


// set all cells with '.'
void initBoard(char board[][20], const int& boardSize)
{
	for (int i = 0; i < boardSize; ++i)
	{
		for (int j = 0; j < boardSize; ++j)
		{
			board[i][j] = '.';
		}
	}
}

// print board to screen
void printBoard(const char board[][20], const int boardSize)
{
	char start = 'A';

	// for loop - sorted letter section
	for (int i = 0; i < boardSize; ++i)
	{
		decltype(start) letter = start + i;
		cout << letter;
	}
	cout << endl;

	// print all cells
	for (int rows = 0; rows < boardSize; ++rows)
	{
		for (int letters = 0; letters < boardSize; ++letters)
		{
			cout << board[rows][letters];
		}
		cout << endl;
	}
}

// takes board, turn, move 
// makes move and returns TRUE
// if board full, returns FALSE
bool makeMove(char board[][20], const int boardSize, const char move, int turn)
{
	int letterIndexValue; 

	if(move >= 'A' && move <= 'Z')
		letterIndexValue = move - 'A';
	if(move >= 'a' && move <= 'z')
		letterIndexValue = move - 'a';

	int fullCells = 0; //count for full cells
	for (int i = boardSize; i != 0; --i)
	{
		//checks is column full or not
		if(board[i-1][letterIndexValue] == '.')
		{
			//if empty, makes move
			if(turn == 1)
			{
				board[i-1][letterIndexValue] = 'X';
				return true;
			}
			if(turn == 2)
			{
				board[i-1][letterIndexValue] = 'O';
				return true;
			}
		}
		else
		{
			++fullCells;
			if(fullCells == boardSize)
			{
				return false;
			}
		}
	}

	return true;
}

// This function plays the game
void playGame(char board[][20], int boardSize, char gameStyle, int& turn)
{
	int error = 0;
	int gameLoaded;
	char move;

	int changeCondition = 0;

	// checks board is finished or not
	while( isFinished(board,boardSize,changeCondition) == 0 )
	{
		gameLoaded = 0;
		//after all moves, prints the board on the screen
		if(error == 0)
		{
			cout << endl;
			printBoard(board,boardSize);
		}

		//takes moves from Player1 via terminal
		if(turn == 1)
			getMoveFromUser(board,boardSize,gameStyle,turn,move,gameLoaded);
		//gets best move for computer moves
		if(turn == 2)
		{
			if(gameStyle == 'C' || gameStyle == 'c' )
				getBestMove(board,boardSize,move);
			if (gameStyle == 'P' || gameStyle == 'p' )
				getMoveFromUser(board,boardSize,gameStyle,turn,move,gameLoaded);
			
		}

		if(gameLoaded == 0)
		{
			//makes move
			if( makeMove(board,boardSize,move,turn) )
			{
				changeTurn(turn);
				error = 0;
			}
			else 
			{
				cerr << "Column is full! Try again!" << endl;
				error = 1;
			}
		}
	}

	cout << endl;
	cout << "------------------------------" << endl;
	cout << "GAME OVER!" << endl;

	changeCondition = 0;
	//stores winner
	int finishSituation = isFinished(board,boardSize,changeCondition);


	if(finishSituation == 1)
		cout << "Winner: Player1" << endl;
	if(finishSituation == 2)
	{
		if(gameStyle == 'C')
			cout << "Winner: Player2(Computer)" << endl;
		if(gameStyle == 'P')
			cout << "Winner: Player2" << endl;
	}
	if(finishSituation == 3)
		cout << "Board is full! No winner (:" << endl;

	changeCondition = 1;
	//makes lower case all connected cells
	isFinished(board,boardSize,changeCondition);

	//prints finished game board
	cout << endl;
	printBoard(board,boardSize);
	cout << "------------------------------" << endl;
}

//takes board and reference move to change move with best move
void getBestMove(char board[][20], const int boardSize, char& move)
{
	char start = 'A';
	int best = 0;

	cout << "Player2: Make move!" << endl;
	

	//makes move to win game 
	for (int i = 0; i < boardSize; ++i)
	{
		decltype(start) letter = start + i;
		if( makeMove(board,boardSize,letter,2) )
		{
			if( isFour(board,boardSize,0) == 2 )
			{
				move = letter;
				best = 1;
			}
			undoMove(board,boardSize,letter);
		}
	}

	//makes move to block Player1 won
	if(best == 0)
	{
		for (int i = 0; i < boardSize; ++i)
		{
			char letter = start + i;
			if( makeMove(board,boardSize,letter,1) )
			{
				if( isFour(board,boardSize,0) == 1)
				{
					move = letter;
					best = 1;	
				}
				undoMove(board,boardSize,letter);
			}
		}
	}

	//if no win or block, makes random moves
	srand (time(NULL));

	while(best == 0)
	{
		char letter = (rand() % boardSize) + 'A';
		
		if( makeMove(board,boardSize,letter,2) )
		{
			best = 1;
			move = letter;
			//makes moves for check moves and undo that moves
			undoMove(board,boardSize,letter);
		}
	}

	cout << "->" << move << endl;
}

//clears last item in the column(letter)
void undoMove(char board[][20], const int boardSize, const char letter)
{
	int letterIndexValue;

	if(letter >= 'a' && letter <= 'z')
		letterIndexValue = letter - 'a';
	if(letter >= 'A' && letter <= 'Z')
		letterIndexValue = letter - 'A';

	for (int i = 0; i < boardSize; ++i)
	{
		if( board[i][letterIndexValue] != '.' )
		{
			board[i][letterIndexValue] = '.';
			return;
		}
	}
}

//Changes turn
void changeTurn(int& turn)
{
	if(turn == 1)
	{
		turn = 2;
		return;
	}
	if(turn == 2)
	{
		turn = 1;
		return;
	}
}

//Gets moves from user via terminal
void getMoveFromUser(char board[][20], int& boardSize, char& gameStyle, int& turn, char& move, int& gameLoaded)
{
	int letterIndexValue = boardSize;
	int error;
	string line;

	do
	{
		error = 0;

		//asks move
		cout << "Player" << turn << ": Make move!" << endl;
		cout << "->";

		getline(cin,line);

		if(checkLine(line) == 1) //if line has LOAD command
		{
			loadGame(board,boardSize,gameStyle,turn,line);
			gameLoaded = 1;
		}
		if(checkLine(line) == 2) //if line has SAVE command
		{
			saveGame(board,boardSize,gameStyle,turn,line);
			gameLoaded = 1;
		}
		if(checkLine(line) == 3) //if line has ONLY LETTER
		{
			if( line.size() == 1 )
			{
				move = line[0];
				
				//error check
				if(move >= 'A' && move <= 'Z')
					letterIndexValue = move - 'A';
				if(move >= 'a' && move <= 'z')
					letterIndexValue = move - 'a';

				if(letterIndexValue >= boardSize)
				{
					error = 1;
					cerr << "Wrong letter! Try again!" << endl;
				}
			}
			else 
			{
				error = 1;
				cerr << "Wrong input! Try again!" << endl;
			}
		}
	}
	while( error );
}

//Checks what's in the line
int checkLine(string line)
{	
	//LOAD KOMUTU
	if(line.find("LOAD") != std::string::npos)
		return 1;
	//SAVE KOMUTU
	else if(line.find("SAVE") != std::string::npos)
		return 2;
	//HARF OLMA DURUMU
	else
		return 3;
}

//Saves the current game with options
bool saveGame(const char board[][20], const int boardSize, const char gameStyle, const int turn, const string line)
{
	size_t found = line.find_first_of(" ") + 1;
	string fileName = line.substr(found);

	ofstream output;
	output.open(fileName); //open file

	//checks file is open or not
	if ( output.is_open() )
	{
		// writes to output file
		output << boardSize << endl;
		output << gameStyle << endl;
		output << turn << endl;

		for (int i = 0; i < boardSize; ++i)
		{
			for (int j = 0; j < boardSize; ++j)
			{
				output << board[i][j] << endl;
			}
		}

		output.close();

		return true;
	}
	else
	{
		cerr << "Cannot open file!" << endl;
		return false;
	}
}

bool loadGame(char board[][20], int& boardSize, char& gameStyle, int& turn, string line)
{
	size_t found = line.find_first_of(" ") + 1;
	string fileName = line.substr(found);

	ifstream input;
	input.open(fileName); //open file

	//checks file is open or not
	if ( input.is_open() )
	{	
		//read from input file
		input >> boardSize;
		input >> gameStyle;
		input >> turn;

		for (int i = 0; i < boardSize; ++i)
		{
			for (int j = 0; j < boardSize; ++j)
			{
				input >> board[i][j];
			}
		}

		input.close();

		//prints loaded game info
		cout << endl;
		cout << "-------------------------------------------" << endl;
		cout << "GAME LOADED!" << endl;
		cout << "   -Board size: " << boardSize << endl;
		cout << "   -Game style: ";
		if(gameStyle == 'P')
			cout << "Player vs Player (P)" << endl;
		else
			cout << "Player vs Computer (C)" << endl;
		cout << "-------------------------------------------" << endl;

		return true;
	}
	else
	{
		cerr << "Cannot open file!" << endl;
		return false;
	}
}

//Checks board is finished or not
//ChangeCondition --> if someone won, makes lower case connected cells
int isFinished(char board[][20], const int boardSize, int changeCondition)
{	
	if( isFour(board,boardSize,changeCondition) == 1)
		return 1;
	if( isFour(board,boardSize,changeCondition) == 2)
		return 2;
	if( isFull(board,boardSize) )
		return 3;
	else 
		return 0;
}
	
// Checks board is full or not
bool isFull(const char board[][20], const int boardSize)
{
	int total = 0;

	for (int i = 0; i < boardSize; ++i)
	{
		for (int j = 0; j < boardSize; ++j)
		{
			if(board[i][j] != '.')
				total++;
		}
	}

	if( total == (boardSize*boardSize) )
		return true;
	else
		return false;
}

//Checks board for search any connected cells
//ChangeCondition --> if someone won, makes lower case connected cells
int isFour(char board[][20], const int boardSize, int changeCondition)
{
	//HORIZONTAL CHECK
	for (int i = 0; i < boardSize; ++i)
	{
		for (int j = 0; j < boardSize-3; ++j)
		{
			//check
			if(board[i][j+0] == 'X' && board[i][j+1] == 'X' 
				&& board[i][j+2] == 'X' && board[i][j+3] == 'X')
			{
				//change
				if(changeCondition == 1)
				{
					board[i][j+0] = 'x';
					board[i][j+1] = 'x';
					board[i][j+2] = 'x';
					board[i][j+3] = 'x';
				}
				return 1;
			}
			//check
			if(board[i][j+0] == 'O' && board[i][j+1] == 'O' 
				&& board[i][j+2] == 'O' && board[i][j+3] == 'O')
			{
				//change
				if(changeCondition == 1)
				{
					board[i][j+0] = 'o';
					board[i][j+1] = 'o';
					board[i][j+2] = 'o';
					board[i][j+3] = 'o';
				}
				return 2;
			}
		}
	}


	//VERTICAL CHECK
	for (int j = 0; j < boardSize; ++j)
	{
		for (int i = 0; i < boardSize-3; ++i)
		{
			//check
			if(board[i+0][j] == 'X' && board[i+1][j] == 'X' 
				&& board[i+2][j] == 'X' && board[i+3][j] == 'X')
			{
				//change
				if(changeCondition == 1)
				{
					board[i+0][j] = 'x';
					board[i+1][j] = 'x';
					board[i+2][j] = 'x';
					board[i+3][j] = 'x';
				}
				return 1;
			}
			//check
			if(board[i+0][j] == 'O' && board[i+1][j] == 'O' 
				&& board[i+2][j] == 'O' && board[i+3][j] == 'O')
			{
				//change
				if(changeCondition == 1)
				{
					board[i+0][j] = 'o';
					board[i+1][j] = 'o';
					board[i+2][j] = 'o';
					board[i+3][j] = 'o';
				}
				return 2;
			}
		}
	}


	//DIAGONAL-LEFTtoRIGHT CHECK
	for (int i = 0; i < boardSize-3; ++i)
	{
		for (int j = 0; j < boardSize-3; ++j)
		{
			//check
			if(board[i+0][j+0] == 'X' && board[i+1][j+1] == 'X' 
				&& board[i+2][j+2] == 'X' && board[i+3][j+3] == 'X')
			{
				//change
				if(changeCondition == 1)
				{
					board[i+0][j+0] = 'x';
					board[i+1][j+1] = 'x';
					board[i+2][j+2] = 'x';
					board[i+3][j+3] = 'x';
				}
				return 1;
			}
			//check
			if(board[i+0][j+0] == 'O' && board[i+1][j+1] == 'O' 
				&& board[i+2][j+2] == 'O' && board[i+3][j+3] == 'O')
			{
				//change
				if(changeCondition == 1)
				{
					board[i+0][j+0] = 'o';
					board[i+1][j+1] = 'o';
					board[i+2][j+2] = 'o';
					board[i+3][j+3] = 'o';
				}
				return 2;
			}
		}
	}


	//DIAGONAL-RIGHTtoLEFT CHECK
	for (int i = 0; i < boardSize-3; ++i)
	{
		for (int j = 3; j < boardSize; ++j)
		{
			//check
			if(board[i+0][j-0] == 'X' && board[i+1][j-1] == 'X' 
				&& board[i+2][j-2] == 'X' && board[i+3][j-3] == 'X')
			{
				//change
				if(changeCondition == 1)
				{
					board[i+0][j-0] = 'x';
					board[i+1][j-1] = 'x';
					board[i+2][j-2] = 'x';
					board[i+3][j-3] = 'x';
				}
				return 1;
			}
			//check
			if(board[i+0][j-0] == 'O' && board[i+1][j-1] == 'O' 
				&& board[i+2][j-2] == 'O' && board[i+3][j-3] == 'O')
			{
				//change
				if(changeCondition == 1)
				{
					board[i+0][j-0] = 'o';
					board[i+1][j-1] = 'o';
					board[i+2][j-2] = 'o';
					board[i+3][j-3] = 'o';
				}
				return 2;
			}
		}
	}

	return 0;
}

/*--------------------------------------------------------------------------*/
/* 							End of hw2.cpp	 								*/
/*--------------------------------------------------------------------------*/
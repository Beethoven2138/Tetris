/*  Tetris is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Tetris is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Tetris.  If not, see <http://www.gnu.org/licenses/>.*/

//AUTHOR: SAXON SUPPLE

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <SFML/Graphics.hpp>

//Defining some preprocessor directives
#define boardWidth 10
#define boardHeight 20
#define startXPos 5
#define startYPos 1
#define scale 2
#define endPoint 2
#define blockLength 20

float timeBetweenMovement = 1;

//Arrray for the main board
//Different chars represent different colors
char board[boardWidth][boardHeight];

char blocksThatPlayerControlls[5][5];

class Piece;

void ResetBoard();

void InitSquares();

void DrawBoard(Piece *piece);

bool RowFull(int row);

void DestroyRow();

sf::Color ColorAssigner(char c);

sf::RenderWindow window(sf::VideoMode(boardWidth * blockLength * scale, boardHeight * blockLength * scale), "Tetris_C++");

sf::RectangleShape squares[boardWidth][boardHeight];


class Piece {
private:
	enum move { LEFT, RIGHT, UP, DOWN };
	int xPos, yPos;

	void ResetPiece() {
		//Start by clearing blocksTHatPlayerControlls
		for (int x = 0; x < 5; x++)
		{
			for (int y = 0; y < 5; y++)
			{
				blocksThatPlayerControlls[x][y] = 'e';
			}
		}
		//Randomly select the piece that's next created
		switch (rand() % 5) {
		case 0:
			blocksThatPlayerControlls[3][1] = 'b';
			blocksThatPlayerControlls[3][2] = 'b';
			blocksThatPlayerControlls[3][3] = 'b';
			blocksThatPlayerControlls[2][3] = 'b';
			break;
		case 1:
			blocksThatPlayerControlls[2][1] = 'p';
			blocksThatPlayerControlls[1][2] = 'p';
			blocksThatPlayerControlls[2][2] = 'p';
			blocksThatPlayerControlls[3][2] = 'p';
			break;
		case 2:
			blocksThatPlayerControlls[1][2] = 'c';
			blocksThatPlayerControlls[2][2] = 'c';
			blocksThatPlayerControlls[3][2] = 'c';
			blocksThatPlayerControlls[4][2] = 'c';
			break;
		case 3:
			blocksThatPlayerControlls[3][1] = 'o';
			blocksThatPlayerControlls[3][2] = 'o';
			blocksThatPlayerControlls[3][3] = 'o';
			blocksThatPlayerControlls[4][3] = 'o';
			break;
		case 4:
			blocksThatPlayerControlls[3][1] = 'y';
			blocksThatPlayerControlls[3][2] = 'y';
			blocksThatPlayerControlls[4][1] = 'y';
			blocksThatPlayerControlls[4][2] = 'y';
			break;
		default:
			break;
		}
		xPos = startXPos;
		yPos = startYPos;
	}

	//Needed to check if it's a collision or a valid move
	int FindLowestPoint() {
		for (int y = 4; y >= 0; y--) {
			for (int x = 0; x < 5; x++) {
				if (blocksThatPlayerControlls[x][y] != 'e') {
					return y;
				}
			}
		}
	}

	bool Collision() {
		//Lower the block by one to see if there's a collision there
		for (int x = 0; x < 5; x++) {
			for (int y = 0; y < 5; y++) {
				if (blocksThatPlayerControlls[x][y] != 'e' && board[x + xPos][y + yPos] != 'e') {//Removed + 1 from secont part of block array
					return true;
				}
			}
		}
		//Check if the piece is bellow the ground where 0,0 is the top left of the screen
		if (FindLowestPoint() + yPos + 1 > boardHeight)
			return true;
		return false;
	}

	void AddPlayerBlocksToBoard() {
		for (int x = 0; x < 5; x++) {
			for (int y = 0; y < 5; y++) {
				//Handle collisions in piece class
				if (blocksThatPlayerControlls[x][y] != 'e') {
					//xPos and yPos used to determine the piece's position relative to the main board
					board[x + xPos][y + yPos - 1] = blocksThatPlayerControlls[x][y];
				}
			}
		}
	}

	int FindFurthestPoint() {
		for (int x = 4; x >= 0; x--)
		{
			for (int y = 0; y < 5; y++)
			{
				if (blocksThatPlayerControlls[x][y] != 'e') {
					return x;
				}
			}
		}
	}

	int FindClosestPoint() {
		for (int x = 0; x < 5; x++)
		{
			for (int y = 0; y < 5; y++)
			{
				if (blocksThatPlayerControlls[x][y] != 'e') {
					return x;
				}
			}
		}
	}

	bool IsValidMove(int xDir, int yDir) {//ALso works for rotations
		std::cout << "IsValidMove being tested" << std::endl;
		//Find the furthest out point then check if it's outside the board. Do the same with the minimum
		if (xPos + FindFurthestPoint() + xDir + 1 > boardWidth || xPos + FindClosestPoint() + xDir < 0) {
			std::cout << "Invalid move" << std::endl;
			return false;
		}
		return true;
	}

public:
	Piece(int xPos, int yPos) {
		//Position of the piece relative to the main board
		this->xPos = xPos; /* Is this required since it's already in ResetPiece()?*/
		this->yPos = yPos;
		ResetPiece();
	}

	void SetXPosition(int x) {
		xPos = x;
	}
	void SetYPosition(int y) {
		yPos = y;
	}
	int GetXPosition() {
		return xPos;
	}
	int GetYPosition() {
		return yPos;
	}
	
	void Move(int direction) {
		
		//based on the previously defined enum
		
		switch (direction) {
		case LEFT:
			if (IsValidMove(-1, 0)) {
				xPos--;
			}	
			break;
		case RIGHT:
			if (IsValidMove(1, 0)) {
				xPos++;
			}
			break;
		case DOWN:
			if (IsValidMove(0, 1)) {
				std::cout << "Move valid" << std::endl;
				yPos++;
			}
			break;
		default:
			break;
		}
		if (Collision()) {
			std::cout << "Collision" << std::endl;
			
			//end the game if the piece collided above the endPoint
			if (yPos <= endPoint) {
				window.close();
			}
			//Add the piece to the board, get a new piece and check if any rows need to be destroyed
			AddPlayerBlocksToBoard();
			ResetPiece();
			DestroyRow();
		}
	}

	void AddPlayerBlocksToDraw() {
		for (int x = 0; x < 5; x++) {
			for (int y = 0; y < 5; y++) {
				//If there is a block in the array
				if (blocksThatPlayerControlls[x][y] != 'e') {
					squares[x + xPos][y + yPos].setFillColor(ColorAssigner(blocksThatPlayerControlls[x][y]));
				}
			}
		}
	}

	//Incomplete. Somebody finish it pls
	void Rotate() {
		char tempArray[5][5];	
		
		//duplicate the array
		for (int x = 0; x < 5; x++) {
			for (int y = 0; y < 5; y++) {
				tempArray[x][y] = blocksThatPlayerControlls[x][y];
			}
		}

		for (int x = 0; x < 5; x++) {
			for (int y = 0; y < 5; y++) {
				blocksThatPlayerControlls[x][y] = tempArray[y][x];
			}
		}

		//Revert the changes if there's a collision
		if (Collision()) {
			for (int x = 0; x < 5; x++) {
				for (int y = 0; y < 5; y++) {
					blocksThatPlayerControlls[x][y] = tempArray[x][y];
				}
			}
		}
	}
};

int main()
{
	//Set a random seed
	srand(time(NULL));

	//Instantiate the piece object
	Piece* piece = new Piece(startXPos, startYPos);

	//Start a clock
	sf::Clock clock;
	
	//The framerate will be the same as the monitor's refresh rate
	window.setVerticalSyncEnabled(true);

	ResetBoard();

	InitSquares();

	while (window.isOpen()) {

		sf::Event event;

		//Event handling
		while (window.pollEvent(event)) {

			switch (event.type) {

			case sf::Event::Closed:
				free (piece);
				window.close();
			case sf::Event::KeyPressed:
				if (event.key.code == sf::Keyboard::A)piece->Move(0);
				if (event.key.code == sf::Keyboard::D)piece->Move(1);
				if (event.key.code == sf::Keyboard::W)piece->Move(2);
				if (event.key.code == sf::Keyboard::S)piece->Move(3);
				if (event.key.code == sf::Keyboard::Escape){
					free (piece);
					window.close();}
				if (event.key.code == sf::Keyboard::Space)piece->Rotate();
			}
		}

		//Time for the next move
		if ((float)(clock.getElapsedTime().asSeconds()) >= timeBetweenMovement) {
			piece->Move(3);
			clock.restart();
		}

		window.clear(sf::Color::Black);

		DrawBoard(piece);

		window.display();

	}
	return 0;
}

void ResetBoard() {
	for (int x = 0; x < boardWidth; x++)
	{
		for (int y = 0; y < boardHeight; y++)
		{
			board[x][y] = 'e';
		}
	}
}

void DrawBoard(Piece *piece) {

	//Reset the squares to white (maybe it would be better to reference another function instead?)
	for (int x = 0; x < boardWidth; x++) {
		for (int y = 0; y < boardHeight; y++) {
			squares[x][y].setFillColor(sf::Color::White);
		}
	}

	//Set add the non player controlled pieces to the board
	for (int x = 0; x < boardWidth; x++)
	{
		for (int y = 0; y < boardHeight; y++)
		{
			if(board[x][y] != 'e')
			squares[x][y].setFillColor(ColorAssigner(board[x][y]));
		}
	}
	
	//Add a red line to mark the endpoint
	for (int x = 0; x < boardWidth; x++) {
		squares[x][endPoint].setFillColor(sf::Color::Red);
	}

	piece->AddPlayerBlocksToDraw();

	//Draw the squares on the screen
	for (int x = 0; x < boardWidth; x++) {
		for (int y = 0; y < boardHeight; y++) {
			window.draw(squares[x][y]);
		}
	}
}

//Initial settings for the squares on the board
void InitSquares() {
	for (int x = 0; x < boardWidth; x++)
	{
		for (int y = 0; y < boardHeight; y++)
		{
			squares[x][y].setSize(sf::Vector2f(blockLength * scale, blockLength * scale));
			squares[x][y].setPosition(x*blockLength * scale, y*blockLength * scale);
			squares[x][y].setFillColor(sf::Color::White);
			squares[x][y].setOutlineThickness(1 * scale);
			squares[x][y].setOutlineColor(sf::Color::Black);
		}
	}

	//Mark the endPoint as a red line
	for (int x = 0; x < boardWidth; x++) {
		squares[x][endPoint].setFillColor(sf::Color::Red);
	}
}

//Check if a row's full (has no 'e' values)
bool RowFull(int row) {
	for (int x = 0; x < boardWidth; x++)
	{
		if (board[x][row] == 'e') {
			return false;
		}
	}
	return true;
}

void DestroyRow() {
	for (int i = 0; i < boardHeight; i++)
	{
		if (RowFull(i)) {
			std::cout << "Row full" << std::endl;
			//Set each row to the row above it
			for (i - 1; i > 0; i--) {
				for (int j = 0; j < boardWidth; j++) {
					board[j][i] = board[j][i - 1];
				}
			}
			//Set the top row to empty since there aren't any rows above it
			for (int i = 0; i < boardWidth; i++)
			{
				board[i][0] = 'e';
			}
		}
	}
}

//Takes the char values in the arrays as an argument and then returns the color that they represent
sf::Color ColorAssigner(char c) {
	switch (c) {
	case 'b':
		return sf::Color::Blue;
		break;
	case 'c':
		return sf::Color::Cyan;
		break;
	case 'y':
		return sf::Color::Yellow;
		break;
	case 'o':
		return sf::Color(255, 165, 0);
		break;
	case 'p':
		return sf::Color(150, 50, 250);
		break;
	default:
		break;
	}
}

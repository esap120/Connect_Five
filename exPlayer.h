#ifndef exh
#define exh
#include "player.h"

class exPlayer:public player{
public:
  exPlayer(square **initTable);     // initialize player
  boardSquare nextMove();        // computes a next move for the player and stores coordinates in (xx,yy)
  void otherMove(boardSquare bs);// records the move of the opponent
  void printBoard();
  void scanBoard();
  boardSquare bestMove(square color);
  bool block(square color);
  int exCounter(square color);

private:
	boardSquare bs;
	int **table; // maintain my own copy of the board
};
#endif

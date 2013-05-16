#ifndef ohh
#define ohh
#include "player.h"

class ohPlayer:public player{
public:
  ohPlayer(square **initTable);     // initialize player
  boardSquare nextMove();        // computes a next move for the player and stores coordinates in (xx,yy)
  void otherMove(boardSquare bs);// records the move of the opponent
  void printBoard();
  void scanBoard();
  boardSquare bestMove(square color);
  bool block(square color);
  int ohCounter (square color);

private:
	int **table; // maintain my own copy of the board
};
#endif

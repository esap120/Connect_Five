#ifndef PLAYER
#define PLAYER

// constants
enum square {blank, oh, ex, unusable};

class boardSquare {
 public:
  boardSquare(int xxV=0, int yyV=0) {xx=xxV, yy=yyV;}
  int xx, yy; // coordinates of the boardSquare
};

class player {
 public:
  player() {};          // default constructor
  player(square **board, square color=oh) {
	/* initializes a player to player color "color" with initial board "board";
	** board[ii][jj] contains oh, ex, blank, or unused corresponding to the initial setup of the board
	*/
    myColor=color;
  }

  virtual boardSquare nextMove()=0;         // computes a next move for the player and stores coordinates in (xx,yy)
  virtual void otherMove(boardSquare bs)=0; // records the move of the opponent
 protected:
  square myColor;                           // the color of this player
};
#endif

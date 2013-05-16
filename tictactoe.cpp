#include <iostream>
#include <csignal>
#include <cstdlib>
#include <ctime>
#include "player.h"
#include "exPlayer.h"
#include "ohPlayer.h"
using namespace std;

// global variables
int turn;                          // whose turn is it?
int moves;                         // how many moves have already taken place
square **table;                       // the current board
const int boardSize=10;            // the size of the square board (fixed at 10x10)
const int moveTime=2;              // number of seconds between moves
const int setupTime=5;             // number of seconds for setup

// auxilliary functions
inline int min(int a, int b) {
  if (a<b) return a; else return b;
}

inline int max(int a, int b) {
  if (a<b) return b; else return a;
}

bool wonQ() {
  // %E:  Teturns true iff there are five X's or five O's in a row.

  int ii,jj;

  // check horizontal
  int streak=1;
  for (ii=0; ii<boardSize; ii++,streak=1)
    for (jj=1; jj<boardSize; jj++)
      if ((table[ii][jj]!=blank) && (table[ii][jj]==table[ii][jj-1]))
	{if (++streak==5) // we have a winner
	    return true;}
      else
	streak=1;
	
  //check vertical
  streak=1;
  for (jj=0; jj<boardSize && streak<5; jj++,streak=1)
    for (ii=1; ii<boardSize && streak<5; ii++)
      if ((table[ii][jj]!=blank) && (table[ii][jj]==table[ii-1][jj]))
	{if (++streak==5) // we have a winner
	    return true;}
      else
	streak=1;

  //check diagonal
  streak=1;
  int diff;
  for (diff=-boardSize+5; diff<=boardSize-5 && streak<5; diff++, streak=1)
    for (jj=max(1-diff,1); jj<min(boardSize-diff,boardSize) && streak<5; jj++)
      if ((table[jj][jj+diff]!=blank) && (table[jj][jj+diff]==table[jj-1][jj+diff-1]))
	{if (++streak==5) // we have a winner
	    return true;}
      else
	streak=1;

  //check other diagonal
  streak=1;
  int sum;
  for (sum=4; (sum<2*boardSize-5) && streak<5; sum++, streak=1)
    for (jj=max(1,sum-boardSize+2); jj<min(sum+1,boardSize) && streak<5; jj++)
      if ((table[jj][sum-jj]!=blank) && (table[jj][sum-jj]==table[jj-1][sum-jj+1]))
	{if (++streak==5) // we have a winner
	    return true;}
      else
	streak=1;

  return false;	 // i.e. no winner
}

void redrawBoard() {
  // %E:  Draws the current board position to standard output
  int ii,jj;
 
  cout << "Move " << moves << ":" << endl;
  for (jj=0; jj<boardSize; jj++) {
    for (ii=0; ii<boardSize; ii++)
      switch(table[ii][jj]) {
      case blank: cout << "-"; break;
      case oh: cout << "O"; break;
      case ex: cout << "X"; break;
      case unusable: cout << "#"; break;
      }
    cout << endl;
  }
  cout << endl;
}

inline bool checkBounds(int xx, int yy) {
  // %E:  Checks that a position is within the board boundaries
  if ((xx<0 || xx>boardSize-1) || (yy<0 || yy>boardSize-1))
    return false;
  return true;
}

void Sig(int i) {
  // %E:  Catches signals - if one side crashes or runs out of time, the other side wins.

  cout << "RUNTIME ERROR - signal " << i << endl;
  if (turn == ex) // give the game to whoever was still playing
    cout << "Player O wins" << endl;
  else
    cout << "Player X wins" << endl;
  exit(-1);
}

void TimeOver(int i) {
  cout << "TIME OUT!" << endl;
  if (turn == ex) // give the game to whoever was still playing
    cout << "Player O wins" << endl;
  else
    cout << "Player X wins" << endl;
  exit(-1);
}

void getRandomEmpty(int& xx, int &yy) {
  // %R:  At least one empty square exists on the board
  // %E:  Finds a random empty square on the board, and puts its coordinates into xx and yy
  do {
    xx = rand()%boardSize;
    yy = rand()%boardSize;
  }
  while (table[xx][yy]!=blank);
}

int main(int argc, char* argv[]) {
  int ii,jj;


  // 0. PRE- GAME INITIALIZATIONS
  // Set up random number generator
  srand(time(NULL));

  // Catch all signals (out of time, program crashes, etc.)
  signal(SIGALRM,&TimeOver);
  signal(SIGABRT,&Sig);
  signal(SIGBUS,&Sig);
  signal(SIGHUP,&Sig);
  signal(SIGILL,&Sig);
  signal(SIGINT,&Sig);
  signal(SIGKILL,&Sig);
  signal(SIGSEGV,&Sig);
  signal(SIGSYS,&Sig);

  // Initialize table
  table = new square*[boardSize];
  for (int ii=0; ii<boardSize; ii++) table[ii]=new square[boardSize];
  for (ii=0; ii<boardSize; ii++)
    for (jj=0; jj<boardSize; jj++)
      table[ii][jj]=blank;

  // Add up to 4 random X's, O's, and unusables to the board
  int randNum=rand()%4; // the number of extra entries
  for (ii=0; ii<randNum; ii++) {
    int xx,yy;
    getRandomEmpty(xx,yy);   table[xx][yy]=ex;
    getRandomEmpty(xx,yy);   table[xx][yy]=oh;
    getRandomEmpty(xx,yy);   table[xx][yy]=unusable;  
  }

  const int MAXMOVES = boardSize*boardSize - 3*randNum; // i.e. how many free squares are left

  // Initialize players
  player *A,*B;
	
  // ... set up X
  turn = ex;
  alarm(setupTime);
  A = new exPlayer(table);
  alarm(0); // i.e. cancel alarm

  // ... set up O
  turn = oh;
  alarm(setupTime);
  B = new ohPlayer(table);
  alarm(0); // i.e. cancel alarm

  // 1. THE GAME STARTS
  turn = ex; // first player to move
  redrawBoard(); // display the empty board
  do {
    int xx=0, yy=0;
    moves++;

    if (turn==ex) {
      // A's move
      boardSquare A_move;
      int xx,yy;

      alarm(moveTime); // allow 2 seconds per move
      A_move = A->nextMove(); xx=A_move.xx; yy=A_move.yy;
      alarm(0); // cancel alarm
      B->otherMove(A_move); // record the move for B
      if ((table[xx][yy]==blank) && checkBounds(xx,yy))
	table[xx][yy]=ex;
      else
	{ cout << "Invalid move by A:	 " << xx << ", " << yy << endl;
	  exit(-1);}
    }
    else {
      // B's move
      boardSquare B_move;
      int xx,yy;
      alarm(moveTime);
      B_move=B->nextMove(); xx=B_move.xx; yy=B_move.yy;
      alarm(0);
      A->otherMove(B_move); // record the move for A
      if ((table[xx][yy]==blank) && checkBounds(xx,yy))
	table[xx][yy]=oh;
      else
	{ cout << "Invalid move by B:	 " << xx << ", " << yy << endl;
	  exit(-1);}
    }
    redrawBoard();
    turn=(turn==ex?oh:ex); // change moves
  } while (!wonQ() && moves<MAXMOVES);

  // 3. RECORD THE WINNER
  if (moves==MAXMOVES)
    cout << "DRAW!!!!!!!!" << endl;
  else
    if (turn == ex) // recall that turn was changed in the last iteration
      cout << "Player O wins" << endl;
    else
      cout << "Player X wins" << endl;
}

#include <iostream>
#include <cstdlib>
#include "player.h"
#include "exPlayer.h"
#define size 10
#define boardSize 10

using namespace std;

boardSquare bs;

exPlayer::exPlayer(square **initTable) {
	myColor=ex;
    table = new int*[size];
    for (int ii=0; ii<size; ii++)
		table[ii]=new int[size];
    for (int ii=0; ii<size; ii++)
		for (int jj=0; jj<size; jj++)
			table[ii][jj]=initTable[ii][jj];
}

void exPlayer::otherMove(boardSquare bs) {
	table[bs.xx][bs.yy]=(myColor==ex?oh:ex);
}

void exPlayer::printBoard()
{

	cout << "THE EX BOARD" << endl;
  for (int j=0; j<boardSize; j++) {
    for (int i=0; i<boardSize; i++)
      switch(table[i][j]) {
      case blank: cout << "-"; break;
      case oh: cout << "O"; break;
      case ex: cout << "X"; break;
      case unusable: cout << "#"; break;
      }
    cout << endl;
  }
  cout << endl;
}

boardSquare exPlayer::nextMove() {
	int exCount = exCounter(ex);
	if((block(oh)) && (exCount != 4)) {
		cout << "BLOCKING" << endl;
		exCount = 0;
		return bestMove(oh);
	}
	else
		return bestMove(ex);

}

void exPlayer::scanBoard() {
	int exCount = 0;
	int ohCount = 0;
	int blankCount = 0;
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			if (table[i][j] == ex)
				exCount++;
			else if (table[i][j] == oh)
				ohCount++;			
			else
				blankCount++;
		}
	}
	cout << exCount << " X's" << endl;
	cout << ohCount << " O's" << endl;
	cout << blankCount << " blanks" << endl;
}
static int fiveFrame[5];
static int maxStart[2];
static int maxStartH[2];
static int maxStartV[2];
static int maxStartTR[2];
static int maxStartTL[2];

boardSquare exPlayer::bestMove(square color) {
	int exMaxH = 0;
	int exMaxV = 0;
	int exMaxTR = 0;
	int exMaxTL = 0;
	int exMax = 0;
	int exCount = 0;
	
	int blockXH = -1;
	int blockYH = -1;
	
	int blockXV = -1;
	int blockYV = -1;

	int blockXTR = -1;
	int blockYTR = -1;
	
	int blockXTL = -1;
	int blockYTL = -1;
	
	bool horizontal = false;
	bool vertical = false;
	bool topRight = false;
	bool topLeft = false;
	
	//horizontal
	for(int j = 0; j < 10; j++){
		for(int i = 0; i < 6; i++){
			for (int k = 0; k < 5; k++){
				fiveFrame[k] = table[i+k][j];
				if (fiveFrame[k] == color)
					exCount++;
				if ((fiveFrame[k] != color) && (fiveFrame[k] != blank)){
					exCount = 0;
					break;
				}
			}
			if ((fiveFrame[0] == blank) && (fiveFrame[1] == color) && (fiveFrame[2] == color) && (fiveFrame[3] == color) && (fiveFrame[4] == blank)) {
				blockXH = i;
				blockYH = j;
			}
			if (exCount > exMaxH){
				exMaxH = exCount;
				maxStartH[0] = i;
				maxStartH[1] = j;
			}
			exCount = 0;
		}
	}

	exCount = 0;
	//vertical
	for(int i = 0; i < 10; i++){
		for(int j = 0; j < 6; j++){
			for (int k = 0; k < 5; k++){
				fiveFrame[k] = table[i][j+k];
				if (fiveFrame[k] == color)
					exCount++;
				if ((fiveFrame[k] != color) && (fiveFrame[k] != blank)){
					exCount = 0;
					break;
				}
			}
			if ((fiveFrame[0] == blank) && (fiveFrame[1] == color) && (fiveFrame[2] == color) && (fiveFrame[3] == color) && (fiveFrame[4] == blank)) {
				blockXV = i;
				blockYV = j;
			}
			if (exCount > exMaxV){
				exMaxV = exCount;
				maxStartV[0] = i;
				maxStartV[1] = j;
			}
			exCount = 0;
		}
	}
	
	exCount = 0;
	//diagonal top left to bottom right
	for(int j = 0; j < 6; j++){
		for(int i = 0; i < 6; i++){
			for (int k = 0; k < 5; k++){
				fiveFrame[k] = table[i+k][j+k];
				if (fiveFrame[k] == color)
					exCount++;
				if ((fiveFrame[k] != color) && (fiveFrame[k] != blank)){
					exCount = 0;
					break;
				}
			}
			if ((fiveFrame[0] == blank) && (fiveFrame[1] == color) && (fiveFrame[2] == color) && (fiveFrame[3] == color) && (fiveFrame[4] == blank)) {
				blockXTL = i;
				blockYTL = j;
			}
			if (exCount > exMaxTL){
				exMaxTL = exCount;
				maxStartTL[0] = i;
				maxStartTL[1] = j;
			}
			exCount = 0;
		}
	}
	
	exCount = 0;
	//diagonal top right to bottom left
	for(int j = 0; j < 6; j++){
		for(int i = 9; i > 3; i--){
			for (int k = 0; k < 5; k++){
				fiveFrame[k] = table[i-k][j+k];
				if (fiveFrame[k] == color)
					exCount++;
				if ((fiveFrame[k] != color) && (fiveFrame[k] != blank)){
					exCount = 0;
					break;
				}
			}
			if ((fiveFrame[0] == blank) && (fiveFrame[1] == color) && (fiveFrame[2] == color) && (fiveFrame[3] == color) && (fiveFrame[4] == blank)) {
				blockXTR = i;
				blockYTR = j;
			}
			if (exCount > exMaxTR){
				exMaxTR = exCount;
				maxStartTR[0] = i;
				maxStartTR[1] = j;
			}
			exCount = 0;
		}
	}

	if (exMaxTL > exMax){
		exMax = exMaxTL;
		vertical = false;
		horizontal = false;
		topRight = false;
		topLeft = true;
		maxStart[0] = maxStartTL[0];
		maxStart[1] = maxStartTL[1];
	}
	if (exMaxTR > exMax){
		exMax = exMaxTR;
		vertical = false;
		horizontal = false;
		topRight = true;
		topLeft = false;
		maxStart[0] = maxStartTR[0];
		maxStart[1] = maxStartTR[1];
	}
	if (exMaxV > exMax){
		exMax = exMaxV;
		horizontal = false;
		vertical = true;
		topRight = false;
		topLeft = false;
		maxStart[0] = maxStartV[0];
		maxStart[1] = maxStartV[1];
	}
	if (exMaxH > exMax){
		exMax = exMaxH;
		vertical = false;
		horizontal = true;
		topRight = false;
		topLeft = false;
		maxStart[0] = maxStartH[0];
		maxStart[1] = maxStartH[1];
	}
	
	if ((exMax != 4) && (blockXH != -1) && (blockYH != -1)) {
		if (color == oh){
			cout << "Defensive Block" << endl;
			bs.xx = blockXH;
			bs.yy = blockYH;
			table[bs.xx][bs.yy]=ex;
			return bs;
		}
	}
	if ((exMax != 4) && (blockXV != -1) && (blockYV != -1)) {
		if (color == oh){
			cout << "Defensive Block" << endl;
			bs.xx = blockXV;
			bs.yy = blockYV;
			table[bs.xx][bs.yy]=ex;
			return bs;
		}
	}
	if ((exMax != 4) && (blockXTR != -1) && (blockYTR != -1)) {
		if (color == oh){
			cout << "Defensive Block" << endl;
			bs.xx = blockXTR;
			bs.yy = blockYTR;
			table[bs.xx][bs.yy]=ex;
			return bs;
		}
	}
	if ((exMax != 4) && (blockXTL != -1) && (blockYTL != -1)) {
		if (color == oh){
			cout << "Defensive Block" << endl;
			bs.xx = blockXTL;
			bs.yy = blockYTL;
			table[bs.xx][bs.yy]=ex;
			return bs;
		}
	}
	// if (horizontal)
		// cout << "horizontal" << endl;
	// if (vertical)
		// cout << "vertical" << endl;
	// if (topRight)
		// cout << "Top Right" << endl;
	// if (topLeft)
		// cout << "Top Left" << endl;
	
	 // cout << "Overall Max: " << exMax << " starting at " << maxStart[0] << "," << maxStart[1] << endl;
	
	
	if (exMax == 0) {
		cout << "Randomly Placing" << endl;
	  while (table[bs.xx][bs.yy]!=blank)
		{bs.xx = rand()%boardSize; bs.yy = rand()%boardSize; }
		table[bs.xx][bs.yy]=ex;
		return bs;
	}	
	
	for (int i = 0; i < 6; i++){
		if (horizontal){
			if(table[maxStart[0]+i][maxStart[1]] == blank){
				bs.xx = maxStart[0]+i;
				bs.yy = maxStart[1];
				table[bs.xx][bs.yy]=ex;
				return bs;
			}
		}
		else if (vertical){
			if(table[maxStart[0]][maxStart[1]+i] == blank){
				bs.xx = maxStart[0];
				bs.yy = maxStart[1]+i;
				table[bs.xx][bs.yy]=ex;
				return bs;
			}
		}
		else if (topLeft){
			if(table[maxStart[0]+i][maxStart[1]+i] == blank){
				bs.xx = maxStart[0]+i;
				bs.yy = maxStart[1]+i;
				table[bs.xx][bs.yy]=ex;
				return bs;
			}
		}
		else if (topRight){
			if(table[maxStart[0]-i][maxStart[1]+i] == blank){
				bs.xx = maxStart[0]-i;
				bs.yy = maxStart[1]+i;
				table[bs.xx][bs.yy]=ex;
				return bs;
			}
		}
	}
}

bool exPlayer::block(square color) {

	int exMaxH = 0;
	int exMaxV = 0;
	int exMaxTR = 0;
	int exMaxTL = 0;
	int exMax = 0;
	int exCount = 0;
	
	//horizontal
	for(int j = 0; j < 10; j++){
		for(int i = 0; i < 6; i++){
			for (int k = 0; k < 5; k++){
				fiveFrame[k] = table[i+k][j];
				if (fiveFrame[k] == color)
					exCount++;
				if ((fiveFrame[k] != color) && (fiveFrame[k] != blank)){
					exCount = 0;
					break;
				}
			}
			if ((fiveFrame[0] == blank) && (fiveFrame[1] == color) && (fiveFrame[2] == color) && (fiveFrame[3] == color) && (fiveFrame[4] == blank))
				return true;
				
			if (exCount > exMaxH){
				exMaxH = exCount;
				maxStartH[0] = i;
				maxStartH[1] = j;
			}
			exCount = 0;
		}
	}

	exCount = 0;
	//vertical
	for(int i = 0; i < 10; i++){
		for(int j = 0; j < 6; j++){
			for (int k = 0; k < 5; k++){
				fiveFrame[k] = table[i][j+k];
				if (fiveFrame[k] == color)
					exCount++;
				if ((fiveFrame[k] != color) && (fiveFrame[k] != blank)){
					exCount = 0;
					break;
				}
			}
			if ((fiveFrame[0] == blank) && (fiveFrame[1] == color) && (fiveFrame[2] == color) && (fiveFrame[3] == color) && (fiveFrame[4] == blank))
				return true;
				
			if (exCount > exMaxV){
				exMaxV = exCount;
				maxStartV[0] = i;
				maxStartV[1] = j;
			}
			exCount = 0;
		}
	}
	
	exCount = 0;
	//diagonal top left to bottom right
	for(int j = 0; j < 6; j++){
		for(int i = 0; i < 6; i++){
			for (int k = 0; k < 5; k++){
				fiveFrame[k] = table[i+k][j+k];
				if (fiveFrame[k] == color)
					exCount++;
				if ((fiveFrame[k] != color) && (fiveFrame[k] != blank)){
					exCount = 0;
					break;
				}
			}
			if ((fiveFrame[0] == blank) && (fiveFrame[1] == color) && (fiveFrame[2] == color) && (fiveFrame[3] == color) && (fiveFrame[4] == blank))
				return true;
				
			if (exCount > exMaxTL){
				exMaxTL = exCount;
				maxStartTL[0] = i;
				maxStartTL[1] = j;
			}
			exCount = 0;
		}
	}
	
	exCount = 0;
	//diagonal top right to bottom left
	for(int j = 0; j < 6; j++){
		for(int i = 9; i > 3; i--){
			for (int k = 0; k < 5; k++){
				fiveFrame[k] = table[i-k][j+k];
				if (fiveFrame[k] == color)
					exCount++;
				if ((fiveFrame[k] != color) && (fiveFrame[k] != blank)){
					exCount = 0;
					break;
				}
			}
			if ((fiveFrame[0] == blank) && (fiveFrame[1] == color) && (fiveFrame[2] == color) && (fiveFrame[3] == color) && (fiveFrame[4] == blank))
				return true;
			
			if (exCount > exMaxTR){
				exMaxTR = exCount;
				maxStartTR[0] = i;
				maxStartTR[1] = j;
			}
			exCount = 0;
		}
	}

	if (exMaxTL > exMax)
		exMax = exMaxTL;
		
	if (exMaxTR > exMax)
		exMax = exMaxTR;
		
	if (exMaxV > exMax)
		exMax = exMaxV;
		
	if (exMaxH > exMax)
		exMax = exMaxH;
		
	if(exMax == 4)
		return true;
	else
		return false;

}

int exPlayer::exCounter(square color) {

	static int fiveFrame1[5];
	static int maxStart1[2];
	static int maxStartH1[2];
	static int maxStartV1[2];
	static int maxStartTR1[2];
	static int maxStartTL1[2];

	int exMaxH = 0;
	int exMaxV = 0;
	int exMaxTR = 0;
	int exMaxTL = 0;
	int exMax = 0;
	int exCount = 0;
	
	bool vertical;
	bool horizontal;
	bool topRight;
	bool topLeft;
	
	//horizontal
	for(int j = 0; j < 10; j++){
		for(int i = 0; i < 6; i++){
			for (int k = 0; k < 5; k++){
				fiveFrame1[k] = table[i+k][j];
				if (fiveFrame1[k] == color)
					exCount++;
				if ((fiveFrame1[k] != color) && (fiveFrame1[k] != blank)){
					exCount = 0;
					break;
				}
			}
			if (exCount > exMaxH){
				exMaxH = exCount;
				maxStartH1[0] = i;
				maxStartH1[1] = j;
			}
			exCount = 0;
		}
	}

	exCount = 0;
	//vertical
	for(int i = 0; i < 10; i++){
		for(int j = 0; j < 6; j++){
			for (int k = 0; k < 5; k++){
				fiveFrame1[k] = table[i][j+k];
				if (fiveFrame1[k] == color)
					exCount++;
				if ((fiveFrame1[k] != color) && (fiveFrame1[k] != blank)){
					exCount = 0;
					break;
				}
			}
			if (exCount > exMaxV){
				exMaxV = exCount;
				maxStartV1[0] = i;
				maxStartV1[1] = j;
			}
			exCount = 0;
		}
	}
	
	exCount = 0;
	//diagonal top left to bottom right
	for(int j = 0; j < 6; j++){
		for(int i = 0; i < 6; i++){
			for (int k = 0; k < 5; k++){
				fiveFrame1[k] = table[i+k][j+k];
				if (fiveFrame1[k] == color)
					exCount++;
				if ((fiveFrame1[k] != color) && (fiveFrame1[k] != blank)){
					exCount = 0;
					break;
				}
			}
			if (exCount > exMaxTL){
				exMaxTL = exCount;
				maxStartTL1[0] = i;
				maxStartTL1[1] = j;
			}
			exCount = 0;
		}
	}
	
	exCount = 0;
	//diagonal top right to bottom left
	for(int j = 0; j < 6; j++){
		for(int i = 9; i > 3; i--){
			for (int k = 0; k < 5; k++){
				fiveFrame1[k] = table[i-k][j+k];
				if (fiveFrame1[k] == color)
					exCount++;
				if ((fiveFrame1[k] != color) && (fiveFrame1[k] != blank)){
					exCount = 0;
					break;
				}
			}
			if (exCount > exMaxTR){
				exMaxTR = exCount;
				maxStartTR1[0] = i;
				maxStartTR1[1] = j;
			}
			exCount = 0;
		}
	}

	if (exMaxTL > exMax){
		exMax = exMaxTL;
		vertical = false;
		horizontal = false;
		topRight = false;
		topLeft = true;
		maxStart1[0] = maxStartTL1[0];
		maxStart1[1] = maxStartTL1[1];
	}
	if (exMaxTR > exMax){
		exMax = exMaxTR;
		vertical = false;
		horizontal = false;
		topRight = true;
		topLeft = false;
		maxStart1[0] = maxStartTR1[0];
		maxStart1[1] = maxStartTR1[1];
	}
	if (exMaxV > exMax){
		exMax = exMaxV;
		horizontal = false;
		vertical = true;
		topRight = false;
		topLeft = false;
		maxStart1[0] = maxStartV1[0];
		maxStart1[1] = maxStartV1[1];
	}
	if (exMaxH > exMax){
		exMax = exMaxH;
		vertical = false;
		horizontal = true;
		topRight = false;
		topLeft = false;
		maxStart1[0] = maxStartH1[0];
		maxStart1[1] = maxStartH1[1];
	}
	
	return exMax;
	
}


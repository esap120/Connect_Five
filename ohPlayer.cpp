#include <iostream>
#include <cstdlib>
#include "player.h"
#include "ohPlayer.h"
#define size 10
#define boardSize 10

using namespace std;

boardSquare bs_oh;

ohPlayer::ohPlayer(square **initTable) {
	myColor=oh;
    table = new int*[size];
    for (int ii=0; ii<size; ii++)
		table[ii]=new int[size];
    for (int ii=0; ii<size; ii++)
		for (int jj=0; jj<size; jj++)
			table[ii][jj]=initTable[ii][jj];
}

void ohPlayer::otherMove(boardSquare bs_oh) {
	table[bs_oh.xx][bs_oh.yy]=(myColor==ex?oh:ex);
}

void ohPlayer::printBoard()
{

	cout << "THE OH BOARD" << endl;
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

boardSquare ohPlayer::nextMove() {
	int ohCount = ohCounter(oh);
	if((block(ex)) && (ohCount != 4)) {
		cout << "BLOCKING" << endl;
		ohCount = 0;
		return bestMove(ex);
	}
	else
		return bestMove(oh);

}

void ohPlayer::scanBoard() {
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
static int fiveFrame_oh[5];
static int maxStart_oh[2];
static int maxStartH_oh[2];
static int maxStartV_oh[2];
static int maxStartTR_oh[2];
static int maxStartTL_oh[2];

boardSquare ohPlayer::bestMove(square color) {
	int exMaxH = 0;
	int exMaxV = 0;
	int exMaxTR = 0;
	int exMaxTL = 0;
	int exMax = 0;
	int exCount = 0;
	
	bool horizontal = false;
	bool vertical = false;
	bool topRight = false;
	bool topLeft = false;
	
	int blockXH_oh = -1;
	int blockYH_oh = -1;
	
	int blockXV_oh = -1;
	int blockYV_oh = -1;

	int blockXTR_oh = -1;
	int blockYTR_oh = -1;
	
	int blockXTL_oh = -1;
	int blockYTL_oh = -1;
	
	//horizontal
	for(int j = 0; j < 10; j++){
		for(int i = 0; i < 6; i++){
			for (int k = 0; k < 5; k++){
				fiveFrame_oh[k] = table[i+k][j];
				if (fiveFrame_oh[k] == color)
					exCount++;
				if ((fiveFrame_oh[k] != color) && (fiveFrame_oh[k] != blank)){
					exCount = 0;
					break;
				}
			}
			if ((fiveFrame_oh[0] == blank) && (fiveFrame_oh[1] == color) && (fiveFrame_oh[2] == color) && (fiveFrame_oh[3] == color) && (fiveFrame_oh[4] == blank)) {
				blockXH_oh = i;
				blockYH_oh = j;
			}
			if (exCount > exMaxH){
				exMaxH = exCount;
				maxStartH_oh[0] = i;
				maxStartH_oh[1] = j;
			}
			exCount = 0;
		}
	}

	exCount = 0;
	//vertical
	for(int i = 0; i < 10; i++){
		for(int j = 0; j < 6; j++){
			for (int k = 0; k < 5; k++){
				fiveFrame_oh[k] = table[i][j+k];
				//cout << i << "," << j+k << " : " << table[i][j+k] << "    ";
				if (fiveFrame_oh[k] == color)
					exCount++;
				if ((fiveFrame_oh[k] != color) && (fiveFrame_oh[k] != blank)){
					exCount = 0;
					break;
				}
			}
			if ((fiveFrame_oh[0] == blank) && (fiveFrame_oh[1] == color) && (fiveFrame_oh[2] == color) && (fiveFrame_oh[3] == color) && (fiveFrame_oh[4] == blank)) {
				blockXV_oh = i;
				blockYV_oh = j;
			}
			//cout << endl;
			if (exCount > exMaxV){
				exMaxV = exCount;
				maxStartV_oh[0] = i;
				maxStartV_oh[1] = j;
			}
			exCount = 0;
		}
	}
	
	exCount = 0;
	//diagonal top left to bottom right
	for(int j = 0; j < 6; j++){
		for(int i = 0; i < 6; i++){
			for (int k = 0; k < 5; k++){
				fiveFrame_oh[k] = table[i+k][j+k];
				if (fiveFrame_oh[k] == color)
					exCount++;
				if ((fiveFrame_oh[k] != color) && (fiveFrame_oh[k] != blank)){
					exCount = 0;
					break;
				}
			}
			if ((fiveFrame_oh[0] == blank) && (fiveFrame_oh[1] == color) && (fiveFrame_oh[2] == color) && (fiveFrame_oh[3] == color) && (fiveFrame_oh[4] == blank)) {
				blockXTL_oh = i;
				blockYTL_oh = j;
			}
			if (exCount > exMaxTL){
				exMaxTL = exCount;
				maxStartTL_oh[0] = i;
				maxStartTL_oh[1] = j;
			}
			exCount = 0;
		}
	}
	
	exCount = 0;
	//diagonal top right to bottom left
	for(int j = 0; j < 6; j++){
		for(int i = 9; i > 3; i--){
			for (int k = 0; k < 5; k++){
				fiveFrame_oh[k] = table[i-k][j+k];
				if (fiveFrame_oh[k] == color)
					exCount++;
				if ((fiveFrame_oh[k] != color) && (fiveFrame_oh[k] != blank)){
					exCount = 0;
					break;
				}
			}
			if ((fiveFrame_oh[0] == blank) && (fiveFrame_oh[1] == color) && (fiveFrame_oh[2] == color) && (fiveFrame_oh[3] == color) && (fiveFrame_oh[4] == blank)) {
				blockXTR_oh = i;
				blockYTR_oh = j;
			}
			if (exCount > exMaxTR){
				exMaxTR = exCount;
				maxStartTR_oh[0] = i;
				maxStartTR_oh[1] = j;
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
		maxStart_oh[0] = maxStartTL_oh[0];
		maxStart_oh[1] = maxStartTL_oh[1];
	}
	if (exMaxTR > exMax){
		exMax = exMaxTR;
		vertical = false;
		horizontal = false;
		topRight = true;
		topLeft = false;
		maxStart_oh[0] = maxStartTR_oh[0];
		maxStart_oh[1] = maxStartTR_oh[1];
	}
	if (exMaxV > exMax){
		exMax = exMaxV;
		horizontal = false;
		vertical = true;
		topRight = false;
		topLeft = false;
		maxStart_oh[0] = maxStartV_oh[0];
		maxStart_oh[1] = maxStartV_oh[1];
	}
	if (exMaxH > exMax){
		exMax = exMaxH;
		vertical = false;
		horizontal = true;
		topRight = false;
		topLeft = false;
		maxStart_oh[0] = maxStartH_oh[0];
		maxStart_oh[1] = maxStartH_oh[1];
	}
	
	if ((exMax != 4) && (blockXH_oh != -1) && (blockYH_oh != -1)) {
		if (color == ex){
			cout << "Defensive Block Oh" << endl;
			bs_oh.xx = blockXH_oh;
			bs_oh.yy = blockYH_oh;
			table[bs_oh.xx][bs_oh.yy]=oh;
			return bs_oh;
		}
	}
	if ((exMax != 4) && (blockXV_oh != -1) && (blockYV_oh != -1)) {
		if (color == ex){
			cout << "Defensive Block Oh" << endl;
			bs_oh.xx = blockXV_oh;
			bs_oh.yy = blockYV_oh;
			table[bs_oh.xx][bs_oh.yy]=oh;
			return bs_oh;
		}
	}
	if ((exMax != 4) && (blockXTR_oh != -1) && (blockYTR_oh != -1)) {
		if (color == ex){
			cout << "Defensive Block Oh" << endl;
			bs_oh.xx = blockXTR_oh;
			bs_oh.yy = blockYTR_oh;
			table[bs_oh.xx][bs_oh.yy]=oh;
			return bs_oh;
		}
	}
	if ((exMax != 4) && (blockXTL_oh != -1) && (blockYTL_oh != -1)) {
		if (color == ex){
			cout << "Defensive Block Oh" << endl;
			bs_oh.xx = blockXTL_oh;
			bs_oh.yy = blockYTL_oh;
			table[bs_oh.xx][bs_oh.yy]=oh;
			return bs_oh;
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
	
	 // cout << "Overall Max: " << exMax << " starting at " << maxStart_oh[0] << "," << maxStart_oh[1] << endl;
	
	
	if (exMax == 0) {
		cout << "Randomly Placing" << endl;
	  while (table[bs_oh.xx][bs_oh.yy]!=blank)
		{bs_oh.xx = rand()%boardSize; bs_oh.yy = rand()%boardSize; }
		table[bs_oh.xx][bs_oh.yy]=oh;
		return bs_oh;
	}	
	
	for (int i = 0; i < 6; i++){
		if (horizontal){
			if(table[maxStart_oh[0]+i][maxStart_oh[1]] == blank){
				bs_oh.xx = maxStart_oh[0]+i;
				bs_oh.yy = maxStart_oh[1];
				table[bs_oh.xx][bs_oh.yy]=oh;
				return bs_oh;
			}
		}
		else if (vertical){
			if(table[maxStart_oh[0]][maxStart_oh[1]+i] == blank){
				bs_oh.xx = maxStart_oh[0];
				bs_oh.yy = maxStart_oh[1]+i;
				table[bs_oh.xx][bs_oh.yy]=oh;
				return bs_oh;
			}
		}
		else if (topLeft){
			if(table[maxStart_oh[0]+i][maxStart_oh[1]+i] == blank){
				bs_oh.xx = maxStart_oh[0]+i;
				bs_oh.yy = maxStart_oh[1]+i;
				table[bs_oh.xx][bs_oh.yy]=oh;
				return bs_oh;
			}
		}
		else if (topRight){
			if(table[maxStart_oh[0]-i][maxStart_oh[1]+i] == blank){
				bs_oh.xx = maxStart_oh[0]-i;
				bs_oh.yy = maxStart_oh[1]+i;
				table[bs_oh.xx][bs_oh.yy]=oh;
				return bs_oh;
			}
		}
	}
}

bool ohPlayer::block(square color) {

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
				fiveFrame_oh[k] = table[i+k][j];
				if (fiveFrame_oh[k] == color)
					exCount++;
				if ((fiveFrame_oh[k] != color) && (fiveFrame_oh[k] != blank)){
					exCount = 0;
					break;
				}
			}
			if ((fiveFrame_oh[0] == blank) && (fiveFrame_oh[1] == color) && (fiveFrame_oh[2] == color) && (fiveFrame_oh[3] == color) && (fiveFrame_oh[4] == blank))
				return true;
				
			if (exCount > exMaxH){
				exMaxH = exCount;
				maxStartH_oh[0] = i;
				maxStartH_oh[1] = j;
			}
			exCount = 0;
		}
	}

	exCount = 0;
	//vertical
	for(int i = 0; i < 10; i++){
		for(int j = 0; j < 6; j++){
			for (int k = 0; k < 5; k++){
				fiveFrame_oh[k] = table[i][j+k];
				if (fiveFrame_oh[k] == color)
					exCount++;
				if ((fiveFrame_oh[k] != color) && (fiveFrame_oh[k] != blank)){
					exCount = 0;
					break;
				}
			}
			if ((fiveFrame_oh[0] == blank) && (fiveFrame_oh[1] == color) && (fiveFrame_oh[2] == color) && (fiveFrame_oh[3] == color) && (fiveFrame_oh[4] == blank))
				return true;
				
			if (exCount > exMaxV){
				exMaxV = exCount;
				maxStartV_oh[0] = i;
				maxStartV_oh[1] = j;
			}
			exCount = 0;
		}
	}
	
	exCount = 0;
	//diagonal top left to bottom right
	for(int j = 0; j < 6; j++){
		for(int i = 0; i < 6; i++){
			for (int k = 0; k < 5; k++){
				fiveFrame_oh[k] = table[i+k][j+k];
				if (fiveFrame_oh[k] == color)
					exCount++;
				if ((fiveFrame_oh[k] != color) && (fiveFrame_oh[k] != blank)){
					exCount = 0;
					break;
				}
			}
			if ((fiveFrame_oh[0] == blank) && (fiveFrame_oh[1] == color) && (fiveFrame_oh[2] == color) && (fiveFrame_oh[3] == color) && (fiveFrame_oh[4] == blank))
				return true;
				
			if (exCount > exMaxTL){
				exMaxTL = exCount;
				maxStartTL_oh[0] = i;
				maxStartTL_oh[1] = j;
			}
			exCount = 0;
		}
	}
	
	exCount = 0;
	//diagonal top right to bottom left
	for(int j = 0; j < 6; j++){
		for(int i = 9; i > 3; i--){
			for (int k = 0; k < 5; k++){
				fiveFrame_oh[k] = table[i-k][j+k];
				if (fiveFrame_oh[k] == color)
					exCount++;
				if ((fiveFrame_oh[k] != color) && (fiveFrame_oh[k] != blank)){
					exCount = 0;
					break;
				}
			}
			if ((fiveFrame_oh[0] == blank) && (fiveFrame_oh[1] == color) && (fiveFrame_oh[2] == color) && (fiveFrame_oh[3] == color) && (fiveFrame_oh[4] == blank))
				return true;
			
			if (exCount > exMaxTR){
				exMaxTR = exCount;
				maxStartTR_oh[0] = i;
				maxStartTR_oh[1] = j;
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

int ohPlayer::ohCounter(square color) {

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


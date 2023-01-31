#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <ctype.h>

typedef char tictac_board[3][3];

// struct to get the row and col of a move, used for displaying where O is played
struct Move{
	int row, col;	
};

//sets the board from the commmand line arguments 
void set_board(tictac_board b, char **argv){
	int n = 0;
	for(int r = 0; r < 3; r++){
		for(int c = 0; c < 3; c++){
			// sets position on board to the first character of the char*
			b[r][c] = argv[n++][0];		
		}
	}
}

//sets a move on the board
bool set_move(tictac_board b, int r, int c, char turn){
	// if the inputs are not a number or are out of range, returns false
	if(!isdigit(r) && !isdigit(c) && (r > 2 || r < 0) && (c > 2 || c < 0)){		
		return false;
	}
	// if space is empty, the spot is filled with char
	if(b[r][c] == '?'){		
		b[r][c] = turn;
		return true;
	}
	// returns false if spot is already played
	return false;	
}

//prints board
void print_board(tictac_board b){
	printf("%c | %c | %c\n", b[0][0], b[0][1], b[0][2]);
	printf("--+---+--\n");
	printf("%c | %c | %c\n", b[1][0], b[1][1], b[1][2]);
	printf("--+---+--\n");
	printf("%c | %c | %c\n", b[2][0], b[2][1], b[2][2]);
}

//checks for row win
bool check_row_win(tictac_board b, char turn, int r){
	for(int c = 0; c < 3; c++){
		char ch = b[r][c];
		if(ch != turn){
			return false;
		}
	}
	return true;
}

//checks for column win
bool check_col_win(tictac_board b, char turn, int c){
	for(int r = 0; r < 3; r++){
		char ch = b[r][c];
		if(ch != turn){
			return false;
		}
	}
	return true;
}

//checks for diagonal win --> top left to bottom right
bool check_diag1_win(tictac_board b, char turn){
	if(b[0][0] == turn && b[1][1] == turn && b[2][2] == turn){
		return true;
	}

	return false;

}

//checks for diagonal win --> bottom left to top right
bool check_diag2_win(tictac_board b, char turn){
	if(b[2][0] == turn && b[1][1] == turn && b[0][2] == turn){
		return true;
	}

	return false;
}

//checks if there is a winning scenario on the board
bool check_win(tictac_board b, char turn){
	for(int r = 0; r < 3; r++){
		if(check_row_win(b, turn, r)){
			return true;
		}
	}

	for(int c = 0; c < 3; c++){
		if(check_col_win(b, turn, c)){
			return true;
		}
	}

	if(check_diag1_win(b, turn)){
		return true;
	}

	if(check_diag2_win(b, turn)){
		return true;
	}
	// returns false is there is no winning scenario found on board
	return false;	
}

//cheks if there is a draw 
bool check_draw(tictac_board b){
	for(int r = 0; r < 3; r++){
		for(int c = 0; c < 3; c++){
			// empty space means the game is not finished
			if(b[r][c] == '?'){		
				return false;
			}
		}
	}
	// if there is not winner, game result is a tie
	return true;	
}

//gets the value of the maximizer and minimizer
int getMinMaxVal(tictac_board board){
	char turn = 'X';
	// if X wins, returns 10: maximizer value
	if(check_win(board, turn)){		
		return 10;
	}
	else{
		turn = 'O';
		// if O wins, returns -10: minimizer value
		if(check_win(board, turn)){		
			return -10;
		}
	}

	return 0;
}

//goes through every available spot available on the board and decides which move is most optimal based on score
int minimax(tictac_board board, int depth, bool isMaximizer){
	int score = getMinMaxVal(board);	
	// scenario where the Maximizer wins, returns score
	if(score == 10){	
		return score;
	}
	// scenario where the Minimizer wins, returns score
	if(score == -10){	
		return score;
	}
	// if there is a draw, returns 0.
	if(check_draw(board)){	
		return 0;
	}
	// looks at X move
	if(isMaximizer){	
	
		int bestScore = -1000;	

		// goes through each available spot on the board and calculates the best score based on the effectiveness of a move
		for(int r = 0; r < 3; r++){		
			for(int c = 0; c < 3; c++){
				if(board[r][c] == '?'){
					// places X if spot on board is empty so it can be evaluated
					board[r][c] = 'X';	
					// calls minimax to find the final score of that move. See's how many moves it would take for game to finish
					int score = minimax(board, depth + 1, false);	
					// after score is calculated, position on board is reset to an empty spot 
					board[r][c] = '?';	
					if(score > bestScore){	
						// bestScore is the largest score found from all the possible moves
						bestScore = score;	
					}
				}
			}
		}

		return bestScore; 
		
	}
	// looks at O move
	else{	

		int bestScore = 1000;

		// goes through each available spot on the board and calculates the best score based on the effectiveness of a move
		for(int r = 0; r < 3; r++){
			for(int c = 0; c < 3; c++){
				if(board[r][c] == '?'){
					// places O if spot on board is empty so it can be evaluated
					board[r][c] = 'O';	
					// calls minimax to find the final score of that move. See's how many moves it would take for game to finish
					int score = minimax(board, depth + 1, true);		
					// after score is calculated, position on board is reset to an empty spot 
					board[r][c] = '?';	
					if(score < bestScore){
						// bestScore is the smallest score found from all the possible moves
						bestScore = score;	
					}
				}
			}
		}

		return bestScore; 

	}	
}

// calculates the best move for player or computer 
struct Move getBestMove(tictac_board board){
	int bestScore = 10000;
	// creates a new struct to store row and column values
	struct Move bestMove;	
	// assigned -1 because it is not a possible move on the board
	bestMove.row = -1;	
	// ^^
	bestMove.col = -1;	

	// goes through each available spot on the board and calculates the best move for Minimizer
	for(int r = 0; r < 3; r++){
		for(int c = 0; c < 3; c++){
			if(board[r][c] == '?'){
				board[r][c] = 'O';
				int val = minimax(board, 0, true);
				board[r][c] = '?';
				if(val < bestScore){	
					// assigns the row value based on value calculated from minimax
					bestMove.row = r;	
					// assigns the column value based on value calculated from minimax
					bestMove.col = c;	
					// bestScore is the smallest score found from all possible moves
					bestScore = val;	
				}
			}
		}
	}

	// has to calculate the best move for X after possibilites of O are found
	if(bestMove.row == -1 && bestMove.col == -1){	// 
		for(int r = 0; r < 3; r++){
			for(int c = 0; c < 3; c++){
				if(board[r][c] == '?'){
					board[r][c] = 'X';
					int val = minimax(board, 0, false);
					board[r][c] = '?';
					if(val > bestScore){
						bestMove.row = r;
						bestMove.col = c;
						bestScore = val;
					}
				}
			}
		}
	}

	return bestMove;
}


int main(int argc, char* argv[]){
	if(argc == 1){
		// initializes an empty board
		tictac_board board = {{'?', '?', '?'}, {'?', '?', '?'},  {'?', '?', '?'}};	
		// first turn is the player: X
		char turn = 'X';	
		print_board(board);
		
		// infinie loop until broken by win or draw
		while(1){	
			int r, c;
			printf("%c: ", turn);
			// receives the move from player
			scanf("%d %d", &r, &c);	
			
			// checks if move is legal
			if(set_move(board, r, c, turn)){
				// checks if there is a win
				if(check_win(board, turn)){		
					print_board(board);
					printf("%c wins\n", turn);
					// ends infinite loop
					break;	
				}
				// checks if there is a draw
				else if(check_draw(board)){	
					print_board(board);
					printf("draw\n");
					// ends infinite loop
					break;	
				}
				
				// swithces turn to computer: O
				if(turn == 'X'){	
					turn = 'O';
				}

				if(turn == 'O'){	
					// creates struct to get row and column
					struct Move theMove;	
					// loops for best move 
					theMove = getBestMove(board);	 
					int row = theMove.row;	
					int col = theMove.col;
					// checks if move is legal
					if(set_move(board, row, col, turn)){	
						// statement to show where O moved
						printf("%c moves to %d %d\n", turn, row, col);	
						// checks if there is a win
						if(check_win(board, turn)){		
							print_board(board);
							printf("%c wins\n", turn);
							// breaks infinite loop
							break;	
						}
						// checks if there is a draw
						else if(check_draw(board)){		
							print_board(board);
							printf("draw\n");	
							// breaks infinite loop
							break;	
						}
						
						// prints updated board
						print_board(board);	
					}
					
					// switches turn to player: X
					turn = 'X'; 	
				}
			}
			else{
				// if it not a legal move, prints message and gets a new input from player
				printf("Illegal Move\n");	
			}	
		}
	}
	
	// if board is partially filled 
	if(argc > 2){	
		// creates a new board
		tictac_board board;		
		// sets the value from command line to their positions on board
		set_board(board, &argv[1]);		
		print_board(board);

		// creates struct to get row and column
		struct Move theMove;	
		// loops for best move
		theMove = getBestMove(board);	
		int row = theMove.row;		
		int col = theMove.col;
		// sets O on the board after best move is calculated 
		if(set_move(board, row, col, 'O')){		
			printf("%c: %d %d\n", 'O', row, col);
		}
	}
}

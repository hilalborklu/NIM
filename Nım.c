#include <stdio.h>
#include <stdlib.h>

#define MAX_ROWS 5   // maximum number of rows

// Board structure to hold information about the matchsticks
struct Board {
    int num_rows;                // number of rows
    int num_matches[MAX_ROWS];   // number of matches in each row
};

// Player structure to interact with the board
struct Player {
    char name[50];      // name of the player
    int id;             // id of the player
};

// Initialize the board with the given number of rows and matches
void initializeBoard(struct Board *board, int num_rows);

//print the board before start the game
void printBoard(struct Board *board);

// Remove n matches from the specified row in the board
void removeMatches(struct Board *board, int row, int num_matches);

// Check if the game is over
int gameOver(struct Board *board);

// Play the game between two players
void playGame(struct Board *board, struct Player *p1, struct Player *p2);

// Play the game between computerr
void playGameComputer(struct Board *board, struct Player *p1, struct Player *p);


int main(){

    struct Board board;
     // initialize the players
    struct Player player1 = {"Player 1", 1};
    struct Player player2 = {"Player 2", 2};
    struct Player computer = {"Computer", 3};

    // initialize the board
    initializeBoard(&board, 4);

    // ask the user whether to play with two players or against the computer
    char choice;
    do {
        printf("Do you want to play with two players (enter 'p') or the computer (enter 'c')? ");
        scanf(" %c", &choice);
    } while (choice != 'p' && choice != 'c');

    if (choice == 'p') {
        // play with two players
        playGame(&board, &player1, &player2);
    }
    else {
        // play against the computer
        playGameComputer(&board, &player1, &computer);
    }

    return 0;
}


// Initialize the board with the given number of rows and matches
void initializeBoard(struct Board *board, int num_rows) {
    if (num_rows < 1 || num_rows > MAX_ROWS) {
        printf("Invalid number of rows. Please enter a number between 1 and %d.\n", MAX_ROWS);
        exit(1);
    }
    board->num_rows = num_rows;
    for (int i = 0; i < num_rows; i++) {
        board->num_matches[i] = 2 * i + 1;
    }
}
// Print the current state of the board
void printBoard(struct Board *board) {
    int num_spaces = 6;
    for (int i = 1; i <= board->num_rows; i++) {
        for (int j = 1; j <= num_spaces; j++) {
            printf(" ");
        }
        num_spaces--;
        for (int j = 1; j <= board->num_matches[i-1]; j++) {
            printf("I ");
        }
        printf("\n");
    }
}


// Remove num_matches matches from the given row
void removeMatches(struct Board *board, int row, int num_matches) {
    if (board->num_matches[row-1] < num_matches) {
        printf("Invalid number of matches. Please try again.\n");
        return;
    }
    for (int i = 0; i < num_matches; i++) {
        if (board->num_matches[row-1] > 0) {
            board->num_matches[row-1]--;
        }
    }
}

// Check if the game is over (i.e., all matches have been removed)
int gameOver(struct Board *board) {
    for (int i = 0; i < board->num_rows; i++) {
        if (board->num_matches[i] > 0) {
            return 0;   // game is not over
        }
    }
    return 1;   // game is over
}
 

 
// Play the game between two players
void playGame(struct Board *board, struct Player *p1, struct Player *p2) {
    struct Player *current_player = p1;
    int invalid_input;
    while (!gameOver(board)) {
        printf("\n%s's turn:\n", current_player->name);
        printBoard(board);
        int row, num_matches;
        do {
            invalid_input = 0;
            printf("Enter row and number of matches to remove: ");
            if (scanf("%d %d", &row, &num_matches) != 2) {
                invalid_input = 1;
                printf("Invalid input. Please enter two integers.\n");
                continue;
            }
            if (row < 1 || row > board->num_rows) {
                invalid_input = 1;
                printf("Invalid row. Please try again.\n");
                continue;
            }
            if (num_matches < 1 || num_matches > board->num_matches[row-1]) {
                invalid_input = 1;
                printf("Invalid number of matches. Please try again.\n");
                continue;
            }
            removeMatches(board, row, num_matches);
            break;
        } while (invalid_input || 1);
        if (gameOver(board)) {
            printf("\n%s wins!\n", current_player->name);
        }
        else {
            current_player = (current_player == p1) ? p2 : p1;   // switch players
        }
    }
    // The last player to make a move is the loser
    struct Player *loser = (current_player == p1) ? p2 : p1;
    printf("\n%s removed the last match and loses!\n", loser->name);
}


// Play the game between player and computer
void playGameComputer(struct Board *board, struct Player *p1, struct Player *p2) {
    struct Player *current_player = p1;
    int invalid_input;
    while (!gameOver(board)) {
        printf("\n%s's turn:\n", current_player->name);
        printBoard(board);
        int row, num_matches;
        if (current_player->id == 1) {  // human player
            do {
                invalid_input = 0;
                printf("Enter row and number of matches to remove: ");
                if (scanf("%d %d", &row, &num_matches) != 2) {
                    invalid_input = 1;
                    printf("Invalid input. Please enter two integers.\n");
                    continue;
                }
                if (row < 1 || row > board->num_rows) {
                    invalid_input = 1;
                    printf("Invalid row. Please try again.\n");
                    continue;
                }
                if (num_matches < 1 || num_matches > board->num_matches[row-1]) {
                    invalid_input = 1;
                    printf("Invalid number of matches. Please try again.\n");
                    continue;
                }
                removeMatches(board, row, num_matches);
                break;
            } while (invalid_input || 1);
        } else {  // computer player
            // select the optimal move
            int nim_sum = 0;
            for (int i = 0; i < board->num_rows; i++) {
                nim_sum ^= board->num_matches[i];
            }
            for (int i = 0; i < board->num_rows; i++) {
                if ((board->num_matches[i] ^ nim_sum) < board->num_matches[i]) {
                    row = i + 1;
                    num_matches = board->num_matches[i] - (board->num_matches[i] ^ nim_sum);
                    printf("%s removes %d matches from row %d.\n", current_player->name, num_matches, row);
                    removeMatches(board, row, num_matches);
                    break;
                }
            }
        }
        if (gameOver(board)) {
            printf("\n%s wins!\n", current_player->name);
        } else {
            current_player = (current_player == p1) ? p2 : p1;   // switch players
        }
    }
    // The last player to make a move is the loser
    struct Player *loser = (current_player == p1) ? p2 : p1;
    printf("\n%s removed the last match and loses!\n", loser->name);
}









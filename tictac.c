#include <stdio.h>
#include <ncurses.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 3

char board[SIZE][SIZE];

void init_board() {
    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++)
            board[i][j] = ' ';
}

void draw_board(int sel_row, int sel_col) {
    clear();
    mvprintw(0, 0, "Tic Tac Toe (Use arrow keys, Enter to mark, q to quit)");
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (i == sel_row && j == sel_col)
                attron(A_REVERSE);
            mvprintw(2 + i * 2, 4 + j * 4, " %c ", board[i][j]);
            attroff(A_REVERSE);
            if (j < SIZE - 1)
                mvprintw(2 + i * 2, 7 + j * 4, "|");
        }
        if (i < SIZE - 1)
            mvprintw(3 + i * 2, 4, "---+---+---");
    }
    refresh();
}

int check_winner(char player) {
    for (int i = 0; i < SIZE; i++) {
        if (board[i][0] == player && board[i][1] == player && board[i][2] == player)
            return 1;
        if (board[0][i] == player && board[1][i] == player && board[2][i] == player)
            return 1;
    }
    if (board[0][0] == player && board[1][1] == player && board[2][2] == player)
        return 1;
    if (board[0][2] == player && board[1][1] == player && board[2][0] == player)
        return 1;
    return 0;
}

int is_draw() {
    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++)
            if (board[i][j] == ' ')
                return 0;
    return 1;
}

void computer_move() {
    int empty_cells[SIZE * SIZE][2];
    int count = 0;
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (board[i][j] == ' ') {
                empty_cells[count][0] = i;
                empty_cells[count][1] = j;
                count++;
            }
        }
    }
    if (count > 0) {
        int idx = rand() % count;
        board[empty_cells[idx][0]][empty_cells[idx][1]] = 'O';
    }
}

int main() {
    initscr();
    curs_set(0);
    keypad(stdscr, TRUE);
    noecho();
    srand(time(0));

    init_board();
    int sel_row = 0, sel_col = 0;
    int game_over = 0;

    while (!game_over) {
        draw_board(sel_row, sel_col);
        mvprintw(10, 0, "Your turn (X). Use arrows, Enter to mark and q to quit.");
        int ch = getch();
        switch (ch) {
            case KEY_UP:    if (sel_row > 0) sel_row--; break;
            case KEY_DOWN:  if (sel_row < SIZE - 1) sel_row++; break;
            case KEY_LEFT:  if (sel_col > 0) sel_col--; break;
            case KEY_RIGHT: if (sel_col < SIZE - 1) sel_col++; break;
            case '\n':
            case KEY_ENTER:
                if (board[sel_row][sel_col] == ' ') {
                    board[sel_row][sel_col] = 'X';
                    if (check_winner('X')) {
                        draw_board(sel_row, sel_col);
                        mvprintw(12, 0, "You win! Press any key to exit.");
                        getch();
                        game_over = 1;
                        break;
                    } else if (is_draw()) {
                        draw_board(sel_row, sel_col);
                        mvprintw(12, 0, "It's a draw! Press any key to exit.");
                        getch();
                        game_over = 1;
                        break;
                    }
                    // Computer's move
                    computer_move();
                    if (check_winner('O')) {
                        draw_board(sel_row, sel_col);
                        mvprintw(12, 0, "Computer wins! Press any key to exit.");
                        getch();
                        game_over = 1;
                    } else if (is_draw()) {
                        draw_board(sel_row, sel_col);
                        mvprintw(12, 0, "It's a draw! Press any key to exit.");
                        getch();
                        game_over = 1;
                    }
                }
                break;
            case 'q':
            case 'Q':
                game_over = 1;
                break;
        }
    }
    endwin();
    return 0;
}
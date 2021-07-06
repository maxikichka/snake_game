#include <stdio.h>
#include <stdlib.h>
#include <curses.h>

int snake_length = 1;
int orig_timeUntillMove = 2000;
int score = 0;

_Bool includesPos(int snake[931][3], int x, int y) {
        //printf("%d %d %d %d", x, snake[0][1], y, snake[0][0]);
        for (int i = 0; i < snake_length; i++) {
                if (x == snake[i][1] && y == snake[i][0]) {
                        return 1;
                }
        }
        return 0;
}

_Bool didHeadHitBody(int snake[931][3]) {
        for (int i = 1; i < snake_length; i++) {
                if (snake[i][0] == snake[0][0] && snake[i][1] == snake[0][1]) {
                        return 1;
                }
        }
        return 0;
}
void addToBody(int snake[931][3]) {
        if (snake[snake_length - 1][2] == 1) {
                snake[snake_length][0] = snake[snake_length - 1][0] - 1;
                snake[snake_length][1] = snake[snake_length - 1][1];
                snake[snake_length][2] = snake[snake_length - 1][2];
        } else if (snake[snake_length - 1][2] == 2) {
                snake[snake_length][0] = snake[snake_length - 1][0] + 1;
                snake[snake_length][1] = snake[snake_length - 1][1];
                snake[snake_length][2] = snake[snake_length - 1][2];
        } else if (snake[snake_length - 1][2] == 3) {
                snake[snake_length][0] = snake[snake_length - 1][0];
                snake[snake_length][1] = snake[snake_length - 1][1] - 1;
                snake[snake_length][2] = snake[snake_length - 1][2];
        } else {
                snake[snake_length][0] = snake[snake_length - 1][0];
                snake[snake_length][1] = snake[snake_length - 1][1] + 1;
                snake[snake_length][2] = snake[snake_length - 1][2];
        }
        snake_length++;
}
void moveSnake(int snake[931][3], int aX, int aY) {
        int newSnake[931][3];
        //duplicate array
        for (int i = 0; i < snake_length; i++) {
                newSnake[i][0] = snake[i][0];
                newSnake[i][1] = snake[i][1];
                newSnake[i][2] = snake[i][2];
        }
        for (int i = 0; i < snake_length; i++) {
                //printf("%d\n\n", snake[i][2]);
                if (newSnake[i][2] == 1) {
                        if (i == 0) {
                                newSnake[i][0]++;
                        } else {
                                newSnake[i][0] = snake[i - 1][0];
                        }
                } else if (newSnake[i][2] == 2) {
                        if (i == 0) {
                                newSnake[i][0]--;
                        } else {
                                newSnake[i][0] = snake[i - 1][0];
                        }
                } else if (newSnake[i][2] == 3) {
                        if (i == 0) {
                                newSnake[i][1]--;
                        } else {
                                newSnake[i][1] = snake[i - 1][1];
                        }
                } else if (newSnake[i][2] == 4) {
                        if (i == 0) {
                                newSnake[i][1]++;
                        } else {
                                newSnake[i][1] = snake[i - 1][1];
                        }
                }
                if (i > 0) {
                        newSnake[i][2] = snake[i - 1][2];
                }
        }
        //update orginal array
        for (int i = 0; i < snake_length; i++) {
                snake[i][0] = newSnake[i][0];
                snake[i][1] = newSnake[i][1];
                snake[i][2] = newSnake[i][2];
        }
}
int main() {
        // 1: r, 2: l, 3: t, 4: b
        int width = 20;
        int height = 50;
        int board_area = (width - 1) * (height - 1);
        int snake_poses[931][3] = {{10, 13, 1}};
        int lastArrowPressed = snake_poses[0][2];
        int appleX = 18;
        int appleY = 13;
        int timeUntillMove = orig_timeUntillMove;
        initscr();
        noecho();
        keypad(stdscr, TRUE);
        nodelay(stdscr, TRUE);
        for (;;) {
                int k = getch();
                mvprintw(width + 5, 0, "Score: %d", score);
                if (k == 259 && snake_poses[0][2] != 4) {
                        snake_poses[0][2] = 3;
                } else if (k == 261 && snake_poses[0][2] != 2) {
                        snake_poses[0][2] = 1;
                } else if (k == 260 && snake_poses[0][2] != 1) {
                        snake_poses[0][2] = 2;
                } else if (k == 258 && snake_poses[0][2] != 3) {
                        snake_poses[0][2] = 4;
                }
                for (int i = 0; i < width; i++) {
                        for (int j = 0; j < height; j++) {
                                if (includesPos(snake_poses, i, j)) {
                                        mvprintw(i, j, "S");
                                } else if (j == appleX && i == appleY) {
                                        mvprintw(i, j, "A");
                                } else if (i == 0 || i == width - 1 || j == 0 || j == height - 1) {
                                        mvprintw(i, j, ".");
                                } else {
                                        mvprintw(i, j, " ");
                                }
                        }
                        mvprintw(i, height, "\n");
                }
                if (timeUntillMove == 0) {
                        moveSnake(snake_poses, appleX, appleY);
                        timeUntillMove = orig_timeUntillMove;
                }
                if (snake_poses[0][0] == appleX && snake_poses[0][1] == appleY) {
                        score++;
                        int randArr[931][2] = {};
                        int randArrLen = 0;
                        appleX = (rand() % ((height - 1) - 1 + 1)) + 1;
                        appleY = (rand() % ((width - 1) - 1 + 1)) + 1;
                        addToBody(snake_poses);
                } else if (snake_poses[0][0] == 0 || snake_poses[0][0] == height || snake_poses[0][1] == 0 || snake_poses[0][1] == width || didHeadHitBody(snake_poses)) {
                        break;
                }
                fflush(stdout);
                timeUntillMove--;
        }
        endwin();
        printf("\nGame over!!\n\nYour score is: %d\n", score);
}

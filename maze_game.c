#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <stdbool.h>

#define WIDTH 20
#define HEIGHT 20

char maze[HEIGHT][WIDTH];
int playerX, playerY;
int exitX, exitY;
int enemyX, enemyY;
int moves = 0;

int dx[] = {-1, 1, 0, 0};
int dy[] = {0, 0, -1, 1};

bool isSolvable() {
    bool visited[HEIGHT][WIDTH] = {false};
    int queue[HEIGHT * WIDTH][2];
    int front = 0, rear = 0;

    queue[rear][0] = playerX;
    queue[rear][1] = playerY;
    rear++;
    visited[playerX][playerY] = true;

    while (front < rear) {
        int x = queue[front][0];
        int y = queue[front][1];
        front++;

        if (x == exitX && y == exitY) return true;

        for (int i = 0; i < 4; i++) {
            int nx = x + dx[i];
            int ny = y + dy[i];

            if (nx >= 0 && nx < HEIGHT && ny >= 0 && ny < WIDTH &&
                !visited[nx][ny] && maze[nx][ny] != '#') {
                visited[nx][ny] = true;
                queue[rear][0] = nx;
                queue[rear][1] = ny;
                rear++;
            }
        }
    }

    return false;
}

void generateMaze(int difficulty) {
    while (1) {
        // Generate random maze based on difficulty
        for (int i = 0; i < HEIGHT; i++) {
            for (int j = 0; j < WIDTH; j++) {
                maze[i][j] = (rand() % 100 < difficulty) ? '#' : '.';
            }
        }

        // Place player at a random open spot
        do {
            playerX = rand() % HEIGHT;
            playerY = rand() % WIDTH;
        } while (maze[playerX][playerY] == '#');
        maze[playerX][playerY] = 'P';

        // Place exit at a random open spot
        do {
            exitX = rand() % HEIGHT;
            exitY = rand() % WIDTH;
        } while (maze[exitX][exitY] == '#' || (exitX == playerX && exitY == playerY));
        maze[exitX][exitY] = 'E';

        // Place enemy at a random open spot
        do {
            enemyX = rand() % HEIGHT;
            enemyY = rand() % WIDTH;
        } while (maze[enemyX][enemyY] == '#' || (enemyX == playerX && enemyY == playerY) || (enemyX == exitX && enemyY == exitY));
        maze[enemyX][enemyY] = 'X';

        // ✅ Ensure the maze is solvable
        if (isSolvable()) break;
    }
}

void printMaze() {
    printf("\033[H"); // Move cursor to top of terminal
    printf("=== MAZE GAME ===\n");
    printf("Use W/A/S/D to move. Reach 'E' to win!\n");
    printf("Avoid the enemy 'X'!\n\n");

    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            printf("%c ", maze[i][j]);
        }
        printf("\n");
    }

    printf("\nMoves: %d\n", moves);
    fflush(stdout);
}

void movePlayer(char input) {
    int newX = playerX;
    int newY = playerY;

    if (input == 'w') newX--;
    if (input == 's') newX++;
    if (input == 'a') newY--;
    if (input == 'd') newY++;

    if (newX >= 0 && newX < HEIGHT && newY >= 0 && newY < WIDTH && maze[newX][newY] != '#') {
        maze[playerX][playerY] = '.';

        playerX = newX;
        playerY = newY;

        if (playerX == exitX && playerY == exitY) {
            printMaze();
            printf("\n🏆 YOU WIN! Total Moves: %d 🏆\n", moves + 1);
            exit(0);
        }

        if (playerX == enemyX && playerY == enemyY) {
            printMaze();
            printf("\n💀 GAME OVER! You were caught by the enemy! 💀\n");
            exit(0);
        }

        maze[playerX][playerY] = 'P';
        moves++;
    }
}

void moveEnemy() {
    int bestX = enemyX;
    int bestY = enemyY;
    int bestDist = abs(playerX - enemyX) + abs(playerY - enemyY);

    for (int i = 0; i < 4; i++) {
        int nx = enemyX + dx[i];
        int ny = enemyY + dy[i];

        if (nx >= 0 && nx < HEIGHT && ny >= 0 && ny < WIDTH &&
            maze[nx][ny] != '#' && maze[nx][ny] != 'E') {
            int dist = abs(playerX - nx) + abs(playerY - ny);
            if (dist < bestDist) {
                bestX = nx;
                bestY = ny;
                bestDist = dist;
            }
        }
    }

    if (maze[bestX][bestY] != 'P') {
        maze[enemyX][enemyY] = '.';
        enemyX = bestX;
        enemyY = bestY;
        maze[enemyX][enemyY] = 'X';
    }
}

int main() {
    srand(time(NULL));
    system("clear");

    int choice;
    printf("Select Difficulty:\n");
    printf("1. Easy\n");
    printf("2. Medium\n");
    printf("3. Hard\n");
    printf("Enter choice: ");
    scanf("%d", &choice);

    // ✅ Balanced difficulty values
    int difficulty = (choice == 1) ? 20 : (choice == 2) ? 30 : 45;

    generateMaze(difficulty);
    printMaze();

    while (1) {
        char input;
        scanf(" %c", &input);
        movePlayer(input);
        moveEnemy();
        printMaze();

        usleep(100000);
    }

    return 0;
}

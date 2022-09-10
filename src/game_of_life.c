#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define HEIGHT_FIELD 27
#define WIDTH_FIELD 82
#define Default_Speed 1
#define MATRIX_WIDTH 80
#define MATRIX_HEIGHT 25
#define NARR 3

void new_basis(int arr[][MATRIX_WIDTH], int x, int y, int res[][MATRIX_WIDTH]);
int empty(int a[][NARR]);
int full(int a[][NARR]);
void Greeting();
void Field_Generate(int a[][MATRIX_WIDTH], int width, int height, int speed,
                    int gen);
int checkField(int a[][MATRIX_WIDTH], int res[][MATRIX_WIDTH]);
int exit_check(int arr[][MATRIX_WIDTH], int width, int height,
               int res[][MATRIX_WIDTH]);
void end();
void end2();

// Это мейн!
int main() {
  int speed = Default_Speed;
  int a[MATRIX_HEIGHT][MATRIX_WIDTH], res[MATRIX_HEIGHT][MATRIX_WIDTH];
  char c;
  initscr();
  nodelay(stdscr, TRUE);
  noecho();
  for (int i = 0; i < MATRIX_HEIGHT; i++) {
    for (int k = 0; k < MATRIX_WIDTH; k++) {
      c = getch();
      if (c == '0') {
        a[i][k] = 0;
      } else {
        a[i][k] = 1;
      }
      getch();
    }
  }
  if (!freopen("/dev/tty", "r", stdin)) {
    perror("freopen");
  }
  Greeting();
  int check = 0, gen = 0;
  do {
    clear();
    Field_Generate(a, MATRIX_WIDTH, MATRIX_HEIGHT, speed, gen);
    c = getch();
    if ((c == ',') && (speed > 1)) {
      speed -= 1;
    } else if ((c == '.') && (speed < 10)) {
      speed += 1;
    }
    check = checkField(a, res);
    usleep(250000 / (speed * 0.50));
    ++gen;
  } while ((check) && (c != 'q'));
  end();
  endwin();
  return 0;
}

// Прощание(
void end() {
  clear();
  printw("\n\n\n\n\n\n");
  printw("\t    /$$$$$$                                       /$$\n");
  printw("\t   /$$__  $$                                     |__/\n");
  printw(
      "\t  | $$  \\__/ /$$$$$$ /$$$$$$/$$$$  /$$$$$$        /$$ /$$$$$$$     "
      "   /$$$$$$ /$$    /$$/$$$$$$  /$$$$$$\n");
  printw(
      "\t  | $$ /$$$$|____  $| $$_  $$_  $$/$$__  $$      | $$/$$_____/      "
      " /$$__  $|  $$  /$$/$$__  $$/$$__  $$\n");
  printw(
      "\t  | $$|_  $$ /$$$$$$| $$ \\ $$ \\ $| $$$$$$$$      | $|  $$$$$$     "
      "  | $$  \\ $$\\  $$/$$| $$$$$$$| $$  \\__/\n");
  printw(
      "\t  | $$  \\ $$/$$__  $| $$ | $$ | $| $$_____/      | $$\\____  $$    "
      "  | $$  | $$ \\  $$$/| $$_____| $$\n");
  printw(
      "\t  |  $$$$$$|  $$$$$$| $$ | $$ | $|  $$$$$$$      | $$/$$$$$$$/      "
      "|  $$$$$$/  \\  $/ |  $$$$$$| $$\n");
  printw(
      "\t   \\______/ \\_______|__/ |__/ |__/\\_______/      |__|_______/    "
      "    \\______/    \\_/   \\_______|__/\n\n\n");
  printw("\t    /$$$$$$                         /$$/$$                        "
         "/$$\n");
  end2();
  refresh();
  usleep(2000000);
}

// Прощание 2
void end2() {
  printw("\t   /$$__  $$                       | $| $$                       | "
         "$$\n");
  printw(
      "\t  | $$  \\__/ /$$$$$$  /$$$$$$  /$$$$$$| $$$$$$$ /$$   /$$ /$$$$$$| "
      "$$\n");
  printw("\t  | $$ /$$$$/$$__  $$/$$__  $$/$$__  $| $$__  $| $$  | $$/$$__  $| "
         "$$\n");
  printw("\t  | $$|_  $| $$  \\ $| $$  \\ $| $$  | $| $$  \\ $| $$  | $| "
         "$$$$$$$|__/\n");
  printw("\t  | $$  \\ $| $$  | $| $$  | $| $$  | $| $$  | $| $$  | $| "
         "$$_____/\n");
  printw("\t  |  $$$$$$|  $$$$$$|  $$$$$$|  $$$$$$| $$$$$$$|  $$$$$$|  "
         "$$$$$$$/$$\n");
  printw("\t   \\______/ \\______/ \\______/ \\_______|_______/ \\____  "
         "$$\\_______|__/\n");
  printw("\t                                                /$$  | $$\n");
  printw("\t                                               |  $$$$$$/\n");
  printw("\t                                                \\______/\n");
}

// Проверка на повторение поколений
int exit_check(int arr[][MATRIX_WIDTH], int width, int height,
               int res[][MATRIX_WIDTH]) {
  int check = 0;
  int sum = 0;
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      if (arr[i][j] == res[i][j]) {
        sum++;
      }
      if (res[i][j] == 1) {
        check = 1;
      }
    }
  }
  if (sum == MATRIX_WIDTH * MATRIX_HEIGHT) {
    check = 0;
  }
  return check;
}

// Проверка всех клеток поля
int checkField(int a[][MATRIX_WIDTH], int res[][MATRIX_WIDTH]) {
  for (int y = 0; y < MATRIX_HEIGHT; ++y) {
    for (int x = 0; x < MATRIX_WIDTH; ++x) {
      new_basis(a, x, y, res);
    }
  }
  int flag = exit_check(a, MATRIX_WIDTH, MATRIX_HEIGHT, res);
  for (int y = 0; y < MATRIX_HEIGHT; ++y) {
    for (int x = 0; x < MATRIX_WIDTH; ++x) {
      a[y][x] = res[y][x];
    }
  }
  return flag;
}

// Выделение клеток вокруг точки
void new_basis(int arr[][MATRIX_WIDTH], int x, int y, int res[][MATRIX_WIDTH]) {
  int check_matr[NARR][NARR];
  for (int i = 0, n = y - 1; i < 3; i++, n++) {
    for (int j = 0, k = x - 1; j < 3; j++, k++) {
      if (n == -1)
        n = MATRIX_HEIGHT - 1;
      if (n == MATRIX_HEIGHT)
        n = 0;
      if (k == -1)
        k = MATRIX_WIDTH - 1;
      if (k == MATRIX_WIDTH)
        k = 0;
      check_matr[i][j] = arr[n][k];
    }
  }
  if (arr[y][x] == 0) {
    res[y][x] = empty(check_matr);
  } else {
    res[y][x] = full(check_matr);
  }
}

// Поверка пустых клеток
int empty(int a[][NARR]) {
  int sum = 0, res;
  for (int i = 0; i < NARR; ++i) {
    for (int j = 0; j < NARR; ++j) {
      sum += a[i][j];
    }
  }
  if (sum == 3) {
    res = 1;
  } else {
    res = 0;
  }
  return res;
}

// Поверка заполненных клеток
int full(int a[][NARR]) {
  int sum = -1, res;
  for (int i = 0; i < NARR; ++i) {
    for (int j = 0; j < NARR; ++j) {
      sum += a[i][j];
    }
  }
  if ((sum < 2) || (sum > 3)) {
    res = 0;
  } else {
    res = 1;
  }
  return res;
}

// Печать поля
void Field_Generate(int a[][MATRIX_WIDTH], int width, int height, int speed,
                    int gen) {
  char c[] = {'S', 'p', 'e', 'e', 'd', 'V'};
  for (int i = 0; i < 2 * width + 1; ++i) {
    printw("=");
  }
  printw("\n");
  for (int i = 0; i < height; i++) {
    printw("|");
    for (int k = 0; k < width; k++) {
      if (a[i][k] == 0) {
        printw(" ");
      } else {
        printw("o");
      }
      if (k != width - 1) {
        printw(" ");
      }
    }
    printw("|");
    if ((i >= 0) && (i < 6))
      printw(" %c", c[i]);
    if ((i <= 2 * (speed + 5) - 5) && (i >= 6)) {
      printw("OO");
    }
    printw("\n");
  }
  for (int i = 0; i < 2 * width + 1; ++i) {
    printw("=");
  }
  if (speed == 10) {
    printw("OO");
  }
  printw("\nGeneration: %d\n", gen);
  printw("Faster - '.'\tSlower - ','\t");
  printw("Exit - 'q'\n");
  refresh();
}

// Преветствие
void Greeting() {
  clear();
  printw("\n\n\n\n\n\n");
  printw("\t    /$$$$$$                                                 "
         "/$$$$$$\n");
  printw("\t   /$$__  $$                                               /$$__  "
         "$$\n");
  printw("\t  | $$  \\__/ /$$$$$$ /$$$$$$/$$$$  /$$$$$$         /$$$$$$| $$  "
         "\\__/\n");
  printw(
      "\t  | $$ /$$$$|____  $| $$_  $$_  $$/$$__  $$       /$$__  $| $$$$\n");
  printw("\t  | $$|_  $$ /$$$$$$| $$ \\ $$ \\ $| $$$$$$$$      | $$  \\ $| "
         "$$_/\n");
  printw("\t  | $$  \\ $$/$$__  $| $$ | $$ | $| $$_____/      | $$  | $| $$\n");
  printw("\t  |  $$$$$$|  $$$$$$| $$ | $$ | $|  $$$$$$$      |  $$$$$$| $$\n");
  printw("\t   \\$$____/ /$$_/$$$$$$/ |__/ |__/\\_______/       "
         "\\______/|__/\n");
  printw("\n\n");
  printw("\t\t\t  | $$     |__//$$__  $$\n");
  printw("\t\t\t  | $$      /$| $$  \\__/$$$$$$\n");
  printw("\t\t\t  | $$     | $| $$$$  /$$__  $$\n");
  printw("\t\t\t  | $$     | $| $$_/ | $$$$$$$$\n");
  printw("\t\t\t  | $$     | $| $$   | $$_____/\n");
  printw("\t\t\t  | $$$$$$$| $| $$   |  $$$$$$$\n");
  printw("\t\t\t  |________|__|__/    \\_______/\n");
  refresh();
  usleep(1000000);
}

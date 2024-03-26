#include "console.h"
#include <ctime>
#include <cstdlib>
using namespace console;
using namespace std;

#define BOARD_SIZE 20
#define MOVE_DELAY 15
#define WALL_VERTICAL_STRING "┃"
#define WALL_HORIZONTAL_STRING "━"
#define WALL_RIGHT_TOP_STRING "┓"
#define WALL_LEFT_TOP_STRING "┏"
#define WALL_RIGHT_BOTTOM_STRING "┛"
#define WALL_LEFT_BOTTOM_STRING "┗"
#define SNAKE_STRING "■"
#define SNAKE_BODY_STRING "■"
#define APPLE_STRING "●"

int x;
int y;
Key direction;
Key nextInput;
clock_t start_t;
clock_t end_t;
int score = 0;

int blank[(BOARD_SIZE-2)*(BOARD_SIZE-2)][2];
int blanks;  // 공백의 개수
int snake[(BOARD_SIZE-2)*(BOARD_SIZE-2)][2];
int snakes;  // 뱀 개수(길이)

bool ateApple;
bool crashed;  // 몸통에 부딪혔는지

int apple[2];

int findBlank(int x, int y) {
  int index;
  for(index = 0 ; index < blanks ; index++) {
    if(blank[index][0] == x && blank[index][1] == y)
      return index;
  }
  return -1;
}
void blankAdd(int x, int y) {
  blank[blanks][0] = x;
  blank[blanks][1] = y;
  blanks++;
}
void blankDel() {
  int index = findBlank(x, y);
  if(index == -1) {
    crashed = true;
    return;
  }
  for(int i = index ; i < blanks-1 ; i++) {
    blank[i][0] = blank[i+1][0];
    blank[i][1] = blank[i+1][1];
  }
  blanks--;
}
void snakeAdd() {
  blankDel();
  snake[snakes][0] = x;
  snake[snakes][1] = y;
  snakes++;
}
void snakeDel() {
  blankAdd(snake[0][0], snake[0][1]);
  for(int i = 0 ; i < snakes-1 ; i++) {
    snake[i][0] = snake[i+1][0];
    snake[i][1] = snake[i+1][1];
  }
  snakes--;
}
void makeApple() {
  int randomPos = rand()%blanks;
  apple[0] = blank[randomPos][0];
  apple[1] = blank[randomPos][1];
}

void initGame() {
  x = BOARD_SIZE/2;
  y = BOARD_SIZE/2;
  // 시작은 오른쪽 방향으로
  direction = K_RIGHT;
  score = 0;
  blanks = 0;
  snakes = 0;
  
  // 공백에 해당하는 좌표 리스트 저장
  for(int r = 1 ; r < BOARD_SIZE-1 ; r++) {
    for(int c = 1 ; c < BOARD_SIZE-1 ; c++) {
      blankAdd(r, c);
    }
  }
  // 뱀에 해당하는 좌표 리스트 저장
  snakeAdd();

  ateApple = false;
  crashed = false;

  srand(time(NULL));
  makeApple();
}

void drawSquare() {
  draw(0, 0, WALL_LEFT_TOP_STRING);
  for(int i = 1 ; i < BOARD_SIZE-1 ; i++)
    draw(i, 0, WALL_HORIZONTAL_STRING);
  draw(BOARD_SIZE-1, 0, WALL_RIGHT_TOP_STRING);
  for(int i = 1 ; i < BOARD_SIZE-1 ; i++) {
    draw(0, i, WALL_VERTICAL_STRING);
    draw(BOARD_SIZE-1, i, WALL_VERTICAL_STRING);
  }
  draw(0, BOARD_SIZE-1, WALL_LEFT_BOTTOM_STRING);
  for(int i = 1 ; i < BOARD_SIZE-1 ; i++)
    draw(i, BOARD_SIZE-1, WALL_HORIZONTAL_STRING);
  draw(BOARD_SIZE-1, BOARD_SIZE-1, WALL_RIGHT_BOTTOM_STRING);
}
void drawApple() {
  draw(apple[0], apple[1], APPLE_STRING);
}

Key getInput(Key direction) {
  // 기존 진행 방향과 완전히 반대로 가려는 시도는
  // 없던 것으로 간주한다.(K_NONE)
  if(key(K_LEFT) && direction!=K_RIGHT)
    return K_LEFT;
  if(key(K_RIGHT) && direction!=K_LEFT)
    return K_RIGHT;
  if(key(K_UP) && direction!=K_DOWN)
    return K_UP;
  if(key(K_DOWN) && direction!=K_UP)
    return K_DOWN;
  if(key(K_ESC))
    return K_ESC;
  return K_NONE;
}

void handleInput(Key k) {
  if(k == K_LEFT)
    x--;
  if(k == K_RIGHT)
    x++;
  if(k == K_UP)
    y--;
  if(k == K_DOWN)
    y++;
}

void appleEat() {
  if(x == apple[0] && y == apple[1]) {
    ateApple = true;
    makeApple();
    score += 10;
  }
}
void snakeMove() {
  snakeAdd();
  if(!ateApple)
    snakeDel();
  ateApple = false;
}

void drawSnake() {
  for(int i = 0 ; i < snakes ; i++)
    draw(snake[i][0], snake[i][1], SNAKE_BODY_STRING);
}
void drawSnakeHead() {
  // x, y 위치에 *을 그린다.
  draw(x, y, SNAKE_STRING);
}
void printScore() {
  string scoreLine = "Score: " + to_string(score);
  draw(BOARD_SIZE/2-(scoreLine.length()/2), BOARD_SIZE, scoreLine);
}

bool gameOverCheck() {
  if(x <= 0 || x >= BOARD_SIZE-1 || y <= 0 || y >= BOARD_SIZE-1)
    return true;
  if(crashed)
    return true;
  return false;
}
void gameOverPrint() {
  if(blanks == 200)
    draw(BOARD_SIZE/2-4, BOARD_SIZE/2, "YOU WON!!");
  else
    draw(BOARD_SIZE/2-4, BOARD_SIZE/2, "YOU LOSE!");
  draw(BOARD_SIZE/2-9, BOARD_SIZE/2+1, "Try again? (Enter)");
}

void game() {
  // 콘솔 라이브러리를 초기화한다.
  init();
  // 게임 데이터를 초기화한다.
  initGame();

  while(!gameOverCheck()) {
    // 화면을 지운다.
    clear();
    // 보드를 그린다.
    drawSquare();
    drawApple();
    
    nextInput = getInput(direction);
    
    start_t = clock();
    while(true) {
      end_t = clock();
      if(end_t - start_t > MOVE_DELAY*1000/60)
        break;
    }
    // 입력이 있었다면 방향을 바꿈
    if(nextInput != K_NONE) {
      direction = nextInput;
      if(direction == K_ESC)
        return;
    }

    handleInput(direction);
    appleEat();
    snakeMove();

    drawSnake();
    drawSnakeHead();
    printScore();

    // 화면을 갱신하고 다음 프레임까지 대기한다.
    wait();
  }
  gameOverPrint();
  wait();
  while(!key(K_ENTER))
    if(key(K_ESC))
      return;
  game();
}
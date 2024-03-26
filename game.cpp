#include "console.h"
#include <windows.h>
#include <utility>
using namespace console;
using namespace std;

#define BOARD_SIZE 10
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

int x = BOARD_SIZE/2;
int y = BOARD_SIZE/2;

pair<int, int> blank[BOARD_SIZE*BOARD_SIZE];
pair<int, int> snake[BOARD_SIZE*BOARD_SIZE];

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

Key getInput(Key direction) {
  // 기존 진행 방향과 완전히 반대로 가려는 시도는
  // 없던 것으로 간주한다.(K_NONE)
  if (key(K_LEFT) && direction!=K_RIGHT)
    return K_LEFT;
  if (key(K_RIGHT) && direction!=K_LEFT)
    return K_RIGHT;
  if (key(K_UP) && direction!=K_DOWN)
    return K_UP;
  if (key(K_DOWN) && direction!=K_UP)
    return K_DOWN;
  return K_NONE;
}

void handleInput(Key k) {
  if (k == K_LEFT)
    x--;
  if (k == K_RIGHT)
    x++;
  if (k == K_UP)
    y--;
  if (k == K_DOWN)
    y++;
}

void restrictInScreen() {
  // x, y 위치를 화면의 최대 크기에서 벗어나지 않게 한다.
  if (x < 0)
    x = 0;
  if (x >= SCREEN_WIDTH)
    x = SCREEN_WIDTH - 1;
  if (y < 0)
    y = 0;
  if (y >= SCREEN_HEIGHT)
    y = SCREEN_HEIGHT - 1;
}

void drawSnake() {
  // x, y 위치에 *을 그린다.
  draw(x, y, SNAKE_STRING);
}

void gameOverCheck() {
  // x, y 위치를 화면의 최대 크기에서 벗어나지 않게 한다.
  if (x < 0)
    x = 0;
  if (x >= SCREEN_WIDTH)
    x = SCREEN_WIDTH - 1;
  if (y < 0)
    y = 0;
  if (y >= SCREEN_HEIGHT)
    y = SCREEN_HEIGHT - 1;
}

void game() {
  // 콘솔 라이브러리를 초기화한다.
  init();
  Key direction = K_NONE;
  Key nextInput;
  while (true) {
    // 화면을 지운다.
    clear();
    // 보드를 그린다.
    drawSquare();
    
    nextInput = getInput(direction);
    Sleep(MOVE_DELAY*1000/60);
    // 입력이 있었다면 방향을 바꿈
    if(nextInput != K_NONE)
      direction = nextInput;

    handleInput(direction);
    
    restrictInScreen();
    drawSnake();

    // 화면을 갱신하고 다음 프레임까지 대기한다.
    wait();
  }
}

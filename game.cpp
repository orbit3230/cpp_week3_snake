#include "console.h"
#include "windows.h"
using namespace console;

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

Key handleInput() {
  if (key(K_LEFT)) {
    x--;
    return K_LEFT;
  }
  if (key(K_RIGHT)) {
    x++;
    return K_RIGHT;
  }
  if (key(K_UP)) {
    y--;
    return K_UP;
  }
  if (key(K_DOWN)) {
    y++;
    return K_DOWN;
  }
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

void game() {
  // 콘솔 라이브러리를 초기화한다.
  init();
  Key direction;
  while (true) {
    // 화면을 지운다.
    clear();
    // 보드를 그린다.
    drawSquare();
    
    Sleep(MOVE_DELAY/60*1000);
    direction = handleInput();
    restrictInScreen();
    drawSnake();

    // 화면을 갱신하고 다음 프레임까지 대기한다.
    wait();
  }
}

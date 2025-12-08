#include "bong.h"

/* A Simple Pong Clone made in C with no prior tutorials */

int main() {
  // initialize scoreboard
  unsigned short P1 = 0;
  unsigned short P2 = 0;

  // create & set props of the projectiles
  obj projectile1;
  projectile1.icon = ball;
  projectile1.position.x = ball_pos_x;
  projectile1.position.y = (ball_pos_y) + 3;
  projectile1.size.x = ball_size_x;
  projectile1.size.y = ball_size_y;
  projectile1.velocity.x = LEFT;
  projectile1.velocity.y = DOWN;

  obj projectile2;
  projectile2.icon = ball;
  projectile2.position.x = ball_pos_x;
  projectile2.position.y = (ball_pos_y)-3;
  projectile2.size.x = ball_size_x;
  projectile2.size.y = ball_size_y;
  projectile2.velocity.x = RIGHT;
  projectile2.velocity.y = UP;

  // create & set the props of the paddles
  obj paddle1;
  paddle1.icon = "█ ";
  paddle1.position.x = paddle_inline;
  paddle1.position.y = paddle_inline + paddle_length;
  paddle1.size.x = paddle_thickness;
  paddle1.size.y = paddle_length;
  paddle1.velocity.x = NONE_X;
  paddle1.velocity.y = NONE_Y;

  obj paddle2;
  paddle2.icon = " █";
  paddle2.position.x = width - paddle_inline;
  paddle2.position.y = paddle_inline + paddle_length + 20;
  paddle2.size.x = paddle_thickness;
  paddle2.size.y = paddle_length;
  paddle2.velocity.x = NONE_X;
  paddle2.velocity.y = NONE_Y;

  // set terminal to raw mode
  set_raw_mode();

  // input buffer for user interaction
  char input = 0;

  // hide cursor
  printf("\e[?25l");

draw:
  // clear screen and redraw
  printf("\e[2J\e[H");

  printf("╭");
  for (s_z i = 0; i < width * 2; i++)
    printf("═");
  printf("╮\n");

  for (s_z i = 1; i <= height; i++) { // draw vertical axis
    printf("│");
    // putchar('0' + i);
    for (s_z j = 1; j <= width; j++) { // draw horizontal axis
      printf("  ");
      draw(&projectile1, j, i);
      draw(&projectile2, j, i);
      draw(&paddle1, j, i);
      draw(&paddle2, j, i);
    }
    printf("│\n");
  }

  printf("╰");
  for (s_z i = 0; i < width * 2; i++) {
    printf("═");
  }
  printf("╯");

  // Log Info:
  printf("\n  %sP1: %02d" RESET, (P1 > P2) ? GREEN : ((P1 < P2) ? RED : YELLOW),
         P1);
  for (s_z i = 0; i < (width * 2) - 14; i++)
    putchar(' ');
  printf("%sP2: %02d\n" RESET, (P2 > P1) ? GREEN : ((P2 < P1) ? RED : YELLOW),
         P2);

  // Print Controls:
  printf("  [W, S]");
  for (s_z i = 0; i < (width * 2) - 16; i++)
    putchar(' ');
  printf("[UP, DOWN]\n");

  for (s_z i = 0;
       // for context, 0.875 is the sum of 1/2, 1/4, and 1/8, more readable and
       // accurate than summing bitwise shifts.
       i < width * 0.875; i++)
    putchar(' ');
  printf("Press [Q] to Quit Game\n");

  // Handle User Input:
  if (kbhit()) {
    input = getchar();

    switch (input) {
    case 'w':
      for (tiny i = 1; i < paddle_speed; i++)
        (paddle1.position.y <= paddle1.size.y) ? 0 : (paddle1.position.y--);
      break;
    case 's':
      for (tiny i = 1; i < paddle_speed; i++)
        (paddle1.position.y >= height) ? 0 : (paddle1.position.y++);
      break;
    case 'q':
      goto end;
      break;
    case '\033':
      input = getchar();
      switch (input = getchar()) {
      case 'A':
        for (tiny i = 1; i < paddle_speed; i++)
          (paddle2.position.y <= paddle2.size.y) ? 0 : (paddle2.position.y--);
        break;
      case 'B':
        for (tiny i = 1; i < paddle_speed; i++)
          (paddle2.position.y >= height) ? 0 : (paddle2.position.y++);
        break;
      }
      break;
    }
  }

  // position updating and calculation
  collide(&projectile1, &paddle1);
  collide(&projectile1, &paddle2);

  collide(&projectile2, &paddle1);
  collide(&projectile2, &paddle2);

  // update the game state
  updateGame(&projectile1, &P1, &P2);
  updateGame(&projectile2, &P1, &P2);

  // wait and update game to not make it go batshit
  usleep(1000000 / speed);

  // loop continously until interrupt
  goto draw;

end:
  printf("\e[?25h");
  restore_mode();
  return 0;
}

void draw(obj *obj, s_z x, s_z y) {
  if (obj->position.x < x || obj->position.y < y)
    return;

  if (obj->position.x > x + obj->size.x - 1)
    return;
  if (obj->position.y > y + obj->size.y - 1)
    return;

  printf("\e[2D%s", obj->icon);
}

void checkCollision(obj *axis1, obj *axis2, axis dir) {
  s_z *axis1_pos1, *axis1_pos2, *axis1_size;
  char *axis1_vel;
  s_z *axis2_pos1, *axis2_pos2;
  s_z *axis2_size1, *axis2_size2;

  switch (dir) {
  case n:
    return;
  case x:
    axis1_pos1 = &axis1->position.x;
    axis2_pos1 = &axis2->position.x;
    axis1_size = &axis1->size.x;
    axis2_size1 = &axis2->size.x;
    axis1_vel = &axis1->velocity.x;

    axis1_pos2 = &axis1->position.y;
    axis2_pos2 = &axis2->position.y;
    axis2_size2 = &axis2->size.y;
    break;
  case y:
    axis1_pos1 = &axis1->position.y;
    axis2_pos1 = &axis2->position.y;
    axis1_size = &axis1->size.y;
    axis2_size1 = &axis2->size.y;
    axis1_vel = &axis1->velocity.y;

    axis1_pos2 = &axis1->position.x;
    axis2_pos2 = &axis2->position.x;
    axis2_size2 = &axis2->size.x;
    break;
  }

  for (s_z i = 1; i <= *axis2_size1; i++) {
    BOOL pos_check = (*axis1_pos1 == *axis2_pos1 - i);

    BOOL size_check = (*axis2_pos2 >= *axis1_pos2 &&
                       *axis2_pos2 <= *axis1_pos2 + *axis2_size2);

    if (pos_check && size_check)
      *axis1_vel = (dir == x) ? LEFT : UP;

    pos_check = (*axis1_pos1 - *axis1_size - 1 == *axis2_pos1 - i);

    if (pos_check && size_check)
      *axis1_vel = (dir == x) ? RIGHT : DOWN;
  }
}

void collide(obj *obj1, obj *obj2) {
  checkCollision(obj1, obj2, x);
  checkCollision(obj1, obj2, y);
}

void updateGame(obj *obj, unsigned short *P1, unsigned short *P2) {
  if (obj->position.y + 1 > height)
    obj->velocity.y = UP;
  if (obj->position.y <= obj->size.y)
    obj->velocity.y = DOWN;

  {
    BOOL RIGHT_BOUNDARY = obj->position.x + 1 > width && (*P1 += 1);
    BOOL LEFT_BOUNDARY = obj->position.x <= obj->size.x && (*P2 += 1);

    if (RIGHT_BOUNDARY || LEFT_BOUNDARY)
      obj->position.x = ball_pos_x;
  }

  obj->position.x += obj->velocity.x;
  obj->position.y += obj->velocity.y;
}

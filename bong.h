#include <stdio.h>

/* A Simple Pong Clone made in C with no prior tutorials */

#ifndef PONG_H
  #define PONG_H

  #define width 95
  #define height 38

  #define paddle_inline 3
  #define paddle_length 10
  #define paddle_thickness 1
  #define paddle_speed 5

  #define ball "██"
  #define ball_pos_x width >> 1
  #define ball_pos_y height >> 1
  #define ball_size_x 2
  #define ball_size_y 2

  #define RED "\e[31m"
  #define GREEN "\e[32m"
  #define YELLOW "\e[33m"
  #define RESET "\e[0m"

  #define speed 24

  #if width < 256 && height < 256
    typedef unsigned char s_z;
  #else
      #warning "width or height larger than 256, memory use will be inefficient!"
    typedef unsigned short s_z;
  #endif

  typedef unsigned char tiny;
  typedef enum : unsigned char { FALSE = 0, TRUE = 1 } BOOL;
  typedef enum : unsigned char { n, x, y } axis;
  typedef struct {
    char *icon;

    struct {
      s_z x;
      s_z y;
    } position;

    struct {
      s_z x;
      s_z y;
    } size;

    struct {
      enum : char { NONE_X = 0, RIGHT = 1, LEFT = -1 } x;
      enum : char { NONE_Y = 0, DOWN = 1, UP = -1 } y;
    } velocity;

  } obj;


  void draw(obj *theObj, s_z x, s_z y);
  void checkCollision(obj *axis1, obj *axis2, axis dir);
  void collide(obj *obj1, obj *obj2);
  void updateGame(obj *obj, unsigned short *P1, unsigned short *P2);


  #ifdef __WIN32
    #include <conio.h>
    #include <windows.h>
  
    #define kbhit _kbhit
    #define getchar _getch
  
  #elif defined(__unix__) || defined(__APPLE__)
    #include <unistd.h>
    #include <stdlib.h>
    #include <termios.h>

    static void set_raw_mode() {
      struct termios term;
      tcgetattr(STDIN_FILENO, &term);
      term.c_lflag &= ~(ICANON | ECHO);
      term.c_cc[VMIN] = 1;
      term.c_cc[VTIME] = 0;
      tcsetattr(STDIN_FILENO, TCSANOW, &term);
    }

    static void restore_mode() {
      struct termios term;
      tcgetattr(STDIN_FILENO, &term);
      term.c_lflag |= (ICANON | ECHO);
      tcsetattr(STDIN_FILENO, TCSANOW, &term);
    }

    static char kbhit() {
      struct timeval tv = {0};
      fd_set fds;
      FD_ZERO(&fds);
      FD_SET(STDIN_FILENO, &fds);
      tv.tv_sec = 0;
      tv.tv_usec = 0;
      return select(1, &fds, NULL, NULL, &tv) > 0;
    }
  #endif // !__WIN32

#endif // !PONG_H

#ifndef SNAKE_H
#define SNAKE_H

#include <ncurses/ncurses.h>
#include <stdint.h>
#include "scheduler.h"

enum {
    MAX_TAIL_SIZE = 100,
    START_TAIL_SIZE = 3,
    HEAD_SYMBOL = '@',
    TAIL_SYMBOL = '#',
    STOP_GAME = KEY_F(10),
    CONTROLS_COUNT = 2,
    SNAKES_COUNT = 2
};

enum {
    DIR_UP = 1,
    DIR_RIGHT = 2,
    DIR_DOWN = 3,
    DIR_LEFT = 4
};

// Здесь храним коды управления змейкой
typedef struct control_buttons
{
    int down;
    int up;
    int left;
    int right;
} control_buttons_t;

control_buttons_t arrow_controls = {KEY_DOWN, KEY_UP, KEY_LEFT, KEY_RIGHT};
control_buttons_t wasd_controls = {'s', 'w', 'a', 'd'};

/*
 Хвост это массив состоящий из координат x,y
 */
typedef struct tail_t
{
    char symbol;
    int x;
    int y;
} tail_t;

/*
 Голова змейки содержит в себе
 x,y - координаты текущей позиции
 direction - направление движения
 tsize - размер хвоста
 *tail -  ссылка на хвост
 */
typedef struct snake_t
{
    char symbol;
    int x;
    int y;
    int direction;
    size_t tsize;
    tail_t *tail;
    control_buttons_t *controls;
} snake_t;

typedef struct {
   char symbol;
   int x;
   int y;
} food_t;

typedef struct {
    snake_t *snakes;
    food_t *food;
    
    int *scores;
    int game_over;
    int should_quit;
} game_context_t;

typedef struct {
    Scheduler *scheduler;
    game_context_t *game;
} task_context_t;

void game_init(game_context_t *ctx);
void game_cleanup(game_context_t *ctx);

void generate_food(game_context_t *ctx);
int check_collision(const game_context_t *ctx);

#endif
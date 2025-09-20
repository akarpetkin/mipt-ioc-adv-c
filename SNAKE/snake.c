#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ncurses/ncurses.h>
#include <inttypes.h>
#include <string.h>
#include <unistd.h>

#define DEFAULT_HEAD_SYMBOL '@'
#define DEFAULT_TAIL_SYMBOL '#'
#define RENDER_DELAY 0.1  // Задержка между кадрами (секунды)

enum
{
    LEFT = 1,
    UP,
    RIGHT,
    DOWN,
    STOP_GAME = KEY_F(10)
};

enum
{
    MAX_TAIL_SIZE = 100,
    START_TAIL_SIZE = 3,
    MAX_FOOD_SIZE = 20,
    FOOD_EXPIRE_SECONDS = 10
};

// Здесь храним коды управления змейкой
typedef struct control_buttons
{
    int down;
    int up;
    int left;
    int right;
} control_buttons_t;

control_buttons_t default_controls = {KEY_DOWN, KEY_UP, KEY_LEFT, KEY_RIGHT};

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
    control_buttons_t controls;
} snake_t;

tail_t *initTail(size_t size, char symbol)
{
    tail_t *tail = (tail_t *)malloc(MAX_TAIL_SIZE * sizeof(tail_t));
    tail_t init_t = {symbol, 0, 0};

    for (size_t i = 0; i < size; i++)
    {
        tail[i] = init_t;
    }

    return tail;
}

snake_t *initSnake(size_t size, int x, int y)
{
    snake_t *head = (snake_t *)malloc(sizeof(snake_t));
    head->symbol = DEFAULT_HEAD_SYMBOL;
    head->x = x;
    head->y = y;
    head->direction = RIGHT;
    head->tail = initTail(MAX_TAIL_SIZE, DEFAULT_TAIL_SYMBOL); // прикрепляем к голове хвост
    head->tsize = size + 1;
    head->controls = default_controls;

    return head;
}

/*
 Движение головы с учетом текущего направления движения
 */
void go(snake_t *head)
{
    int max_x = 0, max_y = 0;
    getmaxyx(stdscr, max_y, max_x); // macro - размер терминала

    mvprintw(head->y, head->x, " "); // очищаем один символ
    switch (head->direction)
    {
        case LEFT:
            if (head->x <= 0)
                head->x = max_x;
            mvprintw(head->y, --(head->x), "%c", head->symbol);
            break;
        case RIGHT:
            if (head->x >= max_x)
                head->x = 0;
            mvprintw(head->y, ++(head->x), "%c", head->symbol);
            break;
        case UP:
            if (head->y <= 0)
                head->y = max_y;
            mvprintw(--(head->y), head->x, "%c", head->symbol);
            break;
        case DOWN:
            if (head->y >= max_y)
                head->y = 0;
            mvprintw(++(head->y), head->x, "%c", head->symbol);
            break;
        default:
            break;
    }

    refresh();
}

/*
 Движение хвоста с учетом движения головы
 */
void goTail(snake_t *head)
{
    mvprintw(head->tail[head->tsize - 1].y, head->tail[head->tsize - 1].x, " ");
    for (size_t i = head->tsize - 1; i > 0; i--)
    {
        head->tail[i] = head->tail[i - 1];
        if (head->tail[i].y || head->tail[i].x)
            mvprintw(head->tail[i].y, head->tail[i].x, "%c", head->tail[i].symbol);
    }
    head->tail[0].x = head->x;
    head->tail[0].y = head->y;
}

void changeDirection(snake_t *snake, const int32_t key)
{
    if (key == snake->controls.down && snake->direction != UP)
        snake->direction = DOWN;
    else if (key == snake->controls.up && snake->direction != DOWN)
        snake->direction = UP;
    else if (key == snake->controls.right && snake->direction != LEFT)
        snake->direction = RIGHT;
    else if (key == snake->controls.left && snake->direction != RIGHT)
        snake->direction = LEFT;
}

uint8_t checkCollision(snake_t *head)
{
    for (size_t i = 1; i < head->tsize - 1; i++)
    {
        if (head->tail[i].x == head->x && head->tail[i].y == head->y) 
        {
            return 1;
        }
    }

    return 0;
}

int main()
{
    initscr();
    keypad(stdscr, TRUE); // Включаем F1, F2, стрелки и т.д.
    nodelay(stdscr, TRUE);
    raw();                // Откдючаем line buffering
    noecho();             // Отключаем echo() режим при вызове getch
    curs_set(FALSE);      // Отключаем курсор

    snake_t *snake = initSnake(START_TAIL_SIZE, 10, 10);

    int max_x = 0, max_y = 0;
    getmaxyx(stdscr, max_y, max_x); // macro - размер терминала
    mvprintw(0, 0, "Use arrows for control. Press 'F10' for EXIT; Terminal size = %d, %d", max_x, max_y);

    clock_t last_time = clock();
    int key_pressed = 0;
    uint8_t game_over = 0;

    while (key_pressed != STOP_GAME && game_over == 0)
    {
        clock_t current_time = clock();
        double elapsed_time = (double)(current_time - last_time) / CLOCKS_PER_SEC;

         if (elapsed_time >= RENDER_DELAY) {
             key_pressed = getch(); // Считываем клавишу

             go(snake);
             goTail(snake);
             changeDirection(snake, key_pressed);

             game_over = checkCollision(snake);
             last_time = current_time;
        }
        
        usleep(10000);
    }

    free(snake->tail);
    free(snake);
    endwin(); // Завершаем режим curses mod
    return 0;
}
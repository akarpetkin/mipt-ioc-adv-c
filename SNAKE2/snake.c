#include "snake.h"
#include "scheduler.h"
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <ctype.h>

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
        case DIR_LEFT:
            if (head->x <= 0)
                head->x = max_x;
            mvprintw(head->y, --(head->x), "%c", head->symbol);
            break;
        case DIR_RIGHT:
            if (head->x >= max_x)
                head->x = 0;
            mvprintw(head->y, ++(head->x), "%c", head->symbol);
            break;
        case DIR_UP:
            if (head->y <= 0)
                head->y = max_y;
            mvprintw(--(head->y), head->x, "%c", head->symbol);
            break;
        case DIR_DOWN:
            if (head->y >= max_y)
                head->y = 0;
            mvprintw(++(head->y), head->x, "%c", head->symbol);
            break;
        default:
            break;
    }
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

void change_direction_task(void *context) {
    game_context_t *game = ((task_context_t *)context)->game;
    snake_t *snake = game->snake;

    int key = tolower(getch()); 
    for (int i = 0; i < 2; i++) {
        if (key == snake->controls[i].down && snake->direction != DIR_UP)
            snake->direction = DIR_DOWN;
        else if (key == snake->controls[i].up && snake->direction != DIR_DOWN)
            snake->direction = DIR_UP;
        else if (key == snake->controls[i].right && snake->direction != DIR_LEFT)
            snake->direction = DIR_RIGHT;
        else if (key == snake->controls[i].left && snake->direction != DIR_RIGHT)
            snake->direction = DIR_LEFT;
    }
    
    if (key == STOP_GAME)
         game->should_quit = 1;   

     mvprintw(1, 0, "key %d", key);     
}

void render_task(void *context) {
    game_context_t *game = ((task_context_t *)context)->game;

    int max_x = 0, max_y = 0;
    getmaxyx(stdscr, max_y, max_x); // macro - размер терминала

    if (game->game_over || game->should_quit) {
        mvprintw(max_y / 2, max_x / 2 - 5, "GAME OVER!");

        Scheduler *scheduler = ((task_context_t *)context)->scheduler;
        scheduler_stop(scheduler);
        return;
    }

    snake_t *snake = game->snake;
    food_t *food = game->food;
    
    go(snake);
    goTail(snake);
    
    // Еда
    mvprintw(food->y, food->x, "*");
    
    // Интерфейс
    mvprintw(0, 0, "Use arrows for control. Press 'F10' for EXIT; Score: %d", game->score);

     // Проверяем еду
    if (snake->x == food->x && snake->y == food->y) {
        if (MAX_TAIL_SIZE > game->snake->tsize) {
            game->snake->tsize++;
            game->score += 10;
        } else {
            game->game_over = 1;
        }
        
        generate_food(game);
    } 

    // Проверяем столкновения
    if (check_collision(game)) {
        game->game_over = 1;
    }
    
    refresh();
}

void game_init(game_context_t *game) {
    int max_x = 0, max_y = 0;
    getmaxyx(stdscr, max_y, max_x); // macro - размер терминала

    // Инициализация змейки
    snake_t *head = (snake_t *)malloc(sizeof(snake_t));
    head->symbol = HEAD_SYMBOL;
    head->x = max_x / 2;
    head->y = max_y / 2;
    head->direction = DIR_RIGHT;
    head->tail = initTail(MAX_TAIL_SIZE, TAIL_SYMBOL); // прикрепляем к голове хвост
    head->tsize = START_TAIL_SIZE + 1;

    control_buttons_t *buttons = (control_buttons_t *)malloc(sizeof(control_buttons_t) * 2);
    buttons[0] = arrow_controls;
    buttons[1] = wasd_controls;

    head->controls = buttons;
    
    game->snake = head;
    srand(time(NULL));

    food_t *food = (food_t *)malloc(sizeof(food_t));
    game->food = food;

    generate_food(game);
    
    game->score = 0;
    game->game_over = 0;
    game->should_quit = 0;
}

void game_cleanup(game_context_t *game) {
    free(game->snake->controls);
    free(game->snake->tail);
    free(game->snake);
    free(game->food);
}

void generate_food(game_context_t *game) {
    food_t *food = game->food;
    snake_t *snake = game->snake;

    int max_x = 0, max_y = 0;
    getmaxyx(stdscr, max_y, max_x); // macro - размер терминала
    
    int valid_position;
    do {
        food->x = rand() % (max_x - 2) + 1;
        food->y = rand() % (max_y - 2) + 1;
        
        valid_position = !(food->x == snake->x && food->y == snake->y);
        for (size_t i = 0; i < snake->tsize - 1; i++) {
            if (food->x == snake->tail[i].x && food->y == snake->tail[i].y) {
                valid_position = 0;
                break;
            }
        }
    } while (!valid_position);
}

int check_collision(const game_context_t *game) {
    snake_t *snake = game->snake;

    for (size_t i = 1; i < snake->tsize - 1; i++) {
        if (snake->tail[i].x == snake->x && snake->tail[i].y == snake->y)  {
            return 1;
        }
    }
    
    return 0;
}

int main() {
    // Инициализация ncurses
    initscr();
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);
    noecho();
    curs_set(0);
    
    // Инициализация игры
    game_context_t game;
    game_init(&game);
    
    // Инициализация планировщика
    Scheduler scheduler;
    scheduler_init(&scheduler);

    task_context_t *ctx = (task_context_t *)malloc(sizeof(task_context_t));
    ctx->game = &game;
    ctx->scheduler = &scheduler;

    // Добавление задач
    scheduler_add_task(&scheduler, change_direction_task, ctx, 0.01); 
    scheduler_add_task(&scheduler, render_task, ctx, 0.1);
    
    // Запуск игрового цикла
    scheduler_run(&scheduler);
    
    // Очистка
    nodelay(stdscr, FALSE);
    getch();

    game_cleanup(&game);
    endwin();
    
    return 0;
}
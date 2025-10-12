#include "snake.h"
#include "scheduler.h"
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <ctype.h>

tail_t *initTail(size_t size, char symbol) {
    tail_t *tail = (tail_t *)malloc(MAX_TAIL_SIZE * sizeof(tail_t));
    tail_t init_t = {symbol, 0, 0};

    for (size_t i = 0; i < size; i++) {
        tail[i] = init_t;
    }

    return tail;
}

/*
 Движение головы с учетом текущего направления движения
 */
void go(snake_t *head) {
    int max_x = 0, max_y = 0;
    getmaxyx(stdscr, max_y, max_x); // macro - размер терминала

    mvprintw(head->y, head->x, " "); // очищаем один символ
    switch (head->direction) {
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
void goTail(snake_t *head) {
    mvprintw(head->tail[head->tsize - 1].y, head->tail[head->tsize - 1].x, " ");
    for (size_t i = head->tsize - 1; i > 0; i--) {
        head->tail[i] = head->tail[i - 1];
        if (head->tail[i].y || head->tail[i].x)
            mvprintw(head->tail[i].y, head->tail[i].x, "%c", head->tail[i].symbol);
    }

    head->tail[0].x = head->x;
    head->tail[0].y = head->y;
}

void change_direction_task(void *context) {
    game_context_t *game = ((task_context_t *)context)->game;

    int key = tolower(getch()); 
    for (size_t i = 0; i < SNAKES_COUNT; i++) {
        snake_t *snake = &game->snakes[i];
        if (snake->controls == NULL) {
            continue;
        }

        for (int i = 0; i < CONTROLS_COUNT; i++) {
            if (key == snake->controls[i].down && snake->direction != DIR_UP)
                snake->direction = DIR_DOWN;
            else if (key == snake->controls[i].up && snake->direction != DIR_DOWN)
                snake->direction = DIR_UP;
            else if (key == snake->controls[i].right && snake->direction != DIR_LEFT)
                snake->direction = DIR_RIGHT;
            else if (key == snake->controls[i].left && snake->direction != DIR_RIGHT)
                snake->direction = DIR_LEFT;
        }
    }

    if (key == STOP_GAME)
         game->should_quit = 1;
}

void change_direction_auto_task(void *context) {
    game_context_t *game = ((task_context_t *)context)->game;
    food_t *food = game->food;

    for (size_t i = 0; i < SNAKES_COUNT; i++) {
        snake_t *snake = &game->snakes[i];
        if (snake->controls != NULL) {
            continue;
        }

        if ((snake->direction == DIR_RIGHT || snake->direction == DIR_LEFT) && (snake->y != food->y)) {  // горизонтальное движение
            snake->direction = (food->y > snake->y) ? DIR_DOWN : DIR_UP;
        } else if ((snake->direction == DIR_DOWN || snake->direction == DIR_UP) && (snake->x != food->x)) {  // вертикальное движение
            snake->direction = (food->x > snake->x) ? DIR_RIGHT : DIR_LEFT;
        }

    }
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
    
    food_t *food = game->food;
    // Еда
    mvprintw(food->y, food->x, "*");
    
    // Интерфейс
    mvprintw(0, 0, "Use arrows for control. Press 'F10' for EXIT; Score1: %d, Score2: %d", game->scores[0], game->scores[1]);
 
    for (size_t i = 0; i < SNAKES_COUNT; i++) {
        attron(COLOR_PAIR(i + 1));
        go(&game->snakes[i]);
        goTail(&game->snakes[i]);
        attroff(COLOR_PAIR(i + 1));
    }
    refresh();
}

void check_food_task(void *context) {
    game_context_t *game = ((task_context_t *)context)->game;
    food_t *food = game->food;

    for (size_t i = 0; i < SNAKES_COUNT; i++) {
        snake_t *snake = &game->snakes[i];

        // Проверяем еду
        if (snake->x == food->x && snake->y == food->y) {
            if (MAX_TAIL_SIZE > snake->tsize) {
                snake->tsize++;
                game->scores[i] += 10;
            
             } else {
                game->game_over = 1;
            }
        
            generate_food(game);
        } 
    }
}

void check_collision_task(void *context) {
     game_context_t *game = ((task_context_t *)context)->game;

    // Проверяем столкновения
    if (check_collision(game)) {
        game->game_over = 1;
    }
}

void game_init(game_context_t *game) {
    int max_x = 0, max_y = 0;
    getmaxyx(stdscr, max_y, max_x); // macro - размер терминала

    // Инициализация
    game->snakes = (snake_t *)malloc(SNAKES_COUNT * sizeof(snake_t));;

    // ручное управление
    snake_t manual;
    manual.symbol = HEAD_SYMBOL;
    manual.x = max_x / 2;
    manual.y = max_y / 2;
    manual.direction = DIR_RIGHT;
    manual.tail = initTail(MAX_TAIL_SIZE, TAIL_SYMBOL); // прикрепляем к голове хвост
    manual.tsize = START_TAIL_SIZE + 1;

    control_buttons_t *buttons = (control_buttons_t *)malloc(sizeof(control_buttons_t) * 2);
    buttons[0] = arrow_controls;
    buttons[1] = wasd_controls;

    manual.controls = buttons;
    
    // автопилот
    snake_t snakeAuto;
    snakeAuto.symbol = HEAD_SYMBOL;
    snakeAuto.x = max_x / 2 + 5;
    snakeAuto.y = max_y / 2;
    snakeAuto.direction = DIR_UP;
    snakeAuto.tail = initTail(MAX_TAIL_SIZE, TAIL_SYMBOL);
    snakeAuto.tsize = START_TAIL_SIZE + 1;
    snakeAuto.controls = NULL;
    
    game->snakes[0] = manual;
    game->snakes[1] = snakeAuto;

    srand(time(NULL));

    food_t *food = (food_t *)malloc(sizeof(food_t));
    game->food = food;

    generate_food(game);
    
    game->scores = (int *)malloc(SNAKES_COUNT * sizeof(int));
    game->scores[0] = 0;
    game->scores[1] = 0;

    game->game_over = 0;
    game->should_quit = 0;
}

void game_cleanup(game_context_t *game) {
    for (size_t i = 0; i < SNAKES_COUNT; i++) {
        free(game->snakes[i].controls);
        free(game->snakes[i].tail);
    }

    free(game->snakes);
    free(game->food);
}

void generate_food(game_context_t *game) {
    food_t *food = game->food;

    int max_x = 0, max_y = 0;
    getmaxyx(stdscr, max_y, max_x); // macro - размер терминала
    
    int valid_position;
    do {
        food->x = rand() % (max_x - 2) + 1;
        food->y = rand() % (max_y - 2) + 1;
        valid_position = 0;
        
        for (size_t i = 0; i < SNAKES_COUNT; i++) {
            snake_t *snake = &game->snakes[i];

            int curr_valid_position = !(food->x == snake->x && food->y == snake->y);
            for (size_t i = 0; i < snake->tsize - 1; i++) {
                if (food->x == snake->tail[i].x && food->y == snake->tail[i].y) {
                    curr_valid_position = 0;
                    break;
                }
            }
            valid_position += curr_valid_position;
        }

    } while (valid_position < 2);
}

int check_collision(const game_context_t *game) {
    for (size_t i = 0; i < SNAKES_COUNT; i++) {
        snake_t *snake = &game->snakes[i];

        for (size_t i = 1; i < snake->tsize - 1; i++) {
            if (snake->tail[i].x == snake->x && snake->tail[i].y == snake->y)  {
                return 1;
            }
        }
    }
    
    return 0;
}

int main() {
    // Инициализация ncurses
    initscr();
    noecho();
    curs_set(0);

    int max_x = 0, max_y = 0;
    getmaxyx(stdscr, max_y, max_x); 

    mvprintw(max_y / 2, max_x / 2 - 5, "Press any key to START!");
    getch();
    clear();
    refresh();

    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE); 

    /* Initialize all the colors */
    start_color();
    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    
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
    scheduler_add_task(&scheduler, change_direction_auto_task, ctx, 0.1); 
    scheduler_add_task(&scheduler, render_task, ctx, 0.1);
    scheduler_add_task(&scheduler, check_food_task, ctx, 0.1);
    scheduler_add_task(&scheduler, check_collision_task, ctx, 0.1);

    // Запуск игрового цикла
    scheduler_run(&scheduler);
    
    // Очистка
    nodelay(stdscr, FALSE);
    getch();

    game_cleanup(&game);
    endwin();
    
    return 0;
}
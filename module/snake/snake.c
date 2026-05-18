#include <cli.h>
#include <scheduler.h>
#include <snake.h>
#include <stdio.h>
#include <utils.h>

#define FIELD_SIZE_X 30
#define FIELD_SIZE_Y 30
#define SNAKE_MAX_LEN 256

#define CHAR_BOARDER '#'
#define CHAR_HEAD 'X'
#define CHAR_BODY 'B'

enum snake_direction : uint8_t {
    DIRECTION_RIGHT,
    DIRECTION_LEFT,
    DIRECTION_UP,
    DIRECTION_DOWN
};

struct coordinate {
    uint8_t x;
    uint8_t y;
};

struct snake {
    struct coordinate cord_head;
    struct coordinate body_cordinates[SNAKE_MAX_LEN];
    uint8_t snake_len;
    enum snake_direction dir;
};

static volatile struct snake *snake_ptr = NULL;
static volatile struct coordinate apple = {.x = 10, .y = 10};
static volatile bool run = 1;
static uint8_t score = 0;

static void render_field() {
    if (!snake_ptr) {
        return;
    }

    char buffer[FIELD_SIZE_Y][FIELD_SIZE_X];
    for (unsigned y = 0; y < FIELD_SIZE_Y; y++) {
        for (unsigned x = 0; x < FIELD_SIZE_X; x++) {
            buffer[y][x] = ' ';
        }
    }

    if (apple.y < FIELD_SIZE_Y && apple.x < FIELD_SIZE_X) {
        buffer[apple.y][apple.x] = 'O';
    }

    for (int i = 0; i < snake_ptr->snake_len; i++) {
        uint8_t bx = snake_ptr->body_cordinates[i].x;
        uint8_t by = snake_ptr->body_cordinates[i].y;
        if (bx < FIELD_SIZE_X && by < FIELD_SIZE_Y) {
            buffer[by][bx] = CHAR_BODY;
        }
    }

    uint8_t hx = snake_ptr->cord_head.x;
    uint8_t hy = snake_ptr->cord_head.y;
    if (hx < FIELD_SIZE_X && hy < FIELD_SIZE_Y) {
        buffer[hy][hx] = CHAR_HEAD;
    }

    printf("\033[1;1H" C_B_WHITE);
    for (unsigned x = 0; x < FIELD_SIZE_X + 2; x++) {
        putchar(CHAR_BOARDER);
    }

    for (unsigned y = 0; y < FIELD_SIZE_Y; y++) {
        printf("\033[%d;1H" C_B_WHITE "%c", y + 2, CHAR_BOARDER);

        for (unsigned x = 0; x < FIELD_SIZE_X; x++) {
            if (buffer[y][x] == CHAR_HEAD) {
                printf(C_B_GREEN "%c", CHAR_HEAD);
            } else if (buffer[y][x] == CHAR_BODY) {
                printf(C_GREEN "%c", CHAR_BODY);
            } else if (buffer[y][x] == 'O') {
                printf(C_B_RED "%c", buffer[y][x]);
            } else {
                putchar(' ');
            }
        }

        printf(C_B_WHITE "%c", CHAR_BOARDER);
    }

    printf("\033[%d;1H" C_B_WHITE, FIELD_SIZE_Y + 2);
    for (unsigned x = 0; x < FIELD_SIZE_X + 2; x++) {
        putchar(CHAR_BOARDER);
    }

    printf("\033[%d;1H" C_B_YELLOW "Score: %d\033[K" C_RESET, FIELD_SIZE_Y + 3, score);

    fflush(stdout);
}

static void input_task() {
    while (run) {
        if (!snake_ptr) {
            continue;
        }
        int c = getchar();
        switch (c) {
        case 'w':
            if (snake_ptr->dir != DIRECTION_DOWN) {
                snake_ptr->dir = DIRECTION_UP;
            }
            break;
        case 's':
            if (snake_ptr->dir != DIRECTION_UP) {
                snake_ptr->dir = DIRECTION_DOWN;
            }
            break;
        case 'a':
            if (snake_ptr->dir != DIRECTION_RIGHT) {
                snake_ptr->dir = DIRECTION_LEFT;
            }
            break;
        case 'd':
            if (snake_ptr->dir != DIRECTION_LEFT) {
                snake_ptr->dir = DIRECTION_RIGHT;
            }
            break;
        case 3:
            run = 0;
            break;
        }
        sleep(10);
    }
}

static void snake_tick() {
    if (!snake_ptr) {
        return;
    }

    struct coordinate next_head = snake_ptr->cord_head;

    switch (snake_ptr->dir) {
    case DIRECTION_UP:
        next_head.y--;
        break;
    case DIRECTION_DOWN:
        next_head.y++;
        break;
    case DIRECTION_LEFT:
        next_head.x--;
        break;
    case DIRECTION_RIGHT:
        next_head.x++;
        break;
    }

    if (next_head.x >= FIELD_SIZE_X) {
        next_head.x = (snake_ptr->dir == DIRECTION_LEFT) ? FIELD_SIZE_X - 1 : 0;
    }
    if (next_head.y >= FIELD_SIZE_Y) {
        next_head.y = (snake_ptr->dir == DIRECTION_UP) ? FIELD_SIZE_Y - 1 : 0;
    }

    for (int i = 0; i < snake_ptr->snake_len; i++) {
        if (snake_ptr->body_cordinates[i].x == next_head.x && snake_ptr->body_cordinates[i].y == next_head.y) {
            run = 0;
            return;
        }
    }

    struct coordinate old_tail;
    if (snake_ptr->snake_len > 0) {
        old_tail = snake_ptr->body_cordinates[snake_ptr->snake_len - 1];
    } else {
        old_tail = snake_ptr->cord_head;
    }

    for (int i = snake_ptr->snake_len - 1; i > 0; i--) {
        snake_ptr->body_cordinates[i] = snake_ptr->body_cordinates[i - 1];
    }
    if (snake_ptr->snake_len > 0) {
        snake_ptr->body_cordinates[0] = snake_ptr->cord_head;
    }

    snake_ptr->cord_head = next_head;

    if (next_head.x == apple.x && next_head.y == apple.y) {
        if (snake_ptr->snake_len < SNAKE_MAX_LEN) {
            snake_ptr->body_cordinates[snake_ptr->snake_len] = old_tail;
            snake_ptr->snake_len++;
        }

        score++;
        apple.x = (next_head.x * 7 + 13) % FIELD_SIZE_X;
        apple.y = (next_head.y * 11 + 17) % FIELD_SIZE_Y;
    }
}

static void snake_task() {
    cli_block_input();
    cli_block_output();
    score = 0;
    volatile struct snake s;
    snake_ptr = &s;
    run = 1;
    while (run) {
        snake_tick();
        render_field();
        sleep(100);
    }
    snake_ptr = NULL;

    printf("\033[?25h\033[0m\033[2J");
    fflush(stdout);
    cli_unblock_input();
    cli_unblock_output();
    cli_printf(C_B_GREEN "Score: %d\n" C_RESET, score);
}

void cmd_snake(int argc UNUSED, char **argv UNUSED) {
    cli_unblock_input();
    cli_unblock_output();
    sleep(5);
    scheduler_create_task(input_task, TASK_PRIORITY_HIGH);
    scheduler_create_task(snake_task, TASK_PRIORITY_HIGH);
}

#include <stdbool.h>
#include <stdio.h>

#define ROW 16
#define N_TOTAL_STEPS 256
#define N_WIN_STEPS 3

const char PLAYER[2] = {'A', 'B'};

typedef struct
{
    int x;
    int y;
} Position;

typedef struct
{
    int square[ROW][ROW];
    bool read_permission;
    bool write_permission;
} Map;

typedef struct
{
    Position location;
    Map map;
    int step_count;
    int observer[8];
    bool has_winner;
} Board;

void select_game_mode();

void init_board(Board *p_board);

void manager_human_vs_human(Board *);

void manager_human_vs_computer();

void get_input(Position *, bool *);

bool check_input(Board *, Position *);

void board_observer(Board *);

void judge_game(Board *);

int read_map(Board *, Position *);

void write_map(Board *);

void print_map(Board *);

// void print_map_test(Board *);

int main()
{
    // select game mode(human vs human or human vs computer)
    select_game_mode();
    int game_mode = 0;
    scanf("%d", &game_mode);
    while (1 != game_mode && 2 != game_mode)
    {
        printf("---------------------------------\n");
        printf("You can only choose:\n");
        printf("    1 for Human v.s. Human\n");
        printf("    2 for Human v.s. Computer\n");
        printf("Please input your choice again: ");
        while ((game_mode = getchar()) != EOF && game_mode != '\n')
            ;
        game_mode = (int)getchar();
    }

    // init chess board
    Board board;
    Board *ptr_board = &board;
    init_board(ptr_board);
    print_map(ptr_board);
    // print_map_test(ptr_board);

    if (1 == game_mode)
    {
        printf("\nYou have selected Human v.s. Human mode.\n");
        // game loop for human v.s. human
        while (ptr_board->has_winner == false && ptr_board->step_count < N_TOTAL_STEPS)
        {
            manager_human_vs_human(ptr_board);
            print_map(ptr_board);
            // print_map_test(ptr_board);
            ptr_board->map.read_permission = true;
            judge_game(ptr_board);
        }
        // 所有位置均被占满，但仍然没有胜者
        if (ptr_board->has_winner == false && ptr_board->step_count == N_TOTAL_STEPS)
        {
            manager_human_vs_computer();
            printf("==> All position has picked and still no winner, tie.\n");
        }
    }
    else
    {
        printf("to be continued\n");
    }
    return 0;
}

void select_game_mode()
{
    printf("         ---------------------------------\n");
    printf("         ======= It's time to play! ======\n");
    printf("         ---------------------------------\n");
    printf("Select the mode you wanna play:\n");
    printf("       1: Human v.s. Human\n");
    printf("       2: Human v.s. Computer\n");
    printf("Your choice is: ");
}

void init_board(Board *p_board)
{
    printf("\n");
    printf("         -------------------------------\n");
    printf("         ======= Welcome to play! ======\n");
    printf("         -------------------------------\n");
    printf("==> Rules you need to know:\n      1. A go first;\n      2. Your "
           "input should be shaped as \"x,y\"\n");
    printf("--------------------------------------------------\n");
    // step1: init the map
    for (int i = 0; i < ROW; i++)
    {
        for (int j = 0; j < ROW; j++)
        {
            p_board->map.square[i][j] = 2;
        }
    }
    // init observer array
    for (int i = 0; i < 8; i++)
    {
        p_board->observer[i] = 0;
    }
    p_board->map.write_permission = false;
    p_board->map.read_permission = false;

    // step2: init the position
    p_board->location.x = 20; // 20 is a wrong value
    p_board->location.y = 20; // avoid init position is written

    // step3: put the total step to 0
    p_board->step_count = 0;

    // step4: init has_winner flag to false
    p_board->has_winner = false;
}

void manager_human_vs_human(Board *p_board)
{
    // 确定当前步该由谁下子
    int step_counter = (p_board->step_count) % 2;
    char current_player = PLAYER[step_counter];
    printf("==> Player %c's turn              <Round:%d, step:%d>\n", current_player, (p_board->step_count) / 2 + 1,
           p_board->step_count + 1);
    printf("    %c's choice is:", current_player);

    p_board->map.read_permission = true;

    // 确保获得的输入位置是合法的
    bool ok = false;
    int tmp_x = 0, tmp_y = 0;
    Position tmp_position = {tmp_x, tmp_y};
    do
    {
        get_input(&tmp_position, &ok);
    } while (ok == false);

    // 确保输入的位置是可以下子的
    while (false == check_input(p_board, &tmp_position))
    {
        p_board->map.read_permission = true;
        get_input(&tmp_position, &ok);
    }
    p_board->location = tmp_position;
    p_board->map.write_permission = true;
    write_map(p_board);
}

void manager_human_vs_computer()
{
    printf("--> Computer's move is done!\n");
}

void get_input(Position *p_position, bool *p_ok)
{
    int c;
    int tmp_x, tmp_y;
    scanf("%d,%d", &tmp_x, &tmp_y);
    while ((c = getchar()) != EOF && c != '\n')
        ; //!!!清除输入缓冲区中的回车，耗了5个小时。。。
    if (tmp_x == 0 || tmp_y == 0)
    {
        printf("--> You should enter a number between 1 and 16\n    enter again:");
        *p_ok = false;
    }
    else
    {
        p_position->x = tmp_x - 1;
        p_position->y = tmp_y - 1;
        *p_ok = true;
    }
}

bool check_input(Board *p_board, Position *p_input)
{
    if (p_board->map.read_permission == false)
    {
        printf("Error: read map permission denied. From func: check_position\n");
        return false;
    }
    else
    {
        if (p_input->x < 0 || p_input->x > 16)
        {
            printf("--> The x you entered is out of range,\n    enter again:");
            p_board->map.read_permission = false;
            p_board->map.write_permission = false;
            return false;
        }
        else if (p_input->y < 0 || p_input->y > 16)
        {
            printf("--> The y you entered is out of range,\n    enter again:");
            p_board->map.read_permission = false;
            p_board->map.write_permission = false;
            return false;
        }
        else if (read_map(p_board, p_input) != 2)
        {
            printf("--> The location you choose is already picked,\n    enter "
                   "again:");
            p_board->map.read_permission = false;
            p_board->map.write_permission = false;
            return false;
        }
        else
        {
            p_board->map.read_permission = false;
            p_board->map.write_permission = false;
            return true;
        }
    }
}

void board_observer(Board *p_board)
{
    int player_mark = (p_board->step_count) % 2;
    int check_direct[8][2] = {{0, 1}, {1, 1}, {1, 0}, {1, -1}, {0, -1}, {-1, -1}, {-1, 0}, {-1, 1}};
    // 对observer数组置0
    for (int i = 0; i < 8; i++)
    {
        p_board->observer[i] = 0;
    }
    for (int i = 0; i < 8; ++i)
    {
        for (int j = 1; j < N_WIN_STEPS; ++j)
        {
            // 向每个方向跨j步远，统计那个位置上的情况
            if ((p_board->location.x + j * check_direct[i][0]) < 0 ||
                (p_board->location.x + j * check_direct[i][0]) > 15 ||
                (p_board->location.y + j * check_direct[i][1]) < 0 ||
                (p_board->location.y + j * check_direct[i][1]) > 15)
            {
                break; // 对非常规情况进行规避
            }
            else
            {
                int position_value = p_board->map.square[p_board->location.x + j * check_direct[i][0]]
                                                        [p_board->location.y + j * check_direct[i][1]];
                if (player_mark == position_value)
                {
                    p_board->observer[i] += 1;
                }
                else
                {
                    break;
                }
            }
        }
    }
}

void judge_game(Board *p_board)
{
    if (p_board->map.read_permission == false)
    {
        printf("Error: read map permission denied. From func: judge_game\n");
        return;
    }

    /*
        测试输出：记录最后一次落子位置周围8个方向上连续相同棋子数目的数组
        printf("~~> Test[judge_game]: the around of last pick\n");
        printf("    [ ");
        for (int l = 0; l < 8; ++l) {
            printf("%d ", same_count[l]);
        }
        printf("]\n");
    */

    board_observer(p_board);
    // 对获取的周围情况进行评判是否有获胜者产生
    for (int k = 0; k < 4; ++k)
    {
        if ((p_board->observer[k] + p_board->observer[k + 4]) == (N_WIN_STEPS - 1))
        {
            p_board->has_winner = true;
            break;
        }
        else
        {
            p_board->has_winner = false;
        }
    }
    if (p_board->has_winner == true)
    {
        printf("==> ===============================\n");
        printf("    |      The winner is %c        |\n", PLAYER[(p_board->step_count) % 2]);
        printf("    |    Congratulations !!!      |\n");
        printf("    |      %c used %d moves         |\n", PLAYER[(p_board->step_count) % 2],
               (int)((p_board->step_count + 1) / 2 + (p_board->step_count + 1) % 2));
        printf("    ===============================\n");
    }
    else
    {
        printf("==> No winner. Please continue...\n");
    }

    p_board->map.read_permission = false;
    ++p_board->step_count;
}

int read_map(Board *p_board, Position *p_position)
{
    if (p_board->map.read_permission == false)
    {
        printf("Error: read map permission denied. From func: read_map\n");
        return 2;
    }
    else
    {
        p_board->map.read_permission = false;
        p_board->map.write_permission = false;
        return p_board->map.square[p_position->x][p_position->y];
    }
}

void write_map(Board *p_board)
{
    if (p_board->map.write_permission == false)
    {
        printf("Error: write map permission denied. From func: write_map\n");
        return;
    }
    else
    {
        p_board->map.square[p_board->location.x][p_board->location.y] = (p_board->step_count) % 2;
        printf("==> Ok, player %c's input is (%d, %d)\n", PLAYER[(p_board->step_count) % 2], p_board->location.x + 1,
               p_board->location.y + 1);
        p_board->map.read_permission = false;
        p_board->map.write_permission = false;
    }
}

void print_map(Board *p_board)
{
    for (int i = 0; i < ROW + 1; i++)
    {
        for (int j = 0; j < ROW + 1; j++)
        {
            if (0 == i)
            {
                printf("%2d ", j);
            }
            else if (0 == j)
            {
                printf("%2d ", i);
            }
            else
            {
                switch (p_board->map.square[i - 1][j - 1])
                {
                case 0:
                    printf(" X ");
                    break;
                case 1:
                    printf(" O ");
                    break;
                case 2:
                    printf(" . ");
                    break;
                default:
                    printf("Error during print map");
                }
            }
        }
        printf("\n");
    }
    printf("--------------------------------------------------\n");
}

/*
void print_map_test(Board *p_board) {
    for (int i = 0; i < ROW + 1; i++) {
        for (int j = 0; j < ROW + 1; j++) {
            if (0 == i) {
                printf("%2d ", j);
            } else if (0 == j) {
                printf("%2d ", i);
            } else {
                printf(" %d ", p_board->map.square[i - 1][j - 1]);
            }
        }
        printf("\n");
    }
    printf("--------------------------------------------------\n");
}
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "chess_notations.h"

//~~~~~ 0-1.将棋子类型转换为记谱符号（如：KNIGHT -> 'N'）
static char piece_type_to_symbol(PieceType type) {
    switch (type) {
        case PAWN:   return 'P';
        case ROOK:   return 'R';
        case KNIGHT: return 'N';
        case BISHOP: return 'B';
        case QUEEN:  return 'Q';
        case KING:   return 'K';
        default:     return '?';
    }
}

// 0-2.将记谱符号转换为棋子类型（如：'N' -> KNIGHT）
static PieceType symbol_to_piece_type(char symbol) {
    switch (symbol) {
        case 'P': case 'p': return PAWN;
        case 'R': case 'r': return ROOK;
        case 'N': case 'n': return KNIGHT;
        case 'B': case 'b': return BISHOP;
        case 'Q': case 'q': return QUEEN;
        case 'K': case 'k': return KING;
        default:            return EMPTY;
    }
}

// ~~1.初始化棋盘（要是给上一步、下一步走到哪个格子的参数，就不用自己设置）
void init_board(ChessBoard board) {
    // 初始化所有格子为空
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            board[i][j].type = EMPTY;
            board[i][j].color = WHITE; // 颜色默认，后续会被覆盖
        }
    }
    
    // 白方棋子（第7行）
    board[7][0].type = ROOK;   board[7][0].color = WHITE;
    board[7][1].type = KNIGHT; board[7][1].color = WHITE;
    board[7][2].type = BISHOP; board[7][2].color = WHITE;
    board[7][3].type = QUEEN;  board[7][3].color = WHITE;
    board[7][4].type = KING;   board[7][4].color = WHITE;
    board[7][5].type = BISHOP; board[7][5].color = WHITE;
    board[7][6].type = KNIGHT; board[7][6].color = WHITE;
    board[7][7].type = ROOK;   board[7][7].color = WHITE;
                                  //可以改进为for实现

    // 白方兵（第6行）
    for (int j = 0; j < 8; j++) {
        board[6][j].type = PAWN; board[6][j].color = WHITE;
    }
    
    // 黑方兵（第1行）
    for (int j = 0; j < 8; j++) {
        board[1][j].type = PAWN; board[1][j].color = BLACK;
    }
    
    // 黑方棋子（第0行）
    board[0][0].type = ROOK;   board[0][0].color = BLACK;
    board[0][1].type = KNIGHT; board[0][1].color = BLACK;
    board[0][2].type = BISHOP; board[0][2].color = BLACK;
    board[0][3].type = QUEEN;  board[0][3].color = BLACK;
    board[0][4].type = KING;   board[0][4].color = BLACK;
    board[0][5].type = BISHOP; board[0][5].color = BLACK;
    board[0][6].type = KNIGHT; board[0][6].color = BLACK;
    board[0][7].type = ROOK;   board[0][7].color = BLACK;
}

// ~~~~~2-1.将坐标转换为代数记谱法字符串（如：(7,0) -> "a1"）
void coord_to_algebraic(int row, int col, char* output) {
    if (row < 0 || row > 7 || col < 0 || col > 7) {
        strcpy(output, "??");
        return;
    }
    
    // 列转换为字母（0->a, 1->b, ..., 7->h）
    char file = 'a' + col;
    // 行转换为数字（0->8, 1->7, ..., 7->1）
    char rank = '1' + (7 - row);
    
    output[0] = file;//记录下来记谱格式的字符串
    output[1] = rank;
    output[2] = '\0';
}

// 2-2.将代数记谱法字符串转换为坐标（如："a1" -> (7,0)）
bool algebraic_to_coord(const char* alg, int* row, int* col) {
    if (strlen(alg) != 2) {
        return false;
    }
    
    char file = alg[0];
    char rank = alg[1];
    
    // 检查文件和行是否有效
    if (file < 'a' || file > 'h' || rank < '1' || rank > '8') {
        return false;
    }
    
    // 转换为数组索引
    *col = file - 'a';
    *row = 7 - (rank - '1');
    
    return true;
}

// ~~~~~3.将走法转换为标准代数记谱
void move_to_algebraic(const Move* move, char* output) {
    // 初始化输出字符串
    char notation[20] = "";
    
    // 1. 处理王车易位
    if (move->is_castling) {
        if (move->to_col == 6) {  // 短易位
            strcpy(notation, "O-O");
        } else {  // 长易位
            strcpy(notation, "O-O-O");
        }
    } else {
        // 2. 处理普通走子
        // 2.1 处理棋子符号（兵不写符号）
        if (move->piece != PAWN) {
            char symbol = piece_type_to_symbol(move->piece);
            strncat(notation, &symbol, 1);
        }
        
        // 2.2 处理吃子
        if (move->captured.type != EMPTY) {
            if (move->piece == PAWN) {
                // 兵吃子需要写出发列
                char file = 'a' + move->from_col;
                strncat(notation, &file, 1);
            }
            strcat(notation, "x");
        }
        
        // 2.3 处理目标格子
        char target[3];
        coord_to_algebraic(move->to_row, move->to_col, target);
        strcat(notation, target);
        
        // 2.4 处理升变
        if (move->promotion != PAWN) {
            char promo_symbol = piece_type_to_symbol(move->promotion);
            char promo_str[3] = "=";
            promo_str[1] = promo_symbol;
            promo_str[2] = '\0';
            strcat(notation, promo_str);
        }
        
        // 2.5 处理吃过路兵（标准记谱常省略）
        /*
        if (move->is_en_passant) {
            strcat(notation, " e.p.");
        }
        */
    }
    
    // 3. 处理将军和将死
    if (move->is_checkmate) {
        strcat(notation, "#");
    } else if (move->is_check) {
        strcat(notation, "+");
    }
    
    // 复制到输出
    strcpy(output, notation);
}

// ~~~~~4.初始化棋局记录--每局一开始和结束，记谱只用清空
GameRecord* init_game_record(int initial_capacity) {
    if (initial_capacity <= 0) {
        initial_capacity = 32; // 默认容量
    }
    
    GameRecord* record = (GameRecord*)malloc(sizeof(GameRecord));
    if (record == NULL) {
        return NULL;
    }
    
    record->moves = (Move*)malloc(initial_capacity * sizeof(Move));
    if (record->moves == NULL) {
        free(record);
        return NULL;
    }
    
    record->move_count = 0;
    record->capacity = initial_capacity;
    
    return record;
}

//5.扩展棋局记录容量（这个高级，可以不用吧）
static bool expand_game_record(GameRecord* record) {
    if (record == NULL) {
        return false;
    }
    
    // 容量翻倍
    int new_capacity = record->capacity * 2;
    Move* new_moves = (Move*)realloc(record->moves, new_capacity * sizeof(Move));
    
    if (new_moves == NULL) {
        return false;
    }
    
    record->moves = new_moves;
    record->capacity = new_capacity;
    
    return true;
}

// 6-1.添加走法到棋局记录（不是在记录谁走到哪里了吧！）
bool add_move_to_record(GameRecord* record, const Move* move) {
    if (record == NULL || move == NULL) {
        return false;
    }
    
    // 检查是否需要扩展容量
    if (record->move_count >= record->capacity) {
        if (!expand_game_record(record)) {
            return false;
        }
    }
    
    // 添加走法
    record->moves[record->move_count] = *move;
    record->move_count++;
    
    return true;
}

// 6-2.从棋局记录中获取走法（算是记谱之外，可以在页面上实现记录上一步路径）
const Move* get_move_from_record(const GameRecord* record, int index) {
    if (record == NULL || index < 0 || index >= record->move_count) {
        return NULL;
    }
    
    return &record->moves[index];
}

// ~~~~~7.打印棋局记录（标准代数记谱法）
void print_game_record(const GameRecord* record) {
    if (record == NULL || record->move_count == 0) {
        printf("No moves recorded.\n");
        return;
    }
    
    // 按回合打印走法
    for (int i = 0; i < record->move_count; i += 2) {
        // 回合编号
        printf("%d.", (i / 2) + 1);
        
        // 白方走法
        char white_move[20];
        move_to_algebraic(&record->moves[i], white_move);
        printf(" %s", white_move);
        
        // 黑方走法（如果有的话）
        if (i + 1 < record->move_count) {
            char black_move[20];
            move_to_algebraic(&record->moves[i + 1], black_move);
            printf(" %s", black_move);
        }
        
        printf("\n");
    }
}

// ~~8-1.保存棋局记录到文件（看组上要求需不需要！）
bool save_game_record_to_file(const GameRecord* record, const char* filename) {
    if (record == NULL || filename == NULL) {
        return false;
    }
    
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        return false;
    }
    
    // 写入走法数量
    fprintf(file, "%d\n", record->move_count);
    
    // 写入每一步走法
    for (int i = 0; i < record->move_count; i++) {
        const Move* move = &record->moves[i];
        fprintf(file, "%d %d %d %d %d %d %d %d %d %d %d %d %d\n",
                move->from_row, move->from_col,
                move->to_row, move->to_col,
                move->piece, move->color,
                move->captured.type, move->captured.color,
                move->is_castling, move->promotion,
                move->is_check, move->is_checkmate,
                move->is_en_passant);
    }
    
    fclose(file);
    return true;
}

// 8-2.从文件读取棋局记录
GameRecord* load_game_record_from_file(const char* filename) {
    if (filename == NULL) {
        return NULL;
    }
    
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        return NULL;
    }
    
    // 读取走法数量
    int move_count;
    if (fscanf(file, "%d", &move_count) != 1) {
        fclose(file);
        return NULL;
    }
    
    // 初始化记录
    GameRecord* record = init_game_record(move_count);
    if (record == NULL) {
        fclose(file);
        return NULL;
    }
    
    // 读取每一步走法
    for (int i = 0; i < move_count; i++) {
        Move* move = &record->moves[i];
        if (fscanf(file, "%d %d %d %d %d %d %d %d %d %d %d %d %d",
                &move->from_row, &move->from_col,
                &move->to_row, &move->to_col,
                (int*)&move->piece, (int*)&move->color,
                (int*)&move->captured.type, (int*)&move->captured.color,
                &move->is_castling, (int*)&move->promotion,
                &move->is_check, &move->is_checkmate,
                &move->is_en_passant) != 13) {
            fclose(file);
            free_game_record(record);
            return NULL;
        }
    }
    
    record->move_count = move_count;
    fclose(file);
    
    return record;
}

// 8-3.释放棋局记录内存（清空记录，如果有记录可以用上排名的链表，看组上需不需要吧）
void free_game_record(GameRecord* record) {
    if (record == NULL) {
        return;
    }
    
    if (record->moves != NULL) {
        free(record->moves);
    }
    
    free(record);
}
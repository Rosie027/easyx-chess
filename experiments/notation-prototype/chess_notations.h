#ifndef CHESS_NOTATIONS_H
#define CHESS_NOTATIONS_H

#include <stdbool.h>

// 棋子类型定义
typedef enum {
    EMPTY = 0,
    PAWN,   // 兵
    ROOK,   // 车
    KNIGHT, // 马
    BISHOP, // 象
    QUEEN,  // 后
    KING    // 王
} PieceType;

// 棋子颜色定义
typedef enum {
    WHITE,
    BLACK
} Color;

// 棋子结构体
typedef struct {
    PieceType type;
    Color color;
} Piece;

// 棋盘类型定义（8x8）
typedef Piece ChessBoard[8][8];

// 走法结构体
typedef struct {
    int from_row;    // 起始行（0-7，0为黑方底线，7为白方底线）
    int from_col;    // 起始列（0-7，0为a列，7为h列）
    int to_row;      // 目标行
    int to_col;      // 目标列
    PieceType piece; // 走子的棋子类型
    Color color;     // 走子方颜色
    Piece captured;  // 被吃的棋子（EMPTY表示无吃子）
    bool is_castling; // 是否王车易位
    PieceType promotion; // 升变的棋子类型（PAWN表示无升变）
    bool is_check;   // 是否将军
    bool is_checkmate; // 是否将死
    bool is_en_passant; // 是否吃过路兵
} Move;

// 棋局记录结构体
typedef struct {
    Move* moves;     // 走法数组
    int move_count;  // 当前走法数量
    int capacity;    // 数组容量
} GameRecord;

// 初始化棋盘
void init_board(ChessBoard board);

// 将坐标转换为代数记谱法字符串（如：(7,0) -> "a1"）
void coord_to_algebraic(int row, int col, char* output);

// 将代数记谱法字符串转换为坐标（如："a1" -> (7,0)）
bool algebraic_to_coord(const char* alg, int* row, int* col);

// 将走法转换为标准代数记谱
void move_to_algebraic(const Move* move, char* output);

// 初始化棋局记录
GameRecord* init_game_record(int initial_capacity);

// 添加走法到棋局记录
bool add_move_to_record(GameRecord* record, const Move* move);

// 从棋局记录中获取走法
const Move* get_move_from_record(const GameRecord* record, int index);

// 打印棋局记录（标准代数记谱法）
void print_game_record(const GameRecord* record);

// 保存棋局记录到文件
bool save_game_record_to_file(const GameRecord* record, const char* filename);

// 从文件读取棋局记录
GameRecord* load_game_record_from_file(const char* filename);

// 释放棋局记录内存
void free_game_record(GameRecord* record);

#endif // CHESS_NOTATIONS_H

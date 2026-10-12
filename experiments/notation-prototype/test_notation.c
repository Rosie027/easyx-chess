#include <stdio.h>
#include <stdlib.h>
#include "chess_notations.h"

// 简单的测试函数，演示记谱模块的使用
int main() {
    printf("=== Chess Notation Module Test ===\n\n");
    
    // 初始化棋局记录
    GameRecord* record = init_game_record(10);
    if (record == NULL) {
        printf("Failed to initialize game record.\n");
        return 1;
    }
    
    // 创建一些测试走法
    // 示例：1. e4 e5
    Move move1 = {
        .from_row = 6, .from_col = 4, // e2
        .to_row = 4, .to_col = 4,     // e4
        .piece = PAWN,
        .color = WHITE,
        .captured.type = EMPTY,
        .is_castling = false,
        .promotion = PAWN,
        .is_check = false,
        .is_checkmate = false,
        .is_en_passant = false
    };
    
    Move move2 = {
        .from_row = 1, .from_col = 4, // e7
        .to_row = 3, .to_col = 4,     // e5
        .piece = PAWN,
        .color = BLACK,
        .captured.type = EMPTY,
        .is_castling = false,
        .promotion = PAWN,
        .is_check = false,
        .is_checkmate = false,
        .is_en_passant = false
    };
    
    // 示例：2. Nf3 Nc6
    Move move3 = {
        .from_row = 7, .from_col = 1, // g1
        .to_row = 5, .to_col = 2,     // f3
        .piece = KNIGHT,
        .color = WHITE,
        .captured.type = EMPTY,
        .is_castling = false,
        .promotion = PAWN,
        .is_check = false,
        .is_checkmate = false,
        .is_en_passant = false
    };
    
    Move move4 = {
        .from_row = 0, .from_col = 6, // b8
        .to_row = 2, .to_col = 5,     // c6
        .piece = KNIGHT,
        .color = BLACK,
        .captured.type = EMPTY,
        .is_castling = false,
        .promotion = PAWN,
        .is_check = false,
        .is_checkmate = false,
        .is_en_passant = false
    };
    
    // 示例：3. Bb5 a6 （吃子测试）
    Move move5 = {
        .from_row = 7, .from_col = 2, // f1
        .to_row = 5, .to_col = 3,     // c4
        .piece = BISHOP,
        .color = WHITE,
        .captured.type = EMPTY,
        .is_castling = false,
        .promotion = PAWN,
        .is_check = false,
        .is_checkmate = false,
        .is_en_passant = false
    };
    
    Move move6 = {
        .from_row = 0, .from_col = 0, // a7
        .to_row = 1, .to_col = 0,     // a6
        .piece = PAWN,
        .color = BLACK,
        .captured.type = EMPTY,
        .is_castling = false,
        .promotion = PAWN,
        .is_check = false,
        .is_checkmate = false,
        .is_en_passant = false
    };
    
    // 示例：4. Ba4 Nf6 （将军测试）
    Move move7 = {
        .from_row = 7, .from_col = 3, // d1
        .to_row = 3, .to_col = 7,     // h5
        .piece = QUEEN,
        .color = WHITE,
        .captured.type = EMPTY,
        .is_castling = false,
        .promotion = PAWN,
        .is_check = true,  // 将军
        .is_checkmate = false,
        .is_en_passant = false
    };
    
    // 添加走法到记录
    add_move_to_record(record, &move1);
    add_move_to_record(record, &move2);
    add_move_to_record(record, &move3);
    add_move_to_record(record, &move4);
    add_move_to_record(record, &move5);
    add_move_to_record(record, &move6);
    add_move_to_record(record, &move7);
    
    // 打印棋局记录
    printf("Generated Chess Notation:\n");
    print_game_record(record);
    
    // 测试单个走法的记谱转换
    printf("\n=== Single Move Notation Test ===\n");
    char notation[20];
    
    move_to_algebraic(&move1, notation);
    printf("White's first move (e4): %s\n", notation);
    
    move_to_algebraic(&move3, notation);
    printf("White's knight move (Nf3): %s\n", notation);
    
    move_to_algebraic(&move7, notation);
    printf("White's queen check (Qh5+): %s\n", notation);
    
    // 测试坐标转换
    printf("\n=== Coordinate Conversion Test ===\n");
    char coord_str[3];
    coord_to_algebraic(6, 4, coord_str); // e2
    printf("Coordinate (6,4) -> %s\n", coord_str);
    
    int row, col;
    if (algebraic_to_coord("e4", &row, &col)) {
        printf("Algebraic 'e4' -> (%d,%d)\n", row, col);
    }
    
    // 测试保存和读取
    printf("\n=== Save and Load Test ===\n");
    const char* filename = "test_game.txt";
    
    if (save_game_record_to_file(record, filename)) {
        printf("Game record saved to %s\n", filename);
        
        // 读取保存的记录
        GameRecord* loaded_record = load_game_record_from_file(filename);
        if (loaded_record != NULL) {
            printf("Loaded game record from %s:\n", filename);
            print_game_record(loaded_record);
            free_game_record(loaded_record);
        } else {
            printf("Failed to load game record from %s\n", filename);
        }
    } else {
        printf("Failed to save game record to %s\n", filename);
    }
    
    // 释放内存
    free_game_record(record);
    
    printf("\n=== Test Completed ===\n");
    
    return 0;
}

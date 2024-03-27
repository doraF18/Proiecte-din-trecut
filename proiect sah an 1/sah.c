#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#define EMPTY_CHAR "."
#define W_PAWN_CHAR "P"
#define W_KNIGHT_CHAR "N"
#define W_BISHOP_CHAR "B"
#define W_ROOK_CHAR "R"
#define W_QUEEN_CHAR "Q"
#define W_KING_CHAR "K"
#define B_PAWN_CHAR "p"
#define B_KNIGHT_CHAR "n"
#define B_BISHOP_CHAR "b"
#define B_ROOK_CHAR "r"
#define B_QUEEN_CHAR "q"
#define B_KING_CHAR "k"
typedef enum {
    EMPTY = -1,
    W_PAWN = 2,
    W_ROOK = 4,
    W_BISHOP = 6,
    W_KNIGHT = 8,
    W_QUEEN = 10,
    W_KING = 12,
    B_PAWN = 1,
    B_ROOK = 3,
    B_BISHOP = 5,
    B_KNIGHT = 7,
    B_QUEEN = 9,
    B_KING = 11,
}piece;
piece board[8][8] = {
        {B_ROOK, B_KNIGHT, B_BISHOP, B_QUEEN, B_KING, B_BISHOP, B_KNIGHT, B_ROOK},
        {B_PAWN, B_PAWN, B_PAWN, B_PAWN, B_PAWN, B_PAWN, B_PAWN, B_PAWN},
        {EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
        {EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
        {EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
        {EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
        {W_PAWN, W_PAWN, W_PAWN, W_PAWN, W_PAWN, W_PAWN, W_PAWN, W_PAWN},
        {W_ROOK, W_KNIGHT, W_BISHOP, W_QUEEN, W_KING, W_BISHOP, W_KNIGHT, W_ROOK},
};
typedef struct {
    int x;
    int y;
}point;
point move_vect[200];
int move_vect_count = 0;
int king_x = 0, king_y = 0;
int player_move = 1;
void showboard() {
    printf("\n ");
    for (int i = 0; i < 8; i++) {
        printf("%c ", 'a' + i);
    }
    printf("\n");
    for (int i = 0; i < 8; i++) {
        printf("%d ", 8 - i);
        for (int j = 0; j < 8; j++) {
            if (board[i][j] == EMPTY)
                printf("%s ", EMPTY_CHAR);
            if (board[i][j] == B_PAWN)
                printf("%s ", B_PAWN_CHAR);
            if (board[i][j] == B_ROOK)
                printf("%s ", B_ROOK_CHAR);
            if (board[i][j] == B_BISHOP)
                printf("%s ", B_BISHOP_CHAR);
            if (board[i][j] == B_KNIGHT)
                printf("%s ", B_KNIGHT_CHAR);
            if (board[i][j] == B_KING)
                printf("%s ", B_KING_CHAR);
            if (board[i][j] == B_QUEEN)
                printf("%s ", B_QUEEN_CHAR);
            if (board[i][j] == W_PAWN)
                printf("%s ", W_PAWN_CHAR);
            if (board[i][j] == W_BISHOP)
                printf("%s ", W_BISHOP_CHAR);
            if (board[i][j] == W_KNIGHT)
                printf("%s ", W_KNIGHT_CHAR);
            if (board[i][j] == W_KING)
                printf("%s ", W_KING_CHAR);
            if (board[i][j] == W_QUEEN)
                printf("%s ", W_QUEEN_CHAR);
            if (board[i][j] == W_ROOK)
                printf("%s ", W_ROOK_CHAR);
        }
        printf("%d", 8 - i);
        printf("\n");
    }
    printf(" ");
    for (int i = 0; i < 8; i++) {
        printf("%c ", 'a' + i);
    }
    printf("\n");
}
int king_adjecent(int startX, int startY,int color){
    int found = 0;
    int offsets[8][2] = {
            {-1, -1}, {-1, 0}, {-1, 1},
            {0, -1}, {0, 1},
            {1, -1}, {1, 0}, {1, 1}
    };
    for (int i = 0; i < 8; i++) {
        int new_x = startX + offsets[i][0];
        int new_y = startY + offsets[i][1];
        if (new_x >= 0 && new_x < 8 && new_y >= 0 && new_y < 8) {
            if ((color == 0) && (board[new_x][new_y] == B_KING))
                return 1;
            if ((color == 1) && (board[new_x][new_y] == W_KING))
                return 1;
        }
    }
    return 0;
}
int is_inside(int endX, int endY) {
    if ((endX < 0 || endX >= 8) && (endY < 0 || endY >= 8)) {
        return 0;
    }else {
        return 1;
    }
}
int is_friendly(int startX, int startY, int endX, int endY) {
    if (board[endX][endY] != EMPTY) {
        if ((board[startX][startY] % 2 == 0 && board[endX][endY] % 2 == 0) ||
            (board[startX][startY] % 2 != 0 && board[endX][endY] % 2 != 0))
            return 1;
    }
    if(board[endX][endY] == EMPTY)
        return 2;
    return 0;
}
void getmove(piece* piece_type,int* startX, int* startY, int* endX, int* endY) {
    char start_square[3];
    char end_square[3];
    printf("Enter starting position(e.g 'a4') ");
    scanf(" %s", start_square);
    printf("Enter ending position(e.g 'a3') ");
    scanf(" %s", end_square);
    *startY = start_square[0] - 'a';
    *startX = '8' - start_square[1];
    *endY = end_square[0] - 'a';
    *endX = '8' - end_square[1];
    *piece_type = board[*startX][*startY];
}
int bishop_check(int startX, int startY, int endX, int endY) {
    int x_diff = abs(endX - startX);
    int y_diff = abs(endY - startY);
    int x_dir = (endX > startX) ? 1 : -1;
    int y_dir = (endY > startY) ? 1 : -1;
    if (x_diff == y_diff) {
        int x = startX + x_dir;
        int y = startY + y_dir;
        while (x != endY && y != endY) {
            if (board[x][y] != EMPTY) {
                return 0;
            }
            x += x_dir;
            y += y_dir;
        }
        return 1;
    }
    return 0;
}
int knight_check(int startX, int startY, int endX, int endY) {
    int x_diff = abs(endX - startX);
    int y_diff = abs(endY - startY);
    if ((x_diff == 2 && y_diff == 1) || (x_diff == 1 && y_diff == 2))
        return 1;
    else
        return 0;
}
int rook_check(int startX, int startY, int endX, int endY) {
    if ((startX == endX) || (startY == endY)){
        if (startX == endX) {
            int min_y = (startY < endY) ? startY : endY;
            int max_y = (startY > endY) ? startY : endY;
            for (int y = min_y + 1; y < max_y; y++) {
                if (board[startX][y] != EMPTY)
                    return 0;
            }
        }
        else
        {
            int min_x = (startX < endX) ? startX : endX;
            int max_x = (startX > endX) ? startX : endX;
            for (int x = min_x + 1; x < max_x; x++){
                if (board[x][startY] != EMPTY)
                    return 0;
            }
        }
        return 1;
    }else {
        return 0;
    }
}
int queen_check(int startX, int startY, int endX, int endY){
    if(bishop_check(startX,startY,endX,endY) == 1 || rook_check(startX,startY,endX,endY) == 1)
        return 1;
    else
        return 0;
}
int pawn_check(int startX, int startY, int endX, int endY){
    int direction;
    if (startX < endX) {
        direction = 1;
    } else if (startX > endX) {
        direction = -1;
    } else
        return 0;
    int diff_x = abs(endX - startX);
    int diff_y = abs(endY - startY);
    if (diff_x == 1 && diff_y == 1) {
        if (board[endX][endY] != EMPTY) {
            if(is_friendly(startX, startY, endX, endY) == 0)
                return 1;
            else
                return 0;
        }
    } else if (diff_x == 1 && diff_y == 0) {
        if (board[endX][endY] == EMPTY)
            return 1;
    } else if (diff_x == 2 && diff_y == 0) {
        if (startX == 1 && direction == 1 && board[endX][endY] == EMPTY && board[startX + direction][startY] == EMPTY) {
            return 1;
        } else if (startX == 6 && direction == -1 && board[endX][endY] == EMPTY && board[startX + direction][startY] == EMPTY)
            return 1;
    }
    return 0;
}
int king_check(int startX, int startY, int endX, int endY) {
    int color;
    for(int i = 0; i < 200; i++)
        if(move_vect[i].x == king_x && move_vect[i].y == king_y)
            return 0;
    if(board[startX][startY] %2 == 0)
        color = 0;
    else
        color = 1;
    int king = king_adjecent(endX,endY, color);
    if (startX < 0 || startY > 8 || endX < 0 || endY > 8)
        return 0;
    int diffX = abs(endX - startX);
    int diffY = abs(endY - startY);
    if (diffX <= 1 && diffY <= 1 && king == 0 && board[endX][endY] == EMPTY) {
        return 1;
    }else if(diffX <= 1 && diffY <= 1 && king == 0 && board[endX][endY] != EMPTY)
        if(is_friendly(startX,startY,endX,endY) == 0)
            return 1;
    return 0;
}
int check(int startX, int startY, int endX, int endY){
    piece piece_type = board[startX][startY];
    if(piece_type == EMPTY) {
        printf("U CHOSE A EMPTY SQUARE ... \n");
        return 0;
    }
    if(is_inside(endX, endY) == 0) {
        printf("out");
        return 0;
    }
    if(is_friendly(startX, startY, endX, endY) == 1) {
        printf("friend");
        return 0;
    }
    switch (piece_type) {
        case W_KING :
        case B_KING :
            return king_check(startX, startY, endX, endY);
        case W_PAWN :
        case B_PAWN :
            return pawn_check(startX, startY, endX, endY);
        case W_BISHOP :
        case B_BISHOP :
            return bishop_check(startX, startY, endX, endY);
        case W_KNIGHT :
        case B_KNIGHT :
            return knight_check(startX, startY, endX, endY);
        case W_ROOK :
        case B_ROOK :
            return rook_check(startX, startY, endX, endY);
        case W_QUEEN :
        case B_QUEEN :
            return queen_check(startX, startY, endX, endY);
        default :
            return 1;
    }
}
int print_bishop(int startX, int startY) {
    int directions[4][2] = { {-1, -1}, {-1, 1}, {1, -1}, {1, 1} };
    int move = 0;
    for (int i = 0; i < 4; i++) {
        int new_x = startX + directions[i][0];
        int new_y = startY + directions[i][1];
        while (new_x >= 0 && new_x < 8 && new_y >= 0 && new_y < 8) {
            if(is_friendly(startX,startY,new_x,new_y) == 1)
                break;
            else if (board[new_x][new_y] == EMPTY) {
                move_vect[move_vect_count].x = new_x;
                move_vect[move_vect_count].y = new_y;
                move_vect_count++;
                move++;
                break;
            }
            else if (is_friendly(startX, startY, new_x, new_y) == 0) {
                move_vect[move_vect_count].x = new_x;
                move_vect[move_vect_count].y = new_y;
                move_vect_count++;
                move++;
                break;
            }
            new_x += directions[i][0];
            new_y += directions[i][1];
        }
    }
    return move;
}
int print_rook(int startX, int startY){
    int directions[4][2] = { {-1, 0}, {1, 0}, {0, -1}, {0, 1} };
    int move = 0;
    for (int i = 0; i < 4; i++) {
        int new_x = startX + directions[i][0];
        int new_y = startY + directions[i][1];
        while (new_x >= 0 && new_x < 8 && new_y >= 0 && new_y < 8) {
            if(is_friendly(startX,startY,new_x,new_y) == 1)
                break;
            else if(board[new_x][new_y] == EMPTY){
                move_vect[move_vect_count].x = new_x;
                move_vect[move_vect_count].y = new_y;
                move_vect_count++;
                move++;
            }
            else if(is_friendly(startX, startY, new_x, new_y) == 0){
                move_vect[move_vect_count].x = new_x;
                move_vect[move_vect_count].y = new_y;
                move_vect_count++;
                move++;
                break;
            }
            new_x += directions[i][0];
            new_y += directions[i][1];
        }
    }
    return move;
}
int print_knight(int startX, int startY){
    int directions[8][2] = { {-2, -1}, {-1, -2}, {1, -2}, {2, -1},
                             {2, 1}, {1, 2}, {-1, 2}, {-2, 1} };
    int move = 0;
    for (int i = 0; i < 8; i++) {
        int new_x = startX + directions[i][0];
        int new_y = startY + directions[i][1];
        if (new_x >= 0 && new_x < 8 && new_y >= 0 && new_y < 8) {
            if(board[new_x][new_y] == EMPTY){
                move_vect[move_vect_count].x = new_x;
                move_vect[move_vect_count].y = new_y;
                move_vect_count++;
                move++;
            }
            else if(is_friendly(startX, startY, new_x, new_y) == 0){
                move_vect[move_vect_count].x = new_x;
                move_vect[move_vect_count].y = new_y;
                move_vect_count++;
                move++;
            }
        }
    }
    return move;
}
int print_queen(int startX, int startY){
    return print_rook(startX,startY) + print_bishop(startX,startY);
}
int print_pawn(int startX, int startY){
    int direction, starting_pos = 0;
    int move = 0;
    if (board[startX][startY] % 2 == 0)
        direction = -1;
    else
        direction = 1;
    if(direction == -1 && startX == 6)
        starting_pos = 1;
    if(direction == 1 && startX == 1)
        starting_pos = 1;
    int new_x = startX + direction;
    if (new_x >= 0 && new_x < 8)
        if(board[new_x][startY] == EMPTY){
            move_vect[move_vect_count].x = new_x;
            move_vect[move_vect_count].y = startY;
            move_vect_count++;
            move++;
        }
    if(starting_pos == 1)
        new_x = startX + 2*direction;
    if(new_x >= 0 && new_x < 8 && starting_pos == 1)
        if(board[new_x][startY] == EMPTY){
            move_vect[move_vect_count].x = new_x;
            move_vect[move_vect_count].y = startY;
            move_vect_count++;
            move++;
        }
    new_x = startX + direction;
    int new_y = startY + 1;
    if (new_x >= 0 && new_x < 8 && new_y >= 0 && new_y < 8)
        if(board[new_x][new_y] != EMPTY){
            move_vect[move_vect_count].x = new_x;
            move_vect[move_vect_count].y = new_y;
            move_vect_count++;
            move++;
        }
    new_x = startX + direction;
    new_y = startY - 1;
    if (new_x >= 0 && new_x < 8 && new_y >= 0 && new_y < 8)
        if(board[new_x][new_y] != EMPTY){
            move_vect[move_vect_count].x = new_x;
            move_vect[move_vect_count].y = new_y;
            move_vect_count++;
            move++;
        }
    return move;
}
int print_king(int startX, int startY){
    int color;
    if(board[startX][startY] %2 == 0)
        color = 0;
    else
        color = 1;
    int direction[8][2] = { {-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1}};
    int move = 0;
    for (int i = 0; i < 8; i++) {
        int new_x = startX + direction[i][0];
        int new_y = startY + direction[i][1];
        if (new_x >= 0 && new_x < 8 && new_y >= 0 && new_y < 8) {
            if(board[new_x][new_y] == EMPTY && king_adjecent(new_x,new_y,color) == 0 && king_check(startX,startY,new_x,new_y) == 1){
                move++;
                move_vect[move_vect_count].x = new_x;
                move_vect[move_vect_count].y = new_y;
                move_vect_count++;
            }
            if(board[new_x][new_y] != EMPTY && king_adjecent(new_x, new_y,color) == 0 && is_friendly(startX, startY, new_x, new_y) == 0 && king_check(startX,startY,new_x,new_y) == 1){
                move++;
                move_vect[move_vect_count].x = new_x;
                move_vect[move_vect_count].y = new_y;
                move_vect_count++;
            }
        }
    }
    return move;
}
int evaluation (){
    double score = 0;
    int white_pawns = 0, black_pawns = 0;
    int white_knights = 0, black_knights = 0;
    int white_bishops = 0, black_bishops = 0;
    int white_rooks = 0, black_rooks = 0;
    int white_queens = 0, black_queens = 0;
    int white_king = 0, black_king = 0;
    int Wpiece_count = 0;
    int Bpiece_count = 0;
    int double_wpawns = 0;
    int double_bpawns = 0;
    int blocked_wpawns = 0;
    int blocked_bpawns = 0;
    int isolated_wpawns = 0;
    int isolated_bpawns = 0;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            piece piece_type = board[i][j];
            switch(piece_type) {
                case W_PAWN:
                    white_pawns++;
                    if(board[i-1][j] == W_PAWN)
                        double_wpawns++;
                    if(board[i-1][j] != EMPTY)
                        blocked_wpawns++;
                    if(board[i+1][j+1] != W_PAWN && board[i+1][j-1] != W_PAWN)
                        isolated_wpawns++;
                    Wpiece_count = Wpiece_count + print_pawn(i,j);
                    break;
                case B_PAWN:
                    black_pawns++;
                    if(board[i+1][j] == B_PAWN)
                        double_bpawns++;
                    if(board[i+1][j] != EMPTY)
                        blocked_bpawns++;
                    if(board[i-1][j+1] != B_PAWN && board[i-1][j-1] != B_PAWN)
                        isolated_bpawns++;
                    Bpiece_count = Bpiece_count + print_pawn(i,j);
                    break;
                case W_KNIGHT:
                    white_knights++;
                    Wpiece_count = Wpiece_count + print_knight(i,j);
                    break;
                case B_KNIGHT:
                    black_knights++;
                    Bpiece_count = Bpiece_count + print_knight(i,j);
                    break;
                case W_BISHOP:
                    white_bishops++;
                    Wpiece_count = Wpiece_count + print_bishop(i,j);
                    break;
                case B_BISHOP:
                    black_bishops++;
                    Bpiece_count = Bpiece_count + print_bishop(i,j);
                    break;
                case W_ROOK:
                    white_rooks++;
                    Wpiece_count = Wpiece_count + print_rook(i,j);
                    break;
                case B_ROOK:
                    black_rooks++;
                    Bpiece_count = Bpiece_count + print_rook(i,j);
                    break;
                case W_QUEEN:
                    white_queens++;
                    Wpiece_count = Wpiece_count + print_queen(i,j);
                    break;
                case B_QUEEN:
                    black_queens++;
                    Bpiece_count = Bpiece_count + print_queen(i,j);
                    break;
                case W_KING:
                    white_king++;
                    Wpiece_count = Wpiece_count + print_king(i,j);
                    break;
                case B_KING:
                    black_king++;
                    Bpiece_count = Bpiece_count + print_king(i,j);
                    break;
                default:
                    break;
            }
        }
    }
    // for(int i = 0; i < 70; i++)
    // printf("move x --> %i, y --> %i\n",move_vect[i].x,move_vect[i].y);
    score += 200 * (white_king - black_king);
    score += 9 * (white_queens - black_queens);
    score += 5 * (white_rooks - black_rooks);
    score += 3 * ((white_bishops + white_knights) - (black_bishops + black_knights));
    score += 1 * (white_pawns - black_pawns);
    score += -0.5 *(double_wpawns - double_bpawns + blocked_wpawns - blocked_bpawns + isolated_bpawns - isolated_wpawns);
    score += 0.1 * (Wpiece_count - Bpiece_count);
    move_vect_count = 0;
    printf("The score is ---->>>> %f\n" , score);
    return score;
}
int king_is_check(int choice){
    if(choice == 1){
        for(int i = 0; i < 8; i++)
            for(int j = 0; j < 8; j++)
                if(board[i][j] == W_KING){
                    king_x = i;
                    king_y = j;
                    break;
                }
    }else{
        for(int i = 0; i < 8; i++)
            for(int j = 0; j < 8; j++)
                if(board[i][j] == B_KING){
                    king_x = i;
                    king_y = j;
                    break;
                }
    }
    for(int i = 0; i < 200; i++)
        if(move_vect[i].x == king_x && move_vect[i].y == king_y)
            return 1;
    return 0;
}
void execute_move(){
    piece current_piece;
    int startX,startY,endX,endY;
    getmove(&current_piece,&startX,&startY,&endX,&endY);
    int aux = check(startX,startY,endX,endY);
    if((current_piece % 2 != 0) && (player_move == 1)){
        printf("It's not your turn \n");
    }else if((current_piece % 2 == 0) && (player_move == 0)){
        printf("It's not your turn \n");
    }else if(aux && king_is_check(player_move) == 0) {
        if (is_friendly(startX, startY, endX, endY) == 0) {
            board[startX][startY] = EMPTY;
            board[endX][endY] = current_piece;
            if (player_move == 0)
                player_move = 1;
            else
                player_move = 0;
        } else {
            board[startX][startY] = board[endX][endY];
            board[endX][endY] = current_piece;
            if (player_move == 0)
                player_move = 1;
            else
                player_move = 0;
        }
    }
    if(aux && king_is_check(player_move) == 1){
        if(board[startX][startY] != W_KING && player_move == 1)
            printf("You have to move the king");
        if(board[startX][startY] != B_KING && player_move == 0)
            printf("You have to move the king");
        else if (is_friendly(startX, startY, endX, endY) == 0) {
            board[startX][startY] = EMPTY;
            board[endX][endY] = current_piece;
            if(player_move == 0)
                player_move = 1;
            else
                player_move = 0;
        } else {
            board[startX][startY] = board[endX][endY];
            board[endX][endY] = current_piece;
            if(player_move == 0)
                player_move = 1;
            else
                player_move = 0;
        }
    }
}
int is_game_over() {
    evaluation();
    if (player_move == 1 && king_is_check(1) == 1)
        if (print_king(king_x, king_y) >= 1) {
            return 0;
        } else {
            return 1;
        }
    if(player_move == 0 && king_is_check(0) == 1)
        if(print_king(king_x,king_y) >= 1) {
            return 0;
        }else {
            return 1;
        }
    return 0;
}
void player_vs_player(){
    do{
        printf("\033[H\033[J");
        showboard();
        evaluation();
        execute_move();
        if(king_is_check(player_move))
            printf("is in check");
    }while(is_game_over() == 0);
    printf("\033[H\033[J");
    if(player_move == 0)
        printf("White won\n");
    else
        printf("Black won\n");
    showboard();
    printf("\t\t WELCOME \n");
    printf("\t PLEASE CHOOSE A OPTION \n\n");
    printf("1.PLAYER VERSUS PLAYER\n");
    printf("2.PLAYER VERSUS COMPUTER\n");
    printf("3.EXIT\n");
}
void menu(){
    printf("\t\t WELCOME \n");
    printf("\t PLEASE CHOOSE A OPTION \n\n");
    printf("1.PLAYER VERSUS PLAYER\n");
    printf("2.PLAYER VERSUS COMPUTER\n");
    printf("3.EXIT\n");
    int choice = 0;
    int result;
    while(choice != 3){
        scanf("%d", &choice);
        switch (choice) {
            case 1 :
                player_vs_player();
                break;
            case 2 :
                printf("u choose 2");
                break;
            case 3 :
                printf("EXIT");
                break;
            default :
                printf("u choose the wrong number");
                break;
        }
    }
}
int main() {
    int startX,startY,endX,endY;
    piece X;
    menu();
    return 0;
}

//
//  global.h
//  chess
//
//  Created by Lennon Chimbumu on 11/27/11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#ifndef chess_global_h
#define chess_global_h

#include "defines.h"
#include "board.h"

char CMD_BUFF[MAX_CMD_BUFF];
int CMD_BUFF_COUNT = 0;

Board board;

extern const int A8 = 56; extern const int B8 = 57; extern const int C8 = 58; extern const int D8 = 59;
extern const int E8 = 60; extern const int F8 = 61; extern const int G8 = 62; extern const int H8 = 63;
extern const int A7 = 48; extern const int B7 = 49; extern const int C7 = 50; extern const int D7 = 51;
extern const int E7 = 52; extern const int F7 = 53; extern const int G7 = 54; extern const int H7 = 55;
extern const int A6 = 40; extern const int B6 = 41; extern const int C6 = 42; extern const int D6 = 43;
extern const int E6 = 44; extern const int F6 = 45; extern const int G6 = 46; extern const int H6 = 47;
extern const int A5 = 32; extern const int B5 = 33; extern const int C5 = 34; extern const int D5 = 35;
extern const int E5 = 36; extern const int F5 = 37; extern const int G5 = 38; extern const int H5 = 39;
extern const int A4 = 24; extern const int B4 = 25; extern const int C4 = 26; extern const int D4 = 27;
extern const int E4 = 28; extern const int F4 = 29; extern const int G4 = 30; extern const int H4 = 31;
extern const int A3 = 16; extern const int B3 = 17; extern const int C3 = 18; extern const int D3 = 19;
extern const int E3 = 20; extern const int F3 = 21; extern const int G3 = 22; extern const int H3 = 23;
extern const int A2 =  8; extern const int B2 =  9; extern const int C2 = 10; extern const int D2 = 11;
extern const int E2 = 12; extern const int F2 = 13; extern const int G2 = 14; extern const int H2 = 15;
extern const int A1 =  0; extern const int B1 =  1; extern const int C1 =  2; extern const int D1 =  3;
extern const int E1 =  4; extern const int F1 =  5; extern const int G1 =  6; extern const int H1 =  7;

const char* SQUARENAME[64] = {"a1","b1","c1","d1","e1","f1","g1","h1",
                              "a2","b2","c2","d2","e2","f2","g2","h2",
                              "a3","b3","c3","d3","e3","f3","g3","h3",
                              "a4","b4","c4","d4","e4","f4","g4","h4",
                              "a5","b5","c5","d5","e5","f5","g5","h5",
                              "a6","b6","c6","d6","e6","f6","g6","h6",
                              "a7","b7","c7","d7","e7","f7","g7","h7",
                              "a8","b8","c8","d8","e8","f8","g8","h8"};

extern const int FILES[64] = {1, 2, 3, 4, 5, 6, 7, 8,
                              1, 2, 3, 4, 5, 6, 7, 8,
                              1, 2, 3, 4, 5, 6, 7, 8,
                              1, 2, 3, 4, 5, 6, 7, 8,
                              1, 2, 3, 4, 5, 6, 7, 8,
                              1, 2, 3, 4, 5, 6, 7, 8,
                              1, 2, 3, 4, 5, 6, 7, 8,
                              1, 2, 3, 4, 5, 6, 7, 8};

extern const int RANKS[64] = {1, 1, 1, 1, 1, 1, 1, 1,
                              2, 2, 2, 2, 2, 2, 2, 2,
                              3, 3, 3, 3, 3, 3, 3, 3,
                              4, 4, 4, 4, 4, 4, 4, 4,
                              5, 5, 5, 5, 5, 5, 5, 5,
                              6, 6, 6, 6, 6, 6, 6, 6,
                              7, 7, 7, 7, 7, 7, 7, 7,
                              8, 8, 8, 8, 8, 8, 8, 8
                            };


// Identifier of next move:
extern const unsigned char WHITE_MOVE  = 0; 
extern const unsigned char BLACK_MOVE  = 1; 

// Piece identifiers, 4 bits each.
// Usefull bitwise properties of this numbering scheme:
// white = 0..., black = 1..., sliding = .1.., nonsliding = .0..
// rank/file sliding pieces = .11., diagonally sliding pieces = .1.1
// pawns and kings (without color bits), are < 3
// major pieces (without color bits set), are > 5
// minor and major pieces (without color bits set), are > 2


extern const unsigned char EMPTY = 0;                //  0000
extern const unsigned char WHITE_PAWN = 1;           //  0001
extern const unsigned char WHITE_KING = 2;           //  0010
extern const unsigned char WHITE_KNIGHT = 3;         //  0011
extern const unsigned char WHITE_BISHOP =  5;        //  0101
extern const unsigned char WHITE_ROOK = 6;           //  0110
extern const unsigned char WHITE_QUEEN = 7;          //  0111
extern const unsigned char BLACK_PAWN = 9;           //  1001
extern const unsigned char BLACK_KING = 10;          //  1010
extern const unsigned char BLACK_KNIGHT = 11;        //  1011
extern const unsigned char BLACK_BISHOP = 13;        //  1101
extern const unsigned char BLACK_ROOK = 14;          //  1110
extern const unsigned char BLACK_QUEEN = 15;         //  1111

const char* PIECENAMES[16] = {"  ","P ","K ","N ","  ","B ","R ","Q ",
                              "  ","P*","K*","N*","  ","B*","R*","Q*"};

BitMap BITSET[64];
int BOARDINDEX[9][9]; // index 0 is not used, only 1..8.

// Value of material, in centipawns:
extern const int PAWN_VALUE = 100;
extern const int KNIGHT_VALUE = 300;
extern const int BISHOP_VALUE = 300;
extern const int ROOK_VALUE = 500;
extern const int QUEEN_VALUE = 900;
extern const int KING_VALUE = 9999;
extern const int CHECK_MATE = KING_VALUE;

// used in Eugene Nalimov's bitScanReverse
int MS1BTABLE[256];

// Attack tables:
BitMap WHITE_PAWN_ATTACKS[64];
BitMap WHITE_PAWN_MOVES[64];
BitMap WHITE_PAWN_DOUBLE_MOVES[64];
BitMap BLACK_PAWN_ATTACKS[64];
BitMap BLACK_PAWN_MOVES[64];
BitMap BLACK_PAWN_DOUBLE_MOVES[64];
BitMap KNIGHT_ATTACKS[64];
BitMap KING_ATTACKS[64];
BitMap RANK_ATTACKS[64][64];      // 32KB
BitMap FILE_ATTACKS[64][64];      // 32KB
BitMap DIAGA8H1_ATTACKS[64][64];  // 32KB
BitMap DIAGA1H8_ATTACKS[64][64];  // 32KB

// Move generator shift for ranks:
extern const int RANKSHIFT[64] = {
  1,  1,  1,  1,  1,  1,  1,  1,
  9,  9,  9,  9,  9,  9,  9,  9,
  17, 17, 17, 17, 17, 17, 17, 17,  
  25, 25, 25, 25, 25, 25, 25, 25,
  33, 33, 33, 33, 33, 33, 33, 33,
  41, 41, 41, 41, 41, 41, 41, 41,
  49, 49, 49, 49, 49, 49, 49, 49,
  57, 57, 57, 57, 57, 57, 57, 57
};

// Move generator magic multiplication numbers for files:
extern const BitMap _FILEMAGICS[8] = {
  0x8040201008040200,
  0x4020100804020100,
  0x2010080402010080,
  0x1008040201008040,
  0x0804020100804020,
  0x0402010080402010,
  0x0201008040201008,
  0x0100804020100804
};

// Move generator magic multiplication numbers for diagonals:
extern const BitMap _DIAGA8H1MAGICS[15] = {
  0x0,
  0x0,
  0x0101010101010100,
  0x0101010101010100,
  0x0101010101010100,
  0x0101010101010100,
  0x0101010101010100,
  0x0101010101010100,
  0x0080808080808080,
  0x0040404040404040,
  0x0020202020202020,
  0x0010101010101010,
  0x0008080808080808,
  0x0,
  0x0
};

// Move generator magic multiplication numbers for diagonals:
extern const BitMap _DIAGA1H8MAGICS[15] = {
  0x0,
  0x0,
  0x0101010101010100,
  0x0101010101010100,
  0x0101010101010100,
  0x0101010101010100,
  0x0101010101010100,
  0x0101010101010100,
  0x8080808080808000,
  0x4040404040400000,
  0x2020202020000000,
  0x1010101000000000,
  0x0808080000000000,
  0x0,
  0x0
};

// Move generator 6-bit masking and magic multiplication numbers:
BitMap RANKMASK[64];
BitMap FILEMASK[64];
BitMap FILEMAGIC[64];
BitMap DIAGA8H1MASK[64];
BitMap DIAGA8H1MAGIC[64];
BitMap DIAGA1H8MASK[64];
BitMap DIAGA1H8MAGIC[64];

// We use one generalized sliding attacks array: [8 squares][64 states]
// the unsigned char (=8 bits) contains the attacks for a rank, file or diagonal
unsigned char GEN_SLIDING_ATTACKS[8][64];

// Used for castling:
unsigned char CANCASTLEOO = 1;
unsigned char CANCASTLEOOO = 2;
BitMap maskEG[2];
BitMap maskFG[2];
BitMap maskBD[2];
BitMap maskCE[2];
unsigned int WHITE_OOO_CASTL;
unsigned int BLACK_OOO_CASTL;
unsigned int WHITE_OO_CASTL;
unsigned int BLACK_OO_CASTL;

int ICAPT;
int IEP;
int IPROM;
int ICASTLOO;
int ICASTLOOO;
int ICHECK;

#endif

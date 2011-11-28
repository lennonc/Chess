//
//  board.cpp
//  chess
//
//  Created by Lennon Chimbumu on 11/27/11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#include <iostream>
#include <iomanip>
#include "defines.h"
#include "protos.h"
#include "extglobals.h"
#include "board.h"
using namespace std;

void Board::init(){
  viewRotated = false;
  
  for (int i = 0; i < 64; i++) {
    square[i] = EMPTY;
  }
  
  square[E1] = WHITE_KING;
  square[D1] = WHITE_QUEEN;
  square[A1] = WHITE_ROOK;
  square[H1] = WHITE_ROOK;
  square[B1] = WHITE_KNIGHT;
  square[G1] = WHITE_KNIGHT;
  square[C1] = WHITE_BISHOP;
  square[F1] = WHITE_BISHOP;
  square[A2] = WHITE_PAWN;
  square[B2] = WHITE_PAWN;
  square[C2] = WHITE_PAWN;
  square[D2] = WHITE_PAWN;
  square[E2] = WHITE_PAWN;
  square[F2] = WHITE_PAWN;
  square[G2] = WHITE_PAWN;
  square[H2] = WHITE_PAWN;
  
  square[E8] = BLACK_KING;
  square[D8] = BLACK_QUEEN;
  square[A8] = BLACK_ROOK;
  square[H8] = BLACK_ROOK;
  square[B8] = BLACK_KNIGHT;
  square[G8] = BLACK_KNIGHT;
  square[C8] = BLACK_BISHOP;
  square[F8] = BLACK_BISHOP;
  square[A7] = BLACK_PAWN;
  square[B7] = BLACK_PAWN;
  square[C7] = BLACK_PAWN;
  square[D7] = BLACK_PAWN;
  square[E7] = BLACK_PAWN;
  square[F7] = BLACK_PAWN;
  square[G7] = BLACK_PAWN;
  square[H7] = BLACK_PAWN;
  
  initFromSquares(square, WHITE_MOVE, 0, CANCASTLEOO + CANCASTLEOOO, CANCASTLEOO + CANCASTLEOOO, 0);
}

void Board::initFromSquares(int input[64], unsigned char next, int fiftyM, int castleW, int castleB, int epSq){
  // sets up the board variables according to the information found in
  // the input[64] array
  // All board & game initializations are done through this function (including readfen and setup).
  
  
  int i;
  
  //bit boards
  whiteKing    = 0;
  whiteQueens  = 0;
  whiteRooks   = 0;
  whiteBishops = 0;
  whiteKnights = 0;
  whitePawns   = 0;
  blackKing    = 0;
  blackQueens  = 0;
  blackRooks   = 0;
  blackBishops = 0;
  blackKnights = 0;
  blackPawns   = 0;
  whitePieces  = 0;
  blackPieces  = 0;
  occupiedSquares = 0;
  
  // populate the 12 bitboard:
  for (i = 0; i < 64; i++){
    square[i] = input[i];
    if (square[i] == WHITE_KING)   whiteKing    = whiteKing    | BITSET[i];
    if (square[i] == WHITE_QUEEN)  whiteQueens  = whiteQueens  | BITSET[i];
    if (square[i] == WHITE_ROOK)   whiteRooks   = whiteRooks   | BITSET[i];
    if (square[i] == WHITE_BISHOP) whiteBishops = whiteBishops | BITSET[i];
    if (square[i] == WHITE_KNIGHT) whiteKnights = whiteKnights | BITSET[i];
    if (square[i] == WHITE_PAWN)   whitePawns   = whitePawns   | BITSET[i];
    if (square[i] == BLACK_KING)   blackKing    = blackKing    | BITSET[i];
    if (square[i] == BLACK_QUEEN)  blackQueens  = blackQueens  | BITSET[i];
    if (square[i] == BLACK_ROOK)   blackRooks   = blackRooks   | BITSET[i];
    if (square[i] == BLACK_BISHOP) blackBishops = blackBishops | BITSET[i];
    if (square[i] == BLACK_KNIGHT) blackKnights = blackKnights | BITSET[i];
    if (square[i] == BLACK_PAWN)   blackPawns   = blackPawns   | BITSET[i];
  }
  
  whitePieces = whiteKing | whiteQueens | whiteRooks | whiteBishops | whiteKnights | whitePawns;
  blackPieces = blackKing | blackQueens | blackRooks | blackBishops | blackKnights | blackPawns;
  occupiedSquares = whitePieces | blackPieces;
  
  nextMove = next;
  castleWhite = castleW;
  castleBlack = castleB;
  epSquare = epSq;
  fiftyMove = fiftyM;
  
  Material    = bitCnt(whitePawns) * PAWN_VALUE +
                      bitCnt(whiteKnights) * KNIGHT_VALUE +
                      bitCnt(whiteBishops) * BISHOP_VALUE +
                      bitCnt(whiteRooks) * ROOK_VALUE +
                      bitCnt(whiteQueens) * QUEEN_VALUE;

  Material   -= (bitCnt(blackPawns) * PAWN_VALUE +
                      bitCnt(blackKnights) * KNIGHT_VALUE +
                      bitCnt(blackBishops) * BISHOP_VALUE +
                      bitCnt(blackRooks) * ROOK_VALUE +
                      bitCnt(blackQueens) * QUEEN_VALUE);
  
  return;
}

void Board::display(){
  int rank, file;
  
  cout << endl;
  
  {
    if (!viewRotated) {
      for (rank = 8; rank >= 1; rank--) {
        cout << "    +---+---+---+---+---+---+---+---+" << endl;
        cout << setw(3) << rank <<  " |";
        for (file= 1; file<=8; file++) {
          cout << " " << PIECENAMES[square[BOARDINDEX[file][rank]]] << "|";
        }
        cout << endl;
      }
      cout << "    +---+---+---+---+---+---+---+---+" << endl;
      cout << "      a   b   c   d   e   f   g   h" << endl << endl;
    }else{
     cout << "      h   g   f   e   d   c   b   a" << endl;
      for (rank = 1; rank <= 8; rank++) {
        cout << "    +---+---+---+---+---+---+---+---+" << endl;
        cout << "    |";
        for (file = 8; file>=1; file--) {
          cout << " " << PIECENAMES[square[BOARDINDEX[file][rank]]] << "|";
        }
        cout << setw(3) << rank << endl;
      }
      cout << "    +---+---+---+---+---+---+---+---+" << endl << endl;
    }
  }
  return;
}


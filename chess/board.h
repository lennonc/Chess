//
//  board.h
//  chess
//
//  Created by Lennon Chimbumu on 11/27/11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#ifndef chess_board_h
#define chess_board_h

#include "defines.h"

struct Board{
  BitMap whiteKing, whiteQueens, whiteRooks, whiteBishops, whiteKnights, whitePawns;
  BitMap blackKing, blackQueens, blackRooks, blackBishops, blackKnights, blackPawns;
  BitMap whitePieces, blackPieces, occupiedSquares;
  
  unsigned char nextMove;        // WHITE_MOVE or BLACK_MOVE
  unsigned char castleWhite;     // White's castle status, CANCASTLEOO = 1, CANCASTLEOOO = 2
  unsigned char castleBlack;     // Black's castle status, CANCASTLEOO = 1, CANCASTLEOOO = 2
  int epSquare;                  // En-passant target square after double pawn move
  int fiftyMove;
  
  // additional variables:
  int Material;                  // incrementally updated, total material on board,
                                 // in centipawns, from white’s side of view
  int square[64];                // incrementally updated, this array is usefull if we want to
                                 // probe what kind of piece is on a particular square.
  BOOLTYPE viewRotated;          // only used for displaying the board. TRUE or FALSE.
  
  void init();
  void initFromSquares(int input[64], unsigned char next, int fiftyM, int castleW, int castleB, int epSq);
  void initFromFen(char fen[], char fencolor[], char fencastling[], char fenenpassant[], int fenhalfmoveclock, int fenfullmovenumber);
  void display();
};

#endif

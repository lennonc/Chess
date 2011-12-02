//
//  board.h
//  chess
//
//  Created by Lennon Chimbumu on 11/27/11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#ifndef WINGLET_BOARD_H_
#define WINGLET_BOARD_H_

#include "defines.h"
#include "move.h"
#include "gameline.h"
#include "timer.h"

struct Board
{
  BitMap whiteKing, whiteQueens, whiteRooks, whiteBishops, whiteKnights, whitePawns;
  BitMap blackKing, blackQueens, blackRooks, blackBishops, blackKnights, blackPawns;
  BitMap whitePieces, blackPieces, occupiedSquares;
  
  unsigned char nextMove;        // WHITE_MOVE or BLACK_MOVE
  unsigned char castleWhite;     // White's castle status, CANCASTLEOO = 1, CANCASTLEOOO = 2
  unsigned char castleBlack;     // Black's castle status, CANCASTLEOO = 1, CANCASTLEOOO = 2
  int epSquare;                  // En-passant target square after double pawn move
  int fiftyMove;                 // Moves since the last pawn move or capture
  U64 hashkey;                   // Random 'almost' unique signature for current board position.
  
  // additional variables:
  int Material;                  // incrementally updated, total material on board,
  // in centipawns, from white’s side of view
  int square[64];                // incrementally updated, this array is usefull if we want to
  // probe what kind of piece is on a particular square.
  
  BOOLTYPE viewRotated;          // only used for displaying the board. TRUE or FALSE.
  
  // storing moves:
  Move moveBuffer[MAX_MOV_BUFF]; // all generated moves of the current search tree are stored in this array.
  int moveBufLen[MAX_PLY];       // this arrays keeps track of which moves belong to which ply
  int endOfGame;                 // index for board.gameLine
  int endOfSearch;               // index for board.gameLine
  GameLineRecord gameLine[MAX_GAME_LINE];
  
  // search variables:
  int triangularLength[MAX_PLY];
  Move triangularArray[MAX_PLY][MAX_PLY];
  Timer timer;
  U64 msStart, msStop;
  int searchDepth;
  int lastPVLength;
  Move lastPV[MAX_PLY];
  unsigned int whiteHeuristics[64][64];
  unsigned int blackHeuristics[64][64];
  BOOLTYPE followpv;
  U64 inodes;
  
  void init();
  int eval();
  Move think();
  int minimax(int ply, int depth);
  int alphabeta(int ply, int depth, int alpha, int beta);
  int alphabetapvs(int ply, int depth, int alpha, int beta);
  int qsearch(int ply, int alpha, int beta);
  void displaySearchStats(int mode, int depth, int score);
  BOOLTYPE isEndOfgame(int &legalmoves, Move &singlemove);
  int repetitionCount();
  void mirror();
  void initFromSquares(int input[64], unsigned char next, int fiftyM, int castleW, int castleB, int epSq);
  void initFromFen(char fen[], char fencolor[], char fencastling[], char fenenpassant[], int fenhalfmoveclock, int fenfullmovenumber);
  void display();
  void rememberPV();
  void selectmove(int &ply, int &i, int &depth, BOOLTYPE &followpv);
  void addCaptScore(int &ifirst, int &index);
  int SEE(Move &move);
  BitMap attacksTo(int &target);
  BitMap revealNextAttacker(BitMap &attackers, BitMap &nonremoved, int &target, int &heading);
};

#endif
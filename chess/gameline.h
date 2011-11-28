//
//  gameline.h
//  chess
//
//  Created by Lennon Chimbumu on 11/27/11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#ifndef chess_gameline_h
#define chess_gameline_h

#include "move.h"

struct GameLineRecord{
  Move move;
  unsigned char castleWhite;     // White's castle status, CANCASTLEOO = 1, CANCASTLEOOO = 2
  unsigned char castleBlack;     // Black's castle status, CANCASTLEOO = 1, CANCASTLEOOO = 2
  int epSquare;                  // En-passant target square after double pawn move
  int fiftyMove;                 // Moves since the last pawn move or capture
};

#endif

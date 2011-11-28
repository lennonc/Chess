//
//  displaymove.cpp
//  chess
//
//  Created by Lennon Chimbumu on 11/27/11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#include <iostream>
#include "defines.h"
#include "protos.h"
#include "extglobals.h"


using namespace std;

void displayMove(Move &move){
  // displays a single move on the console, no disambiguation
  
  if ((move.getPiec() == WHITE_KING) && (move.isCastleOO())){
    cout << "O-O";
    return;      
  };
  
  if ((move.getPiec() == WHITE_KING) && (move.isCastleOOO())){
    cout << "O-O-O";
    return;      
  };
  
  if ((move.getPiec() == BLACK_KING) && (move.isCastleOO())){
    cout << "O-O";
    return;      
  };
  
  if ((move.getPiec() == BLACK_KING) && (move.isCastleOOO())){
    cout << "O-O-O";
    return;      
  };
  
  if ((move.getPiec() == WHITE_ROOK)   || (move.getPiec() == BLACK_ROOK))   cout << "R";
  if ((move.getPiec() == WHITE_BISHOP) || (move.getPiec() == BLACK_BISHOP)) cout << "B";
  if ((move.getPiec() == WHITE_KNIGHT) || (move.getPiec() == BLACK_KNIGHT)) cout << "N";
  if ((move.getPiec() == WHITE_KING)   || (move.getPiec() == BLACK_KING))   cout << "K";
  if ((move.getPiec() == WHITE_QUEEN)  || (move.getPiec() == BLACK_QUEEN))  cout << "Q";
  if (((move.getPiec() == WHITE_PAWN)  || (move.getPiec() == BLACK_PAWN)) && move.isCapture()){
    if (FILES[move.getFrom()] == 1) cout << "a";
    if (FILES[move.getFrom()] == 2) cout << "b";
    if (FILES[move.getFrom()] == 3) cout << "c";
    if (FILES[move.getFrom()] == 4) cout << "d";
    if (FILES[move.getFrom()] == 5) cout << "e";
    if (FILES[move.getFrom()] == 6) cout << "f";
    if (FILES[move.getFrom()] == 7) cout << "g";
    if (FILES[move.getFrom()] == 8) cout << "h";
  }
  
  if (move.isCapture()) cout << "x" ;
  if (FILES[move.getTosq()] == 1) cout << "a";
  if (FILES[move.getTosq()] == 2) cout << "b";
  if (FILES[move.getTosq()] == 3) cout << "c";
  if (FILES[move.getTosq()] == 4) cout << "d";
  if (FILES[move.getTosq()] == 5) cout << "e";
  if (FILES[move.getTosq()] == 6) cout << "f";
  if (FILES[move.getTosq()] == 7) cout << "g";
  if (FILES[move.getTosq()] == 8) cout << "h";
  
  cout << RANKS[move.getTosq()];
  
  if (move.isPromotion()){
    if ((move.getProm() == WHITE_ROOK)   || (move.getProm() == BLACK_ROOK))   cout << "=R";
    if ((move.getProm() == WHITE_BISHOP) || (move.getProm() == BLACK_BISHOP)) cout << "=B";
    if ((move.getProm() == WHITE_KNIGHT) || (move.getProm() == BLACK_KNIGHT)) cout << "=N";
    if ((move.getProm() == WHITE_KING)   || (move.getProm() == BLACK_KING))   cout << "=K";
    if ((move.getProm() == WHITE_QUEEN)  || (move.getProm() == BLACK_QUEEN))  cout << "=Q";
  }
  
  cout.flush();
  return;
}

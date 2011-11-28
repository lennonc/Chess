//
//  protos.h
//  chess
//
//  Created by Lennon Chimbumu on 11/27/11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#ifndef chess_protos_h
#define chess_protos_h

#include "move.h"

unsigned int  bitCnt(BitMap);
void          dataInit();
void          displayBitmap(BitMap);
void          displayPV();
void          displayMove(Move &);
BOOLTYPE      doCommand(const char *);
unsigned int  firstOne(BitMap);
void          info();
BOOLTYPE      isAttacked(BitMap &, const unsigned char &);
BOOLTYPE      isValidTextMove(char *, Move &);
BOOLTYPE      isOtherKingAttacked();
BOOLTYPE      isOwnKingAttacked();
unsigned int  lastOne(BitMap);
void          makeBlackPromotion(unsigned int, unsigned int &);
void          makeCapture(unsigned int &, unsigned int &);
void          makeMove(Move &);
void          makeWhitePromotion(unsigned int, unsigned int &);
int           movegen(int);
U64           perft(int, int);
void          readCommands();
BOOLTYPE      readFen(char *, int);
BOOLTYPE      toSan(Move &, char *);
void          setup();
void          setupFen(char *, char *, char *, char *, int , int );
void          unmakeBlackPromotion(unsigned int, unsigned int &);
void          unmakeCapture(unsigned int &, unsigned int &);
void          unmakeMove(Move &);
void          unmakeWhitePromotion(unsigned int, unsigned int &);
#endif



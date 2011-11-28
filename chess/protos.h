//
//  protos.h
//  chess
//
//  Created by Lennon Chimbumu on 11/27/11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#ifndef chess_protos_h
#define chess_protos_h

unsigned int  bitCnt(BitMap);
void          dataInit();
void          displayBitmap(BitMap);
BOOLTYPE      doCommand(const char *);
unsigned int  firstOne(BitMap);
void          info();
unsigned int  lastOne(BitMap);
void          readCommands();

#endif

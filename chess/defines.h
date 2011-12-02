//
//  defines.h
//  chess
//
//  Created by Lennon Chimbumu on 11/27/11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#ifndef chess_defines_h
#define chess_defines_h

#define WINGLET_PROG_VERSION "winglet 0.0, Copyright (C) 2011, Lennon Chimbumu"

#define MAX_CMD_BUFF 256   // Console command input buffer
#define MAX_MOV_BUFF 4096    // Number of moves that we can store (all plies)
#define MAX_PLY      64      // Search depth
#define MAX_GAME_LINE    1024   // Number of moves in the (game + search) line that we can store
//#define WINGLET_DEBUG_PERFT
//#define WINGLET_DEBUG_MOVES
//#define WINGLET_VERBOSE_EVAL
//#define WINGLET_DEBUG_EVAL
//#define WINGLET_VERBOSE_SEE


typedef int BOOLTYPE;
typedef unsigned long long U64;
typedef unsigned long long BitMap;
typedef short SHORTINT;
typedef unsigned short USHORTINT;
typedef int BOOLTYPE;

#endif

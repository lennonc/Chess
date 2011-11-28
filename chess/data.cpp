//
//  data.cpp
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

void dataInit(){
  int i, rank, file;
  
  // BITSET has only one bit set:
  BITSET[0] = 0x1;
  for (i = 1; i < 64 ; i++){
    BITSET[i] = BITSET[i-1] << 1;
  }
  
  //  BOARDINDEX is used to translate [file][rank] to [square],
  //  Note that file is from 1..8 and rank from 1..8 (not starting from 0)
  for (rank = 0 ; rank < 9; rank++){
    for (file = 0 ; file < 9; file++)
    {
      BOARDINDEX[file][rank] = (rank-1) * 8 + file - 1;
    }
  }
  
  // Initialize the board
  board.init();
  
  //     Initialize MS1BTABLE, used in lastOne (see bitops.cpp)
  for (i = 0; i < 256; i++){
    MS1BTABLE[i] = (
                    (i > 127) ? 7 :
                    (i >  63) ? 6 :
                    (i >  31) ? 5 :
                    (i >  15) ? 4 :
                    (i >   7) ? 3 :
                    (i >   3) ? 2 :
                    (i >   1) ? 1 : 0 );
  }
  
  return;
}

void info()
{
  
  std::cout << std::endl << "============ info start ==============" << std::endl;
  std::cout << "size of board, in bytes   = " << sizeof(board) << std::endl;
  std::cout << "Material value            = " << board.Material << std::endl;
  std::cout << "White castling rights     = " << int(board.castleWhite) << std::endl;
  std::cout << "Black castling rights     = " << int(board.castleBlack) << std::endl;
  std::cout << "En-passant square         = " << board.epSquare << std::endl;
  std::cout << "Fifty move count          = " << board.fiftyMove << std::endl;
  
  std::cout << "bitCnt of white pawns     = " << bitCnt(board.whitePawns) << std::endl;
  std::cout << std::endl << "bitmap of blackKnights | board.whitePawns:" << std::endl;
  displayBitmap(board.blackKnights | board.whitePawns);
  std::cout << "============ info end ================" << std::endl << std::endl;
  
  return;
}

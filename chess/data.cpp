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
  
  unsigned char CHARBITSET[8];
  int i, square, rank, file, arank, afile, state, slide, diaga1h8, diaga8h1, attackbit;
  unsigned char state6Bit, state8Bit, attack8Bit;
  Move move;
  
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
  
  
  
  //     Initialize rank, file and diagonal 6-bit masking bitmaps, to get the
  //     occupancy state, used in the movegenerator (see movegen.ccp)
  for (square = 0; square < 64; square++){
    RANKMASK[square] = 0x0;
    FILEMASK[square] = 0x0;
    DIAGA8H1MASK[square] = 0x0;
    DIAGA1H8MASK[square] = 0x0;
    FILEMAGIC[square] = 0x0;
    DIAGA8H1MAGIC[square] = 0x0;
    DIAGA1H8MAGIC[square] = 0x0;
  }  
  
  for (file = 1; file < 9; file++) {
    for (rank = 1; rank < 9; rank++) {
      //initialize 6-bit rank mask, used in the movegenerator (see movegen.ccp)
      RANKMASK[BOARDINDEX[file][rank]]  = BITSET[BOARDINDEX[2][rank]] | BITSET[BOARDINDEX[3][rank]] | BITSET[BOARDINDEX[4][rank]] ;
      RANKMASK[BOARDINDEX[file][rank]] |= BITSET[BOARDINDEX[5][rank]] | BITSET[BOARDINDEX[6][rank]] | BITSET[BOARDINDEX[7][rank]] ;
      
      //initialize 6-bit file mask, used in the movegenerator (see movegen.ccp)
      FILEMASK[BOARDINDEX[file][rank]]  = BITSET[BOARDINDEX[file][2]] | BITSET[BOARDINDEX[file][3]] | BITSET[BOARDINDEX[file][4]] ;
      FILEMASK[BOARDINDEX[file][rank]] |= BITSET[BOARDINDEX[file][5]] | BITSET[BOARDINDEX[file][6]] | BITSET[BOARDINDEX[file][7]] ;
      
      //Initialize diagonal magic multiplication numbers, used in the movegenerator (see movegen.ccp)
      diaga8h1 = file + rank; // from 2 to 16, longest diagonal = 9
      DIAGA8H1MAGIC[BOARDINDEX[file][rank]] = _DIAGA8H1MAGICS[diaga8h1 - 2];
      
      //Initialize 6-bit diagonal mask, used in the movegenerator (see movegen.ccp)
      DIAGA8H1MASK[BOARDINDEX[file][rank]] = 0x0;
      if (diaga8h1 < 10){  // lower half, diagonals 2 to 9
        for (square = 2 ; square < diaga8h1-1 ; square ++){
          DIAGA8H1MASK[BOARDINDEX[file][rank]] |= BITSET[BOARDINDEX[square][diaga8h1-square]];
        }
      }else{  // upper half, diagonals 10 to 16
        for (square = 2 ; square < 17 - diaga8h1 ; square ++){
          DIAGA8H1MASK[BOARDINDEX[file][rank]] |= BITSET[BOARDINDEX[diaga8h1+square-9][9-square]];
        }
      }
      
      //Initialize diagonal magic multiplication numbers, used in the movegenerator (see movegen.ccp)
      diaga1h8 = file - rank; // from -7 to +7, longest diagonal = 0
      DIAGA1H8MAGIC[BOARDINDEX[file][rank]] = _DIAGA1H8MAGICS[diaga1h8+7];
      
      //Initialize 6-bit diagonal mask, used in the movegenerator (see movegen.ccp)
      DIAGA1H8MASK[BOARDINDEX[file][rank]] = 0x0;
      if (diaga1h8 > -1){  // lower half, diagonals 0 to 7
      
        for (square = 2 ; square < 8 - diaga1h8 ; square ++){
          DIAGA1H8MASK[BOARDINDEX[file][rank]] |= BITSET[BOARDINDEX[diaga1h8 + square][square]];
        }
      }else{
        for (square = 2 ; square < 8 + diaga1h8 ; square ++){
          DIAGA1H8MASK[BOARDINDEX[file][rank]] |= BITSET[BOARDINDEX[square][square - diaga1h8]];
        }
      }
      
      //Initialize file magic multiplication numbers, used in the movegenerator (see movegen.ccp)
      FILEMAGIC[BOARDINDEX[file][rank]] = _FILEMAGICS[file-1];
    }
  }
  /*
   Now initialize the GEN_SLIDING_ATTACKS array, used to generate the sliding
   attack bitboards.
   unsigned char GEN_SLIDING_ATTACKS[8 squares][64 states] holds the attacks
   for any file, rank or diagonal - it is going to be usefull when generating the
   RANK_ATTACKS[64][64], FILE_ATTACKS[64][64], DIAGA8H1_ATTACKS[64][64] and
   DIAGA1H8_ATTACKS[64][64] arrays
  */
  
  // initialize CHARBITSET, this array is equivalant to BITSET for bitboards:
  // 8 chars, each with only 1 bit set.
  
  CHARBITSET[0] = 1;
  for (square = 1; square <= 7; square++){
    CHARBITSET[square] = CHARBITSET[square-1] << 1;
  }
  
  for (square = 0; square <= 7; square++){
    // loop of occupancy states
    // state6Bit represents the 64 possible occupancy states of a rank,
    // except the 2 end-bits, because they don't matter for calculating attacks
    for (state6Bit = 0; state6Bit < 64; state6Bit++){
      state8Bit = state6Bit << 1; // create an 8-bit occupancy state
      attack8Bit = 0;
      if (square < 7){
        attack8Bit |= CHARBITSET[square + 1];
      }
      slide = square + 2;
      while (slide <= 7) // slide in '+' direction
      {
        if ((~state8Bit) & (CHARBITSET[slide - 1])){
          attack8Bit |= CHARBITSET[slide];
        }
        else break;
        slide++;
      }
      if (square > 0){
        attack8Bit |= CHARBITSET[square - 1];
      }
      slide = square - 2;
      while (slide >= 0) // slide in '-' direction
      {
        if ((~state8Bit) & (CHARBITSET[slide + 1])){
          attack8Bit |= CHARBITSET[slide];
        }
        else break;
        slide--;
      }
      GEN_SLIDING_ATTACKS[square][state6Bit] = attack8Bit;
    }
  }
  
  
  //Initialize all attack bitmaps, used in the movegenerator (see movegen.ccp)
  for (square = 0; square < 64; square++)
  {
    KNIGHT_ATTACKS[square] = 0x0;
    KING_ATTACKS[square] = 0x0;
    WHITE_PAWN_ATTACKS[square] = 0x0;
    WHITE_PAWN_MOVES[square] = 0x0;
    WHITE_PAWN_DOUBLE_MOVES[square] = 0x0;
    BLACK_PAWN_ATTACKS[square] = 0x0;
    BLACK_PAWN_MOVES[square] = 0x0;
    BLACK_PAWN_DOUBLE_MOVES[square] = 0x0;
    
    for (state = 0; state < 64; state++){
      RANK_ATTACKS[square][state] = 0x0;
      FILE_ATTACKS[square][state] = 0x0;
      DIAGA8H1_ATTACKS[square][state] = 0x0;
      DIAGA1H8_ATTACKS[square][state] = 0x0;
    }
  }
  
  // WHITE_PAWN_ATTACKS
  for (square = 0; square < 64; square++)
  {
    file = FILES[square]; rank = RANKS[square];
    afile = file - 1; arank = rank + 1;
    if ((afile >= 1) & (afile <= 8) & (arank >= 1) & (arank <= 8))
      WHITE_PAWN_ATTACKS[square] |= BITSET[BOARDINDEX[afile][arank]];
    afile = file + 1; arank = rank + 1;
    if ((afile >= 1) & (afile <= 8) & (arank >= 1) & (arank <= 8))
      WHITE_PAWN_ATTACKS[square] |= BITSET[BOARDINDEX[afile][arank]];
  }
  
  // WHITE_PAWN_MOVES
  for (square = 0; square <64; square++)
  {
    file = FILES[square]; rank = RANKS[square];
    afile = file; arank = rank + 1;
    if ((afile >= 1) & (afile <= 8) & (arank >= 1) & (arank <= 8))
      WHITE_PAWN_MOVES[square] |= BITSET[BOARDINDEX[afile][arank]];       
    if (rank == 2)
    {
      afile = file; arank = rank + 2;
      if ((afile >= 1) & (afile <= 8) & (arank >= 1) & (arank <= 8))
        WHITE_PAWN_DOUBLE_MOVES[square] |= BITSET[BOARDINDEX[afile][arank]];
    }
  }
  
  // BLACK_PAWN_ATTACKS
  for (square = 0; square < 64; square++)
  {
    file = FILES[square]; rank = RANKS[square];
    afile = file - 1; arank = rank - 1;
    if ((afile >= 1) & (afile <= 8) & (arank >= 1) & (arank <= 8))
      BLACK_PAWN_ATTACKS[square] |= BITSET[BOARDINDEX[afile][arank]];
    afile = file + 1; arank = rank - 1;
    if ((afile >= 1) & (afile <= 8) & (arank >= 1) & (arank <= 8))
      BLACK_PAWN_ATTACKS[square] |= BITSET[BOARDINDEX[afile][arank]];
  }
  
  // BLACK_PAWN_MOVES
  for (square = 0; square < 64; square++)
  {
    file = FILES[square]; rank = RANKS[square];
    afile = file; arank = rank - 1;
    if ((afile >= 1) & (afile <= 8) & (arank >= 1) & (arank <= 8))
      BLACK_PAWN_MOVES[square] |= BITSET[BOARDINDEX[afile][arank]];
    if (rank == 7)
    {
      afile = file; arank = rank - 2;
      if ((afile >= 1) & (afile <= 8) & (arank >= 1) & (arank <= 8))
        BLACK_PAWN_DOUBLE_MOVES[square] |= BITSET[BOARDINDEX[afile][arank]];
    }
  }
  
  // KNIGHT attacks;
  for (square = 0; square < 64; square++)
  {
    file = FILES[square];
    rank = RANKS[square];
    afile = file - 2; arank = rank + 1;
    if ((afile >= 1) & (afile <= 8) & (arank >= 1) & (arank <= 8))
      KNIGHT_ATTACKS[square] |= BITSET[BOARDINDEX[afile][arank]];
    afile = file - 1; arank = rank + 2;
    if ((afile >= 1) & (afile <= 8) & (arank >= 1) & (arank <= 8))
      KNIGHT_ATTACKS[square] |= BITSET[BOARDINDEX[afile][arank]];
    afile = file + 1; arank = rank + 2;
    if ((afile >= 1) & (afile <= 8) & (arank >= 1) & (arank <= 8))
      KNIGHT_ATTACKS[square] |= BITSET[BOARDINDEX[afile][arank]];
    afile = file + 2; arank = rank + 1;
    if ((afile >= 1) & (afile <= 8) & (arank >= 1) & (arank <= 8))
      KNIGHT_ATTACKS[square] |= BITSET[BOARDINDEX[afile][arank]];
    afile = file + 2; arank = rank - 1;
    if ((afile >= 1) & (afile <= 8) & (arank >= 1) & (arank <= 8))
      KNIGHT_ATTACKS[square] |= BITSET[BOARDINDEX[afile][arank]];
    afile = file + 1; arank = rank - 2;
    if ((afile >= 1) & (afile <= 8) & (arank >= 1) & (arank <= 8))
      KNIGHT_ATTACKS[square] |= BITSET[BOARDINDEX[afile][arank]];
    afile = file - 1; arank = rank - 2;
    if ((afile >= 1) & (afile <= 8) & (arank >= 1) & (arank <= 8))
      KNIGHT_ATTACKS[square] |= BITSET[BOARDINDEX[afile][arank]];
    afile = file - 2; arank = rank - 1;
    if ((afile >= 1) & (afile <= 8) & (arank >= 1) & (arank <= 8))
      KNIGHT_ATTACKS[square] |= BITSET[BOARDINDEX[afile][arank]];
  }
  
  // KING attacks;
  for (square = 0; square < 64; square++)
  {
    file = FILES[square]; 
    rank = RANKS[square];
    afile = file - 1; arank = rank;
    if ((afile >= 1) & (afile <= 8) & (arank >= 1) & (arank <= 8))
      KING_ATTACKS[square] |= BITSET[BOARDINDEX[afile][arank]];
    afile = file - 1; arank = rank + 1;
    if ((afile >= 1) & (afile <= 8) & (arank >= 1) & (arank <= 8))
      KING_ATTACKS[square] |= BITSET[BOARDINDEX[afile][arank]];
    afile = file; arank = rank + 1;
    if ((afile >= 1) & (afile <= 8) & (arank >= 1) & (arank <= 8))
      KING_ATTACKS[square] |= BITSET[BOARDINDEX[afile][arank]];
    afile = file + 1; arank = rank + 1;
    if ((afile >= 1) & (afile <= 8) & (arank >= 1) & (arank <= 8))
      KING_ATTACKS[square] |= BITSET[BOARDINDEX[afile][arank]];
    afile = file + 1; arank = rank;
    if ((afile >= 1) & (afile <= 8) & (arank >= 1) & (arank <= 8))
      KING_ATTACKS[square] |= BITSET[BOARDINDEX[afile][arank]];
    afile = file + 1; arank = rank - 1;
    if ((afile >= 1) & (afile <= 8) & (arank >= 1) & (arank <= 8))
      KING_ATTACKS[square] |= BITSET[BOARDINDEX[afile][arank]];
    afile = file; arank = rank - 1;
    if ((afile >= 1) & (afile <= 8) & (arank >= 1) & (arank <= 8))
      KING_ATTACKS[square] |= BITSET[BOARDINDEX[afile][arank]];
    afile = file - 1; arank = rank - 1;
    if ((afile >= 1) & (afile <= 8) & (arank >= 1) & (arank <= 8))
      KING_ATTACKS[square] |= BITSET[BOARDINDEX[afile][arank]];
  }
  
  //  RANK attacks (ROOKS and QUEENS):
  //  use           unsigned char GEN_SLIDING_ATTACKS[8 squares] [64 states]
  //  to initialize BitMap        RANK_ATTACKS       [64 squares][64 states]
  //
  for (square = 0; square < 64; square++)
  {
    for (state6Bit = 0; state6Bit < 64; state6Bit++)
    {
      RANK_ATTACKS[square][state6Bit] = 0;
      RANK_ATTACKS[square][state6Bit] |=
      BitMap(GEN_SLIDING_ATTACKS[FILES[square]-1][state6Bit]) << (RANKSHIFT[square] - 1);
    }
  }
  
  //  FILE attacks (ROOKS and QUEENS):
  //  use           unsigned char GEN_SLIDING_ATTACKS[8 squares] [64 states]
  //  to initialize BitMap        FILE_ATTACKS       [64 squares][64 states]
  //
  //  Occupancy transformation is as follows:
  //
  //   occupancy state bits of the file:               occupancy state bits in GEN_SLIDING_ATTACKS:
  //
  //        . . . . . . . . MSB                           LSB         MSB
  //        . . . . . A . .                    =>         A B C D E F . .                            
  //        . . . . . B . .
  //        . . . . . C . .
  //        . . . . . D . .
  //        . . . . . E . .
  //        . . . . . F . .
  //    LSB . . . . . . . .
  //
  //  The reverse transformation is as follows:
  //
  //   attack bits in GEN_SLIDING_ATTACKS:             attack bits in the file:
  //
  //        LSB         MSB                               . . . . . m . . MSB
  //        m n o p q r s t                    =>         . . . . . n . .
  //                                                      . . . . . o . .
  //                                                      . . . . . p . .
  //                                                      . . . . . q . .
  //                                                      . . . . . r . .
  //                                                      . . . . . s . .
  //                                                 LSB  . . . . . t . .
  //
  for (square = 0; square < 64; square++)
  {
    for (state6Bit = 0; state6Bit < 64; state6Bit++)
    {
      FILE_ATTACKS[square][state6Bit] = 0x0;
      
      // check to see if attackbit'-th  bit is set in GEN_SLIDING_ATTACKS, for this combination of square/occupancy state
      for (attackbit = 0; attackbit < 8; attackbit++) // from LSB to MSB
      {
        //  conversion from 64 board squares to the 8 corresponding positions in the GEN_SLIDING_ATTACKS array: "8-RANKS[square]"
        if (GEN_SLIDING_ATTACKS[8-RANKS[square]][state6Bit] & CHARBITSET[attackbit])
        {
          // the bit is set, so we need to update FILE_ATTACKS accordingly:
          // conversion of square/attackbit to the corresponding 64 board FILE: FILES[square]
          // conversion of square/attackbit to the corresponding 64 board RANK: 8-attackbit
          file = FILES[square];
          rank = 8 - attackbit;
          FILE_ATTACKS[square][state6Bit] |=  BITSET[BOARDINDEX[file][rank]];
        }
      }
    }
  }
  
  //  DIAGA8H1_ATTACKS attacks (BISHOPS and QUEENS):
  for (square = 0; square < 64; square++)
  {
    for (state6Bit = 0; state6Bit < 64; state6Bit++)
    {
      DIAGA8H1_ATTACKS[square][state6Bit] = 0x0;
      for (attackbit = 0; attackbit < 8; attackbit++) // from LSB to MSB
      {
        //  conversion from 64 board squares to the 8 corresponding positions in the GEN_SLIDING_ATTACKS array: MIN((8-RANKS[square]),(FILES[square]-1))
        if (GEN_SLIDING_ATTACKS[(8-RANKS[square]) < (FILES[square]-1) ? (8-RANKS[square]) : (FILES[square]-1)][state6Bit] & CHARBITSET[attackbit])
        {
          // the bit is set, so we need to update FILE_ATTACKS accordingly:
          // conversion of square/attackbit to the corresponding 64 board file and rank:
          diaga8h1 = FILES[square] + RANKS[square]; // from 2 to 16, longest diagonal = 9
          if (diaga8h1 < 10)
          {
            file = attackbit + 1;
            rank = diaga8h1 - file;
          }
          else
          {
            rank = 8 - attackbit;
            file = diaga8h1 - rank;
          }
          if ((file > 0) && (file < 9) && (rank > 0) && (rank < 9))
          {
            DIAGA8H1_ATTACKS[square][state6Bit] |=  BITSET[BOARDINDEX[file][rank]];
          }
        }
      }
    }
  }
  
  //  DIAGA1H8_ATTACKS attacks (BISHOPS and QUEENS):
  for (square = 0; square < 64; square++)
  {
    for (state6Bit = 0; state6Bit < 64; state6Bit++)
    {
      DIAGA1H8_ATTACKS[square][state6Bit] = 0x0;
      for (attackbit = 0; attackbit < 8; attackbit++) // from LSB to MSB
      {
        //  conversion from 64 board squares to the 8 corresponding positions in the GEN_SLIDING_ATTACKS array: MIN((8-RANKS[square]),(FILES[square]-1))
        if (GEN_SLIDING_ATTACKS[(RANKS[square]-1) < (FILES[square]-1) ? (RANKS[square]-1) : (FILES[square]-1)][state6Bit] & CHARBITSET[attackbit])
        {
          // the bit is set, so we need to update FILE_ATTACKS accordingly:
          // conversion of square/attackbit to the corresponding 64 board file and rank:
          diaga1h8 = FILES[square] - RANKS[square]; // from -7 to 7, longest diagonal = 0
          if (diaga1h8 < 0)
          {
            file = attackbit + 1;
            rank = file - diaga1h8;
          }
          else
          {
            rank = attackbit + 1;
            file = diaga1h8 + rank;
          }
          if ((file > 0) && (file < 9) && (rank > 0) && (rank < 9))
          {
            DIAGA1H8_ATTACKS[square][state6Bit] |=  BITSET[BOARDINDEX[file][rank]];
          }
        }
      }
    }
  }
  
  
  //     Masks for castling, index 0 is for white, 1 is for black
  
  maskEG[0] = BITSET[E1] | BITSET[F1] | BITSET[G1];
  maskEG[1] = BITSET[E8] | BITSET[F8] | BITSET[G8];
  
  maskFG[0] = BITSET[F1] | BITSET[G1];
  maskFG[1] = BITSET[F8] | BITSET[G8];
  
  maskBD[0] = BITSET[B1] | BITSET[C1] | BITSET[D1];
  maskBD[1] = BITSET[B8] | BITSET[C8] | BITSET[D8];
  
  maskCE[0] = BITSET[C1] | BITSET[D1] | BITSET[E1];
  maskCE[1] = BITSET[C8] | BITSET[D8] | BITSET[E8];
  
  
  //The 4 castling moves can be predefined:
  
  move.clear();
  move.setCapt(EMPTY);
  move.setPiec(WHITE_KING);
  move.setProm(WHITE_KING);
  move.setFrom(E1);
  move.setTosq(G1);
  WHITE_OO_CASTL = move.moveInt;
  move.setTosq(C1);
  WHITE_OOO_CASTL = move.moveInt;
  
  move.setPiec(BLACK_KING);
  move.setProm(BLACK_KING);
  move.setFrom(E8);
  move.setTosq(G8);
  BLACK_OO_CASTL = move.moveInt;
  move.setTosq(C8);
  BLACK_OOO_CASTL = move.moveInt;
  
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

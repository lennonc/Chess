//
//  qsearch.cpp
//  chess
//
//  Created by Lennon Chimbumu on 11/28/11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#include "defines.h"
#include "extglobals.h"
#include "protos.h"
#include "board.h"

int Board::qsearch(int ply, int alpha, int beta)
{
  // quiescence search
  
  int i, j, val;
  
  triangularLength[ply] = ply;
  
  if (isOwnKingAttacked()) return alphabetapvs(ply, 1, alpha, beta);
  val = board.eval();
  if (val >= beta) return val;
  if (val > alpha) alpha = val;
  
  // generate captures & promotions:
  // captgen returns a sorted move list
  moveBufLen[ply+1] = captgen(moveBufLen[ply]);
  for (i = moveBufLen[ply]; i < moveBufLen[ply+1]; i++)
  {
    makeMove(moveBuffer[i]);
    {
      if (!isOtherKingAttacked())
      {
        inodes++;
        val = -qsearch(ply+1, -beta, -alpha);
        unmakeMove(moveBuffer[i]);
        if (val >= beta) return val;
        if (val > alpha)
        {
          alpha = val;
          triangularArray[ply][ply] = moveBuffer[i];
          for (j = ply + 1; j < triangularLength[ply+1]; j++)
          {
            triangularArray[ply][j] = triangularArray[ply+1][j];
          }
          triangularLength[ply] = triangularLength[ply+1];
        }
      }
      else unmakeMove(moveBuffer[i]);
    }
  }
  return alpha;
}

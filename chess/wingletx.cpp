//
//  wingletx.cpp
//  chess
//
//  Created by Lennon Chimbumu on 11/27/11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#include <iostream>
#include "defines.h"
#include "protos.h"
#include "globals.h"

int main(int argc, char *argv[])
{
  std::cout << WINGLET_PROG_VERSION << std::endl;
#ifdef WINGLET_DEBUG_PERFT
  std::cout << "WINGLET_DEBUG_PERFT defined" << std::endl;
#endif
#ifdef WINGLET_DEBUG_MOVES
  std::cout << "WINGLET_DEBUG_MOVES defined" << std::endl;
#endif
#ifdef WINGLET_VERBOSE_EVAL
  std::cout << "WINGLET_VERBOSE_EVAL defined" << std::endl;
#endif
#ifdef WINGLET_DEBUG_EVAL
  std::cout << "WINGLET_DEBUG_EVAL defined" << std::endl;
#endif
  dataInit();
  readCommands();
  return 0;
}
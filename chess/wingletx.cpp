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
  readCommands();
  return 0;
}
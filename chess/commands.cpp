//
//  commands.cpp
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
void readCommands(){
  int nextc;
  
  {
    cout << "wt> ";
  }
  
  cout.flush();
  
  //Read a command and call doCommand
  while (nextc = getc(stdin)) != EOF {
    if (nextc == '\n') {
      CMD_BUFF[CMD_BUFF_COUNT] = '\0';
      while (CMD_BUFF_COUNT) {
        if(!doCommand(CMD_BUFF)) return;
      }
      
      {
        cout << "wt> ";
      }
      
      cout.flush();
      
    }else{
      if (CMD_BUFF_COUNT >= MAX_CMD_BUFF-1){
        cout << "Warning: command buffer full !! " << endl;
        CMD_BUFF_COUNT = 0;
      }
      CMD_BUFF[CMD_BUFF_COUNT++] = nextc;
    }
  }
}

BOOLTYPE doCommand(const char* buf){
  //return when command buffer is empty
  if (!strcmp(buf, "")){
    CMD_BUFF_COUNT = '\0';
    return true;
  }
  
  if ((!strcmp(buf, "help")) || (!strcmp(buf, "h")) || (!strcmp(buf, "?"))){
    cout << endl << "help:" << endl;
    cout << "black               : BLACK to move" << endl;
    cout << "cc                  : play computer-to-computer " << endl;
    cout << "d                   : display board " << endl;
    cout << "exit                : exit program " << endl;
    cout << "eval                : show static evaluation of this position" << endl;
    cout << "game                : show game moves " << endl;
    cout << "go                  : computer next move " << endl;
    cout << "help, h, or ?       : show this help " << endl;
    cout << "info                : display variables (for testing purposes)" << endl;
    cout << "move e2e4, or h7h8q : enter a move (use this format)" << endl;
    cout << "moves               : show all legal moves" << endl;
    cout << "new                 : start new game" << endl;
    cout << "perf                : benchmark a number of key functions" << endl;
    cout << "perft n             : calculate raw number of nodes from here, depth n " << endl;
    cout << "quit                : exit program " << endl;
    cout << "r                   : rotate board " << endl;
    cout << "readfen filename n  : reads #-th FEN position from filename" << endl;
    cout << "sd n                : set the search depth to n" << endl;
    cout << "setup               : setup board... " << endl;
    cout << "undo                : take back last move" << endl;
    cout << "white               : WHITE to move" << endl;
    cout << endl;
    CMD_BUFF_COUNT = '\0';
    return true;
  }
  
  //exit
  if ((!strcmp(buf, "exit")) || (!strcmp(buf, "quit"))){
    CMD_BUFF_COUNT = '\0';
    return false;
  }
  
  //unknown command
  cout << "    command not implemented: " << buf << ", type 'help' for more info" << std::endl;
  CMD_BUFF_COUNT = '\0';
  return true;
}
//
//  commands.cpp
//  chess
//
//  Created by Lennon Chimbumu on 11/27/11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#ifndef _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_DEPRECATE 1
#endif
#include <iostream>
#include "defines.h"
#include "protos.h"
#include "extglobals.h"
#include "board.h"
using namespace std;

void readCommands(){
  int nextc;
  
  if (board.nextMove == WHITE_MOVE){
    cout << "wt> ";
  }
  else{
    cout << "bl> ";
  }
  cout.flush();
  //     Read a command and call doCommand:
  while ((nextc = getc(stdin)) != EOF){
    if (nextc == '\n'){
      CMD_BUFF[CMD_BUFF_COUNT] = '\0';
      while (CMD_BUFF_COUNT){
        if (!doCommand(CMD_BUFF)) return;
      }     
      if (board.nextMove == WHITE_MOVE){
        cout << "wt> ";
      }else{
        cout << "bl> ";
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

BOOLTYPE doCommand(const char *buf){
  
  char userinput[80];
  int number;
  
  //  return when command buffer is empty
  if (!strcmp(buf, "")){
    CMD_BUFF_COUNT = '\0';
    return true;    
  }
  
  //  help, h, or ?: show this help
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
  
  //  black: black to move
  if (!strcmp(buf, "black")){
    board.nextMove = BLACK_MOVE;
    CMD_BUFF_COUNT = '\0';
    return true;
  }     
  
  //  d: display board
  if (!strcmp(buf, "d")){
    board.display();
    CMD_BUFF_COUNT = '\0';
    return true;
  }     
  
  //  exit or quit: exit program
  if ((!strcmp(buf, "exit")) || (!strcmp(buf, "quit"))){
    CMD_BUFF_COUNT = '\0';
    return false;
  }
  
  //  info: display variables (for testing purposes)
  if (!strcmp(buf, "info")){
    info();
    CMD_BUFF_COUNT = '\0';
    return true;
  }
  
  
  //  new: start new game
  if (!strcmp(buf, "new")){
    dataInit();
    board.init();
    board.display();
    CMD_BUFF_COUNT = '\0';
    return true;
  }
  
  //  r: rotate board
  if (!strcmp(buf, "r")){
    board.viewRotated = !board.viewRotated;
    board.display();
    CMD_BUFF_COUNT = '\0';
    return true;
  }
  
  //readfen filename n: reads #-th position from filename
  if (!strncmp(buf, "readfen", 7)){
    sscanf(buf+7,"%s %d", userinput, &number);
    board.init();
    readFen(userinput, number);
    board.display();
    CMD_BUFF_COUNT = '\0';
    return true;
  }
  //  white: white to move
  if (!strcmp(buf, "white")){
    board.nextMove = WHITE_MOVE;
    CMD_BUFF_COUNT = '\0';
    return true;
  }     
  
  //  unknown command
  cout << "    command not implemented: " << buf << ", type 'help' for more info" << endl;
  CMD_BUFF_COUNT = '\0';
  return true;
}

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
#include "timer.h"
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
  
  Move move;
  Timer timer;
  U64 msStart;
  U64 msStop;
  U64 perftcount;
  char userinput[80];
  int i, number;
  
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
  
  //game: show game moves
  if (!strcmp(buf, "game")){
    if (board.endOfGame){
      for (i = 0 ; i < board.endOfGame ; i++){
        cout << i+1 << ". ";
        displayMove(board.gameLine[i].move);
        cout <<endl;
      }
    }
    else
    {
      std::cout << "there are no game moves" << std::endl;        
    }
    CMD_BUFF_COUNT = '\0';
    return true;
  }
  
  //  info: display variables (for testing purposes)
  if (!strcmp(buf, "info")){
    info();
    CMD_BUFF_COUNT = '\0';
    return true;
  }
  
  //moves: shows all legal moves
  if (!strcmp(buf, "moves")){
    board.moveBufLen[0] = 0;
    board.moveBufLen[1] = movegen(board.moveBufLen[0]);
    std::cout << std::endl << "moves from this position:" << std::endl;
    for (i = board.moveBufLen[0]; i < board.moveBufLen[1]; i++){
      makeMove(board.moveBuffer[i]);
      if (isOtherKingAttacked())             {
        unmakeMove(board.moveBuffer[i]);
      }else{
        cout << i+1 << ". " ;
        displayMove(board.moveBuffer[i]);       
        cout << endl;
        unmakeMove(board.moveBuffer[i]);
      }
    }
    CMD_BUFF_COUNT = '\0';
    return true;
  }  
  
  //move: do a move [console mode only]
  if (!strncmp(buf, "move", 4)){
    sscanf(buf+4,"%s",userinput);
    
    // generate the pseudo-legal move list
    board.moveBufLen[0] = 0;
    board.moveBufLen[1] = movegen(board.moveBufLen[0]);
    
    if (isValidTextMove(userinput, move))        // check to see if the user move is also found in the pseudo-legal move list
    {
      makeMove(move);
      
      if (isOtherKingAttacked())              // post-move check to see if we are leaving our king in check
      {
        unmakeMove(move);
        std::cout << "    invalid move, leaving king in check: " << userinput << std::endl;
      }else{
        board.endOfGame++;
        board.endOfSearch = board.endOfGame;
        board.display();
      }
    }else{
      std::cout << "    move is invalid or not recognized: " << userinput << std::endl;
    }
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
  
  //  perft n  : calculate raw number of nodes from here, depth n
  
  if (!strncmp(buf, "perft", 5))
  {
    sscanf(buf+5,"%d", &number);
    std::cout << "    starting perft " << number << "..." << std::endl;
    timer.init();
    board.moveBufLen[0] = 0;
    
#ifdef WINGLET_DEBUG_PERFT
    ICAPT = 0;
    IEP = 0;
    IPROM = 0;
    ICASTLOO = 0;
    ICASTLOOO = 0;
    ICHECK = 0;
#endif
    
    msStart = timer.getms();
    perftcount = perft(0, number);
    msStop = timer.getms();
    
    std::cout << "nodes        = " << perftcount << ", " << msStop - msStart << " ms, ";
    if ((msStop - msStart) > 0)
      std::cout << (perftcount/(msStop - msStart)) << " knods/s";
    std::cout << std::endl;
    CMD_BUFF_COUNT = '\0';
    
#ifdef WINGLET_DEBUG_PERFT
    std::cout << "captures     = " << ICAPT << std::endl;
    std::cout << "en-passant   = " << IEP << std::endl;
    std::cout << "castlings    = " << ICASTLOO + ICASTLOOO << std::endl;
    std::cout << "promotions   = " << IPROM << std::endl;
    std::cout << "checks       = " << ICHECK << std::endl;
#endif
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
  
  //setup: setup board
  if (!strncmp(buf, "setup", 5)){
    setup();
    CMD_BUFF_COUNT = '\0';
    return true;
  }
  
  //  white: white to move
  if (!strcmp(buf, "white")){
    board.nextMove = WHITE_MOVE;
    CMD_BUFF_COUNT = '\0';
    return true;
  }     
  
  //  undo: take back last move
  if (!strcmp(buf, "undo")){
    if (board.endOfGame){
      unmakeMove(board.gameLine[--board.endOfGame].move);
      board.endOfSearch = board.endOfGame;
      board.display();
    }else{
      std::cout << "already at start of game" << std::endl;
    }
    CMD_BUFF_COUNT = '\0';
    return true;
  }
  
  //  unknown command
  cout << "    command not implemented: " << buf << ", type 'help' for more info" << endl;
  CMD_BUFF_COUNT = '\0';
  return true;
}

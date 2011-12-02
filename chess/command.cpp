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
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
using namespace std;

int kbhit(void);

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
  
  Move move, dummy;
  char sanMove[12];
  Timer timer;
  U64 msStart;
  U64 msStop;
  U64 perftcount;
  char userinput[80];
  int i, j, number;
  
  //  return when command buffer is empty
  if (!strcmp(buf, "")){
    CMD_BUFF_COUNT = '\0';
    return true;    
  }
  
  //  help, h, or ?: show this help
  if ((!strcmp(buf, "help")) || (!strcmp(buf, "h")) || (!strcmp(buf, "?"))){
    std::cout << std::endl << "help:" << std::endl;
    std::cout << "black               : BLACK to move" << std::endl;
    std::cout << "cc                  : play computer-to-computer " << std::endl;
    std::cout << "d                   : display board " << std::endl;
    std::cout << "exit                : exit program " << std::endl;
    std::cout << "eval                : show static evaluation of this position" << std::endl;
    std::cout << "game                : show game moves " << std::endl;
    std::cout << "go                  : computer next move " << std::endl;
    std::cout << "help, h, or ?       : show this help " << std::endl;
    std::cout << "info                : display variables (for testing purposes)" << std::endl;
    std::cout << "move e2e4, or h7h8q : enter a move (use this format)" << std::endl;
    std::cout << "moves               : show all legal moves" << std::endl;
    std::cout << "new                 : start new game" << std::endl;
    std::cout << "perf                : benchmark a number of key functions" << std::endl;
    std::cout << "perft n             : calculate raw number of nodes from here, depth n " << std::endl;
    std::cout << "quit                : exit program " << std::endl;
    std::cout << "r                   : rotate board " << std::endl;
    std::cout << "readfen filename n  : reads #-th FEN position from filename" << std::endl;
#ifdef WINGLET_VERBOSE_SEE
    std::cout << "qsearch             : shows sorted capture movelist" << std::endl;
#endif
    std::cout << "sd n                : set the search depth to n" << std::endl;
    std::cout << "setup               : setup board... " << std::endl;
    std::cout << "time s              : time per move in seconds" << std::endl;
    std::cout << "undo                : take back last move" << std::endl;
    std::cout << "white               : WHITE to move" << std::endl;
    std::cout << std::endl;
    CMD_BUFF_COUNT = '\0';
    return true;
  }  
  //  black: black to move
  if (!strcmp(buf, "black")){
    if (board.nextMove == WHITE_MOVE){
      board.hashkey ^= KEY.side;
      board.endOfSearch = 0;
      board.endOfGame = 0;
    }
    board.nextMove = BLACK_MOVE;
    CMD_BUFF_COUNT = '\0';
    return true;
  }    
  
  // cc: play computer-to-computer
  if (!kbhit() && !strcmp(buf, "cc")){
    while (!board.isEndOfgame(i, dummy))
    {
      move = board.think();
      if (move.moveInt)
      {
        makeMove(move);
        board.endOfGame++;
        board.endOfSearch = board.endOfGame;
        board.display();
      }
      else
      {
        CMD_BUFF_COUNT = '\0';
        return true;
      }
    }
    CMD_BUFF_COUNT = '\0';
    return true;
  }
  
  
  //  d: display board
  if (!strcmp(buf, "d")){
    board.display();
    CMD_BUFF_COUNT = '\0';
    return true;
  }     
  //eval : do a static evaluation of the board
  if (!strcmp(buf, "eval")){
    number = board.eval();
    std::cout << "eval score = " << number << std::endl;
#ifdef WINGLET_DEBUG_EVAL
    board.mirror();
    board.display();
    i = board.eval();
    std::cout << "eval score = " << i << std::endl;
    board.mirror();
    if (number != i) std::cout << "evaluation is not symmetrical! " << number << std::endl;
    else std::cout << "evaluation is symmetrical" << std::endl;
#endif
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
      // make a temporary copy of board.gameLine[];
      number = board.endOfGame;
      GameLineRecord *tmp = new GameLineRecord[number];
      memcpy(tmp, board.gameLine, number * sizeof(GameLineRecord));
      
      // unmake all moves:
      for (i = number-1 ; i >= 0 ; i--){
        unmakeMove(tmp[i].move);
        board.endOfSearch = --board.endOfGame;
      }
      
      // redo all moves:
      j = board.nextMove;
      for (i = 0 ; i < number; i++)
      {
        // move numbering:
        if (!((i+j+2)%2)) std::cout << (i+2*j+2)/2 << ". ";
        else if (!i) std::cout << "1. ... ";
        
        // construct the move string
        toSan(tmp[i].move, sanMove);
        std::cout << sanMove;
        
        // output CRLF, or space:
        if (!((i+j+1)%2)) std::cout << std::endl;
        else std::cout << " ";
        
        // make the move:
        makeMove(tmp[i].move);
        board.endOfSearch = ++board.endOfGame;
      }
      std::cout << std::endl;
      
      // delete the temporary copy:
      delete[] tmp;
    }else{
      std::cout << "there are no game moves" << std::endl;       
    }
    CMD_BUFF_COUNT = '\0';
    return true;
  }  
  
  //go: computer next move
  if (!strcmp(buf, "go")){
    if (!board.isEndOfgame(i, dummy)){
      move = board.think();
      if (move.moveInt){
        makeMove(move);
        board.endOfGame++;
        board.endOfSearch = board.endOfGame;
      }
      board.display();
      board.isEndOfgame(i, dummy);
      CMD_BUFF_COUNT = '\0';
      return true;
    }else{
      board.display();
      CMD_BUFF_COUNT = '\0';
      return true;
    }
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
    number = 0;
    for (i = board.moveBufLen[0]; i < board.moveBufLen[1]; i++){
      makeMove(board.moveBuffer[i]);
      if (isOtherKingAttacked()){
        unmakeMove(board.moveBuffer[i]);
      }else{
        unmakeMove(board.moveBuffer[i]);
        toSan(board.moveBuffer[i], sanMove);
        std::cout << ++number << ". " << sanMove << std::endl;
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
  
  //  sd n: set the search depth to n
  if (!strncmp(buf, "sd", 2)){
    sscanf(buf+2,"%d", &board.searchDepth);
    std::cout << "winglet> search depth " << board.searchDepth << std::endl;
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
    if (board.nextMove == BLACK_MOVE){
      board.hashkey ^= KEY.side;
      board.endOfSearch = 0;
      board.endOfGame = 0;
    }
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
  
#ifdef WINGLET_VERBOSE_SEE
  // qsearch : shows sorted capture move list
  if (!strncmp(buf, "qsearch", 3))
  {
    board.moveBufLen[0] = 0;
    board.moveBufLen[1] = captgen(board.moveBufLen[0]);
    std::cout << std::endl << "sorted capturing moves from this position:" << std::endl;
    std::cout << std::endl << "        score:" << std::endl;
    number = 0;
    for (i = board.moveBufLen[0]; i < board.moveBufLen[1]; i++)
    {
      makeMove(board.moveBuffer[i]);
      if (isOtherKingAttacked())
      {
        unmakeMove(board.moveBuffer[i]);
      }
      else
      {
        unmakeMove(board.moveBuffer[i]);
        std::cout << ++number << ". ";
        displayMove(board.moveBuffer[i]);
        std::cout << "   " << board.moveBuffer[i + OFFSET].moveInt << std::endl;
      }
    }
    CMD_BUFF_COUNT = '\0';
    return true;
  }
#endif
  
  //  unknown command
  cout << "    command not implemented: " << buf << ", type 'help' for more info" << endl;
  CMD_BUFF_COUNT = '\0';
  return true;
}

int kbhit(void){
  struct termios oldt, newt;
  int ch;
  int oldf;
  
  tcgetattr(STDIN_FILENO, &oldt);
  newt = oldt;
  newt.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);
  oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
  fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
  
  ch = getchar();
  
  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
  fcntl(STDIN_FILENO, F_SETFL, oldf);
  
  if(ch != EOF){
    ungetc(ch, stdin);
    return 1;
  }
  
  return 0;
}

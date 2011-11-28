//
//  hash.cpp
//  chess
//
//  Created by Lennon Chimbumu on 11/28/11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#include <ctime>
#include <iostream>
#include <stdlib.h>
#include "hash.h"
#include "timer.h"
#include "protos.h"

void HashKeys::init(){
  //initialize
  
  int i, j;
  time_t now;
  
  //use current time as random seed
  srand((unsigned int)time(&now));

  for (i = 0 ; i < 64; i++ ) {
    ep[i] = rand64();
    for (j = 0; j < 16; j++) {
      keys[i][j] = rand64();
    }
  }
  side = rand64();
  wk = rand64();
  wq = rand64();
  bk = rand64();
  bq = rand64();
  
  return;
}

U64 HashKeys::rand64(){
  return rand()^((U64)rand()<<15)^((U64)rand()<<30)^((U64)rand()<<45)^((U64)rand()<<60);
}

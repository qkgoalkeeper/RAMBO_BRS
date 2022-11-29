//
// Created by dase on 11/23/22.
//

#ifndef RAMBO_BLOCKRANGESEARCH_MYRAMBO_H
#define RAMBO_BLOCKRANGESEARCH_MYRAMBO_H

#include <iomanip>
#include <iostream>
#include <fstream>
#include <math.h>
#include <sstream>
#include <string>
#include <string.h>
#include <algorithm>
#include <chrono>
#include "MyBloom.h"
#include "MurmurHash3.h"
#include "Rambo_construction.h"
#include "utils.h"
#include "constants.h"
#include "bitArray.h"
#include <ctime>
#include <unordered_map>
#include <unordered_set>
#include <thread>
#include <future>

class MyRamboApi{
public:
    MyRamboApi(int n_perSet_,int R_all_,int B_all,int all_start_,int delta_range);


    std::vector<RAMBO> rambo_vector;
    int n_perSet;
    int R_all;
    int B_all;
    int all_start;
    int start;
    int end;
    int delta_range;
    int current_key_num;

};






#endif //RAMBO_BLOCKRANGESEARCH_MYRAMBO_H

//
// Created by dase on 11/23/22.
//

#include "MyRambo.h"

//todo 后续将main函数里的东西整合成类使用
MyRamboApi::MyRamboApi(int n_perSet_, int R_all_, int B_all_, int all_start_, int delta_range_) {
    n_perSet = n_perSet_; //num of items(judge how to build bloomfilter)
    R_all = R_all_;
    B_all = B_all_;
    all_start = all_start_;
    delta_range = delta_range_;
    current_key_num = 0;
    start = all_start_;
    end = delta_range_;
    RAMBO temprambo = RAMBO(n_perSet, R_all, B_all, end-start+1);
    rambo_vector.emplace_back(temprambo);
}

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
using namespace std;



int main(int argc, char **argv) {

    bool insert = false;
    bool ser = false;
    bool test = false;
    bool deser = true;

    int n_perSet = 100000; //num of items(judge how to build bloomfilter)
    int R_all = 3;
    int B_all = 1000;

    //int K = Ki; // total number of sets
    int K = 10000000;

    double insert_raw = 0;
    double insert_rambo = 0;
    double search_time = 0;
    double search_time2 = 0;

    //test blockchain data！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！

    //fstream f("test_modified.txt");
    fstream f("tag_and_edge_modified.txt");
    //fstream f("xaa");
    //unordered_set<string> key_set;
    std::unordered_map<string, unordered_set<int>> key_set;
    //int start = 4000000; //开始块范围
    int start = 4638567;
    //int end = 4981465;
    //int end = 13000000; 
    int end = 12210044;
    //int end = 8165992; //结束块范围
    string line;
    
    int bias = start - 1;
    K = end - bias;

    //创建Rambo
    cout<<"start build myrambo"<<endl;
    RAMBO myRambo(n_perSet, R_all, B_all, K);

    //
    int now_insert_num = 0;
    cout<<"start insert lines"<<endl;

    //文件获取每行插入
    while(getline(f,line)){
        std::vector<string> KeySets = line2array(line, ';');


        now_insert_num++;
        if(now_insert_num%100000==0){
            cout<<"having insert "<<now_insert_num<<" keys!"<<endl;
        }

        std::clock_t insert1 = std::clock();
        key_set[KeySets[0]].insert(atoi(KeySets[1].c_str()));
        std::clock_t insert2 = std::clock();

        myRambo.createMetaRambo_single(atoi(KeySets[1].c_str()));

        myRambo.insertion_pair(make_pair(KeySets[0], KeySets[1]));
        std::clock_t insert3 = std::clock();

        insert_raw += double(insert2-insert1)/CLOCKS_PER_SEC;
        insert_rambo += double(insert3-insert2)/CLOCKS_PER_SEC;
    }


    cout<<"start query"<<endl;
    //ofstream out("1110answer.txt");
    
    
    
    ofstream out(argv[1]);
    out<<"bias = "<<bias<<endl;
    out<<key_set.size()<<endl;
    
    double fp = 0;
    int now_key_num = 0;

    int test_num = 0;
    std::clock_t search1 = std::clock();

    if(argv[2][0]=='0'){ //0表示查询返回的是bias bitset
        out<<"using bitset"<<endl;
        for (auto &x: key_set) {

            
            test_num++;
            if(test_num == 200000) break;

            int true_num = x.second.size();
            int guess_num = 0;
            //boost::dynamic_bitset<> MemVec = myRambo.query_bias(x, x.size(), bias);

            //using bitset
            boost::dynamic_bitset<> MemVec = myRambo.query_bias(x.first, x.first.size(), bias);

            //guess_num = MemVec.count();
            //set<int> MemVec = myRambo.query_bias_set(x.first,x.first.size());
            //guess_num = MemVec.size();

            out<<x.first<<endl;
            guess_num = MemVec.count();


            now_key_num++;
            if(now_key_num%10000==0){
                cout<<"having searched "<<now_key_num<<" keys!"<<endl;
            }

            // out<<"guess answer num:"<<endl;
            // out<<guess_num<<endl;
            // for(auto &y:MemVec){
            //     out << y<< " ";
            // }
            //out<<endl;
            // for (size_t i = MemVec.find_first(); i != boost::dynamic_bitset<>::npos;) {
            //     out << i+bias << " ";
            //     //guess_num++;
            //     i = MemVec.find_next(i);
            // }
            //out<<endl;
            
            
            // out<<"true answer num:"<<endl;
            // out<<true_num<<endl;
            // for(auto &y:x.second){
            //     out << y << " ";
            // }
            // out<<endl;
            
            
            if(guess_num<true_num){
                out<<"wrong: "<<x.first<<endl;
                // for (size_t i = MemVec.find_first(); i != boost::dynamic_bitset<>::npos;) {
                //   out << i+bias << " ";
                //   i = MemVec.find_next(i);
                // }
                // out<<endl;
                // for(auto &y:x.second){
                //   out << y << " ";
                // }
                out<<endl;
            }
            

            out<<((guess_num-true_num)*0.1)/(guess_num*0.1)<<endl;

            fp+=((guess_num-true_num)*0.1)/(guess_num*0.1);


        }
    }else if(argv[2][0]=='1'){ //1表示查询返回的是std::set
        out<<"using set"<<endl;
        int a = 0;
        for (auto &x: key_set) {
            a++;
            if(a==200000){
                break;
            }
            int true_num = x.second.size();
            int guess_num = 0;
            //boost::dynamic_bitset<> MemVec = myRambo.query_bias(x, x.size(), bias);

            //using bitset
            //boost::dynamic_bitset<> MemVec = myRambo.query_bias(x.first, x.first.size(), bias);

            //guess_num = MemVec.count();
            set<int> MemVec = myRambo.query_bias_set(x.first,x.first.size());
            guess_num = MemVec.size();

            out<<x.first<<endl;



            now_key_num++;
            if(now_key_num%10000==0){
                cout<<"having searched "<<now_key_num<<" keys!"<<endl;
            }

            // out<<"guess answer num:"<<endl;
            // out<<guess_num<<endl;
            // for(auto &y:MemVec){
            //     out << y<< " ";
            // }
            //out<<endl;
            // for (size_t i = MemVec.find_first(); i != boost::dynamic_bitset<>::npos;) {
            //     out << i+bias << " ";
            //     //guess_num++;
            //     i = MemVec.find_next(i);
            // }
            //out<<endl;
            
            
            // out<<"true answer num:"<<endl;
            // out<<true_num<<endl;
            // for(auto &y:x.second){
            //     out << y << " ";
            // }
            // out<<endl;
            
            
            if(guess_num<true_num){
                out<<"wrong: "<<x.first<<endl;
                // for (size_t i = MemVec.find_first(); i != boost::dynamic_bitset<>::npos;) {
                //   out << i+bias << " ";
                //   i = MemVec.find_next(i);
                // }
                // out<<endl;
                // for(auto &y:x.second){
                //   out << y << " ";
                // }
                out<<endl;
            }
            

            out<<((guess_num-true_num)*0.1)/(guess_num*0.1)<<endl;

            fp+=((guess_num-true_num)*0.1)/(guess_num*0.1);


        }


    }else{
        cout<<"argv[2] should be 0(use bitset) or 1(use std::set)"<<endl;
    }
    

    std::clock_t search2 = std::clock();

    std::clock_t search3 = std::clock();
    int b = 0;
    for (auto &x: key_set) {
      b++;
      if(b==100000){
          break;
      }
      auto answer = x.second;
    }

    std::clock_t search4 = std::clock();





    search_time += double(search2-search1)/CLOCKS_PER_SEC;
    search_time2 += double(search4-search3)/CLOCKS_PER_SEC;
    out<<"key set size:"<<key_set.size()<<endl;
    out<<"average fp:"<<(fp*1.0)/(key_set.size())<<endl;
    out<<"raw map insert time:"<<insert_raw<<endl;
    out<<"rambo insert time:"<<insert_rambo<<endl;
    out<<"raw map search time:"<<search_time2<<endl;
    out<<"rambo search time:"<<search_time<<endl;

    

    return 0;
}

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

using namespace std;


int main(int argc, char **argv) {

    int n_perSet = 100000; //num of items(judge how to build bloomfilter)
    int R_all = 3;  //（参数修改R！！！！！！！！！！！）
    int B_all = 10; // （参数修改B！！！！！！！！！！！！！！！！）

    //int K = Ki; // total number of sets
    int K = 0;

    double insert_raw = 0;
    double insert_rambo = 0;
    double search_time = 0;
    double search_time2 = 0;

    //test blockchain data

    fstream f("test_modified.txt"); //（读取的文件！！！！！！！！！！！！！！！！！！！！！！！！）
    //fstream f("rambo_data_10000000.txt");
    //fstream f("xaa");
    //unordered_set<string> key_set;
    std::unordered_map<string, unordered_set<int>> key_set;
    int delta_range = 100; //每个Rambo负责file数    （参数修改位置file数！！！！！！！！！！！！！！）
    int all_start = 0;//表示file开始号
    int start = 0; //当前Rambo开始file

    int end = 100;  // 当前Rambo结束file（deltarange修改后对应修改end！！！！！！！！！！！！！！）

    string line;


    std::map<string,vector<int>> file_cache;
    int file_num = 0;
    const int file_target = 10000;//keys per file （参数修改：每个文件的key数量！！！！！！！！！！！！）
    int current_key_num = 0;

    if((end-start)!=delta_range){
        cout<<"delta range should equal to (end-start)"<<endl;
    }
    //int bias = start - 1;
    //K = end - bias;

    //创建Rambo
    //cout<<"start build myrambo"<<endl;
    //RAMBO myRambo(n_perSet, R_all, B_all, K);

    ofstream f_write;




    vector<RAMBO> rambo_vector;
    RAMBO temprambo = RAMBO(n_perSet, R_all, B_all, end-start+1);
    rambo_vector.emplace_back(temprambo); //一开始就一个Rambo
    
    int now_insert_num = 0;
    cout<<"start insert lines"<<endl;

    //文件获取每行插入
    while(getline(f,line)){
        std::vector<string> KeySets = line2array(line, ';');
        int block_number = atoi(KeySets[1].c_str());

        now_insert_num++;
        if(now_insert_num%100000==0){
            cout<<"having insert "<<now_insert_num<<" keys!"<<endl;
        }
        while(file_num>=end){ //如果需要一个新的Rambo
            start+=delta_range;
            end+=delta_range;
            RAMBO temprambo = RAMBO(n_perSet, R_all, B_all, end-start+1);
            rambo_vector.emplace_back(temprambo);
        }
        //f_write<<KeySets[0]<<" "<<KeySets[1]<<endl;

        file_cache[KeySets[0]].emplace_back(block_number);

        std::clock_t insert1 = std::clock();
        key_set[KeySets[0]].insert(file_num);

        std::clock_t insert2 = std::clock();

        rambo_vector[file_num/delta_range-all_start/delta_range].createMetaRambo_single(file_num);
        rambo_vector[file_num/delta_range-all_start/delta_range].insertion_pair(make_pair(KeySets[0], to_string(file_num)));

        // myRambo.createMetaRambo_single(atoi(KeySets[1].c_str()));

        //myRambo.insertion_pair(make_pair(KeySets[0], KeySets[1]));
        std::clock_t insert3 = std::clock();

        insert_raw += double(insert2-insert1)/CLOCKS_PER_SEC;
        insert_rambo += double(insert3-insert2)/CLOCKS_PER_SEC;


        current_key_num++;
        if(current_key_num==file_target){

            f_write.open("file/"+to_string(file_num)+".txt");
            for(auto &x:file_cache){
                f_write<<x.first<<";";
                for(auto &y:x.second){
                    f_write<<y<<",";
                }
                f_write<<endl;
            }

            f_write.close();
            file_cache.clear();
            file_num++;
            current_key_num = 0;
        }
    }

    //final file
    f_write.open("file/"+to_string(file_num)+".txt");
    for(auto &x:file_cache){
        f_write<<x.first<<";";
        for(auto &y:x.second){
            f_write<<y<<",";
        }
        f_write<<endl;
    }

    f_write.close();
    file_cache.clear();
    file_num++;
    current_key_num = 0;



    cout<<"start query"<<endl;
    //ofstream out("1110answer.txt");
    
    
    
    ofstream out(argv[1]);
    //out<<"bias = "<<bias<<endl;
    out<<key_set.size()<<endl;
    
    double fp = 0;
    int now_key_num = 0;

    int test_num = 0;
    std::clock_t search1 = std::clock();

    if(argv[2][0]=='0'){ //0表示查询返回的是bias bitset
        out<<"using bitset"<<endl;
        for (auto &x: key_set) {

            
            //test_num++;
            //if(test_num == 200000) break;

            int true_num = x.second.size();
            int guess_num = 0;
            //boost::dynamic_bitset<> MemVec = myRambo.query_bias(x, x.size(), bias);
            out<<x.first<<endl;
            //using bitset
            /*
            out<<"guess answer :"<<endl;
             */
            for(int i=0;i<rambo_vector.size();i++){
                
                int bias = all_start+i*delta_range-1;

                
                boost::dynamic_bitset<> MemVec = rambo_vector[i].query_bias(x.first, x.first.size(), bias);
                guess_num += MemVec.count();

//
//                for (size_t i = MemVec.find_first(); i != boost::dynamic_bitset<>::npos;) {
//                    out << i+bias << " ";
//                    //guess_num++;
//                    i = MemVec.find_next(i);
//                }
//                out<<endl;

            }

            now_key_num++;
            if(now_key_num%10000==0){
                cout<<"having searched "<<now_key_num<<" keys!"<<endl;
            }
/*
            out<<"true answer:"<<endl;
            //out<<true_num<<endl;
            for(auto &y:x.second){
                out << y << " ";
            }
            out<<endl;
*/
            if(guess_num<true_num){
                out<<"wrong: "<<x.first<<endl;
                out<<endl;
            }



            out<<((guess_num-true_num)*0.1)/(guess_num*0.1)<<endl;

            fp+=((guess_num-true_num)*0.1)/(guess_num*0.1);


        }
    }else if(argv[2][0]=='1'){ //1表示查询返回的是std::set
        /*
        out<<"using set"<<endl;
        //int a = 0;
        for (auto &x: key_set) {
            // a++;
            // if(a==200000){
            //     break;
            // }
            int true_num = x.second.size();
            int guess_num = 0;
            out<<x.first<<endl;

            //using set
            for(int i=0;i<rambo_vector.size();i++){

                set<int> MemVec = rambo_vector[i].query_bias_set(x.first, x.first.size());
                guess_num += MemVec.size();

            }

            now_key_num++;
            if(now_key_num%10000==0){
                cout<<"having searched "<<now_key_num<<" keys!"<<endl;
            }


            
            
            if(guess_num<true_num){
                out<<"wrong: "<<x.first<<endl;
                out<<endl;
            }
            

            out<<((guess_num-true_num)*0.1)/(guess_num*0.1)<<endl;

            fp+=((guess_num-true_num)*0.1)/(guess_num*0.1);


        }
         */


    }else{
        cout<<"argv[2] should be 0(use bitset) or 1(use std::set)"<<endl;
    }
    

    std::clock_t search2 = std::clock();

    std::clock_t search3 = std::clock();
    int b = 0;
    for (auto &x: key_set) {
    //   b++;
    //   if(b==200000){
    //       break;
    //   }
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

    f.close();
    f_write.close();

    return 0;
}

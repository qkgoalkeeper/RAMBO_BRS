#ifndef _RamboConstruction_
#define _RamboConstruction_

#include <vector>
#include <set>
#include <string>
#include <bitset>
#include "constants.h"
#include "bitArray.h"
#include <unordered_map>
#include <unordered_set>
#include <boost/dynamic_bitset.hpp>

// vector<uint> hashfunc( void* key, int len, int R, int B){
// }

class RAMBO {
public:

    RAMBO(int n, int r1, int b1, int K);

    std::vector<uint> hashfunc(std::string key, int len);

    void insertion(std::string setID, std::vector<std::string> keys);

    std::set<int> takeunion(std::set<int> set1, std::set<int> set2);

    std::set<int> takeIntrsec(std::set<int> *setArray);

    std::vector<std::string> getdata(std::string filenameSet);

    //bitArray query(std::string query_key, int len);

    //void createMetaRambo(int K, bool verbose);

    void serializeRAMBO(std::string dir);

    void deserializeRAMBO(std::vector<std::string> dir);

    //void insertion2(std::vector<std::string> alllines);

    //bitArray queryseq(std::string query_key, int len);

    //void insertionRare(std::string setID, std::vector<std::string> keys);



    //method for blockchain data
    //void createMetaRambo_range(std::unordered_map<std::string, std::vector<int>> &testKeys, bool verbose);

    void insertion_pairs(std::vector<std::pair<std::string, std::string>> &data_key_number);

    void createMetaRambo_single(int value);

    void insertion_pair(std::pair<std::string, std::string> pair1);

    boost::dynamic_bitset<> query_bias(std::string query_key, int len, int bias);
    std::set<int> query_bias_set(std::string query_key, int len);


    int R;
    int B;
    int n;
    float p;
    int range;
    int k;
    float FPR;
    int K1;
    BloomFiler **Rambo_array;
    //std::vector<int> *metaRambo;
    std::unordered_set<int> *metaRambo;
    

    
};

#endif

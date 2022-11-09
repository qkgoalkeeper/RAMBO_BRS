#include <iomanip>
#include <fstream>
#include <iostream>
#include <chrono>
#include <vector>
#include <math.h>
#include <sstream>
#include <string>
#include <string.h>
#include <algorithm>
#include "MyBloom.h"
#include "MurmurHash3.h"
#include "Rambo_construction.h"
#include "utils.h"
#include "constants.h"
#include "bitArray.h"
#include <set>
#include <iterator>
#include <bitset>

using namespace std;

vector<uint> RAMBO::hashfunc(std::string key, int len) {
    // int hashvals[k];
    vector<uint> hashvals;
    uint op;
    for (int i = 0; i < R; i++) {
        MurmurHash3_x86_32(key.c_str(), len, i, &op); //seed i
        hashvals.push_back(op % B);
    }
    return hashvals;
}

std::vector<std::string> RAMBO::getdata(string filenameSet) {
    //get the size of Bloom filter by count
    ifstream cntfile(filenameSet);
    std::vector<std::string> allKeys;
    int totKmerscnt = 0;
    while (cntfile.good()) {
        string line1, vals;
        while (getline(cntfile, line1)) {
            stringstream is;
            is << line1;
            if (line1[0] != '>' && line1.size() > 30) {
                for (uint idx = 0; idx < line1.size() - 31 + 1; idx++) {
                    allKeys.push_back(line1.substr(idx, 31));
                    totKmerscnt++;
                }
            }
        }
    }
    std::cout << "total inserted from one file: " << totKmerscnt << std::endl;
    return allKeys;
}


RAMBO::RAMBO(int n, int r1, int b1, int K) {
//  R = r1;
//  B = b1;
//  K = K;
//
//  //range = ceil(-(n*log(p))/(log(2)*log(2))); //range
//  range = n;
//  k = 3;
//  std::cout << "range" <<range<< '\n';
//
//  //k = ceil(-log(p)/log(2)); //number of hash, k is 7 for 0.01
//  Rambo_array = new BloomFiler*[B*R]; //array of pointers
//  metaRambo = new vector<int>[B*R]; //constains set info in it.
//  for(int b=0; b<B; b++){
//    for(int r=0; r<R; r++){
//      Rambo_array[b + B*r] = new BloomFiler(range, p, k);
//    }
//  }
    R = r1;
    B = b1;
    K1 = K;
    p = 0.01; //false positive rate
    range = ceil(-(n * log(p)) / (log(2) * log(2))); //range
    // range = n;
    // k = 3;
    // std::cout << "range" <<range<< '\n';

    k = ceil(-log(p) / log(2)); //number of hash, k is 7 for 0.01
    Rambo_array = new BloomFiler *[B * R]; //array of pointers
    metaRambo = new vector<int>[B * R]; //constains set info in it.
    for (int b = 0; b < B; b++) {
        for (int r = 0; r < R; r++) {
            Rambo_array[b + B * r] = new BloomFiler(range, p, k);
        }
    }
}

// one time process- a preprocess step
void RAMBO::createMetaRambo(int K, bool verbose) {
    for (int i = 0; i < K; i++) {
        vector<uint> hashvals = RAMBO::hashfunc(std::to_string(i),
                                                std::to_string(i).size()); // R hashvals, each with max value B
        // if(i==62){
        //   cout<<"62 hashval:";
        //   for(auto x:hashvals){
        //     cout<<x;
        //   }
        //   cout<<endl;
        // }
        for (int r = 0; r < R; r++) {
            metaRambo[hashvals[r] + B * r].push_back(i);
        }
    }

    //print RAMBO meta deta
    if (verbose) {
        for (int b = 0; b < B; b++) {
            for (int r = 0; r < R; r++) {
                for (auto it = metaRambo[b + B * r].begin(); it != metaRambo[b + B * r].end(); ++it) {
                    std::cout << " " << *it;
                }
                { std::cout << "////"; }
            }
            std::cout << '\n';
        }
    }
}

// give set and keys in the set
void RAMBO::insertion(std::string setID, std::vector<std::string> keys) {
    vector<uint> hashvals = RAMBO::hashfunc(setID, setID.size()); // R hashvals

    //make this loop parallel
#pragma omp parallel for
    for (std::size_t i = 0; i < keys.size(); ++i) {
        for (int r = 0; r < R; r++) {
            vector<uint> temp = myhash(keys[i].c_str(), keys[i].size(), k, r, range);
            Rambo_array[hashvals[r] + B * r]->insert(temp);
        }
    }
}

// given inverted index type arrangement, kmer;files;files;..
void RAMBO::insertion2(std::vector<string> alllines) {
    //make this loop parallel
    //#pragma omp parallel for
    for (std::size_t i = 0; i < alllines.size(); ++i) {
        char d = ';';
        std::vector<string> KeySets = line2array(alllines[i], d);//sets for a key

        std::vector<string> KeySet = line2array(KeySets[1], ',');

        cout << "+++++++++++++++insert key: " << KeySets[0] << endl;

        for (uint j = 0; j < KeySet.size(); j++) {
            cout << KeySet[j] << "+--+-+--+-+" << endl;

            if (KeySet[j].size() == 3) {
                cout << KeySet[j][2] << endl;
                cout << "equal" << endl;
            }

            cout << to_string(atoi(KeySet[j].c_str())).size() << endl;
            string temp = to_string(atoi(KeySet[j].c_str()));


            cout << "                      value: " << KeySet[j] << endl;
            // vector<uint> hashvals = RAMBO::hashfunc(KeySet[j], KeySet[j].size()); // R hashvals

            vector<uint> hashvals = RAMBO::hashfunc(temp, temp.size()); // R hashvals
            cout << "                           value hashval:";
            for (auto x: hashvals) {
                cout << x;
            }
            cout << endl;
            for (int r = 0; r < R; r++) {
                vector<uint> temp = myhash(KeySets[0], KeySets[0].size(), k, r, range);// i is the key
                cout << "                         key hashval:";
                for (auto x: temp) {
                    cout << x;
                }
                cout << endl;
                Rambo_array[hashvals[r] + B * r]->insert(temp);
            }
        }
    }
}

// // give set and keys in the set
// void RAMBO::insertionRare (std::string setID, std::vector<std::string> keys){
//   vector<uint> hashvals = RAMBO::hashfunc(setID, setID.size()); // R hashvals
//
//   //make this loop parallel
//   int skip =0;
//   #pragma omp parallel for
//   for(std::size_t i=0; i<keys.size(); ++i){
//     bitArray MemVec = RAMBO::query(keys[i].c_str(), keys[i].size());
//     if ( MemVec.getcount() <10 ){
//       for(int r=0; r<R; r++){
//       vector<uint> temp = myhash(keys[i].c_str(), keys[i].size() , k, r, range);
//         Rambo_array[hashvals[r] + B*r]->insert(temp);
//     }
//   }
//     else{ skip++;}
//   }
//   cout<<"skipped "<<to_string(skip)<<endl;
// }


bitArray RAMBO::query(string query_key, int len) {
    // set<int> resUnion[R]; //constains union results in it.
    bitArray bitarray_K(Ki);
    // bitset<Ki> bitarray_K;
    // set<int> res;
    float count = 0.0;
    vector<uint> check;
    // cout<<"---------------insert key: "<<query_key<<endl;
    for (int r = 0; r < R; r++) {
        check = myhash(query_key, len, k, r, range); //hash values correspondign to the keys

        // cout<<"                         key hashval:";
        // for(auto x:check){
        //   cout<<x;
        // }


        bitArray bitarray_K1(Ki);
        // bitset<Ki> bitarray_K1;
        for (int b = 0; b < B; b++) {
            if (Rambo_array[b + B * r]->test(check)) {
                // cout<<"hash check! r,b= "<<r<<" "<<b<<endl;
                chrono::time_point<chrono::high_resolution_clock> t5 = chrono::high_resolution_clock::now();
                for (uint j = 0; j < metaRambo[b + B * r].size(); j++) {
                    bitarray_K1.SetBit(metaRambo[b + B * r][j]);
                }
                chrono::time_point<chrono::high_resolution_clock> t6 = chrono::high_resolution_clock::now();
                count += ((t6 - t5).count() / 1000000000.0);
            }
        }
        if (r == 0) {
            bitarray_K = bitarray_K1;
        } else {
            bitarray_K.ANDop(bitarray_K1.A);
        }
    }
    vector<uint>().swap(check);
    return bitarray_K;
}

void RAMBO::serializeRAMBO(string dir) {
    for (int b = 0; b < B; b++) {
        for (int r = 0; r < R; r++) {
            string br = dir + to_string(b) + "_" + to_string(r) + ".txt";
            Rambo_array[b + B * r]->serializeBF(br);
        }
    }
}

void RAMBO::deserializeRAMBO(vector<string> dir) {
    for (int b = 0; b < B; b++) {
        for (int r = 0; r < R; r++) {
            vector<string> br;
            for (uint j = 0; j < dir.size(); j++) {
                br.push_back(dir[j] + to_string(b) + "_" + to_string(r) + ".txt");
            }
            Rambo_array[b + B * r]->deserializeBF(br);

        }
    }
}

void RAMBO::createMetaRambo_range(std::unordered_map<std::string, std::vector<int>> &testKeys, bool verbose) {
    // for(int i=start;i<end;i++){
    //   vector<uint> hashvals = RAMBO::hashfunc(std::to_string(i), std::to_string(i).size()); // R hashvals, each with max value B
    //   for(int r=0; r<R; r++){
    //     metaRambo[hashvals[r] + B*r].push_back(i);
    //   }
    // }

    for (auto &values: testKeys) {
        for (auto &value: values.second) {
            vector<uint> hashvals = RAMBO::hashfunc(std::to_string(value),
                                                    std::to_string(value).size()); // R hashvals, each with max value B
            for (int r = 0; r < R; r++) {
                metaRambo[hashvals[r] + B * r].push_back(value);
            }
        }
    }

    //print RAMBO meta deta
    if (verbose) {
        for (int b = 0; b < B; b++) {
            for (int r = 0; r < R; r++) {
                for (auto it = metaRambo[b + B * r].begin(); it != metaRambo[b + B * r].end(); ++it) {
                    std::cout << " " << *it;
                }
                { std::cout << "////"; }
            }
            std::cout << '\n';
        }
    }
}


void RAMBO::insertion_pairs(std::vector<std::pair<std::string, std::string>> &data_key_number) {
    for (auto pair: data_key_number) {
        string temp = to_string(atoi(pair.second.c_str()));
        vector<uint> hashvals = RAMBO::hashfunc(temp, temp.size());
        for (int r = 0; r < R; r++) {
            vector<uint> temp = myhash(pair.first, pair.first.size(), k, r, range);// i is the key
            Rambo_array[hashvals[r] + B * r]->insert(temp);
        }
    }
}

bitArray RAMBO::query_bias(std::string query_key, int len, int bias) {

    // set<int> resUnion[R]; //constains union results in it.
    bitArray bitarray_K(K1);
    // bitset<Ki> bitarray_K;
    // set<int> res;
    // float count=0.0;
    vector<uint> check;
    for (int r = 0; r < R; r++) {
        check = myhash(query_key, len, k, r, range); //hash values correspondign to the keys
        // bitArray bitarray_K1(Ki);
        bitArray bitarray_K1(K1);
        // bitset<Ki> bitarray_K1;
        for (int b = 0; b < B; b++) {
            if (Rambo_array[b + B * r]->test(check)) {
                // chrono::time_point<chrono::high_resolution_clock> t5 = chrono::high_resolution_clock::now();
                for (uint j = 0; j < metaRambo[b + B * r].size(); j++) {
                    bitarray_K1.SetBit(metaRambo[b + B * r][j] - bias);
                }
                // chrono::time_point<chrono::high_resolution_clock> t6 = chrono::high_resolution_clock::now();
                // count+=((t6-t5).count()/1000000000.0);
            }
        }
        if (r == 0) {
            bitarray_K = bitarray_K1;
        } else {
            bitarray_K.ANDop(bitarray_K1.A);
        }
    }
    vector<uint>().swap(check);
    return bitarray_K;
}

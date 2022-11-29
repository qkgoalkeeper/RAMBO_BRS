# RAMBO_BRS BLOCK RANGE SEARCH IN BLOCKCHAIN
RAMBO_BRS uses RAMBO to help us find the blocks that a certain account belongs to. 



# RAMBO Fast Processing and Querying of 170TB of Genomics Data via a Repeated And Merged BloOm Filter (RAMBO)
RAMBO is a method to reduce the query cost of sequence search over the archive of dataset files to address the sheer scale and explosive increase of new sequence files.
It solves achives sublinear query time (O(\sqrt{K} log K)) in number of files with memory requirement of slightly more then the information theoretical limit. 

This code is the implementation of: 
[https://dl.acm.org/doi/10.1145/3448016.3457333](https://dl.acm.org/doi/10.1145/3448016.3457333)
for gene sequence search.

If you use RAMBO in an academic context or for any publication, please cite our paper:
```
@inproceedings{10.1145/3448016.3457333,
author = {Gupta, Gaurav and Yan, Minghao and Coleman, Benjamin and Kille, Bryce and Elworth, R. A. Leo and Medini, Tharun and Treangen, Todd and Shrivastava, Anshumali},
title = {Fast Processing and Querying of 170TB of Genomics Data via a Repeated And Merged BloOm Filter (RAMBO)},
year = {2021},
isbn = {9781450383431},
publisher = {Association for Computing Machinery},
address = {New York, NY, USA},
url = {https://doi.org/10.1145/3448016.3457333},
doi = {10.1145/3448016.3457333},
pages = {2226–2234},
numpages = {9},
keywords = {information retrieval, bloom filter, genomic sequence search},
location = {Virtual Event, China},
series = {SIGMOD/PODS '21}
}
```



before run：

1.  install boost

（Ubuntu）

```
sudo apt-get install libboost-all-dev
```



2. Set parameters and run code：
   start,end in line 40-52 of src/main.cpp (start means the start blocknumber and end means the end blocknumber)
   m, B and R in line 30-32 of src/main.cpp (three parameter for bloomfilter)

   data filename in line 45 of src/main.cpp ，The format of test file is like test_mofified.txt

​		



run:

```	
make
./build/program answer.txt 0
```

Argv[1] is the file name to record the performance of rambo.

Argv[2] is 0 or 1,deciding  how to save the search result. (0 for bitset,1 for std::set)


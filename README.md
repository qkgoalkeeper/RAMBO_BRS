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

Set parameters and run code
number of sets in line 7 of include/constants.h
m, B and R in line 29-31 of src/main.cpp
run:
```	
make
./build/program 
```



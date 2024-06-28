#ifndef BLOCK_HPP
#define BLOCK_HPP

#include<iostream>
#include<string>
using namespace std;


#include "MerkleTree.hpp"

class Block{
    public:
        int year;
        string value;
        string dgst;
        Block* next;
        Block* previous;
        MerkleTree* mtree;

        Block(){}
        Block(int y, string val, string dgs, Block* ne, Block* pr, MerkleTree* mt){
            year = y;
            value = val;
            dgst = dgs;
            next = ne;
            previous = pr;
            mtree = mt;
        }
};




#endif //BLOCK_HPP
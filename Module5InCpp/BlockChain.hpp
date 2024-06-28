#ifndef BLOCKCHAIN_HPP
#define BLOCKCHAIN_HPP


#include<iostream>
#include<string>
#include<vector>

using namespace std;

#define CRF_IMPLEMENTATION
#include "CRF.hpp"

#include "Exceptions.hpp"
#include "Pair.hpp"
#include "Block.hpp"
#include "MerkleTree.hpp"



const string BlockChain::start_string = "LabModule5";

class BlockChain{
    static const string start_string;
    Block* firstBlock;
    Block* lastBlock;

    string InsertBlock(vector<Pair<string,int>> Documents, int inputyear);
    Pair<vector<Pair<string,string>>, vector<Pair<string,string>>> ProofofScore(int student_id, int year);

};

string BlockChain::InsertBlock(vector<Pair<string,int>> Documents, int inputyear){
    //make a new Block and insert it into the chain...
    CRF obj(64);

    MerkleTree* mt = new MerkleTree();
    mt->Build(Documents); // Building the tree

    string val = mt->root->val + "_" + to_string(mt->root->maxleafval);
    string dgs = "";
    if(firstBlock == nullptr){ // meaning the chain is already empty
        dgs = obj.Fn(start_string + "#" + val);
        Block* newBlock = new Block(inputyear, val, dgs, nullptr, nullptr, mt);

        firstBlock = newBlock;
        lastBlock = newBlock;
    }
    else{ // meaning the chain already has some blocks
        dgs = obj.Fn(lastBlock->dgst + "#" + val);
        Block* newBlock = new Block(inputyear, val, dgs, nullptr, nullptr, mt);

        lastBlock->next = newBlock;
        newBlock->previous = lastBlock;
        lastBlock = newBlock;
    }
    return dgs;

}

Pair<vector<Pair<string,string>*>, vector<Pair<string,string>*>> BlockChain::ProofofScore(int student_id, int year){
    Block* curr = lastBlock;
    while(curr != nullptr && curr->year != year) curr = curr->previous;
    if(curr == nullptr) throw DocumentNotFoundException();
    
    //We are at the required Block...
    vector<Pair<string, string>*> sCPR = curr->mtree->QueryScore(student_id);
    vector<Pair<string, string>*> dVpair;
    while(curr != nullptr){ // retrace the path...
        dVpair.push_back(new Pair(curr->dgst, curr->value));
        curr = curr->next;
    }

    Pair<vector<Pair<string,string>*>, vector<Pair<string,string>*>> ans;
    ans.first = sCPR;
    ans.second = dVpair;

    return ans;
}





#endif // BLOCKCHAIN_HPP
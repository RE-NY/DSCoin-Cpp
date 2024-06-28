#ifndef MERKLETREEIMPROVED_HPP
#define MERKLETREEIMPROVED_HPP


#include<iostream>
#include<string>
using namespace std;

#include "Conversion.hpp"
#include "Node.hpp"

#define CRF_IMPLEMENTATION
#include "CRF.hpp"
#include "Exceptions.hpp"
#include "Pair.hpp"



class MerkleTree{
    public:
        TreeNode* root;
        int numDocs;

        string InsertDocument(string document);
        string DeleteDocument(string document);

        TreeNode* InsertDocumentUtil(string& document, TreeNode* curr, CRF& obj);
};
/*
NOTES:
1. This Merkle trees arranges documents in sorted order(i.e. the leaves are sorted)
*/

string MerkleTree::InsertDocument(string document){
    CRF obj(64);
    root = InsertDocumentUtil(document, root, obj);
    return root->val;
}

TreeNode* MerkleTree::InsertDocumentUtil(string& document, TreeNode* curr, CRF& obj){
    if(curr == nullptr){
        return new TreeNode(document, nullptr, nullptr, true, 1, document, document); // basically garbage min and max for leaf
    }
    if(curr->isLeaf){ // the case when curr is a leaf;
        string toPut = "";
        TreeNode* node;
        TreeNode* newLeaf = new TreeNode(document, nullptr, nullptr, true, 1, document, document);
        if(document < curr->val){
            toPut = obj.Fn(document + "#" + curr->val);
            node = new TreeNode(toPut, newLeaf, curr, false, 2, document, curr->val);
        }
        else{
            toPut = obj.Fn(curr->val + "#" + document);
            node = new TreeNode(toPut, curr, newLeaf, false, 2, curr->val, document);
        }
        return node;
    }
    if(curr->left != nullptr && document < curr->left->maxleafval){
        if(!(curr->left->isLeaf)){
                curr->left = InsertDocumentUtil(document, curr->left, obj);
        }
        else{
            TreeNode* temp = curr->left;
            curr->left = nullptr;
            string toPut = "";
            TreeNode* newLeaf = new TreeNode(document, nullptr, nullptr, true, 1, document, document);
            TreeNode* node;
            if(document < temp->val){
                toPut = obj.Fn(document + "#" + temp->val);
                node = new TreeNode(toPut, newLeaf, temp, false, 2, document, temp->val);
            }
            else{
                toPut = obj.Fn(temp->val + "#" + document);
                node = new TreeNode(toPut, temp, newLeaf, false, 2, temp->val, document);
            }
            curr->left = node;
        }
    }
    else if(curr->right != nullptr && document > curr->right->minleafval){
        if(!(curr->right->isLeaf)){
            if(document > curr->right->minleafval){
                curr->right = InsertDocumentUtil(document, curr->right, obj);
            }
        }
        else{
            TreeNode* temp = curr->right;
            curr->right = nullptr;
            string toPut = "";
            TreeNode* newLeaf = new TreeNode(document, nullptr, nullptr, true, 1, document, document);
            TreeNode* node;
            if(document < temp->val){
                toPut = obj.Fn(document + "#" + temp->val);
                node = new TreeNode(toPut, newLeaf, temp, false, 2, document, temp->val);
            }
            else{
                toPut = obj.Fn(temp->val + "#" + document);
                node = new TreeNode(toPut, temp, newLeaf, false, 2, temp->val, document);
            }
            curr->right = node;
        }
    }
    //We are desining the algo such that we always insert at non-leaf node

    //Now the document is inserted, time to return but let's now update the properties....
    curr->isLeaf = false;
    //Since by design any non leaf always has two chils(otherwise hash can't be calculated)
    //So no need to worry as now both left and right are non-nullptr....

    curr->val = obj.Fn(curr->left->val + "#" + curr->right->val);
    curr->height = 1 + max(curr->left->height, curr->right->height);

    curr->minleafval = min(curr->left->minleafval, curr->right->minleafval);
    curr->maxleafval = max(curr->left->maxleafval, curr->right->maxleafval);

    int heightBalance = curr->left->height - curr->right->height;

    if(heightBalance > 1 && document <= curr->left->left->maxleafval){
        return curr->SingleRightRotation();
    }
    if(heightBalance > 1){
        return curr->DoubleLeftRightRotation();
    }
    if(heightBalance < -1 && document >= curr->right->right->minleafval){
        return curr->SingleLeftRotation();
    }
    if(heightBalance < -1){
        return curr->DoubleRightLeftRotation();
    }
    return curr;

}
string DeleteDocument(string document);

TreeNode* DeleteDocumentUtil(string& document, TreeNode* curr){
    if(curr == nullptr){
        throw DocumentNotFoundException();
    }
    if(curr->isLeaf){
        if(curr->val == document) return nullptr;
        else throw DocumentNotFoundException();
    }
    if(curr->left != nullptr && document < curr->left->maxleafval){

    }
    else if(){

    }
    return nullptr; //TO BE IMPLEMENTED
}





#endif //MERKLETREEIMPROVED_HPP
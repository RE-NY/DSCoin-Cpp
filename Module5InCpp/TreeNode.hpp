#ifndef TREENODE_HPP
#define TREENODE_HPP

#include<iostream>
#include<string>
using namespace std;

class TreeNode{
    public:
        string val;
        TreeNode* parent;
        TreeNode* left;
        TreeNode* right;
        bool isLeaf;
        int numleaves;
        int maxleafval;

        TreeNode(){}
        TreeNode(string s, TreeNode* pa, TreeNode* le, TreeNode* ri, bool isL, int numL, int maxL){
            val = s;
            parent = pa;
            left = le;
            right = ri;
            isLeaf = isL;
            numleaves = numL;
            maxleafval = maxL;
        }

};


#endif //TREENODE_HPP
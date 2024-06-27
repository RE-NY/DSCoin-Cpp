#ifndef NODE_HPP
#define NODE_HPP


#include<string>
using namespace std;

class TreeNode{
    public:
        string val;
        TreeNode* parent;
        TreeNode* left;
        TreeNode* right;
        TreeNode(){}
        TreeNode(string va, TreeNode* pa, TreeNode* le, TreeNode* ri){
            val = va;
            parent = pa;
            left = le;
            right = ri;
        }
};








#endif // NODE_HPP
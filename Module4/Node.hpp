#ifndef NODE_HPP
#define NODE_HPP


#include<string>
#include<cmath>
using namespace std;

#include "CRF.hpp"

class TreeNode{
    public:
        string val;
        TreeNode* left;
        TreeNode* right;

        int height;
        bool isLeaf;
        int numberLeaves;
        string minleafval;
        string maxleafval;

        TreeNode(){}
        TreeNode(string v, TreeNode* le, TreeNode* ri, bool isL, int heig, string minLeafv, string maxLeafv){
            val = v;
            left = le;
            right = ri;
            isLeaf = isL;
            height = heig;
            minleafval = minLeafv;
            maxleafval = maxLeafv;
        }

        static int updateHeight(TreeNode* curr);

        TreeNode* SingleLeftRotation();
        TreeNode* SingleRightRotation();
        TreeNode* DoubleLeftRightRotation();
        TreeNode* DoubleRightLeftRotation();
};


int TreeNode::updateHeight(TreeNode* curr){ // just a wrapper function to take care of nullptrs.
    if(curr == nullptr) return 0;
    return curr->height;
}


TreeNode* TreeNode::SingleRightRotation(){
    CRF obj(64);

    TreeNode* temp = left;
    left = temp->right;
    temp->right = this;


    //update height and etc...
    val = obj.Fn(left->val + "#" + right->val);
    height = 1 + max(updateHeight(left), updateHeight(right));
    isLeaf = false; // cause we only rotate non leaf nodes and they always have a left and right!!. Leaves are already balanced.
    minleafval = min(left->minleafval, right->minleafval);
    maxleafval = max(left->maxleafval, right->maxleafval);


    temp->val = obj.Fn(temp->left->val + "#" + temp->right->val);
    temp->height = 1 + max(updateHeight(temp->left), updateHeight(temp->right));
    temp->isLeaf = false;
    temp->minleafval = min(temp->left->minleafval, temp->right->minleafval);
    temp->maxleafval = max(temp->left->maxleafval, temp->right->maxleafval);

    return temp;
}


TreeNode* TreeNode::SingleLeftRotation(){
    CRF obj(64);

    TreeNode* temp = right;
    right = temp->left;
    temp->left = this;

    //update height and etc...
    val = obj.Fn(left->val + "#" + right->val);
    height = 1 + max(updateHeight(left), updateHeight(right));
    isLeaf = false; // cause we only rotate non leaf nodes and they always have a left and right!!. Leaves are already balanced.
    minleafval = min(left->minleafval, right->minleafval);
    maxleafval = max(left->maxleafval, right->maxleafval);


    temp->val = obj.Fn(temp->left->val + "#" + temp->right->val);
    temp->height = 1 + max(updateHeight(temp->left), updateHeight(temp->right));
    temp->isLeaf = false;
    temp->minleafval = min(temp->left->minleafval, temp->right->minleafval);
    temp->maxleafval = max(temp->left->maxleafval, temp->right->maxleafval);


    return temp;
}
TreeNode* TreeNode::DoubleLeftRightRotation(){
    TreeNode* tempRoot = this->SingleLeftRotation();
    return tempRoot->SingleRightRotation();
}
TreeNode* TreeNode::DoubleRightLeftRotation(){
    TreeNode* tempRoot = this->SingleRightRotation();
    return tempRoot->SingleLeftRotation();
}



#endif // NODE_HPP
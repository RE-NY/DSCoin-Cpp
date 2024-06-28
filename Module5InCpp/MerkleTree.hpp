#ifndef MERKLETREE_HPP
#define MERKLETREE_HPP

#include<iostream>
#include<string>
#include<vector>
#include<cmath>
#include<cassert>


using namespace std;


#define CRF_IMPLEMENTATION
#include "CRF.hpp"

#include "TreeNode.hpp"
#include "Exceptions.hpp"
#include "Pair.hpp"



class MerkleTree{
    public:
        TreeNode* root;
        int numStudents;

        string Build(vector<Pair<string,int>> documents);
        string UpdateScore(int student_id, int newScore);
        vector<Pair<string,string>*> QueryScore(int student_idx);

//Below are the helper functions.....
        TreeNode* buildUtil(vector<Pair<string,int>>& documents, int& n, CRF& obj, int level, int start_index, int end_index);
        string UpdateScoreUtil(int& student_id, int& newScore, CRF& obj, int level, int lowBound, int highBound, TreeNode* curr);
        void QueryScoreUtil(int& student_idx, vector<Pair<string,string>*>& ans, int level, int docsSizeBelowLow, int docsSizeBelowHigh, TreeNode* curr);
};


TreeNode* MerkleTree::buildUtil(vector<Pair<string,int>>& documents, int& n, CRF& obj, int level, int start_index, int end_index){
    if(level == (int)log2(n)){ //Automatically at this point start_index == end_index
        assert(start_index == end_index);
        int score = documents[start_index].second;
        string data = documents[start_index].first + "_" + to_string(score);
        return new TreeNode(data, nullptr, nullptr, nullptr, true, 0, score);
    }

    int mid = start_index + (end_index - start_index)/2;

    TreeNode* left = buildUtil(documents, n, obj, level+1, start_index, mid);
    TreeNode* right = buildUtil(documents, n, obj, level+1, mid+1, end_index);

    string val = obj.Fn(left->val + "#" + right->val);
    
    //update all the paremeters
    int leaves = left->numleaves + right->numleaves;
    int maxleaf = max(left->maxleafval, right->maxleafval);

    TreeNode* node = new TreeNode(val, nullptr, left, right, false, leaves, maxleaf);

    left->parent = node;
    right->parent = node;

    return node;
}



string MerkleTree::Build(vector<Pair<string,int>> documents){
    int n = documents.size(); // Assume n = power of 2....

    if(n == 0) throw EmptyInputException();

    CRF obj(64);
    root = buildUtil(documents, n, obj, 0, 0, n-1);
    numStudents = n;

    return root->val;
}


string MerkleTree::UpdateScoreUtil(int& student_id, int& newScore, CRF& obj, int level, int lowBound, int highBound, TreeNode* curr){
    if(level == (int)log2(numStudents)){
        assert(lowBound == highBound);

        if(newScore > curr->maxleafval) curr->maxleafval = newScore;
        //update curr->val to name_newScore;
        int b=0;
        for(int i=0; i<(curr->val).size(); i++){
            if((curr->val)[i] == '_'){b = i; break;}
        }
        curr->val = curr->val.substr(0, b+1) + to_string(newScore);
        return curr->val;
    }

    int mid = lowBound + (highBound-lowBound)/2;
    string fromChild = "";
    string toPut= "";
    if(student_id <= mid){ // meaning document is to the left of it.
        fromChild = UpdateScoreUtil(student_id, newScore, obj, level+1, lowBound, mid, curr->left);
        toPut = obj.Fn(fromChild + "#" + curr->right->val);
        curr->val = toPut;
        curr->maxleafval = max(curr->left->maxleafval, curr->right->maxleafval);
        return curr->val;
    }
    else{ //means document is to the right of it.
        fromChild = UpdateScoreUtil(student_id, newScore, obj, level+1, mid+1, highBound, curr->right);
        toPut = obj.Fn(curr->left->val + "#" + fromChild);
        curr->val = toPut;
        curr->maxleafval = max(curr->left->maxleafval, curr->right->maxleafval);
        return curr->val;
    }
}


string MerkleTree::UpdateScore(int student_id, int newScore){
    if(student_id < 0 || student_id >= numStudents) throw DocumentNotFoundException();

    CRF obj(64);
    int totalStudents = numStudents;
    string new_summary = UpdateScoreUtil(student_id, newScore, obj, 0, 0, totalStudents-1, root);
    return new_summary;
}



void MerkleTree::QueryScoreUtil(int& student_idx, vector<Pair<string,string>*>& ans, int level, int docsSizeBelowLow, int docsSizeBelowHigh, TreeNode* curr){
     //Initially docsSizeBelow = numStudents - 1;
    if(level == (int)log2(numStudents) - 1){
            string first_string = curr->left->val;
            string second_string = curr->right->val;
            Pair<string,string>* p = new Pair(first_string, second_string);
            ans.push_back(p);
            return;
    }

    int mid = docsSizeBelowLow + (docsSizeBelowHigh - docsSizeBelowLow)/2;
    if(student_idx <= mid){
        QueryScoreUtil(student_idx, ans, level+1, docsSizeBelowLow, mid, curr->left);
    }
    else{
        QueryScoreUtil(student_idx, ans, level+1, mid+1, docsSizeBelowHigh, curr->right);
    }

    string first_string = curr->left->val;
    string second_string = curr->right->val;
    Pair<string,string>* p = new Pair(first_string, second_string);
    ans.push_back(p);
}





vector<Pair<string,string>*> MerkleTree::QueryScore(int student_idx){
    if(student_idx >= numStudents || student_idx < 0) throw DocumentNotFoundException();

    vector<Pair<string,string>*> ans;
    QueryScoreUtil(student_idx, ans, 0, 0, numStudents-1, root);
    string second_Arg = "\0";
    ans.push_back(new Pair(root->val, second_Arg));

    return ans;
}




#endif // MERKLETREE_HPP
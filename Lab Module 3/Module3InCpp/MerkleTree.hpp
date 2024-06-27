#ifndef MERKLETREE_HPP
#define MERKLETREE_HPP

#include<string>
#include <cmath>
#include <cassert>
#include <vector>

#include "Node.hpp"
#include "Pair.hpp"
#include "CRF.hpp"
#define CRF_IMPLEMENTATION
#include "Exceptions.hpp"

using namespace std;

class MerkleTree{
    public:
        TreeNode* root;
        int numDocs;
        string build(string documents[], int n); // assume n(documents' size) is power of 2.
        vector<Pair<string,string>*> QueryDocument(int doc_idx);
        static bool Authenticate(vector<Pair<string,string>*> path, string summary);
        string UpdateDocument(int doc_idx, string new_document);
    
//Below are the helper functions......
    private:
        // n is the length of documents(power of 2).
        TreeNode* buildUtil(vector<string> documents, int& n, CRF& obj, int level, int start_index, int end_index);
        void QueryDocumentUtil(int& doc_idx, vector<Pair<string,string>*>& ans, int level, int docsSizeBelow, int docsSizeBelowHigh, TreeNode* cur);
        string UpdateDocumentUtil(int& doc_idx, string& new_document, CRF& obj, int level, int lowBound, int highBound, TreeNode* curr); // assumes tree is already built.

};


TreeNode* MerkleTree::buildUtil(vector<string> documents, int& n, CRF& obj, int level, int start_index, int end_index){    
    if(level == (int)log2(n)){ //Automatically at this point start_index == end_index
        assert(start_index == end_index);
        return new TreeNode(documents[start_index], nullptr, nullptr, nullptr);
    }

    int mid = start_index + (end_index - start_index)/2;

    TreeNode* left = buildUtil(documents, n, obj, level+1, start_index, mid);
    TreeNode* right = buildUtil(documents, n, obj, level+1, mid+1, end_index);

    string val = obj.Fn(left->val + "#" + right->val);

    TreeNode* node = new TreeNode(val, nullptr, left, right);

    left->parent = node;
    right->parent = node;

    return node;
/*
Idea of Algo : Yeh toh clear hai ki approach must be bottom up or in other words we need to start the algo
from top and base case will be at the bottom....
*/
}

string MerkleTree::build(string documents[], int n){
    if(n == 0){
        throw EmptyInputException();
    }
    vector<string> docs(n);
    for(int i=0; i<n; i++) docs[i] = documents[i];

    CRF obj(64);
    root = buildUtil(docs, n, obj, 0, 0, n-1);
    numDocs = n;
    return root->val;
}

void MerkleTree::QueryDocumentUtil(int& doc_idx, vector<Pair<string,string>*>& ans, int level, int docsSizeBelowLow, int docsSizeBelowHigh, TreeNode* curr){
    //Initially docsSizeBelow = numDocs - 1;
    if(level == (int)log2(numDocs) - 1){
            string first_string = curr->left->val;
            string second_string = curr->right->val;
            Pair<string,string>* p = new Pair(first_string, second_string);
            ans.push_back(p);
            return ;
    }

    int mid = docsSizeBelowLow + (docsSizeBelowHigh - docsSizeBelowLow)/2;
    if(doc_idx <= mid){
        QueryDocumentUtil(doc_idx, ans, level+1, docsSizeBelowLow, mid, curr->left);
    }
    else{
        QueryDocumentUtil(doc_idx, ans, level+1, mid+1, docsSizeBelowHigh, curr->right);
    }

    string first_string = curr->left->val;
    string second_string = curr->right->val;
    Pair<string,string>* p = new Pair(first_string, second_string);
    ans.push_back(p);

}

vector<Pair<string,string>*> MerkleTree::QueryDocument(int doc_idx){ // doc_idx must be b/w 0 to numDocs-1
    if(doc_idx >= numDocs || doc_idx < 0) throw DocumentNotFoundException();

    vector<Pair<string,string>*> ans;
    QueryDocumentUtil(doc_idx, ans, 0, 0, numDocs-1, root);
    string second_Arg = "\0";
    ans.push_back(new Pair(root->val, second_Arg));

    return ans;

}

bool MerkleTree::Authenticate(vector<Pair<string,string>*> path, string summary){
    int d = path.size();
    CRF obj(64);
    for(int i=0; i<d-1; i++){
        string toCheck = obj.Fn((path[i])->first + "#" + (path[i])->second);
        if(toCheck != (path[i-1])->first && toCheck != (path[i-1])->second) return false;
    }
    if(obj.Fn((path[d-1])->first + "#" + (path[d-1])->second) != summary) return false;

    return true;
}

string MerkleTree::UpdateDocumentUtil(int& doc_idx, string& new_document, CRF& obj, int level, int lowBound, int highBound, TreeNode* curr){
    if(level == (int)log2(numDocs)){
        assert(lowBound == highBound);
        curr->val = new_document;
        return new_document;
    }

    int mid = lowBound + (highBound-lowBound)/2;
    string fromChild = "";
    string toPut= "";
    if(doc_idx <= mid){ // meaning document is to the left of it.
        fromChild = UpdateDocumentUtil(doc_idx, new_document, obj, level+1, lowBound, mid, curr->left);
        toPut = obj.Fn(fromChild + "#" + curr->right->val);
        curr->val = toPut;
        return curr->val;
    }
    else{ //means document is to the right of it.
        fromChild = UpdateDocumentUtil(doc_idx, new_document, obj, level+1, mid+1, highBound, curr->right);
        toPut = obj.Fn(curr->left->val + "#" + fromChild);
        curr->val = toPut;
        return curr->val;
    }
}

string MerkleTree::UpdateDocument(int doc_idx, string new_document){
    if(doc_idx < 0 || doc_idx >= numDocs) throw DocumentNotFoundException();

    CRF obj(64);
    string new_summary = UpdateDocumentUtil(doc_idx, new_document, obj, 0, 0, numDocs-1, root);
    return new_summary;
}



#endif //MERKLETREE_HPP


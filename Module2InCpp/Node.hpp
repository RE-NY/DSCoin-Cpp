#ifndef NODE_HPP
#define NODE_HPP

#include<string>

using namespace std;

class Data{
    public:
    string value;
};


class Node{
    public:
        Data data;
        Node* previous;
	    Node* next;
	    string dgst;
        Node(){}
        Node(Data d, string dgs){
            data = d;
            dgst = dgs;
            previous = nullptr;
            next = nullptr;
        }

};


class StackNode{
    public:
        Data data;
        string dgst;
};




#endif NODE_HPP
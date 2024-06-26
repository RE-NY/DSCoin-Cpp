#ifndef AUTHSTACK_HPP
#define AUTHSTACK_HPP

#include <iostream>
#include <string>

#define CRF_IMPLEMENTATION
#include "CRF.hpp"
#include "Pair.hpp"
#include "Node.hpp"
#include "Exceptions.hpp"
#include "AuthList.hpp"

using namespace std;


const string AuthStack::start_string = "2022ME12005";

//We will use AuthList for AuthStack....Implementing stack using linkedlist
//Basically using AuthList and pushing/popping from lastNode only....

class AuthStack{
    private:
    static const string start_string;
    StackNode* top;
    AuthList* stack;

    public:
    AuthStack(){
        stack = new AuthList();
    }

//Description of class methods are given in Module2 pdf

    static bool CheckStack(AuthStack* current, string proof){
        return AuthList::CheckList(current->stack, proof);
    }
    string push(Data datainsert, string proof){
        return stack->InsertNode(datainsert, proof);
    }
    string pop(string proof){
        try{
            return stack->DeleteLast(proof);
        }
        catch(EmptyListException& e){
            throw EmptyStackException();
        }
        
    }
    StackNode* GetTop(string proof){
        if(stack->lastNode == nullptr){
            throw EmptyStackException();
        }
        Data d = stack->lastNode->data;
        string dgs = stack->lastNode->dgst;
        StackNode* temp = new StackNode();
        temp->data = d;
        temp->dgst = dgs;
        return temp;
    }
};


#endif //AUTHSTACK_HPP
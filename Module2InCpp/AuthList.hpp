#ifndef AUTHLIST_HPP
#define AUTHLIST_HPP

#include <iostream>
#include <string>

#define CRF_IMPLEMENTATION
#include "CRF.hpp"
#include "Pair.hpp"
#include "Node.hpp"
#include "Exceptions.hpp"

using namespace std;


const string AuthList::start_string = "2022ME12005";

class AuthList{
    public:
        static const string start_string;
        Node* firstNode;
        Node* lastNode;

//Description of class methods are given in Module2 pdf

        static bool CheckList(AuthList* current, string proof){ // client has the proof of lastnode!
            CRF obj(64);
            Node* curr = current->firstNode;
            while(curr != nullptr){
                if(curr == current->firstNode){
                    string hash = obj.Fn(start_string + "#" + curr->data.value);
                    if(hash != curr->dgst){
                        throw AuthenticationFailedException();
                    }
                }
                else if(curr == current->lastNode){
                    string hash = obj.Fn(curr->previous->dgst + "#" + curr->data.value);
                    if( hash != curr->dgst || hash != proof){
                        throw AuthenticationFailedException();
                    }
                }
                else{
                    string hash = obj.Fn(curr->previous->dgst + "#" + curr->data.value);
                    if( hash != curr->dgst){
                        throw AuthenticationFailedException();
                    }
                }
                curr = curr->next;
            }
            return true;
        }

        string InsertNode(Data datainsert, string proof){
            //First the original Linked list is checked and if accepted then the node is inserted at the back
            bool check = AuthList::CheckList(this, proof);
            if(check){
                CRF obj(64);
                if(lastNode != nullptr){
                    string toEncrypt = lastNode->dgst + "#" + datainsert.value;
                    string new_dgst = obj.Fn(toEncrypt);

                    Node* new_node = new Node(datainsert, new_dgst);
                    lastNode->next = new_node;
                    new_node->previous = lastNode;
                    lastNode = new_node;

                    proof = new_dgst;
                    return proof;
                }
                else{                                       // inserting the first node
                    string toEncrypt = start_string + "#" + datainsert.value;
                    string new_dgst = obj.Fn(toEncrypt);
                    
                    Node* new_node = new Node(datainsert, new_dgst);
                    firstNode = new_node;
                    lastNode = new_node;

                    proof = new_dgst;
                    return proof;
                }
            }
            else throw AuthenticationFailedException();


        }
        string DeleteFirst(string proof){
            if(firstNode == nullptr){
                throw EmptyListException();
            }
            bool check = AuthList::CheckList(this, proof);
            if(check){
                //dont know what to do if only one element hai!!!!!
                if(firstNode->next == nullptr){
                    firstNode = nullptr;
                    lastNode = nullptr;
                    return proof;
                }

                firstNode->next = firstNode;
                Node* toDelete = firstNode->previous;
                firstNode->previous = nullptr;
                
                //Delete the head node
                delete toDelete;

                //update all dgsts
                CRF obj(64);
                Node* curr = firstNode;
                string new_proof = proof;
                while(curr != nullptr){
                    if(curr == firstNode){
                        curr->dgst = obj.Fn(start_string + "#" + curr->data.value);
                    }
                    else if(curr == lastNode){
                        curr->dgst = obj.Fn(start_string + "#" + curr->data.value);
                        new_proof = curr->dgst;
                    }
                    else{
                        curr->dgst = obj.Fn(curr->previous->dgst + "#" + curr->data.value);
                    }
                    curr = curr->next;
                }
                return new_proof; // returns dgst of lastNode
            }
            else {
                throw AuthenticationFailedException();
            }


        }
        string DeleteLast(string proof){
            if(firstNode == nullptr){
                throw EmptyListException();
            }
            bool check = AuthList::CheckList(this, proof);
            if(check){
                //dont know what to do if only one element hai!!!!!
                if(firstNode->next == nullptr){
                    firstNode = nullptr;
                    lastNode = nullptr;
                    return proof;
                }
                lastNode = lastNode->previous;
                Node* toDelete = lastNode->next;
                lastNode->next = nullptr;

                delete toDelete;

                return lastNode->dgst;
            }
            else{
                throw AuthenticationFailedException();
            }
        }
        /* 
	    Notice that this function is static, the reason why this is static is that we don't want this to be tied with
	    an object of the class AuthList. 	
	    */
        static Node* RetrieveNode(AuthList* current, string proof, Data data){
            bool check = AuthList::CheckList(current, proof);
            if(check){
                Node* curr = current->firstNode;
                while(curr != nullptr){
                    if(curr->data.value == data.value) return curr;
                    curr = curr->next;
                }
                throw DocumentNotFoundException();
            }
            else{
                throw AuthenticationFailedException();
            }
        }
        void AttackList(AuthList current, string new_data){
            if(lastNode == nullptr){
                throw EmptyListException();
            }
            CRF obj(64);
            firstNode->data.value = new_data;
            int new_dgst = obj.Fn(start_string + "#" + new_data);
            firstNode->dgst = new_dgst;
            
            Node* curr = firstNode->next;
            while(curr != nullptr){
                curr->dgst = obj.Fn(curr->previous->dgst + "#" + curr->data.value);
                curr = curr->next;
            }
            //But the problem is we can give this new "proof" to the client and that is why 
            //this attack can be checked...
        }

};


#endif //AUTHLIST_HPP



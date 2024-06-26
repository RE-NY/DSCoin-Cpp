#include <iostream>

#define CRF_IMPLEMENTATION
#include "CRF.hpp"
#include "Pair.hpp"
#include "Node.hpp"
#include "AuthList.hpp"
#include "AuthStack.hpp"


using namespace std;



int main(){

    string password = "Hello World";
    string hashed = SHA256::encrypt(password);

    cout << "SHA-256 hash of '" << password << "' is: " << hashed << endl;
    cout << "size : " << hashed.size() << endl;
    
}
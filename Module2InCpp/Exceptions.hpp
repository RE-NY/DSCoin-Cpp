#ifndef EXCEPTIONS_HPP
#define EXCEPTIONS_HPP

#include <iostream>
#include <exception>

class AuthenticationFailedException : public std::exception {
public:
    AuthenticationFailedException() {
        std::cerr << "Authentication Failed!" << std::endl;
    }

    const char* what() const noexcept override {
        return "Authentication Failed!";
    }
};

class DocumentNotFoundException : public std::exception {
public:
    DocumentNotFoundException() {
        std::cerr << "Document Not Found!" << std::endl;
    }

    const char* what() const noexcept override {
        return "Document Not Found!";
    }
};

class EmptyListException : public std::exception {
public:
    EmptyListException() {
        std::cerr << "Error: List Empty" << std::endl;
    }

    const char* what() const noexcept override {
        return "Error: List Empty";
    }
};

class EmptyStackException : public std::exception {
public:
    EmptyStackException() {
        std::cerr << "Error: Stack Empty" << std::endl;
    }

    const char* what() const noexcept override {
        return "Error: Stack Empty";
    }
};

#endif // EXCEPTIONS_HPP

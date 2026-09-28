#ifndef STACK_H
#define STACK_H

#include <string>
#include <vector>

class Stack
{
public:
    struct Operator
    {
        std::string word;
        int lineNumber;
    };
    
    bool isEmpty();

    void push(std::string str, int lineNumber);

    Operator top();

    Operator pop();

private:
    std::vector<Operator> data;
};

#endif
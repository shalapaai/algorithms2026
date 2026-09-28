#include "stack.h"

bool Stack::isEmpty()
{
    return data.empty();
}

void Stack::push(std::string str, int lineNumber)
{
    data.push_back({
        str,
        lineNumber
    });
}

Stack::Operator Stack::top()
{
    if (!isEmpty())
        return data.back();

    return {"", 0};
}

Stack::Operator Stack::pop()
{
    if (!isEmpty())
    {
        Operator element = data.back();
        data.pop_back();

        return element;
    }

    return {"", 0};
}
/*
    Условие: 
    Программа на ПАСКАЛЕ включает такие  сочетания ключевых
    слов,    как    REPEAT..UNTIL,   IF..THEN..ELSE,   BEGIN..END,
    RECORD..END. Конец оператора  определяется  точкой  с  запятой 
    (";").  Требуется  проверить  правильность  вложенности данных
    конструкций с учетом допустимости взаимных вложений. В случае
    ошибок указать номер первой некорректной строки (12).

    Общими требованиями к лабораторной работе являются:
        1) организовать  ввод  данных  из  файла  в   понятной  для 
    пользователя форме;
        2) обеспечить   возможность   многократных   запросов   без
    повторного запуска программы;
        3) при реализации в С++ не использовать контейнерные классы
    для работы с линейными списками типа stack, queue и т. п.

    Автор: Маймеску Максим

    Среда: Linux, Visual Studio Code

    Источники: 
    лекция
*/

#include <iostream>
#include <fstream>
#include <string>
#include "stack.h"

Stack::Operator getWord(std::ifstream &file, int &lineNumber)
{
    char ch;
    std::string str = "";
    while(file.get(ch))
    {
        if (ch == '(' || ch == ')')
        {
            if (str.size() > 0)
            {
                file.seekg(-1, std::ios::cur);
                return { str, lineNumber };
            }
            else
            {
                str += ch;
                return { str, lineNumber };
            }
        }
        if (ch == '\n')
        {
            lineNumber++;
            if (str.size() == 0)
                continue;
            return { str, lineNumber - 1 };
        }
        if (ch == ' ' || ch == ';' || ch == '.')
        { 
            if (str.size() == 0)
                continue;
            return { str, lineNumber };
        }
        str += std::tolower(ch);
    }
    return { str, lineNumber };
}

int main()
{
    int answer = 1;
    while (answer == 1)
    {
        std::cout << "1) Check nesting" << std::endl;
        std::cout << "2) Finish" << std::endl;
        std::cout << "Type your answer: ";
        std::cin >> answer;
        if (answer == 2)
            break;

        std::cout << "Type the path to the input file: ";
        std::string fileName;
        std::cin >> fileName;
        std::ifstream file(fileName);

        if (!file.is_open())
        {
            std::cout << "Failed to open the input file" << '\n';
            continue;
        }
        file.seekg(0, std::ios::beg);

        Stack stack;
        Stack::Operator oper;
        std::string str;
        int lineNumber = 1;
        bool hasExeption = false;

        while (!file.eof() && !hasExeption)
        {
            oper = getWord(file, lineNumber);
            str = oper.word;

            if (str == "record")
            {
                if (stack.isEmpty())
                {
                    stack.push(str, lineNumber);
                    continue;
                }
                else
                {
                    std::cout << "Exeption: pushing '" << str << "' to a not empty stack" << std::endl;
                    hasExeption = true;
                    break;
                }
            }

            if ((str == "repeat" || str == "if") && stack.isEmpty())
            {
                std::cout << "Exeption: pushing '" << str << "' to an empty stack" << std::endl;
                hasExeption = true;
                break;
            }

            if (str == "begin" || str == "repeat" || str == "if")
            {
                if (stack.top().word != "record" && stack.top().word != "(" && stack.top().word != "if")
                {
                    stack.push(str, lineNumber);
                    continue;
                }
                else
                {
                    std::cout << "Exeption: pushing '" << str << "' to stack with not closed 'record' or '('" << std::endl;
                    hasExeption = true;
                    break;
                }
            }

            if (str == "(")
            {
                stack.push(str, lineNumber);
                continue;
            }

            if (str == ")")
            {
                if (stack.top().word == "(")
                {
                    stack.pop();
                    continue;
                }
                else
                {
                    std::cout << "Exeption: pushing '" << str << "' to stack where top element is not '('" << std::endl;
                    hasExeption = true;
                    break;
                }
            }

            if (str == "then")
            {
                if (stack.top().word == "if")
                {
                    stack.pop();
                    stack.push(str, lineNumber);
                    continue;
                }
                else
                {
                    std::cout << "Exeption: pushing '" << str << "' to stack where top element 'if' not exists" << std::endl;
                    hasExeption = true;
                    break;
                }
            }

            if (str == "else")
            {
                if (stack.top().word == "then")
                {
                    stack.pop();
                    continue;
                }
                else
                {
                    std::cout << "Exeption: pushing '" << str << "' to stack where top element 'then' not exists" << std::endl;
                    hasExeption = true;
                    break;
                }
            }

            while ((str == "end" || str == "until") && stack.top().word == "then")
            {
                stack.pop();
            }

            if (str == "end")
            {
                if (stack.top().word == "begin" || stack.top().word == "record")
                {
                    stack.pop();
                    continue;
                }
                else
                {
                    std::cout << "Exeption: pushing '" << str << "' to stack where top element 'begin' or 'record' not exists" << std::endl;
                    hasExeption = true;
                    break;
                }
            }

            if (str == "until")
            {
                if (stack.top().word == "repeat")
                {
                    stack.pop();
                    continue;
                }
                else
                {
                    std::cout << "Exeption: pushing '" << str << "' to stack where top element 'repeat' not exists" << std::endl;
                    hasExeption = true;
                    break;
                }
            }
        }

        if (!stack.isEmpty() && hasExeption == false)
        {
            std::cout << "Exeption: out stack is not empty, nesting error" << std::endl;
            while (!stack.isEmpty())
                lineNumber = stack.pop().lineNumber;
            hasExeption = true;
        }

        if (hasExeption == false)
            std::cout << "\033[32mThe nesting check was successful!\033[0m" << std::endl;
        else
            std::cout << "\033[31mThere is a nesting error! Line number with the error: " << oper.lineNumber << "\033[0m" << std::endl;
    }
    return 0;
}
/*
    Условие: 
    В текстовом  файле  имеется  некоторое   информационнное
    письмо. Требуется сформировать ответ на письмо. Если  последнее
    предложение письма является  вопросом,  начинающимся  со  слова 
    "где", то слово "где" заменяется  словами "в Караганде", а знак
    вопроса  заменяется  на  знак  восклицания.  В противном случае
    нужно дать ответ: "Спасибо за информацию" (8).

    Автор: Маймеску Максим

    Среда: Linux, Visual Studio Code

    Источники: 
    https://docs.google.com/presentation/d/1gEZ257qhYFC-hL9hmtzyDozDa8JcdyH0/edit?slide=id.p23#slide=id.p23
    https://www.w3schools.com/cpp/ref_fstream_fstream.asp
    http://www.regatta.cs.msu.su/doc/usr/share/man/info/ru_RU/a_doc_lib/aixprggd/genprogc/writing_converters_iconv_interface.htm
*/

#include <iostream>
#include <fstream>
#include <string>
#include "encoding.h"

std::string reverseString(std::string str)
{
    std::string reversedStr;
    for (int i = str.size() - 1; i >= 0; i--)
        reversedStr += str[i];
    return reversedStr;
}

std::string trimString(std::string str)
{
    int startPos = 0, endPos = 0;
    for (int i = 0; i < str.size(); i++)
    {
        if (str[i] != ' ') {
            startPos = i;
            break;
        }
    }
    for (int i = str.size() - 1; i >= 0; i--)
    {
        if (str[i] != ' ') {
            endPos = i + 1;
            break;
        }
    }
    std::string trimmedStr = "";

    for (int i = startPos; i < endPos; i++) {
        trimmedStr += str[i];
    }
    return trimmedStr;
}

std::string getSubString(std::string str, int startPos, int endPos)
{
    std::string subStr;
    if (str == "") return str;
    for (int i = startPos; i <= endPos; i++)
        subStr += str[i];
    return subStr;
}

std::string getLastSentence(std::ifstream &file)
{
    std::string str;
    char ch;

    while (file.tellg() > 0)
    {
        file.seekg(-1, std::ios::cur);
        file.get(ch);
        file.seekg(-1, std::ios::cur);

        if (ch == '.' || ch == '!')
            break;
        if (ch == '\n')
            if (str.size() == 0)
                continue;
            else
                break;
        if (ch == '?' && trimString(str).size() > 0)
            break;
        str += ch;
    }
    return trimString(reverseString(str));
}

int main() 
{
    std::cout << cp1251ToUtf8("Введите путь до входного файла: ");
    std::string fileName;
    std::cin >> fileName;
    std::ifstream file(fileName);
    if (file.is_open()) 
    {
        file.seekg(0, std::ios::end);

        std::string str = getLastSentence(file);
        if (getSubString(str, 0, 2) == "где" && str[str.size() - 1] == '?') 
            std::cout << cp1251ToUtf8("в Караганде" + getSubString(str, 3, str.size() - 2) + '!') << '\n';
        else
            std::cout << cp1251ToUtf8("Спасибо за информацию") << '\n';
    }
    else
        std::cout << cp1251ToUtf8("Не удалось открыть файл") << '\n';
}
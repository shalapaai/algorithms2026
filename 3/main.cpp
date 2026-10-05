/*
Условие:
    Общими требованиями к лабораторной работе являются:
    1) вводить   исходное   дерево  из  файла  в  понятной  для
    пользователя форме, а не с клавиатуры;
    2) по требованию прользователя показывать дерево на экране;
    3) обрабатывать  дерево  в  оперативной памяти,  а не путем
    многократного обращения к файлу;
    4) обеспечить   возможность   многократных   запросов   без
    повторного запуска программы.

    26. Информация  о  файлах  на  жестких  дисках   компьютера
    записана  с  помощью  дерева.  Обеспечить выполнение следующих
    операций:
    1) загрузку дерева в память из файла;
    2) обход дерева папок в  режиме  диалога  (раскрытие папок,
        подъем на уровень и т. п.); 
    3) корректировку  дерева при создании новых папок и файлов,
    их переименовании, копировании, переносе и удалении. 
    4) сохранение дерева в файле (13).

    Автор: Маймеску Максим

    Среда: Linux, Visual Studio Code

    Источники: 
    лекция
    https://labex.io/tutorials/cpp-how-to-use-stringstream-in-c-425236
*/

#include <iostream>
#include <string>
#include <sstream>
#include "fileManager.h"

void printHelp()
{
    std::cout << "\n=== Список доступных команд ===\n"
              << "  ls [path]                     - Вывести содержимое директории\n"
              << "  pwd                           - Показать текущий путь\n"
              << "  cd <path>                     - Перейти в директорию\n"
              << "  touch <path>                  - Создать файл\n"
              << "  mkdir <path>                  - Создать директорию\n"
              << "  rmfile <path>                 - Удалить файл\n"
              << "  rmdir <path>                  - Удалить директорию\n"
              << "  mvfile <src> <target_dir>     - Переместить файл\n"
              << "  mvdir <src> <target_dir>      - Переместить директорию\n"
              << "  cpfile <src> <target_dir>     - Скопировать файл\n"
              << "  cpdir <src> <target_dir>      - Скопировать директорию\n"
              << "  save <filename>               - Сохранить дерево в файл\n"
              << "  load <filename>               - Загрузить дерево из файла\n"
              << "  help                          - Показать эту справку\n"
              << "  exit                          - Выйти из программы\n"
              << "===============================\n\n";
}

int main() 
{
    FileManager fm;
    fm.initFileManager("root");

    std::cout << "Файловый менеджер запущен. Введите 'help' для просмотра списка команд.\n";

    std::string line;
    while (true)
    {
        std::cout << fm.getCurrentDirectoryPath() << "> ";

        if (!std::getline(std::cin, line)) 
            break; 

        if (line.empty()) 
            continue;

        std::stringstream ss(line);
        std::string cmd;
        ss >> cmd;

        if (cmd == "exit")
        {
            std::cout << "Завершение работы.\n";
            break;
        }
        else if (cmd == "help")
        {
            printHelp();
        }
        else if (cmd == "pwd")
        {
            std::cout << fm.getCurrentDirectoryPath() << '\n';
        }
        else if (cmd == "ls")
        {
            std::string path = "";
            ss >> path; 
            fm.printChildNodes(path);
        }
        else if (cmd == "cd")
        {
            std::string path;
            if (ss >> path)
                fm.goToDirectory(path);
            else
                std::cout << "Ошибка: укажите путь для cd.\n";
        }
        else if (cmd == "touch")
        {
            std::string path;
            if (ss >> path)
                fm.addFile(path);
            else
                std::cout << "Ошибка: укажите путь/имя файла.\n";
        }
        else if (cmd == "mkdir")
        {
            std::string path;
            if (ss >> path)
                fm.addDirectory(path);
            else
                std::cout << "Ошибка: укажите путь/имя директории.\n";
        }
        else if (cmd == "rmfile")
        {
            std::string path;
            if (ss >> path)
                fm.deleteFile(path);
            else
                std::cout << "Ошибка: укажите путь/имя файла.\n";
        }
        else if (cmd == "rmdir")
        {
            std::string path;
            if (ss >> path)
                fm.deleteDirectory(path);
            else
                std::cout << "Ошибка: укажите путь/имя директории.\n";
        }
        else if (cmd == "mvfile")
        {
            std::string src, target;
            if (ss >> src >> target)
                fm.moveFile(src, target);
            else
                std::cout << "Ошибка: использование: mvfile <src> <target_dir>\n";
        }
        else if (cmd == "mvdir")
        {
            std::string src, target;
            if (ss >> src >> target)
                fm.moveDirectory(src, target);
            else
                std::cout << "Ошибка: использование: mvdir <src> <target_dir>\n";
        }
        else if (cmd == "cpfile")
        {
            std::string src, target;
            if (ss >> src >> target)
                fm.copyFile(src, target);
            else
                std::cout << "Ошибка: использование: cpfile <src> <target_dir>\n";
        }
        else if (cmd == "cpdir")
        {
            std::string src, target;
            if (ss >> src >> target)
                fm.copyDirectory(src, target);
            else
                std::cout << "Ошибка: использование: cpdir <src> <target_dir>\n";
        }
        else if (cmd == "save")
        {
            std::string filename;
            if (ss >> filename)
                fm.saveToFile(filename);
            else
                std::cout << "Ошибка: укажите имя файла для сохранения.\n";
        }
        else if (cmd == "load")
        {
            std::string filename;
            if (ss >> filename)
                fm.loadFromFile(filename);
            else
                std::cout << "Ошибка: укажите имя файла для загрузки.\n";
        }
        else
        {
            std::cout << "Неизвестная команда '" << cmd << "'. Введите 'help' для списка команд.\n";
        }
    }

    return 0;
}
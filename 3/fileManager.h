#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include <string>
#include <vector>
#include <iostream>
#include <fstream>

class FileManager
{
public:
    void initFileManager(const std::string& rootName);

    void printChildNodes(const std::string& path = "");

    void addFile(const std::string& path);
    void addDirectory(const std::string& path);

    void renameFile(const std::string& path, const std::string& newName);
    void renameDirectory(const std::string& path, const std::string& newName);

    void deleteFile(const std::string& path);
    void deleteDirectory(const std::string& path);

    void moveFile(const std::string& sourcePath, const std::string& targetDirectoryPath);
    void moveDirectory(const std::string& sourcePath, const std::string& targetDirectoryPath);

    void copyFile(const std::string& sourcePath, const std::string& targetDirectoryPath);
    void copyDirectory(const std::string& sourcePath, const std::string& targetDirectoryPath);

    std::string getCurrentDirectoryPath();
    void goToDirectory(const std::string& path);

    bool saveToFile(const std::string& filename);
    bool loadFromFile(const std::string& filename);

private:
    enum NodeType {
        File,
        Directory
    };

    struct Node {
        std::string name;
        NodeType type;

        Node* parent;
        Node* firstChild;
        Node* nextSibling;
    };

    Node* root = nullptr;
    Node* currentDirectory = nullptr;

    std::vector<std::string> splitPath(const std::string& path);
    Node* findDirectoryByPath(const std::string& path);
    
    void freeMemory(Node* node);

    Node* findChild(const std::string& name, NodeType type, Node* parent = nullptr);
    Node* getParentDirectoryAndName(const std::string& path, std::string& targetName);
    void attachChild(Node* parent, Node* child);

    void addNode(const std::string& path, NodeType type);
    void deleteNode(const std::string& path, NodeType type);

    void renameNode(const std::string& path, const std::string& newName, NodeType type);

    void detachChild(Node* parent, Node* child);
    Node* cloneTree(Node* sourceNode, Node* newParent);
    bool isSubdirectory(Node* potentialParent, Node* node);

    void moveNode(const std::string& sourcePath, const std::string& targetDirectoryPath, NodeType type);
    void copyNode(const std::string& sourcePath, const std::string& targetDirectoryPath, NodeType type);

    void saveNodeRecursive(Node* node, std::ofstream& outFile, int depth);
    void freeSubtree(Node* node);
};

#endif
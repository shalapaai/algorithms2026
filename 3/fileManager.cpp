#include "fileManager.h"

void FileManager::initFileManager(const std::string& rootName)
{
    if (root != nullptr)
        freeMemory(root);

    root = new Node{rootName, Directory, nullptr, nullptr, nullptr};
    currentDirectory = root;
}

void FileManager::printChildNodes(const std::string& path)
{
    Node* targetDir = path.empty() ? currentDirectory : findDirectoryByPath(path);

    if (targetDir == nullptr)
    {
        std::cout << "Error: Directory '" << path << "' not found.\n";
        return;
    }

    Node* temp = targetDir->firstChild;
    while (temp != nullptr)
    {
        std::cout << temp->name << (temp->type == Directory ? "/" : "") << '\n';
        temp = temp->nextSibling;
    }
}

void FileManager::addFile(const std::string& path)
{
    addNode(path, File);
}

void FileManager::addDirectory(const std::string& path)
{
    addNode(path, Directory);
}

void FileManager::renameFile(const std::string& path, const std::string& newName)
{
    renameNode(path, newName, File);
}

void FileManager::renameDirectory(const std::string& path, const std::string& newName)
{
    renameNode(path, newName, Directory);
}

void FileManager::deleteFile(const std::string& path)
{
    deleteNode(path, File);
}

void FileManager::deleteDirectory(const std::string& path)
{
    deleteNode(path, Directory);
}

void FileManager::moveFile(const std::string& sourcePath, const std::string& targetDirectoryPath)
{
    moveNode(sourcePath, targetDirectoryPath, File);
}

void FileManager::moveDirectory(const std::string& sourcePath, const std::string& targetDirectoryPath)
{
    moveNode(sourcePath, targetDirectoryPath, Directory);
}

void FileManager::copyFile(const std::string& sourcePath, const std::string& targetDirectoryPath)
{
    copyNode(sourcePath, targetDirectoryPath, File);
}

void FileManager::copyDirectory(const std::string& sourcePath, const std::string& targetDirectoryPath)
{
    copyNode(sourcePath, targetDirectoryPath, Directory);
}

std::string FileManager::getCurrentDirectoryPath()
{
    if (currentDirectory == nullptr) 
        return "";

    std::string fullPath = "";
    Node* nav = currentDirectory;

    while (nav != nullptr)
    {
        if (fullPath.empty())
            fullPath = nav->name;
        else
            fullPath = nav->name + "/" + fullPath;
        nav = nav->parent;
    }

    return fullPath;
}

void FileManager::goToDirectory(const std::string& path)
{
    if (path.empty()) 
        return;

    Node* targetDir = findDirectoryByPath(path);

    if (targetDir != nullptr)
        currentDirectory = targetDir;
    else
        std::cout << "Error: Directory '" << path << "' not found.\n";
}

void FileManager::copyFileToBuf(const std::string& path) {
    copyNodeToBuf(path, File);
}

void FileManager::copyDirToBuf(const std::string& path) {
    copyNodeToBuf(path, Directory);
}

void FileManager::cutFileToBuf(const std::string& path)
{
    cutNodeToBuf(path, File);
}

void FileManager::cutDirToBuf(const std::string& path)
{
    cutNodeToBuf(path, Directory);
}

void FileManager::pasteFileToBuf(const std::string& targetPath) {
    pasteNodeToBuf(targetPath, File);
}

void FileManager::pasteDirToBuf(const std::string& targetPath) {
    pasteNodeToBuf(targetPath, Directory);
}

bool FileManager::saveToFile(const std::string& filename)
{
    if (root == nullptr) 
        return false;

    std::ofstream outFile(filename);
    if (!outFile.is_open())
    {
        std::cout << "Error: Unable to open file for writing: " << filename << "\n";
        return false;
    }
    saveNodeRecursive(root, outFile, 0);

    outFile.close();
    std::cout << "Tree successfully saved to '" << filename << "'.\n";
    return true;
}

bool FileManager::loadFromFile(const std::string& filename)
{
    std::ifstream inFile(filename);
    if (!inFile.is_open())
    {
        std::cout << "Error: Unable to open file for reading: " << filename << "\n";
        return false;
    }

    std::string line;
    std::vector<Node*> stack; 

    if (root != nullptr)
    {
        freeSubtree(root);
        root = nullptr;
        currentDirectory = nullptr;
    }

    while (std::getline(inFile, line))
    {
        if (line.empty()) 
            continue;

        int depth = 0;
        while (depth < line.length() && line[depth] == '.')
            depth++;

        std::string name = line.substr(depth);
        if (name.empty()) 
            continue;

        NodeType type = File;
        if (name.back() == '/')
        {
            type = Directory;
            name.pop_back(); 
        }

        Node* newNode = new Node{name, type, nullptr, nullptr, nullptr};

        if (depth == 0)
        {
            root = newNode;
            currentDirectory = root;
            stack.clear();
            stack.push_back(root);
        }
        else
        {
            while (stack.size() > depth)
                stack.pop_back();

            if (!stack.empty())
            {
                Node* parentNode = stack.back();
                attachChild(parentNode, newNode);
            }

            if (type == Directory)
                stack.push_back(newNode);
        }
    }

    inFile.close();
    std::cout << "Tree successfully loaded from '" << filename << "'.\n";
    return true;
}

// private

std::vector<std::string> FileManager::splitPath(const std::string& path)
{
    std::vector<std::string> tokens;
    std::string token = "";
    for (char ch : path)
    {
        if (ch == '/')
        {
            if (!token.empty())
            {
                tokens.push_back(token);
                token.clear();
            }
        }
        else
            token += ch; 
    }
    if (!token.empty())
        tokens.push_back(token);
    return tokens;
}

FileManager::Node* FileManager::findDirectoryByPath(const std::string& path)
{
    if (path.empty()) 
        return currentDirectory;

    std::vector<std::string> tokens = splitPath(path);
    if (tokens.empty()) 
        return currentDirectory;

    Node* nav = currentDirectory;
    int startIndex = 0;

    if (tokens[0] == root->name)
    {
        nav = root;
        startIndex = 1; 
    }

    for (int i = startIndex; i < tokens.size(); ++i)
    {
        const std::string& dirName = tokens[i];

        if (dirName == "..") 
        {
            if (nav->parent == nullptr) 
                return nullptr; 
            nav = nav->parent;
        }
        else if (dirName == ".")
            continue; 
        else
        {
            Node* child = findChild(dirName, Directory, nav);
            if (child == nullptr)
                return nullptr;

            nav = child;
        }
    }

    return nav;
}

void FileManager::freeMemory(Node* node)
{
    if (node == nullptr) return;
    freeMemory(node->firstChild);
    freeMemory(node->nextSibling);
    delete node;
}

FileManager::Node* FileManager::findChild(const std::string& name, NodeType type, Node* parent)
{
    Node* dir = (parent != nullptr) ? parent : currentDirectory;
    Node* temp = dir->firstChild;

    while (temp != nullptr)
    {
        if (temp->name == name && temp->type == type)
            return temp;
        temp = temp->nextSibling;
    }
    return nullptr;
}

FileManager::Node* FileManager::getParentDirectoryAndName(const std::string& path, std::string& targetName)
{
    if (path.empty()) 
        return nullptr;

    std::vector<std::string> tokens = splitPath(path);
    if (tokens.empty()) 
        return nullptr;

    targetName = tokens.back();
    tokens.pop_back();

    if (tokens.empty()) 
        return currentDirectory;

    std::string parentPath = "";
    for (int i = 0; i < tokens.size(); ++i)
        parentPath += tokens[i] + (i + 1 < tokens.size() ? "/" : "");

    return findDirectoryByPath(parentPath);
}

void FileManager::attachChild(Node* parent, Node* child)
{
    child->parent = parent;
    child->nextSibling = nullptr;

    if (parent->firstChild == nullptr)
        parent->firstChild = child;
    else 
    {
        Node* temp = parent->firstChild;
        while (temp->nextSibling != nullptr)
            temp = temp->nextSibling;
        temp->nextSibling = child;
    }
}

void FileManager::addNode(const std::string& path, NodeType type)
{
    std::string newName;
    Node* parentDir = getParentDirectoryAndName(path, newName);

    if (parentDir == nullptr)
    {
        std::cout << "Error: Target directory for '" << path << "' does not exist.\n";
        return;
    }

    if (findChild(newName, type, parentDir) != nullptr)
    {
        std::cout << "Error: " << (type == Directory ? "Directory" : "File") 
            << " '" << newName << "' already exists in target path.\n";
        return;
    }

    Node* newNode = new Node{newName, type, parentDir, nullptr, nullptr};
    attachChild(parentDir, newNode);
}

void FileManager::renameNode(const std::string& path, const std::string& newName, NodeType type)
{
    if (newName.empty())
    {
        std::cout << "Error: New name cannot be empty.\n";
        return;
    }
    if (newName.find('/') != std::string::npos)
    {
        std::cout << "Error: New name cannot contain '/'.\n";
        return;
    }

    std::string oldName;
    Node* parentDir = getParentDirectoryAndName(path, oldName);

    if (parentDir == nullptr)
    {
        std::cout << "Error: Target directory for '" << path << "' does not exist.\n";
        return;
    }

    if (parentDir == root && oldName == root->name && type == Directory)
    {
        std::cout << "Error: Cannot rename root directory.\n";
        return;
    }

    Node* targetNode = findChild(oldName, type, parentDir);
    if (targetNode == nullptr)
    {
        std::cout << "Error: " << (type == Directory ? "Directory" : "File") 
            << " '" << oldName << "' not found.\n";
        return;
    }

    if (findChild(newName, type, parentDir) != nullptr)
    {
        std::cout << "Error: " << (type == Directory ? "Directory" : "File") 
            << " with name '" << newName << "' already exists in target path.\n";
        return;
    }

    targetNode->name = newName;
}

void FileManager::deleteNode(const std::string& path, NodeType type)
{
    std::string targetName;
    Node* parentDir = getParentDirectoryAndName(path, targetName);

    if (parentDir == nullptr)
    {
        std::cout << "Error: Target directory for '" << path << "' does not exist.\n";
        return;
    }

    if (parentDir == root && targetName == root->name && type == Directory)
    {
        std::cout << "Error: You cannot delete root directory.\n";
        return;
    }

    Node* temp = parentDir->firstChild;
    Node* prev = nullptr;

    while (temp != nullptr)
    {
        if (temp->name == targetName && temp->type == type)
        {
            if (prev == nullptr) 
                parentDir->firstChild = temp->nextSibling;
            else 
                prev->nextSibling = temp->nextSibling;

            temp->nextSibling = nullptr; 
            freeMemory(temp);
            return;
        }

        prev = temp;            
        temp = temp->nextSibling;
    }

    std::cout << "Error: " << (type == Directory ? "Directory" : "File") 
        << " '" << targetName << "' does not exist in target path.\n";
}

void FileManager::detachChild(Node* parent, Node* child)
{
    if (parent == nullptr || child == nullptr) 
        return;

    Node* temp = parent->firstChild;
    Node* prev = nullptr;

    while (temp != nullptr)
    {
        if (temp == child)
        {
            if (prev == nullptr)
                parent->firstChild = temp->nextSibling;
            else
                prev->nextSibling = temp->nextSibling;

            temp->nextSibling = nullptr;
            return;
        }
        prev = temp;
        temp = temp->nextSibling;
    }
}

FileManager::Node* FileManager::cloneTree(Node* sourceNode, Node* newParent)
{
    if (sourceNode == nullptr) 
        return nullptr;

    Node* newNode = new Node{sourceNode->name, sourceNode->type, newParent, nullptr, nullptr};

    Node* child = sourceNode->firstChild;
    while (child != nullptr)
    {
        Node* clonedChild = cloneTree(child, newNode);
        attachChild(newNode, clonedChild);
        child = child->nextSibling;
    }

    return newNode;
}

bool FileManager::isSubdirectory(Node* potentialParent, Node* node)
{
    Node* nav = potentialParent;
    while (nav != nullptr)
    {
        if (nav == node) return true;
        nav = nav->parent;
    }
    return false;
}

void FileManager::moveNode(const std::string& sourcePath, const std::string& targetDirectoryPath, NodeType type)
{
    std::string sourceName;
    Node* sourceParent = getParentDirectoryAndName(sourcePath, sourceName);

    if (sourceParent == nullptr)
    {
        std::cout << "Error: Source path directory for '" << sourcePath << "' does not exist.\n";
        return;
    }

    Node* sourceNode = findChild(sourceName, type, sourceParent);
    if (sourceNode == nullptr)
    {
        std::cout << "Error: " << (type == Directory ? "Directory" : "File") 
                  << " '" << sourceName << "' not found.\n";
        return;
    }

    Node* targetDir = targetDirectoryPath.empty() ? currentDirectory : findDirectoryByPath(targetDirectoryPath);
    if (targetDir == nullptr)
    {
        std::cout << "Error: Target directory '" << targetDirectoryPath << "' does not exist.\n";
        return;
    }

    if (findChild(sourceName, type, targetDir) != nullptr)
    {
        std::cout << "Error: " << (type == Directory ? "Directory" : "File") 
                  << " '" << sourceName << "' already exists in target directory.\n";
        return;
    }

    if (type == Directory && isSubdirectory(targetDir, sourceNode))
    {
        std::cout << "Error: Cannot move directory '" << sourceName << "' into itself or its subdirectory.\n";
        return;
    }

    if (sourceParent == targetDir) return;

    detachChild(sourceParent, sourceNode);
    attachChild(targetDir, sourceNode);
}

void FileManager::copyNode(const std::string& sourcePath, const std::string& targetDirectoryPath, NodeType type)
{
    std::string sourceName;
    Node* sourceParent = getParentDirectoryAndName(sourcePath, sourceName);

    if (sourceParent == nullptr)
    {
        std::cout << "Error: Source path directory for '" << sourcePath << "' does not exist.\n";
        return;
    }

    Node* sourceNode = findChild(sourceName, type, sourceParent);
    if (sourceNode == nullptr)
    {
        std::cout << "Error: " << (type == Directory ? "Directory" : "File") 
                  << " '" << sourceName << "' not found.\n";
        return;
    }

    Node* targetDir = targetDirectoryPath.empty() ? currentDirectory : findDirectoryByPath(targetDirectoryPath);
    if (targetDir == nullptr)
    {
        std::cout << "Error: Target directory '" << targetDirectoryPath << "' does not exist.\n";
        return;
    }

    if (findChild(sourceName, type, targetDir) != nullptr)
    {
        std::cout << "Error: " << (type == Directory ? "Directory" : "File") 
                  << " '" << sourceName << "' already exists in target directory.\n";
        return;
    }

    if (type == Directory && isSubdirectory(targetDir, sourceNode))
    {
        std::cout << "Error: Cannot copy directory '" << sourceName << "' into itself or its subdirectory.\n";
        return;
    }

    Node* clonedNode = cloneTree(sourceNode, targetDir);
    attachChild(targetDir, clonedNode);
}

void FileManager::saveNodeRecursive(Node* node, std::ofstream& outFile, int depth)
{
    if (node == nullptr) 
        return;
    for (int i = 0; i < depth; ++i)
        outFile << ".";

    outFile << node->name << (node->type == Directory ? "/" : "") << "\n";

    saveNodeRecursive(node->firstChild, outFile, depth + 1);
    saveNodeRecursive(node->nextSibling, outFile, depth);
}

void FileManager::freeSubtree(Node* node)
{
    if (node == nullptr) 
        return;
    freeSubtree(node->firstChild);
    freeSubtree(node->nextSibling);
    delete node;
}

// buf

void FileManager::clearClipboard() {
    if (!isClipboardEmpty && clipboardNode != nullptr) {
        freeSubtree(clipboardNode);
        clipboardNode = nullptr;
        isClipboardEmpty = true;
    }
}

void FileManager::copyNodeToBuf(const std::string& path, NodeType type) {
    std::string sourceName;
    Node* sourceParent = getParentDirectoryAndName(path, sourceName);

    if (sourceParent == nullptr) {
        std::cout << "Error: Directory '" << path << "' does not exist.\n";
        return;
    }

    Node* sourceNode = findChild(sourceName, type, sourceParent);
    if (sourceNode == nullptr) {
        std::cout << sourceName << " does not exist.\n";
        return;
    }

    clearClipboard(); 
    clipboardNode = cloneTree(sourceNode, nullptr); 
    isClipboardEmpty = false;
}

void FileManager::pasteNodeToBuf(const std::string& targetPath, NodeType type) {
    if (isClipboardEmpty || clipboardNode == nullptr) {
        std::cout << "Error: Buffer is empty.\n";
        return;
    }

    if (clipboardNode->type != type) {
        std::cout << "Error: Wrong NodeType in buffer\n";
        return;
    }

    Node* targetDir = targetPath.empty() ? currentDirectory : findDirectoryByPath(targetPath);
    if (targetDir == nullptr) {
        std::cout << "Err: Directory '" << targetPath << "' does not exist.\n";
        return;
    }

    if (findChild(clipboardNode->name, type, targetDir) != nullptr) {
        std::cout << "Error: '" << clipboardNode->name << "' already exists.\n";
        return;
    }

    Node* pastedNode = cloneTree(clipboardNode, targetDir);
    attachChild(targetDir, pastedNode);
}

void FileManager::cutNodeToBuf(const std::string& path, NodeType type)
{
    std::string sourceName;
    Node* sourceParent = getParentDirectoryAndName(path, sourceName);

    if (sourceParent == nullptr) {
        std::cout << "Error: Directory '" << path << "' does not exist.\n";
        return;
    }

    Node* sourceNode = findChild(sourceName, type, sourceParent);
    if (sourceNode == nullptr) {
        std::cout << sourceName << " does not exist.\n";
        return;
    }

    clearClipboard(); 
    clipboardNode = cloneTree(sourceNode, nullptr); 
    isClipboardEmpty = false;

    detachChild(sourceParent, sourceNode);
    freeMemory(sourceNode);
}
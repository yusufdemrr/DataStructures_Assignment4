#ifndef SPACESECTORLLRBT_H
#define SPACESECTORLLRBT_H

#include "Sector.h"
#include <iostream>
#include <fstream>  
#include <sstream>
#include <vector>
#include <unordered_map>
#include <stack>

class SpaceSectorLLRBT {
public:
    Sector* root;
    SpaceSectorLLRBT();
    ~SpaceSectorLLRBT();
    void readSectorsFromFile(const std::string& filename);
    void insertSectorByCoordinates(int x, int y, int z);
    void displaySectorsInOrder();
    void displaySectorsPreOrder();
    void displaySectorsPostOrder();
    std::vector<Sector*> getStellarPath(const std::string& sector_code);
    void printStellarPath(const std::vector<Sector*>& path);

    Sector *insertRecursive(Sector *node, Sector *parent, int x, int y, int z);

    Sector *rotateRight(Sector *node);

    Sector *rotateLeft(Sector *node);

    void flipColors(Sector *node);

    bool isRed(Sector *node);

    void inOrderTraversal(Sector *node);

    void preOrderTraversal(Sector *node);

    void postOrderTraversal(Sector *node);

    void printSector(Sector *node);

//    bool DFS(Sector *start, Sector *target, std::unordered_map<Sector *, bool>& visited,
//             std::unordered_map<Sector *, Sector *>& parent);

    Sector *findSector(Sector *node, const std::string &sector_code);

    void findPathToRoot(Sector *root, Sector *target, std::stack<Sector *> &path);

    void deleteTree(Sector *node);

    bool getPath(Sector *root, std::vector<std::string> &path, std::string x);
};

#endif // SPACESECTORLLRBT_H

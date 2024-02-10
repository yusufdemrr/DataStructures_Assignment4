#ifndef SPACESECTORBST_H
#define SPACESECTORBST_H

#include <iostream>
#include <fstream>  
#include <sstream>
#include <vector>

#include "Sector.h"

class SpaceSectorBST {
  
public:
    Sector *root;
    SpaceSectorBST();
    ~SpaceSectorBST();
    void readSectorsFromFile(const std::string& filename); 
    void insertSectorByCoordinates(int x, int y, int z);
    void deleteSector(const std::string& sector_code);
    void displaySectorsInOrder();
    void displaySectorsPreOrder();
    void displaySectorsPostOrder();
    std::vector<Sector*> getStellarPath(const std::string& sector_code);
    void printStellarPath(const std::vector<Sector*>& path);

    Sector *insertRecursive(Sector *node, Sector *parent, int x, int y, int z);

    void inorderTraversal(Sector *node);

    void preorderTraversal(Sector *node);

    void postorderTraversal(Sector *node);

    Sector *findSector(Sector *node, const std::string &sector_code);

    void deleteTree(Sector *node);

    Sector *findSectorByCode(const std::string &sector_code) const;

    Sector *findSectorByCodeBFS(Sector *startNode, const std::string &sector_code) const;

    void deleteLeafNode(Sector *nodeToDelete);

    void deleteNodeWithOneChild(Sector *nodeToDelete);

    void deleteNodeWithTwoChildren(Sector *nodeToDelete);

    Sector *findMinNode(Sector *startNode) const;
};

#endif // SPACESECTORBST_H

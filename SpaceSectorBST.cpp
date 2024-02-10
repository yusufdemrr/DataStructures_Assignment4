#include <algorithm>
#include <queue>
#include "SpaceSectorBST.h"

using namespace std;

SpaceSectorBST::SpaceSectorBST() : root(nullptr) {}

SpaceSectorBST::~SpaceSectorBST() {
    deleteTree(root);
}

void SpaceSectorBST::deleteTree(Sector* node) {
    if (node == nullptr) {
        return;
    }

    // Delete the left subtree first, then the right subtree (postorder traversal)
    deleteTree(node->left);
    deleteTree(node->right);

    delete node;
}


void SpaceSectorBST::readSectorsFromFile(const std::string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Unable to open the file: " << filename << endl;
        return;
    }

    string line;
    getline(file, line); // Ignore the first line as it contains headers
    while (getline(file, line)) {
        stringstream ss(line);
        string coordinate;
        vector<int> coordinates;

        while (getline(ss, coordinate, ',')) {
            coordinates.push_back(stoi(coordinate));
        }

        if (coordinates.size() == 3) {
            insertSectorByCoordinates(coordinates[0], coordinates[1], coordinates[2]);
        } else {
            cerr << "Invalid line format: " << line << endl;
        }
    }

    file.close();
}


void SpaceSectorBST::insertSectorByCoordinates(int x, int y, int z) {
    root = insertRecursive(root, nullptr, x, y, z);
}

Sector* SpaceSectorBST::insertRecursive(Sector* node, Sector* parent, int x, int y, int z) {
    if (node == nullptr) {
        Sector* new_node = new Sector(x, y, z);
        new_node->parent = parent; // Set parent node
        return new_node;
    }

    if (x < node->x || (x == node->x && y < node->y) || (x == node->x && y == node->y && z < node->z)) {
        node->left = insertRecursive(node->left, node, x, y, z);
    } else if (x > node->x || (x == node->x && y > node->y) || (x == node->x && y == node->y && z > node->z)) {
        node->right = insertRecursive(node->right, node, x, y, z);
    }

    return node;
}


void SpaceSectorBST::deleteSector(const std::string& sector_code) {
    Sector* nodeToDelete = findSectorByCode(sector_code);

    if (nodeToDelete == nullptr) {
        std::cerr << "Error: Sector with code " << sector_code << " not found." << std::endl;
        return;
    }

    // Case 1: Node with no children (leaf node)
    if (nodeToDelete->left == nullptr && nodeToDelete->right == nullptr) {
        deleteLeafNode(nodeToDelete);
    }
        // Case 2: Node with one child
    else if (nodeToDelete->left == nullptr || nodeToDelete->right == nullptr) {
        deleteNodeWithOneChild(nodeToDelete);
    }
        // Case 3: Node with two children
    else {
        deleteNodeWithTwoChildren(nodeToDelete);
    }
}

void SpaceSectorBST::deleteLeafNode(Sector* nodeToDelete) {
    if (nodeToDelete->parent == nullptr) {
        root = nullptr;
    } else {
        if (nodeToDelete == nodeToDelete->parent->left) {
            nodeToDelete->parent->left = nullptr;
        } else {
            nodeToDelete->parent->right = nullptr;
        }
    }

    delete nodeToDelete;
}

void SpaceSectorBST::deleteNodeWithOneChild(Sector* nodeToDelete) {
    Sector* child = (nodeToDelete->left != nullptr) ? nodeToDelete->left : nodeToDelete->right;

    if (nodeToDelete->parent == nullptr) {
        root = child;
    } else {
        if (nodeToDelete == nodeToDelete->parent->left) {
            nodeToDelete->parent->left = child;
        } else {
            nodeToDelete->parent->right = child;
        }
    }

    child->parent = nodeToDelete->parent;

    delete nodeToDelete;
}

void SpaceSectorBST::deleteNodeWithTwoChildren(Sector* nodeToDelete) {
    Sector* successor = findMinNode(nodeToDelete->right);

    std::swap(nodeToDelete->x, successor->x);
    std::swap(nodeToDelete->y, successor->y);
    std::swap(nodeToDelete->z, successor->z);
    std::swap(nodeToDelete->distance_from_earth, successor->distance_from_earth);
    std::swap(nodeToDelete->sector_code, successor->sector_code);

    if (successor->left != nullptr || successor->right != nullptr) {
        deleteNodeWithOneChild(successor);
    } else {
        deleteLeafNode(successor);
    }
}

Sector* SpaceSectorBST::findMinNode(Sector* startNode) const {
    Sector* current = startNode;
    while (current->left != nullptr) {
        current = current->left;
    }
    return current;
}



void SpaceSectorBST::displaySectorsInOrder() {
    std::cout << "Space sectors inorder traversal:" << std::endl;
    inorderTraversal(root);
    std::cout << std::endl;
}

void SpaceSectorBST::displaySectorsPreOrder() {
    std::cout << "Space sectors preorder traversal:" << std::endl;
    preorderTraversal(root);
    std::cout << std::endl;
}

void SpaceSectorBST::displaySectorsPostOrder() {
    std::cout << "Space sectors postorder traversal:" << std::endl;
    postorderTraversal(root);
    std::cout << std::endl;
}

void SpaceSectorBST::inorderTraversal(Sector* node) {
    if (node != nullptr) {
        inorderTraversal(node->left);
        std::cout << node->sector_code << std::endl;
        inorderTraversal(node->right);
    }
}

void SpaceSectorBST::preorderTraversal(Sector* node) {
    if (node != nullptr) {
        std::cout << node->sector_code << std::endl;
        preorderTraversal(node->left);
        preorderTraversal(node->right);
    }
}

void SpaceSectorBST::postorderTraversal(Sector* node) {
    if (node != nullptr) {
        postorderTraversal(node->left);
        postorderTraversal(node->right);
        std::cout << node->sector_code << std::endl;
    }
}


std::vector<Sector*> SpaceSectorBST::getStellarPath(const std::string& sector_code) {
    std::vector<Sector*> path;
    Sector* destination = findSector(root, sector_code); // Find target sector

    if (destination == nullptr) {
        return path;
    }

    // Move backwards to find the path to travel and put it on the stack
    Sector* current = destination;
    while (current != nullptr) {
        path.push_back(current);
        current = current->parent;
    }

    std::reverse(path.begin(), path.end()); // Get the path in the correct order (Earth to target)

    return path;
}

Sector* SpaceSectorBST::findSector(Sector* node, const std::string& sector_code) {
    if (node == nullptr) {
        return nullptr;
    }

    // If the sector code searched is the sector code of the current node, it is found.
    if (node->sector_code == sector_code) {
        return node;
    }

    // If the sector code being searched is less than the sector code of the current node,
    // the left subtree is looked at. Otherwise, the right subtree is checked.
    Sector* left_search = findSector(node->left, sector_code);
    if (left_search != nullptr) {
        return left_search;
    }

    Sector* right_search = findSector(node->right, sector_code);
    return right_search;
}

Sector* SpaceSectorBST::findSectorByCode(const std::string& sector_code) const {
    return findSectorByCodeBFS(root, sector_code);
}

Sector* SpaceSectorBST::findSectorByCodeBFS(Sector* startNode, const std::string& sector_code) const {
    if (startNode == nullptr) {
        return nullptr;
    }

    std::queue<Sector*> q;
    q.push(startNode);

    while (!q.empty()) {
        Sector* currentNode = q.front();
        q.pop();

        if (currentNode->sector_code == sector_code) {
            return currentNode;
        }

        if (currentNode->left != nullptr) {
            q.push(currentNode->left);
        }

        if (currentNode->right != nullptr) {
            q.push(currentNode->right);
        }
    }

    return nullptr;  // Sector with the given code not found
}



void SpaceSectorBST::printStellarPath(const std::vector<Sector*>& path) {
    if (path.empty()) {
        std::cout << "A path to Dr. Elara could not be found." << std::endl;
        return;
    }

    std::cout << "The stellar path to Dr. Elara: ";
    for (size_t i = 0; i < path.size(); ++i) {
        std::cout << path[i]->sector_code;
        if (i != path.size() - 1) {
            std::cout << "->";
        }
    }
    std::cout << std::endl;
}

#include <algorithm>
#include <stack>
#include <unordered_map>
#include <queue>
#include "SpaceSectorLLRBT.h"

using namespace std;

SpaceSectorLLRBT::SpaceSectorLLRBT() : root(nullptr) {}

void SpaceSectorLLRBT::readSectorsFromFile(const std::string& filename) {
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


SpaceSectorLLRBT::~SpaceSectorLLRBT() {
    // Root düğümünden başlayarak tüm düğümleri sil
    deleteTree(root);
}

void SpaceSectorLLRBT::deleteTree(Sector* node) {
    if (node == nullptr) {
        return;
    }

    // Önce sol alt ağacı, sonra sağ alt ağacı sil (postorder traversal)
    deleteTree(node->left);
    deleteTree(node->right);

    // Düğümü sil
    delete node;
}


void SpaceSectorLLRBT::insertSectorByCoordinates(int x, int y, int z) {
    root = insertRecursive(root, nullptr, x, y, z);
    root->color = false; // Yeni kök düğümü siyah yap
}

Sector* SpaceSectorLLRBT::insertRecursive(Sector* node, Sector* parent, int x, int y, int z) {
    if (node == nullptr) {
        Sector* new_node = new Sector(x, y, z);
        new_node->parent = parent; // Ebeveyn düğümü ayarla
        new_node->color = true;
        return new_node;
    }

    // LLRBT'ye göre düğümleri ekleme
    if (x < node->x || (x == node->x && y < node->y) || (x == node->x && y == node->y && z < node->z)) {
        node->left = insertRecursive(node->left, node, x, y, z);
    } else if (x > node->x || (x == node->x && y > node->y) || (x == node->x && y == node->y && z > node->z)) {
        node->right = insertRecursive(node->right, node, x, y, z);
    }

    // LLRBT özelliklerini kontrol et ve düzenle
    if (isRed(node->right) && !isRed(node->left)) {
        node = rotateLeft(node);
    }
    if (isRed(node->left) && isRed(node->left->left)) {
        node = rotateRight(node);
    }
    if (isRed(node->left) && isRed(node->right)) {
        flipColors(node);
    }

    return node;
}

Sector* SpaceSectorLLRBT::rotateRight(Sector* node) {
    Sector* left_child = node->left;
    node->left = left_child->right;
    if (left_child->right != nullptr) {
        left_child->right->parent = node;
    }
    left_child->right = node;
    left_child->parent = node->parent;
    node->parent = left_child;
    left_child->color = node->color;
    node->color = true; // LLRBT'de rotasyon sonrası renk değişimi

    return left_child;
}

Sector* SpaceSectorLLRBT::rotateLeft(Sector* node) {
    Sector* right_child = node->right;
    node->right = right_child->left;
    if (right_child->left != nullptr) {
        right_child->left->parent = node;
    }
    right_child->left = node;
    right_child->parent = node->parent;
    node->parent = right_child;
    right_child->color = node->color;
    node->color = true; // LLRBT'de rotasyon sonrası renk değişimi

    return right_child;
}


void SpaceSectorLLRBT::flipColors(Sector* node) {
    if (node != nullptr && node->left != nullptr && node->right != nullptr) {
        node->color = !node->color;
        node->left->color = !node->left->color;
        node->right->color = !node->right->color;
    }
}


bool SpaceSectorLLRBT::isRed(Sector* node) {
    if (node == nullptr) {
        return false;
    }
    return node->color;
}


void SpaceSectorLLRBT::displaySectorsInOrder() {
    std::cout << "Space sectors inorder traversal:" << std::endl;
    inOrderTraversal(root);
    std::cout << std::endl;
}

void SpaceSectorLLRBT::displaySectorsPreOrder() {
    std::cout << "Space sectors preorder traversal:" << std::endl;
    preOrderTraversal(root);
    std::cout << std::endl;
}

void SpaceSectorLLRBT::displaySectorsPostOrder() {
    std::cout << "Space sectors postorder traversal:" << std::endl;
    postOrderTraversal(root);
    std::cout << std::endl;
}

void SpaceSectorLLRBT::inOrderTraversal(Sector* node) {
    if (node == nullptr) {
        return;
    }
    inOrderTraversal(node->left);
    printSector(node);
    inOrderTraversal(node->right);
}

void SpaceSectorLLRBT::preOrderTraversal(Sector* node) {
    if (node == nullptr) {
        return;
    }
    printSector(node);
    preOrderTraversal(node->left);
    preOrderTraversal(node->right);
}

void SpaceSectorLLRBT::postOrderTraversal(Sector* node) {
    if (node == nullptr) {
        return;
    }
    postOrderTraversal(node->left);
    postOrderTraversal(node->right);
    printSector(node);
}

void SpaceSectorLLRBT::printSector(Sector* node) {
    if (node != nullptr) {
        cout << (node->color ? "RED" : "BLACK") << " sector: " << node->sector_code << endl;
    }
}
Sector* SpaceSectorLLRBT::findSector(Sector* node, const std::string& sector_code) {
    if (node == nullptr) {
        return nullptr;
    }

    // Eğer aranan sektör kodu, mevcut düğümün sektör koduysa, bulundu demektir.
    if (node->sector_code == sector_code) {
        return node;
    }

    // Eğer aranan sektör kodu, mevcut düğümün sektör kodundan küçükse,
    // sol alt ağaca bakılır. Aksi takdirde sağ alt ağaca bakılır.
    Sector* left_search = findSector(node->left, sector_code);
    if (left_search != nullptr) {
        return left_search;
    }

    Sector* right_search = findSector(node->right, sector_code);
    return right_search;
}

bool SpaceSectorLLRBT::getPath(Sector* root, std::vector<string>& path, string x) {
    if (root == nullptr) {
        return false;
    }

    path.push_back(root->sector_code); // Düğüm değerini vektöre ekle

    if (root->sector_code == x) {
        return true; // Hedef düğüm bulundu
    }

    // Sol alt ağaca bak
    if (getPath(root->left, path, x)) {
        return true; // Hedef düğüm sol alt ağaçta bulundu
    }

    // Sağ alt ağaca bak
    if (getPath(root->right, path, x)) {
        return true; // Hedef düğüm sağ alt ağaçta bulundu
    }

    // Düğümün altında hedef düğüm bulunamadı, düğümü vektörden çıkar
    path.pop_back();
    return false;
}


std::vector<Sector*> SpaceSectorLLRBT::getStellarPath(const std::string& sector_code) {
    std::vector<Sector*> path;

    // Earth ve Dr. Elara düğümlerini bul
    Sector* earth = findSector(root, "0SSS");
    Sector* elara = findSector(root, sector_code);

    if (earth == nullptr || elara == nullptr) {
        return path;
    }

    // Yeni algoritma için gerekli yapıları oluşturma ve çağırma
    std::vector<string> path1, path2;
    getPath(root, path1, "0SSS"); // Earth'ü kök düğüm kabul ederek yolu bulma
    getPath(root, path2, sector_code); // Hedef düğüme kadar olan yolu bulma

    int intersection = -1;

    // Kesişim noktasını bulma
    int i = 0, j = 0;
    while (i != path1.size() || j != path2.size()) {
        if (i == j && path1[i] == path2[j]) {
            i++;
            j++;
        } else {
            intersection = j - 1;
            break;
        }
    }

    // Yolu oluşturma
    for (int i = path1.size() - 1; i > intersection; i--) {
        Sector* sector = findSector(root, (path1[i]));
        path.push_back(sector);
    }

    for (int i = intersection; i < path2.size(); i++) {
        Sector* sector = findSector(root, (path2[i]));
        path.push_back(sector);
    }

    return path;
}


void SpaceSectorLLRBT::findPathToRoot(Sector* root, Sector* target, std::stack<Sector*>& path) {
    if (root == nullptr || target == nullptr) {
        return;
    }

    std::unordered_map<Sector*, Sector*> parent;

    std::queue<Sector*> q;
    q.push(root);

    // Parent'ları bulana kadar BFS
    while (!q.empty()) {
        Sector* current = q.front();
        q.pop();

        if (current->left) {
            parent[current->left] = current;
            q.push(current->left);
        }

        if (current->right) {
            parent[current->right] = current;
            q.push(current->right);
        }
    }

    // Hedef düğümden root'a kadar parent'ları takip ederek path bul
    Sector* current = target;
    while (current != nullptr) {
        path.push(current);
        current = parent[current];
    }
}






void SpaceSectorLLRBT::printStellarPath(const std::vector<Sector*>& path) {
    if (path.empty()) {
        std::cout << "A path to Dr. Elara could not be found." << std::endl;
        return;
    }

    std::cout << "The stellar path to Dr. Elara: ";

    // Yolu ekrana yazdır
    for (size_t i = 0; i < path.size(); ++i) {
        std::cout << path[i]->sector_code;
        if (i != path.size() - 1) {
            std::cout << "->";
        }
    }

    std::cout << std::endl;
}

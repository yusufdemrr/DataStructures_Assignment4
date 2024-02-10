#include <algorithm>
#include "KD_Tree.h"
#include "KDT_Node.h"

// Default constructor implementation
KD_Tree::KD_Tree() : root(nullptr), split_threshold(0.1) {
}

// Parameterized constructor implementation
KD_Tree::KD_Tree(double threshold) : root(nullptr), split_threshold(threshold) {
}

// Destructor implementation
KD_Tree::~KD_Tree() {
    // Implementation for safely deleting the KD_Tree and its nodes
//    clear(root);
}

// Helper function to recursively delete nodes
void KD_Tree::clear(KDTreeNode* node) {
    if (node == nullptr) {
        return;
    }

    auto internalNode = dynamic_cast<kd_tree_inter_node*>(node);
    if (internalNode) {
        if (internalNode->left != nullptr) {
            clear(internalNode->left);
        }
        if (internalNode->right != nullptr) {
            clear(internalNode->right);
        }
    }

    delete node;
}




void KD_Tree::buildRecursive(KDTreeNode*& node, std::vector<Point>& points, size_t depth) {
    // Check if points vector is empty
    if (points.empty()) {
        node = nullptr;
        return;
    }

    // Determine the current split dimension based on the depth
    size_t dim = depth % points[0].features.size();

    // Calculate the median index
    size_t medianIndex = points.size() / 2;

    // Find the median element along the current dimension without sorting the entire vector
    std::nth_element(points.begin(), points.begin() + medianIndex, points.end(),
                     [dim](const Point& a, const Point& b) {
                         return a.features[dim] < b.features[dim];
                     });

    // Create a new internal node
    node = new kd_tree_inter_node(dim, points[medianIndex].features[dim]);

    // Recursively build left and right subtrees
    std::vector<Point> leftPoints(points.begin(), points.begin() + medianIndex);
    std::vector<Point> rightPoints(points.begin() + medianIndex + 1, points.end());
    buildRecursive(reinterpret_cast<kd_tree_inter_node*>(node)->left, leftPoints, depth + 1);
    buildRecursive(reinterpret_cast<kd_tree_inter_node*>(node)->right, rightPoints, depth + 1);
}



void KD_Tree::build(Dataset& data) {
    std::vector<Point> points = data.points;
    buildRecursive(root, points, 0);
}

KDTreeNode* KD_Tree::getRoot() {
    // Implementation to return the root of the KD_Tree
    return root;
}

// Other member function implementations...

std::vector<Point> KD_Tree::kNN(const Point& queryPoint, size_t k) {
    std::vector<Point> nearestNeighbors;

    // Start the search from the root of the KD-Tree
    kNNRecursive(root, queryPoint, k, nearestNeighbors);

    return nearestNeighbors;
}

void KD_Tree::kNNRecursive(KDTreeNode* node, const Point& queryPoint, size_t k, std::vector<Point>& neighbors) {
    // Güvenli bir şekilde node türünü kontrol et
    if (auto leafNode = dynamic_cast<kd_tree_leaf_node*>(node)) {
        // node bir yaprak düğümüdür
        if (!leafNode) {
            return;
        }
        // Yaprak düğümündeki tüm noktaları al
        std::vector<Point> leafPoints = leafNode->data.points;

        // Query noktasından yaprak düğümündeki her noktaya olan uzaklığı hesapla
        for (auto& leafPoint : leafPoints) {
            double distance = queryPoint.calculateDistance(leafPoint);
            leafPoint.features.push_back(distance); // Uzaklığı özelliklere ekleyerek sakla
        }

        // Query noktasına olan uzaklığa göre yaprak noktalarını sırala
        std::sort(leafPoints.begin(), leafPoints.end(), [](const Point& a, const Point& b) {
            return a.features.back() < b.features.back(); // Özelliğin son elemanı, yani uzaklık, kullanılır
        });

        // k-en yakın komşuları neighbors vektörüne ekle
        for (size_t i = 0; i < std::min(k, leafPoints.size()); ++i) {
            neighbors.push_back(leafPoints[i]);
        }

        return;
    } else if (auto internalNode = dynamic_cast<kd_tree_inter_node*>(node)) {
        // node bir iç düğümüdür

        if (!internalNode) {
            return;
        }

        // Split düzlemine olan uzaklığı hesapla
        double distToPlane = std::abs(queryPoint.features[internalNode->split_dimension] - internalNode->split_value);

        // Split düzlemine olan uzaklığa göre sol ve sağ alt ağaçları dolaş
        if (queryPoint.features[internalNode->split_dimension] < internalNode->split_value) {
            kNNRecursive(internalNode->left, queryPoint, k, neighbors);
            if (neighbors.size() < k || distToPlane < neighbors.back().features.back()) {
                kNNRecursive(internalNode->right, queryPoint, k, neighbors);
            }
        } else {
            kNNRecursive(internalNode->right, queryPoint, k, neighbors);
            if (neighbors.size() < k || distToPlane < neighbors.back().features.back()) {
                kNNRecursive(internalNode->left, queryPoint, k, neighbors);
            }
        }
    } else {
        // node geçerli bir tür değil veya nullptr'dir, hata durumunu işlemeliyiz
        return;
    }
}







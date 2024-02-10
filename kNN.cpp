#include "kNN.h"
#include <cmath> // For mathematical functions like sqrt

// Constructor implementation
KNN::KNN(int neighbors, double threshold) : k(neighbors), split_threshold(threshold) {}

// Train function implementation
void KNN::train(Dataset& data) {
    // Standardize the data
    for (size_t j = 0; j < data.points[0].features.size(); ++j) {
        double mean_of_feature_j = 0.0;
        double std_dev_of_feature_j = 0.0;

        // Calculate mean of feature_j
        for (size_t i = 0; i < data.points.size(); ++i) {
            mean_of_feature_j += data.points[i].features[j];
        }
        mean_of_feature_j /= data.points.size();

        // Calculate standard deviation of feature_j
        for (size_t i = 0; i < data.points.size(); ++i) {
            std_dev_of_feature_j += pow(data.points[i].features[j] - mean_of_feature_j, 2);
        }
        std_dev_of_feature_j = sqrt(std_dev_of_feature_j / data.points.size());

        // Standardize each feature
        for (size_t i = 0; i < data.points.size(); ++i) {
            data.points[i].features[j] = (data.points[i].features[j] - mean_of_feature_j) / std_dev_of_feature_j;
        }
    }

    // Build the KD_Tree
    tree.build(data);
}

// Predict function implementation
int KNN::predict(const Point& queryPoint) {
    // Traverse the KD_Tree to find k nearest neighbors
    std::vector<Point> neighbors = tree.kNN(queryPoint, k);

    // Perform majority voting to predict the label
    // Count the number of habitable and non-habitable neighbors
    int habitableCount = 0;
    for (const auto& neighbor : neighbors) {
        if (neighbor.label == "Habitable") {
            ++habitableCount;
        }
    }

    // If at least one neighbor is habitable, predict label as 1 (habitable)
    if (habitableCount > 0) {
        return 1;
    } else {
        // If none of the neighbors are habitable, predict label as 0 (not habitable)
        return 0;
    }
}

// Additional methods or helper functions can be added as needed

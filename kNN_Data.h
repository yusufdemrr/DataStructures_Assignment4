#ifndef kNN_Data_H
#define kNN_Data_H

#include <iostream>
#include <string>
#include <vector>
#include <complex>

// Point structure
struct Point {
    std::vector<double> features; // Features of the point
    std::string label;            // Optional label for classification

    // Define constructors as needed
    Point(const std::vector<double>& f, const std::string& l = "") : features(f), label(l) {}

    double calculateDistance(const Point& other) const {
        double sum = 0.0;
        for (size_t i = 0; i < features.size(); ++i) {
            sum += (features[i] - other.features[i]) * (features[i] - other.features[i]);
        }
        return std::sqrt(sum);
    }
};

// Dataset structure
struct Dataset {
    std::vector<Point> points;    // Points in the dataset
    std::vector<std::string> header; // Header for the dataset
    double threshold;             // Threshold for the dataset

    // Constructors
    Dataset(const std::vector<Point>& p, const std::vector<std::string>& h, double t) : points(p), header(h), threshold(t) {}

    // Default constructor
    Dataset() : threshold(0.0) {} // Default threshold value

};

#endif // kNN_Data_H
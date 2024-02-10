#include <valarray>
#include "Sector.h"

// Constructor implementation

Sector::Sector(int x, int y, int z) : x(x), y(y), z(z), left(nullptr), right(nullptr), parent(nullptr), color(RED) {
    // Calculate distance from Earth using Euclidean distance formula
    distance_from_earth = sqrt(x * x + y * y + z * z);

    // Round the distance to the nearest integer
    int rounded_distance = static_cast<int>(distance_from_earth);

    // Generate sector code based on coordinates and distance
    sector_code = std::to_string(rounded_distance);

    if (x == 0) sector_code += 'S';
    else if (x > 0) sector_code += 'R';
    else sector_code += 'L';

    if (y == 0) sector_code += 'S';
    else if (y > 0) sector_code += 'U';
    else sector_code += 'D';

    if (z == 0) sector_code += 'S';
    else if (z > 0) sector_code += 'F';
    else sector_code += 'B';
}

Sector::~Sector() {
    // TODO: Free any dynamically allocated memory if necessary
}

Sector& Sector::operator=(const Sector& other) {
    if (this != &other) {
        x = other.x;
        y = other.y;
        z = other.z;
        distance_from_earth = other.distance_from_earth;
        sector_code = other.sector_code;
        // Copy other members if any
    }
    return *this;
}

bool Sector::operator==(const Sector& other) const {
    return (x == other.x && y == other.y && z == other.z);
}

bool Sector::operator!=(const Sector& other) const {
    return !(*this == other);
}
#ifndef DATA_LOADER_H
#define DATA_LOADER_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <filesystem>

#include <CGAL/IO/read_points.h>
#include "CICP_Types.h"

namespace fs = std::filesystem;

class DataLoader {
public:

    bool load(const std::string& filepath, std::vector<PointVectorPair>& cloud) {
        std::cout << "Loading file: " << filepath << "..." << std::endl;

        // Check if file exists
        if (!fs::exists(filepath)) {
            std::cerr << "Error: File does not exist: " << filepath << std::endl;
            return false;
        }

        // Get extension and convert to lowercase
        std::string ext = fs::path(filepath).extension().string();
        std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);

        cloud.clear();

        if (ext == ".csv") {
            return loadCSV(filepath, cloud);
        }
        else if (ext == ".ply" || ext == ".xyz") {
            return loadStandardCGAL(filepath, cloud);
        }
        else {
            std::cerr << "Error: Unsupported file extension '" << ext << "'" << std::endl;
            return false;
        }
    }

private:
    bool loadCSV(const std::string& filepath, std::vector<PointVectorPair>& cloud) {
        std::ifstream stream(filepath);
        if (!stream) return false;

        std::string line;
        while (std::getline(stream, line)) {
            if (line.empty()) continue;

            std::stringstream ss(line);
            double x, y, z;

            if (ss >> x >> y >> z) {
                cloud.push_back({ Point(x, y, z), Vector(0,0,0) });
            }
        }

        if (cloud.empty()) {
            std::cerr << "Error: No points found in CSV." << std::endl;
            return false;
        }

        std::cout << "Successfully loaded " << cloud.size() << " points (CSV)." << std::endl;
        return true;
    }

    bool loadStandardCGAL(const std::string& filepath, std::vector<PointVectorPair>& cloud) {
        bool success = CGAL::IO::read_points(filepath,
            std::back_inserter(cloud),
            CGAL::parameters::point_map(CGAL::First_of_pair_property_map<PointVectorPair>()));

        if (!success) {
            std::cerr << "Error: CGAL failed to read file." << std::endl;
            return false;
        }

        std::cout << "Successfully loaded " << cloud.size() << " points (CGAL Standard)." << std::endl;
        return true;
    }
};

#endif
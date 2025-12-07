#ifndef VOXEL_GRID_H
#define VOXEL_GRID_H

#include <vector>
#include <map>
#include <cmath>
#include <algorithm>
#include <iostream>
#include <CGAL/Bbox_3.h>
#include "CICP_Types.h"


struct VoxelKey {
    long i, j, k;

    bool operator<(const VoxelKey& other) const {
        if (i != other.i) return i < other.i;
        if (j != other.j) return j < other.j;
        return k < other.k;
    }
};

class VoxelGrid {
public:
    std::map<VoxelKey, std::vector<size_t>> grid;

    double voxel_size;

    long numDivX, numDivY, numDivZ;

    double min_x, max_x, min_y, max_y, min_z, max_z;

    VoxelGrid(double size) : voxel_size(size),
        min_x(0), max_x(0), min_y(0), max_y(0), min_z(0), max_z(0) {}

    void create(const std::vector<PointVectorPair>& cloud) {
        if (cloud.empty()) return;

        // Step 1: Compute Limits
        const Point& p0 = cloud[0].first;
        min_x = max_x = p0.x();
        min_y = max_y = p0.y();
        min_z = max_z = p0.z();

        for (const auto& pair : cloud) {
            const Point& p = pair.first;
            if (p.x() < min_x) min_x = p.x();
            if (p.x() > max_x) max_x = p.x();
            if (p.y() < min_y) min_y = p.y();
            if (p.y() > max_y) max_y = p.y();
            if (p.z() < min_z) min_z = p.z();
            if (p.z() > max_z) max_z = p.z();
        }

        grid.clear();

        // Step 2: Bin points into voxels using the Struct
        for (size_t idx = 0; idx < cloud.size(); ++idx) {
            const Point& p = cloud[idx].first;

            long i = static_cast<long>((p.x() - min_x) / voxel_size);
            long j = static_cast<long>((p.y() - min_y) / voxel_size);
            long k = static_cast<long>((p.z() - min_z) / voxel_size);

            VoxelKey key = { i, j, k };
            grid[key].push_back(idx);
        }

        std::cout << "Voxelization complete. Created " << grid.size() << " voxels." << std::endl;
    }
};

#endif
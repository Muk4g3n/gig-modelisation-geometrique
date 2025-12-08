#include <iostream>
#include <fstream>
#include <CGAL/IO/read_points.h>
#include <CGAL/pca_estimate_normals.h>
// created types
#include "types/CICP_Types.h"
#include "types/DataLoader.h"
#include "types/VoxelGrid.h"

int main() {


    /// Load Data

    std::vector<PointVectorPair> denseCloud;
    const std::string filename = "../../../data/pavin/scan000.csv";

    DataLoader loader;

    if (!loader.load(filename, denseCloud))
    {
        return -1;
    }

    /// Estimation de normal (à modifier pour ajouter elbow method)

    const int k = 5;

    std::cout << "Computing normals using PCA..." << std::endl;

    CGAL::pca_estimate_normals<CGAL::Parallel_if_available_tag>(
        denseCloud,
        k,
        CGAL::parameters::point_map(CGAL::First_of_pair_property_map<PointVectorPair>())
        .normal_map(CGAL::Second_of_pair_property_map<PointVectorPair>())
        );

    std::cout << "Computed " << denseCloud.size() << " normals." << std::endl;


    // Voxelization
    double voxelSize = 0.5; // à ajouter une méthode de calcul de taille de voxel
    std::cout << "Voxelizing with size " << voxelSize << "..." << std::endl;

    VoxelGrid vGrid(voxelSize);
    vGrid.create(denseCloud);

    // Debug
    /*if (!vGrid.grid.empty()) {
        long firstVoxelId = vGrid.grid.begin()->first;
        size_t count = vGrid.grid.begin()->second.size();
        std::cout << "Voxel ID " << firstVoxelId << " contains " << count << " points." << std::endl;
    }*/

    loader.export_cloud_with_normals("../../../data output/debug_cloud.ply", denseCloud);
    loader.export_voxel_centers("../../../data output/debug_voxels.ply", vGrid);


    return 0;
}
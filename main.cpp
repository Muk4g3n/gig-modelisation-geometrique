#include <iostream>
#include <fstream>
#include <CGAL/IO/read_points.h>
#include <CGAL/pca_estimate_normals.h>
//#include <CGAL/pstl>
#include "types/CICP_Types.h"
#include "types/DataLoader.h"

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



    //

    return 0;
}
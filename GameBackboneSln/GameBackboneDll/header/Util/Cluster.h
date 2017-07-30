#pragma once

#include <Util\Array2D.h>
#include <Util\DllUtil.h>
#include <Util\Point.h>

#include <SFML\Graphics\Sprite.hpp>

#include <set>
#include <tuple>
#include <map>

namespace GB {
    class libGameBackbone Cluster {
    public:
        // also functions as the setter
        // random number generated by caller is passed in
		Cluster(Point2D<int> firstPoint);

        //getter
		std::set<Point2D<int>> const* const Cluster::getClusterPoints();
		std::set<Point2D<int>> const* const Cluster::getBorderPointSet();

		double getClusterFrequency();

        //setter
		void setClusterFrequency(double FrequencyForThisCluster);

		void addPointToCluster(Point2D<int> newPointToAdd);
		unsigned int getNumberBorderPoints();



    private:
		void UpdateBorder(Point2D<int> pointToAdd);

        // Set of all points of the Array2D which are owned by this cluster
        std::set<Point2D<int>> clusterPointSet;

        std::set<Point2D<int>> borderPointSet;

        double clusterFrequency;
    };
}
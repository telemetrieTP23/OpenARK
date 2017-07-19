#pragma once

// OpenCV Libaries
#include <opencv2/imgproc/imgproc.hpp>

// PCL Libaries
#include <pcl/filters/voxel_grid.h>
#include <pcl/point_types.h>
#include <pcl/features/normal_3d.h>
#include <pcl/io/pcd_io.h>
#include <pcl/io/ply_io.h>
#include <pcl/point_cloud.h>
#include <pcl/console/parse.h>
#include <pcl/common/transforms.h>
#include <pcl/visualization/pcl_visualizer.h>
#include <pcl/filters/passthrough.h>
#include <pcl/features/integral_image_normal.h>
#include <pcl/visualization/cloud_viewer.h>
#include <pcl/segmentation/region_growing.h>
#include <pcl/features/normal_3d_omp.h>

/**
* Class defining a plane object.
* The plane object will be defined by its regression equation
* Example on tracking hand and background plane object simulateously
* @include HandandPlane.cpp
*/
class Plane {
public:
	Plane();

	/**
	* Constructs a plane object from a unsegemented xyzMap.
	* @param [in] src the raw xyzMap.
	*/
	explicit Plane(cv::Mat &src);

	/**
	* Deconstructs the plane object.
	*/
	~Plane();
	
	/**
	* Initializes all the required point clouds.
	* @param [in] src the raw xyzMap
	*/
	void initializeCloud(cv::Mat &src);

	/**
	* Returns the PCL representation of the xyzMap.
	* @return PCL point cloud
	*/
	pcl::PointCloud<pcl::PointXYZ>::Ptr getCloud() const;

	/**
	* Returns the PCL representatino of a down sampled xyzMap.
	* @return down sampled PCL point cloud
	*/
	pcl::PointCloud<pcl::PointXYZ>::Ptr getDownCloud() const;

	/**
	* Returns the planar regression equation of the identified plane.
	* @return coefficients for the planar regression equation
	*/
	std::vector<double> getPlaneEquation() const;

	/**
	* Returns the sphereical regression equation of the identified plane.
	* @return coefficients for the sphereical regression equation
	*/
	std::vector<double> getSphereEquation() const;

	/**
	* Returns the (i,j) indices for which the plane appears on the xyzMap.
	* @return vector of (i,j) coordinates defining the points that make up the plane on the xyzMap
	*/
	std::vector<cv::Point2i> getPlaneIndicies() const;

	/**
	* Returns the (i,j) indices for which the plane appears on the xyzMap.
	* @return vector of (i,j) coordinates defining the points that make up the plane on the xyzMap
	*/
	std::vector<cv::Point2i> getSphereIndices() const;

	/**
	* Maximum cloud size (pixel) allowed.
	*/
	const int CLOUD_SIZE_THRESHOLD = 1000;

	/**
	* Maximum distance (mm) allowed between real point and regression equation.
	*/
	const double R_SQUARED_DISTANCE_THRESHOLD = 0.0005;

private:
	/**
	* Performs a series of computations to find the plane.
	*/
	int compute();

	/**
	* Computes the normals at every point.
	* @param [in] normal_estimator a OMP instance of the normal estimator
	* @param [in] tree a KD-tree used by the normal estimator to preform neareest neighbor searches
	*/
	void calculateNormals(pcl::NormalEstimationOMP<pcl::PointXYZ, pcl::Normal> normal_estimator, pcl::search::Search<pcl::PointXYZ>::Ptr tree) const;

	/**
	* Down samples the cloud via a median filter.
	*/
	void voxelDownsample() const;

	/**
	* Aggregates points with like normals.
	* @param [out] reg equation of the final plane
	* @param [in] tree KD-tree used for nearest neighbor search
	*/
	void regionGrow(pcl::RegionGrowing<pcl::PointXYZ, pcl::Normal> &reg, pcl::search::Search<pcl::PointXYZ>::Ptr tree);

	/**
	* Compute a planar regression equation.
	* @param [in] ind all the points on the plane
	*/
	void computePlaneLSE(pcl::PointIndices &ind);

	/**
	* Compute a sphereical regression equation.
	* @param [in] ind all the points on the plane
	*/
	void computeSphereLSE(pcl::PointIndices &ind);

	/**
	* Compute all the (i,j) index of the plane points from the planar regression.
	*/
	int computePlaneIndices();

	/**
	* Compute all the (i,j) index of the plane points from the sphereical regression.
	*/
	int computeSphereIndices();

	static int drawSphereRegressionPoints(cv::Mat &output_mat, cv::Mat &input_mat, std::vector<double> &equation,
		const int rowSize, const int colSize, const double threshold, bool clicked);

	// Private Variables
	pcl::PointCloud<pcl::PointXYZ>::Ptr *cloud;
	pcl::PointCloud <pcl::Normal>::Ptr *normals;
	pcl::PointCloud<pcl::PointXYZ>::Ptr *down_cloud;
	pcl::PointCloud<pcl::Normal>::Ptr *down_normals;
	pcl::PointCloud <pcl::PointXYZRGB>::Ptr *colored_cloud;
	pcl::PointCloud <pcl::PointXYZRGB>::Ptr *upsampled_colored_cloud;
	std::vector<pcl::PointIndices> clusters;
	std::vector<double> plane_equation;
	std::vector<double> sphere_equation;
	std::vector<cv::Point2i> plane_indices;
	std::vector<cv::Point2i> sphere_indices;
	cv::Mat sphere_mat;
	cv::Mat plane_mat;
	cv::Mat display_img;
	cv::Mat depth_img;
	int num_sphere_points;
	int num_plane_points;

	const int NUM_CORES = boost::thread::hardware_concurrency();
};
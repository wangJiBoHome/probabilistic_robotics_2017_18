#include "defs.h"
#include "distance_map.h"
#include "camera.h"
#include "utils.h"
#include "points_utils.h"
#include "distance_map_correspondence_finder.h"
#include <sys/time.h>

using namespace std;
using namespace pr;


const char* banner[]={
  "correspondence_finder_test",
  " demonstrates a simple distance_map based correspondence finder",
  " move the observer with [i,j,k,l]",
  " ESC to quit",
  0
};


int main (int argc, char** argv) {
  printBanner(banner);

  Vector3fVector world_points;
  Eigen::Vector3f lower_left_bottom(-10,-10,-10);
  Eigen::Vector3f upper_right_top(10,10,10);
  int num_segments=20;
  float density=10; // 10 points per meter
  makeWorld(world_points,
	    lower_left_bottom, 
	    upper_right_top,
	    num_segments,
	    density);

  cerr << "Generated Model with " << world_points.size() << " points" << endl;

  // we test the projection
    
  Eigen::Matrix3f camera_matrix;
  camera_matrix << 
    150, 0, 320,
    0, 150,240,
    0, 0, 1;
  
  int rows=480;
  int cols=640;
  Camera cam(rows, cols, camera_matrix);

  // project all points in the world in a camera frame
  // this will be our reference
  Vector2fVector reference_image_points;
  const bool keep_indices=false;
  int num_projected_points=cam.projectPoints(reference_image_points,world_points,keep_indices);
  cerr << "points projected in reference: " << num_projected_points << endl;
  
  // construct a correspondence finder
  DistanceMapCorrespondenceFinder correspondence_finder;
  float max_distance=50;
  
  double t_start_corr_init=getTime();
  correspondence_finder.init(reference_image_points,
			       rows,
			       cols,
			       max_distance);
  double t_end_corr_init=getTime();
  cerr << "corr_init took: " << (t_end_corr_init-t_start_corr_init) << " ms" << endl;
 
  cv::namedWindow("correspondence_finder_test");
  char key=0;
  const char ESC_key=27;
  while (key!=ESC_key) {

    RGBImage shown_image;
    drawDistanceMap(shown_image, 
		       correspondence_finder.distanceImage(), 
		       correspondence_finder.maxDistance()-1);

    drawPoints(shown_image, reference_image_points, cv::Scalar(0,0,255),3);

    Vector2fVector current_image_points;
    const bool keep_indices=true;
    double t_start_projection=getTime();
    cam.projectPoints(current_image_points, world_points, keep_indices);
    double t_end_projection=getTime();
    drawPoints(shown_image, current_image_points, cv::Scalar(0,255,0),3);
    
    double t_start_corr_find=getTime();
    correspondence_finder.compute(current_image_points);
    double t_end_corr_find=getTime();
    drawCorrespondences(shown_image,
			reference_image_points,
			current_image_points,
			correspondence_finder.correspondences());

    cv::imshow("correspondence_finder_test", shown_image);
    Eigen::Isometry3f current_pose=cam.worldToCameraPose();
    Eigen::Isometry3f motion=Eigen::Isometry3f::Identity();
    float dr=0.05;
    float dt=0.1;
    key=cv::waitKey(0);
    switch(key) {
    case 'i': motion.translation()=Eigen::Vector3f(0,0,-dt); break;
    case 'k': motion.translation()=Eigen::Vector3f(0,0,dt); break;
    case 'j': motion.linear()=Ry(dr); break;
    case 'l': motion.linear()=Ry(-dr); break;
    default: ;
    }
    cam.setWorldToCameraPose(motion*current_pose);
    cerr << "projction took: " << (t_end_projection-t_start_projection) << " ms" << endl;
    cerr << "search took: " << (t_end_corr_find-t_start_corr_find) << " ms" << endl;

  }

}

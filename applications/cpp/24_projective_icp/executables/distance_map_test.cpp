#include "points_utils.h"
#include "utils.h"

using namespace pr;
using namespace std;

const char* banner[]={
  "distance_map_test",
  " demonstrates a simple distance_map algorithm",
  " increase or decrease the current maximum distance with [+,-]",
  " ESC to quit",
  0
};

int main(int argc, char** argv){
  printBanner(banner);
  int rows=480;
  int cols=640;
  int num_points=100;

  // generate n random points
  Vector2fVector points(num_points);
  for (size_t i=0; i<points.size(); i++){
    points[i]=Eigen::Vector2f(rows*drand48(), cols*drand48());
  }
  
  // create an image containing the random points;
  IntImage points_image(rows, cols);
  points_image=-1;

  // copy the points in the image
  putPointIndices(points_image, points);

  // build a distance map
  DistanceMap distance_map;
  FloatImage distance_image(rows, cols);
  IntImage indices_image(rows,cols);
  RGBImage shown_image;

  float distance_maximum = 100;
  int distance_current   = 0;
  char key               = 0;

  // show progressive construction of distance map
  while (key != OPENCV_KEY_ESCAPE) {
    distance_map.compute(indices_image, distance_image, points_image, distance_current);
    drawDistanceMap(shown_image, distance_image, distance_current-1);
    cv::imshow("distance_map_test", shown_image);

    //ds wait for key pressed
    key = cv::waitKey(0);
    switch (key) {
      case '+': {
        if (distance_current < distance_maximum) {
          ++distance_current;
        }
        break;
      }
      case '-': {
        if (distance_current > 0) {
          --distance_current;
        }
        break;
      }
      default: {
        break;
      }
    }
    std::cerr << "current maximum distance: " << distance_current << std::endl;
  }
  return 0;
}

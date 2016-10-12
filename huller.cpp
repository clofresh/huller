/**
 * Simple cli util to find the convex hull of a transparent png and output it
 * as a lua table of x/y coordinates. Useful for making collision shapes for
 * for sprites.
 *
 * Adapted from the opencv convex hull example:
 * http://docs.opencv.org/3.0-beta/doc/tutorials/imgproc/shapedescriptors/hull/hull.html
 *
 */

#include <unistd.h>
#include <iostream>
#include "opencv2/highgui.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"

using namespace cv;
using namespace std;

/**
 * @function main
 */
int main(int argc, char** argv) {
  if (argc < 2) {
    cerr << "Must specify a path to an image" << endl;
    return 1;
  } else if (argc > 2) {
    cerr << "Must specify only one image" << endl;
    return 1;
  }

  auto filename = argv[1];
  if (access(filename, F_OK) == -1) {
    cerr << "Image doesn't exists: " << filename << endl;
    return 1;
  }

  Mat src;
  Mat src_gray;
  int thresh = 100;
  int max_thresh = 255;
  RNG rng(12345);

  /// Load source image, making sure to preserve the alpha channel
  src = imread(filename, -1);

  // Convert transparent pixels to black, and non-transparent
  // pixels to white to simplify the contour-finding.
  for (int i = 0; i < src.rows; ++i) {
    Vec4b* row = src.ptr<Vec4b>(i);
    for (int j = 0; j < src.cols; ++j) {
      if (row[j][3] > 0) {
        row[j][0] = 255;
        row[j][1] = 255;
        row[j][2] = 255;
        row[j][3] = 255;
      } else {
        row[j][0] = 0;
        row[j][1] = 0;
        row[j][2] = 0;
        row[j][3] = 255;
      }
    }
  }

  /// Convert image to gray and blur it
  cvtColor(src, src_gray, COLOR_BGR2GRAY);
  blur(src_gray, src_gray, Size(3, 3));

  Mat threshold_output;
  vector<vector<Point> > contours;
  vector<Vec4i> hierarchy;

  /// Detect edges using Threshold
  threshold(src_gray, threshold_output, thresh, 255, THRESH_BINARY);

  /// Find contours
  findContours(threshold_output, contours, hierarchy, RETR_TREE,
               CHAIN_APPROX_SIMPLE, Point(0, 0));

  /// Find the convex hull object for the first contour
  vector<Point> hull;
  convexHull(Mat(contours[0]), hull, false);
  Point p;

  // Output the lua table to stdout so the caller can decide how to write it
  cout << "return {" << endl;
  for (size_t i = 0; i < hull.size(); i++) {
    p = hull[i];
    cout << "  " << p.x << "," << p.y << "," << endl;
  }
  cout << "}" << endl;

  return 0;
}

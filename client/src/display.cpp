#include <opencv2/opencv.hpp>

#include "display.h"

using namespace cv;

typedef Vec<uchar, 1> Vec1b;

static Mat* yv12;
static Mat  rgb;

static void bytes_to_mat(const char* bytes, Mat mat) {
  for (int r = 0; r < mat.rows; r++) {
    for (int c = 0; c < mat.cols; c++) {
      uchar b = (uchar) bytes[r * mat.cols + c];
      Vec1b bs(b);
      mat.at<Vec1b>(r, c) = bs;
    }
  }
}

bool display_init(int width, int height) {
  int rows = (int) (height * 1.5);
  int cols = width;
  yv12     = new Mat(rows, cols, CV_8UC1);

  namedWindow("video", 1);
  return true;
}

bool display_update(const char* yv12_frame) {
  bytes_to_mat(yv12_frame, *yv12);
  cvtColor(*yv12, rgb, CV_YUV2RGB_YV12, 3);

  //imshow("video", rgb);
  imshow("video", *yv12);

  waitKey(1);
  return true;
}

#include <opencv2/opencv.hpp>

#include "cam.h"

using namespace cv;

#define PREVIEW_WINDOW_TITLE "server"

typedef Vec<uchar, 1> Vec1b;

static VideoCapture cap;
static bool         preview;
static int          width, height;
static Mat          rgb, yv12;

bool cam_open(int cam_id, bool _preview) {
  if (!cap.open(cam_id)) {
    printf("Could not open camera %d", cam_id);
    return false;
  }

  cap.set(CV_CAP_PROP_FRAME_WIDTH,  640);
  cap.set(CV_CAP_PROP_FRAME_HEIGHT, 480);
  cap.set(CV_CAP_PROP_FPS,          30);
  cap.set(CV_CAP_PROP_CONVERT_RGB,  0);

  preview = _preview;
  if (preview) namedWindow(PREVIEW_WINDOW_TITLE, 1);

  width  = cap.get(CV_CAP_PROP_FRAME_WIDTH);
  height = cap.get(CV_CAP_PROP_FRAME_HEIGHT);
  printf("Camera %d: %d x %d\n", cam_id, width, height);

  return true;
}

int cam_width() {
  return width;
}

int cam_height() {
  return height;
}

bool cam_yv12_frame(char* ret) {
  if (preview) waitKey(30);

  cap.read(rgb);
  if (!rgb.data) return false;

  if (preview) imshow(PREVIEW_WINDOW_TITLE, rgb);

  cvtColor(rgb, yv12, CV_RGB2YUV_YV12);

  int rows = yv12.rows;
  int cols = yv12.cols;

  for (int r = 0; r < rows; r++) {
    for (int c = 0; c < cols; c++) {
      Vec1b bs  = yv12.at<Vec1b>(r, c);
      int   idx = r * cols + c;
      ret[idx] = bs[0];
    }
  }

  return true;
}

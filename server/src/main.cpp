#include <stdio.h>
#include <stdlib.h>

#include "cam.h"
#include "encode.h"

int main() {
  int cam_id = 0;
  if (!cam_open(cam_id, true)) {
    printf("Could not open camera: %d\n", cam_id);
    return -1;
  }

  int width  = cam_width();
  int height = cam_height();

  vpx_init(width, height, 10);

  int            rows = (int) (height * 1.5);
  int            cols = width;
  unsigned char* yv12 = (unsigned char*) malloc(rows * cols);

  while (true) {
    if (!cam_yv12_frame(yv12)) continue;

    unsigned char* encoded = NULL;
    int            size;
    vpx_encode(yv12, &encoded, &size);

    free(encoded);
  }

  free(yv12);
  vpx_cleanup();
  return 0;
}

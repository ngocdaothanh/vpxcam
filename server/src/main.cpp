#include <stdio.h>
#include <stdlib.h>

#include <boost/thread.hpp>

#include "cam.h"
#include "encode.h"
#include "server.h"

void cam_loop(int width, int height, server* s) {
  int rows = (int) (height * 1.5);
  int cols = width;

  char yv12[rows * cols];
  char encoded[rows * cols];

  while (true) {
    if (!cam_yv12_frame(yv12)) continue;

    int size;
    vpx_encode(yv12, encoded, &size);
    if (!size) continue;

    s->handle_encoded(encoded, size);
    printf("%d", size / 1024);
  }
}

int main() {
  int cam_id = 0;
  int force_key_frame = 10;
  int port = 9000;

  if (!cam_open(cam_id, true)) {
    printf("Could not open camera: %d\n", cam_id);
    return -1;
  }

  int width  = cam_width();
  int height = cam_height();

  vpx_init(width, height, force_key_frame);

  server* s = new server(width, height, port);
  boost::thread server_thread(&server::run, s);

  cam_loop(width, height, s);
  vpx_cleanup();
  server_thread.join();
  return 0;
}

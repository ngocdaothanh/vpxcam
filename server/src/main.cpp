#include <stdio.h>
#include <stdlib.h>

#include <boost/thread.hpp>

#include "cam.h"
#include "encode.h"
#include "server.h"

#define CAM_WIDTH  640
#define CAM_HEIGHT 480

void cam_loop(int width, int height, server* s) {
  int rows = (int) (height * 1.5);
  int cols = width;

  char yv12[rows * cols];
  char encoded[rows * cols];

  while (true) {
    if (!cam_yv12_frame(yv12)) continue;

    int size;
    bool force_key_frame = s->has_new_client_connected();
    vpx_encode(yv12, encoded, &size, force_key_frame);
    if (!size) continue;

    s->handle_encoded(encoded, size);
    printf("%d", size / 1024);
  }
}

int main(int argc, char* argv[]) {
  if (argc != 3) {
    printf("Usage: vpxcams <cam_id> <port>\n");
    return -1;
  }

  int cam_id = atoi(argv[1]);
  int port   = atoi(argv[2]);

  int width  = CAM_WIDTH;
  int height = CAM_HEIGHT;

  if (!cam_open(cam_id, width, height, true)) {
    printf("Could not open camera: %d\n", cam_id);
    return -1;
  }

  vpx_init(width, height);

  server* s = new server(width, height, port);
  boost::thread server_thread(&server::run, s);

  cam_loop(width, height, s);
  vpx_cleanup();
  server_thread.join();
  return 0;
}

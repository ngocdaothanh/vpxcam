#include <cstdlib>
#include <cstring>
#include <iostream>
#include <boost/asio.hpp>

#include "client.h"
#include "decode.h"
#include "display.h"

using boost::asio::ip::tcp;

#define BUFFER_SIZE (1024 * 1024 * 3)

int main(int argc, char* argv[]) {
  if (argc != 3) {
    printf("Usage: vpxcamc <host> <port>\n");
    return -1;
  }

  tcp_connect(argv[1], argv[2]);

  char buffer[BUFFER_SIZE];

  printf("Get camera resolution...\n");
  tcp_read(buffer, 2 * 4);
  int width, height;
  memcpy(&width,  buffer,     4);
  memcpy(&height, buffer + 4, 4);
  printf("Camera: %d x %d\n", width, height);

  vpx_init();

  int rows = (int) (height * 1.5);
  int cols = width;

  char yv12[rows * cols];

  display_init(width, height);
  while (true) {
    tcp_read(buffer, 4);
    int frame_size = *((int*) buffer);

    if (frame_size < 0 || BUFFER_SIZE < frame_size) {
      printf("Invalid frame size: %d\n", frame_size);
      break;
    }

    tcp_read(buffer, frame_size);
    if (vpx_decode(buffer, frame_size, yv12)) display_update(yv12);
  }

  vpx_cleanup();
  return 0;
}

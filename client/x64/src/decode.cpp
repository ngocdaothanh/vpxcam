#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#define VPX_CODEC_DISABLE_COMPAT 1
#include "vpx/vpx_decoder.h"
#include "vpx/vp8dx.h"

#include "decode.h"

static vpx_codec_ctx_t codec;

bool vpx_init(int width, int height) {
  vpx_codec_iface_t* interface = vpx_codec_vp8_dx();

  printf("Using %s\n", vpx_codec_iface_name(interface));

  // Initialize codec
  int flags = 0;
  if (vpx_codec_dec_init(&codec, interface, NULL, flags)) {
    printf("Failed to initialize decoder\n");
    return false;
  }

  return true;
}

bool vpx_decode(const char* encoded, int frame_size, char* yv12_frame) {
  // Decode the frame
  if (vpx_codec_decode(&codec, (const unsigned char*) encoded, frame_size, NULL, 0)) {
    printf("Failed to decode frame (maybe a key frame has not been reached)\n");
    return false;
  }

  // Write decoded data to yv12_frame
  vpx_codec_iter_t iter = NULL;
  vpx_image_t*     img;
  int              total = 0;
  while ((img = vpx_codec_get_frame(&codec, &iter))) {
    for (int plane = 0; plane < 3; plane++) {
      unsigned char* buf = img->planes[plane];
      for (int y = 0; y < (plane ? (img->d_h + 1) >> 1 : img->d_h); y++) {
        int len = (plane ? (img->d_w + 1) >> 1 : img->d_w);
        memcpy(yv12_frame + total, buf, len);
        buf += img->stride[plane];
        total += len;
      }
    }
  }

  return true;
}

void vpx_cleanup() {
  if (vpx_codec_destroy(&codec)) printf("Failed to destroy codec\n");
}

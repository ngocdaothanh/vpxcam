#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

#define VPX_CODEC_DISABLE_COMPAT 1
#include <vpx/vpx_encoder.h>
#include <vpx/vp8cx.h>

#include "encode.h"

static vpx_codec_ctx_t codec;
static vpx_image_t     raw;
static int             frame_cnt = 0;

bool vpx_init(int width, int height) {
  vpx_codec_iface_t* interface = vpx_codec_vp8_cx();
  printf("Using %s\n", vpx_codec_iface_name(interface));

  if (!vpx_img_alloc(&raw, VPX_IMG_FMT_I420, width, height, 1)) {
    printf("Failed to allocate image %d x %d\n", width, height);
    return false;
  }

  vpx_codec_enc_cfg_t cfg;

  // Populate encoder configuration
  vpx_codec_err_t res = vpx_codec_enc_config_default(interface, &cfg, 0);
  if (res) {
    printf("Failed to get config: %s\n", vpx_codec_err_to_string(res));
    return false;
  }

  // Update the default configuration with our settings
  cfg.rc_target_bitrate = width * height * cfg.rc_target_bitrate / cfg.g_w / cfg.g_h;
  cfg.g_w               = width;
  cfg.g_h               = height;

  // Initialize codec
  if (vpx_codec_enc_init(&codec, interface, &cfg, 0)) {
    printf("Failed to initialize encoder\n");
  }

  return true;
}

void vpx_encode(const char* yv12_frame, char* encoded, int* size, bool force_key_frame) {
  *size = 0;

  int flags = force_key_frame ? VPX_EFLAG_FORCE_KF : 0;

  raw.planes[0] = (unsigned char *) yv12_frame;
  if (vpx_codec_encode(&codec, &raw, frame_cnt, 1, flags, VPX_DL_REALTIME)) {
    printf("Failed to encode frame\n");
    return;
  }

  vpx_codec_iter_t iter = NULL;
  const vpx_codec_cx_pkt_t *pkt;
  while ((pkt = vpx_codec_get_cx_data(&codec, &iter))) {
    switch (pkt->kind) {
    case VPX_CODEC_CX_FRAME_PKT:
      if (!*size) {
        *size = pkt->data.frame.sz;
        memcpy(encoded, pkt->data.frame.buf, pkt->data.frame.sz);
      }
      break;

    default:
      break;
    }

    bool key_frame = pkt->kind == VPX_CODEC_CX_FRAME_PKT && (pkt->data.frame.flags & VPX_FRAME_IS_KEY);
    printf(key_frame ? "K":".");
    fflush(stdout);
  }
  frame_cnt++;
}

void vpx_cleanup() {
  printf("Processed %d frames\n", frame_cnt);

  vpx_img_free(&raw);

  if (vpx_codec_destroy(&codec)) {
    printf("Failed to destroy codec\n");
  }
}

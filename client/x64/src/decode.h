#ifndef _DECODE_H_
#define _DECODE_H_

bool vpx_init();

bool vpx_decode(const char* encoded, int frame_size, char* yv12_frame);

void vpx_cleanup();

#endif

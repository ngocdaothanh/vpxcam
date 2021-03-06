#ifndef _ENCODE_H_
#define _ENCODE_H_

boolean vpx_init(int width, int height);

/** encoded will be filled with encoded data; it must be big enough */
void vpx_encode(const char* yv12_frame, char* encoded, int* size, bool force_key_frame);

void vpx_cleanup();

#endif

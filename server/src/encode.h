#ifndef _ENCODE_H_
#define _ENCODE_H_

/** force_key_frame = 10 means force a key frame every 10 frames; 0 means default */
bool vpx_init(int width, int height, int force_key_frame);

/** encoded will be filled with encoded data; it must be big enough */
void vpx_encode(const char* yv12_frame, char* encoded, int* size);

void vpx_cleanup();

#endif

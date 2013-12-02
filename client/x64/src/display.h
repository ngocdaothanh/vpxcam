#ifndef _DISPLAY_H_
#define _DISPLAY_H_

bool display_init(int width, int height);

bool display_update(const char* yv12_frame);

#endif

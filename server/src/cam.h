#ifndef _CAM_H_
#define _CAM_H_

bool cam_open(int cam_id, int width, int height, bool preview);

/** yv12_frame: rows = height * 1.5, cols = width */
bool cam_yv12_frame(char* yv12_frame);

#endif

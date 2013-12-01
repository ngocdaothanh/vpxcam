#ifndef _CAM_H_
#define _CAM_H_

bool cam_open(int cam_id, bool preview);

int cam_width();

int cam_height();

/** yv12_frame: rows = height * 1.5, cols = width */
bool cam_yv12_frame(char* yv12_frame);

#endif

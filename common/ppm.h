/**
 * \file ppm.h
 * \author lihw (lihw81@gmail.com)
 * \changelog
 * - 2019/09/03 initial check in
 *
 * Read and write to .ppm image files.
 */

#ifndef PPM_H
#define PPM_H


extern unsigned char* ReadPPM(char* filename, int* width, int* height);

extern bool WritePPM(char* filename, int width, int height, const unsigned char *image);



#endif // !PPM_H

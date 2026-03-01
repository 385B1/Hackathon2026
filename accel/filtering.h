#ifndef FILTERS
#define FILTERS

bool hasNearbyPixel(int r, int c, bool pixels[7][7]);

void cleanNoise(bool pixels[7][7]);

#endif
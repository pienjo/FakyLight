#pragma once

#include <inttypes.h>
#include <sys/types.h>
#include "imageType.h"

void RGBtoHSV(uint8_t r, uint8_t g, uint8_t b, uint8_t *h, uint8_t *s, uint8_t *v);
void HSVtoRGB(uint8_t h, uint8_t s, uint8_t v, uint8_t *r, uint8_t *g, uint8_t *b);

uint32_t getHueValueHistogram(struct image *img, uint32_t *histogram, uint32_t *valueHistogram, uint8_t *avgSat, int left, int top, int right, int bottom);


// Compatibility shim: Tracy sources include "stb_image_resize.h" (a copy of
// stb_image_resize 0.97) and call its stbir_resize_uint8(). stb_image_resize2
// replaces that function with stbir_resize_uint8_linear(), which takes a
// pixel layout instead of a channel count. As its migration notes suggest,
// the channel count is passed as the pixel layout.
//
#pragma once

#include <stb_image_resize2.h>

static inline int
stbir_resize_uint8 (const unsigned char* input_pixels,
                    int input_w, int input_h, int input_stride_in_bytes,
                    unsigned char* output_pixels,
                    int output_w, int output_h, int output_stride_in_bytes,
                    int num_channels)
{
  return stbir_resize_uint8_linear (input_pixels,
                                    input_w, input_h, input_stride_in_bytes,
                                    output_pixels,
                                    output_w, output_h, output_stride_in_bytes,
                                    (stbir_pixel_layout) num_channels) != 0;
}

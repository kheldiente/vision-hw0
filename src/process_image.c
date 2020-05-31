#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "image.h"

float get_pixel(image im, int x, int y, int c)
{
    int maxIndex = (im.h * im.w * im.c) - 1;
    int calcXIndex = x;
    int calcYIndex = (y - 1) * im.w;
    int calcCIndex = c * im.h * im.w;
    int calcIndex = (calcXIndex + calcYIndex + calcCIndex) - 1;

    // printf("---------------------\n");
    // printf("getPixel()\n");
    // printf("---------------------\n");
    // printf("x: %d\n", x);
    // printf("y: %d\n", y);
    // printf("c: %d\n", c + 1);
    // printf("calcXIndex: %d\n", calcXIndex);
    // printf("calcYIndex: %d\n", calcYIndex);
    // printf("calcCindex: %d\n", calcCIndex);
    // printf("----------------------\n");
    // printf("width: %d\n", im.w);
    // printf("height: %d\n", im.h);
    // printf("channels: %d\n", im.c);
    // printf("max index: %d\n", maxIndex);
    // printf("calc index: %d\n", calcIndex);
    // printf("---------------------\n");

    if (calcIndex > maxIndex) {
        printf("Index out of bounds. No pixel value.");
        return 0;
    }
    return im.data[calcIndex];
}

void set_pixel(image im, int x, int y, int c, float v)
{
    int maxIndex = (im.h * im.w * im.c) - 1;
    int calcXIndex = x;
    int calcYIndex = (y - 1) * im.w;
    int calcCIndex = c * im.h * im.w;
    int calcIndex = (calcXIndex + calcYIndex + calcCIndex) - 1;

    // printf("---------------------\n");
    // printf("set_pixel()\n");
    // printf("---------------------\n");
    // printf("x: %d\n", x);
    // printf("y: %d\n", y);
    // printf("c: %d\n", c + 1);
    // printf("calcXIndex: %d\n", calcXIndex);
    // printf("calcYIndex: %d\n", calcYIndex);
    // printf("calcCindex: %d\n", calcCIndex);
    // printf("----------------------\n");
    // printf("width: %d\n", im.w);
    // printf("height: %d\n", im.h);
    // printf("channels: %d\n", im.c);
    // printf("max index: %d\n", maxIndex);
    // printf("calc index: %d\n", calcIndex);
    // printf("---------------------\n");

    if (calcIndex > maxIndex) {
        printf("Index out of bounds. No pixel value.");
        return;
    }
    im.data[calcIndex] = v;
}

image copy_image(image im)
{
    image copy = make_image(im.w, im.h, im.c);

    for (int i = 0;i < im.h;i++) {
        for (int j = 0;j < im.w;j++) {
            float r = get_pixel(im, j, i, 0);
            float g = get_pixel(im, j, i, 1);
            float b = get_pixel(im, j, i, 2);
            
            set_pixel(copy, j, i, 0, r);
            set_pixel(copy, j, i, 1, g);
            set_pixel(copy, j, i, 2, b);
        }
    }
    return copy;
}

image rgb_to_grayscale(image im)
{
    assert(im.c == 3);
    image gray = make_image(im.w, im.h, 3);
    
    /**
     * Video engineers use a calculation called luma to find an approximation 
     * of perceptual intensity when encoding video signal, 
     * we'll use that to convert our image to grayscale. 
     * It operates directly on the gamma compressed sRGB values that we already have! 
     * We simply perform a weighted sum:
     * Y' = 0.299 R' + 0.587 G' + .114 B'
    **/

    for (int i = 0;i < im.h;i++) {
        for (int j = 0;j < im.w;j++) {  
            float r = get_pixel(im, j, i, 0);
            float g = get_pixel(im, j, i, 1);
            float b = get_pixel(im, j, i, 2);

            float rToGray = r * 0.299;
            float gToGray = g * 0.587;
            float bToGray = b * 0.114;

            float wSum = rToGray + gToGray + bToGray;

            set_pixel(gray, j, i, 0, r + wSum);
            set_pixel(gray, j, i, 1, g + wSum);
            set_pixel(gray, j, i, 2, b + wSum);
        }
    }
    return gray;
}

void shift_image(image im, int c, float v)
{
    for (int i = 0;i < im.h;i++) {
        for (int j = 0;j < im.w;j++) {  
            float pixel = get_pixel(im, j, i, c);
            set_pixel(im, j, i, c, pixel + v);
        }
    }
}

void scale_image(image im, int c, float v)
{
    for (int i = 0;i < im.h;i++) {
        for (int j = 0;j < im.w;j++) {  
            float pixel = get_pixel(im, j, i, c);
            set_pixel(im, j, i, c, pixel * v);
        }
    }
}

void clamp_image(image im)
{
    // Same with shifting image but need to check if
    // pixel value exceeds 0 and 1.
    // If it less than 0, pixel value should be 0.
    // While if greater than 1, pixel value should be 1.
    for (int i = 0;i < im.h;i++) {
        for (int j = 0;j < im.w;j++) {  
            for (int c = 0;c < im.c;c++) {
                float pixel = get_pixel(im, j, i, c);
                if (pixel < 0) {
                    pixel = 0;
                }
                if (pixel > 1) {
                    pixel = 1;
                }

                set_pixel(im, j, i, c, pixel);
            }
        }
    }
}


// These might be handy
float three_way_max(float a, float b, float c)
{
    return (a > b) ? ( (a > c) ? a : c) : ( (b > c) ? b : c) ;
}

float three_way_min(float a, float b, float c)
{
    return (a < b) ? ( (a < c) ? a : c) : ( (b < c) ? b : c) ;
}

void rgb_to_hsv(image im)
{
    for (int i = 0;i < im.h;i++) {
        for (int j = 0;j < im.w;j++) {
            float r = get_pixel(im, j, i, 0);
            float g = get_pixel(im, j, i, 1);
            float b = get_pixel(im, j, i, 2);
            float channels = im.c; 

            float v = three_way_max(r, g, b); // For now, value is blue channel
            float s = (v - three_way_min(r, g, b)) / v; // For now, saturation is green channel
            float h = h = (g - b) / channels; // For now, hue is red channel

            if (h < 0) {
                h = (h / 6) + 1;
            } else {
                h = h / 6;
            }

            // for (int c = 0;c < im.c;c++) {
            //     if (c == 0) {
            //         h = (g - b) / channels;
            //     } else if (c == 1) {
            //         h = ((b - r) / channels) + 2;
            //     } else if (c == 2) {
            //         h = ((r - g) / channels) + 4;
            //     }

            //     if (h < 0) {
            //         h = (h / 6) + 1;
            //     } else {
            //         h = h / 6;
            //     }
            // }

            set_pixel(im, j, i, 0, h);
            set_pixel(im, j, i, 1, s);
            set_pixel(im, j, i, 2, v);
        }
    }
}

void hsv_to_rgb(image im)
{
    // for (int i = 0;i < im.h;i++) {
    //     for (int j = 0;j < im.w;j++) {
    //         float h = get_pixel(im, j, i, 0);
    //         float s = get_pixel(im, j, i, 1);
    //         float v = get_pixel(im, j, i, 2);

    //         // https://stackoverflow.com/questions/51203917/math-behind-hsv-to-rgb-conversion-of-colors
    //         float step = h / (1 / 6);
    //         float pos = step - floor(step); // the hue position within the current step
    //         int fstep = floor(step);
    //         float m = (fstep % 2) ? (1 - pos) * v : pos * v; // mix color value adjusted to the brightness(v)
    //         float max = 1 * v;
    //         float min = (1 - s) * v;
    //         float med = m + ((1 - s) * (v - m));
    //         float r, g, b;

    //         switch (fstep) {
    //             case 0:
    //                 r = max;
    //                 g = med;
    //                 b = min;
    //                 break;
    //             case 1:
    //                 r = med;
    //                 g = max;
    //                 b = min;
    //                 break;
    //             case 2:
    //                 r = min;
    //                 g = max;
    //                 b = med;
    //                 break;
    //             case 3:
    //                 r = min;
    //                 g = med;
    //                 b = max;
    //                 break;
    //             case 4:
    //                 r = med;
    //                 g = min;
    //                 b = max;
    //                 break;
    //             case 5:
    //                 r = max;
    //                 g = min;
    //                 b = med;
    //                 break;
    //         }

    //         set_pixel(im, j, i, 0, r);
    //         set_pixel(im, j, i, 1, g);
    //         set_pixel(im, j, i, 2, b);
    //     }
    // }
}

from uwimg import *

# 1. Getting and setting pixels
im = load_image("data/dog.jpg")
imName = "dog_no_red"
print("Removing red colors...")
for row in range(im.h):
    for col in range(im.w):
        set_pixel(im, col, row, 0, 0)
save_image(im, imName)
print "Removed red colors. See {}".format(imName)

# 2. Copy image
print("Copying image...")
im2 = copy_image(im)
imName2 = "copy2"
save_image(im2, imName2)
print "Copied image. See {}".format(imName2)

# 3. Grayscale image
im = load_image("data/colorbar.png")
graybar = rgb_to_grayscale(im)
save_image(graybar, "graybar")

# 4. Shift Image
im = load_image("data/dog.jpg")
shift_image(im, 0, .4)
shift_image(im, 1, .4)
shift_image(im, 2, .4)
save_image(im, "overflow")

# 5. Clamp Image
clamp_image(im)
save_image(im, "doglight_fixed")

# 6-7. Colorspace and saturation
im = load_image("data/dog.jpg")
rgb_to_hsv(im)
shift_image(im, 1, .2)
# scale_image(im, 1, 2)
clamp_image(im)
hsv_to_rgb(im)
save_image(im, "dog_saturated")



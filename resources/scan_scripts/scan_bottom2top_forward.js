/*
 * bottom to top
 * forward
 */

for (y = image.height - 1; y >= 0; y--) {
    for (x = 0; x < image.width; x++) {
        image.addPoint(x, y);
    }
}

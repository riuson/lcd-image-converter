/*
 * left to right
 * forward
 */

for (x = 0; x < image.width - 1; x++) {
    for (y = 0; y < image.height - 1; y++) {
        image.addPoint(x, y);
    }
}

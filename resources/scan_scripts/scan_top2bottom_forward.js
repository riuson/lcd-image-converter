/*
 * top to bottom
 * forward
 */

for (y = 0; y < image.height(); y++) {
    for (x = 0; x < image.width(); x++) {
        image.addPoint(x, y);
    }
}

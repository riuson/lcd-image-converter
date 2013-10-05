/*
 * top to bottom
 * forward
 * bands
 */

bandX = 0;

do {
    for (y = 0; y < image.height(); y++) {
        for (x = 0; x < image.bandSize(); x++) {
            image.addPoint(bandX + x, y);
        }
    }
    bandX += image.bandSize();
} while (bandX < image.width());

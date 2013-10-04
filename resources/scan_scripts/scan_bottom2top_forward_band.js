/*
 * bottom to top
 * forward
 * bands
 */

bandX = 0;

do {
    for (y = image.height - 1; y >= 0; y--) {
        for (x = 0; x < image.bandSize; x++) {
            image.addPoint(bandX + x, y);
        }
    }
    bandX += bandSize;
} while (bandX < im.width());

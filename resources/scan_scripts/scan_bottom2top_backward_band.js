/*
 * bottom to top
 * backward
 * bands
 */

bandX = image.width - 1;

do {
    for (y = image.height - 1; y >= 0; y--) {
        for (x = 0; x < image.bandSize; x++) {
            image.addPoint(bandX - x, y);
        }
    }
    bandX -= image.bandSize;
} while (bandX >= 0);

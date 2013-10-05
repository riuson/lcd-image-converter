/*
 * top to bottom
 * backward
 * bands
 */

bandX = image.width - 1;

do {
    for (y = 0; y < image.height; y++) {
        for (x = 0; x < image.bandSize; x++) {
            image.addPoint(bandX - x, y);
        }
    }
    bandX -= image.bandSize;
} while (bandX >= 0);

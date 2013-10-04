/*
 * left to right
 * backward
 * bands
 */

bandY = image.height - 1;

do {
    for (x = 0; x < image.width; x++) {
        for (y = 0; y < image.bandSize; y++) {
            image.addPoint(x, bandY - y);
        }
    }
    bandY -= image.bandSize;
} while (bandY >= 0);

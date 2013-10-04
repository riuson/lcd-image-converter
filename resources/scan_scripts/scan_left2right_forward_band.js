/*
 * left to right
 * forward
 * bands
 */

bandY = 0;

do {
    for (x = 0; x < image.width; x++) {
        for (y = 0; y < image.bandSize; y++) {
            image.addPoint(x, bandY + y);
        }
    }
    bandY += image.bandSize;
} while (bandY < image.height);

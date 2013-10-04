/*
 * right to left
 * forward
 * bands
 */

bandY = 0;

do {
    for (x = image.width - 1; x >= 0; x--) {
        for (y = 0; y < image.bandSize; y++) {
            image.addPoint(x, bandY + y);
        }
    }
    bandY += image.bandSize;
} while (bandY < image.height);

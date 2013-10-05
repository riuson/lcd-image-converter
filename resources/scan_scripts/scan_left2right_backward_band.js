/*
 * left to right
 * backward
 * bands
 */

var bandY = image.height - 1;

do {
    for (var x = 0; x < image.width; x++) {
        for (var y = 0; y < image.bandSize; y++) {
            image.addPoint(x, bandY - y);
        }
    }
    bandY -= image.bandSize;
} while (bandY >= 0);

/*
 * right to left
 * backward
 * bands
 */

var bandY = image.height - 1;

do {
    for (var x = image.width - 1; x >= 0; x--) {
        for (var y = 0; y < image.bandSize; y++) {
            image.addPoint(x, bandY - y);
        }
    }
    bandY -= image.bandSize;
} while (bandY >= 0);

/*
 * right to left
 * forward
 * bands
 */

var bandY = 0;

do {
    for (var x = image.width - 1; x >= 0; x--) {
        for (var y = 0; y < image.bandSize; y++) {
            image.addPoint(x, bandY + y);
        }
    }
    bandY += image.bandSize;
} while (bandY < image.height);

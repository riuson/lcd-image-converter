/*
 * left to right
 * forward
 * bands
 */

var bandY = 0;

do {
    for (var x = 0; x < image.width; x++) {
        for (var y = 0; y < image.bandSize; y++) {
            image.addPoint(x, bandY + y);
        }
    }
    bandY += image.bandSize;
} while (bandY < image.height);

/*
 * top to bottom
 * forward
 * bands
 */

var bandX = 0;

do {
    for (var y = 0; y < image.height; y++) {
        for (var x = 0; x < image.bandSize; x++) {
            image.addPoint(bandX + x, y);
        }
    }
    bandX += image.bandSize
} while (bandX < image.width);

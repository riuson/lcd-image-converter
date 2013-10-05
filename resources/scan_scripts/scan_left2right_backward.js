/*
 * left to right
 * backward
 */

for (var x = 0; x < image.width; x++) {
    for (var y = image.height - 1; y >= 0; y--) {
        image.addPoint(x, y);
    }
}

/*
 * right to left
 * backward
 */

for (var x = image.width - 1; x >= 0; x--) {
    for (var y = image.height - 1; y >= 0; y--) {
        image.addPoint(x, y);
    }
}

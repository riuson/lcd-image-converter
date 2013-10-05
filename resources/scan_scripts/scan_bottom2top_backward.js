/*
 * bottom to top
 * backward
 */

for (var y = image.height - 1; y >= 0; y--) {
    for (var x = image.width - 1; x >= 0; x--) {
        image.addPoint(x, y);
    }
}

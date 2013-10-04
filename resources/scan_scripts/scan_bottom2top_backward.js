/*
 * bottom to top
 * backward
 */

for (y = image.height - 1; y >= 0; y--) {
    for (x = image.width - 1; x >= 0; x--) {
        image.addPoint(x, y);
    }
}

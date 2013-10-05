/*
 * bottom to top
 * forward
 */

for (var y = image.height - 1; y >= 0; y--) {
    for (var x = 0; x < image.width; x++) {
        image.addPoint(x, y);
    }
}

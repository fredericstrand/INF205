int num_points(int r) {

    int count = 0;
    // Loop through area
    for (int x = 0; x <= r; x++) {
        for (int y = 0; y <= r; y++) {
            if (x * x + y *y <= r * r) {
                ++count;
            }
        }
    }
    // Multiplying by four for each quadrant
    count = 4 * count;

    // Adjust for points on the axes
    count -= 4 * r + 1;

    return count;
}
static void swapStops(void *stuff, int num)
{
    int i, n;
    CARD32 *stops;
    CARD16 *colors;
    stops = (CARD32 *)(stuff);
    for (i = 0; i < num; ++i) {
        swapl(stops, n);
        ++stops;
    }
    colors = (CARD16 *)(stops);
    for (i = 0; i < 4*num; ++i) {
        swaps(stops, n);
        ++stops;
    }
}

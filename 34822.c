barrier_inside_hit_box(struct PointerBarrier *barrier, int x, int y)
{
    int x1, x2, y1, y2;
    int dir;

    x1 = barrier->x1;
    x2 = barrier->x2;
    y1 = barrier->y1;
    y2 = barrier->y2;
    dir = ~(barrier->directions);

    if (barrier_is_vertical(barrier)) {
        if (dir & BarrierPositiveX)
            x1 -= HIT_EDGE_EXTENTS;
        if (dir & BarrierNegativeX)
            x2 += HIT_EDGE_EXTENTS;
    }
    if (barrier_is_horizontal(barrier)) {
        if (dir & BarrierPositiveY)
            y1 -= HIT_EDGE_EXTENTS;
        if (dir & BarrierNegativeY)
            y2 += HIT_EDGE_EXTENTS;
    }

    return x >= x1 && x <= x2 && y >= y1 && y <= y2;
}

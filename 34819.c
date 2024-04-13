barrier_clamp_to_barrier(struct PointerBarrier *barrier, int dir, int *x,
                         int *y)
{
    if (barrier_is_vertical(barrier)) {
        if ((dir & BarrierNegativeX) & ~barrier->directions)
            *x = barrier->x1;
        if ((dir & BarrierPositiveX) & ~barrier->directions)
            *x = barrier->x1 - 1;
    }
    if (barrier_is_horizontal(barrier)) {
        if ((dir & BarrierNegativeY) & ~barrier->directions)
            *y = barrier->y1;
        if ((dir & BarrierPositiveY) & ~barrier->directions)
            *y = barrier->y1 - 1;
    }
}

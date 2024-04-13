barrier_get_direction(int x1, int y1, int x2, int y2)
{
    int direction = 0;

    /* which way are we trying to go */
    if (x2 > x1)
        direction |= BarrierPositiveX;
    if (x2 < x1)
        direction |= BarrierNegativeX;
    if (y2 > y1)
        direction |= BarrierPositiveY;
    if (y2 < y1)
        direction |= BarrierNegativeY;

    return direction;
}

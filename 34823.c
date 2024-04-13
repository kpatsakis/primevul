barrier_is_blocking(const struct PointerBarrier * barrier,
                    int x1, int y1, int x2, int y2, double *distance)
{
    if (barrier_is_vertical(barrier)) {
        float t, y;
        t = T(barrier->x1, x1, x2);
        if (t < 0 || t > 1)
            return FALSE;

        /* Edge case: moving away from barrier. */
        if (x2 > x1 && t == 0)
            return FALSE;

        y = F(t, y1, y2);
        if (!inside_segment(y, barrier->y1, barrier->y2))
            return FALSE;

        *distance = sqrt((pow(y - y1, 2) + pow(barrier->x1 - x1, 2)));
        return TRUE;
    }
    else {
        float t, x;
        t = T(barrier->y1, y1, y2);
        if (t < 0 || t > 1)
            return FALSE;

        /* Edge case: moving away from barrier. */
        if (y2 > y1 && t == 0)
            return FALSE;

        x = F(t, x1, x2);
        if (!inside_segment(x, barrier->x1, barrier->x2))
            return FALSE;

        *distance = sqrt((pow(x - x1, 2) + pow(barrier->y1 - y1, 2)));
        return TRUE;
    }
}

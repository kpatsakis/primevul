barrier_find_nearest(BarrierScreenPtr cs, DeviceIntPtr dev,
                     int dir,
                     int x1, int y1, int x2, int y2)
{
    struct PointerBarrierClient *c, *nearest = NULL;
    double min_distance = INT_MAX;      /* can't get higher than that in X anyway */

    xorg_list_for_each_entry(c, &cs->barriers, entry) {
        struct PointerBarrier *b = &c->barrier;
        struct PointerBarrierDevice *pbd;
        double distance;

        pbd = GetBarrierDevice(c, dev->id);
        if (pbd->seen)
            continue;

        if (!barrier_is_blocking_direction(b, dir))
            continue;

        if (!barrier_blocks_device(c, dev))
            continue;

        if (barrier_is_blocking(b, x1, y1, x2, y2, &distance)) {
            if (min_distance > distance) {
                min_distance = distance;
                nearest = c;
            }
        }
    }

    return nearest;
}

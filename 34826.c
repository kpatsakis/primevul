input_constrain_cursor(DeviceIntPtr dev, ScreenPtr screen,
                       int current_x, int current_y,
                       int dest_x, int dest_y,
                       int *out_x, int *out_y,
                       int *nevents, InternalEvent* events)
{
    /* Clamped coordinates here refer to screen edge clamping. */
    BarrierScreenPtr cs = GetBarrierScreen(screen);
    int x = dest_x,
        y = dest_y;
    int dir;
    struct PointerBarrier *nearest = NULL;
    PointerBarrierClientPtr c;
    Time ms = GetTimeInMillis();
    BarrierEvent ev = {
        .header = ET_Internal,
        .type = 0,
        .length = sizeof (BarrierEvent),
        .time = ms,
        .deviceid = dev->id,
        .sourceid = dev->id,
        .dx = dest_x - current_x,
        .dy = dest_y - current_y,
        .root = screen->root->drawable.id,
    };
    InternalEvent *barrier_events = events;
    DeviceIntPtr master;

    if (nevents)
        *nevents = 0;

    if (xorg_list_is_empty(&cs->barriers) || IsFloating(dev))
        goto out;

    /**
     * This function is only called for slave devices, but pointer-barriers
     * are for master-devices only. Flip the device to the master here,
     * continue with that.
     */
    master = GetMaster(dev, MASTER_POINTER);

    /* How this works:
     * Given the origin and the movement vector, get the nearest barrier
     * to the origin that is blocking the movement.
     * Clamp to that barrier.
     * Then, check from the clamped intersection to the original
     * destination, again finding the nearest barrier and clamping.
     */
    dir = barrier_get_direction(current_x, current_y, x, y);

    while (dir != 0) {
        int new_sequence;
        struct PointerBarrierDevice *pbd;

        c = barrier_find_nearest(cs, master, dir, current_x, current_y, x, y);
        if (!c)
            break;

        nearest = &c->barrier;

        pbd = GetBarrierDevice(c, master->id);
        new_sequence = !pbd->hit;

        pbd->seen = TRUE;
        pbd->hit = TRUE;

        if (pbd->barrier_event_id == pbd->release_event_id)
            continue;

        ev.type = ET_BarrierHit;
        barrier_clamp_to_barrier(nearest, dir, &x, &y);

        if (barrier_is_vertical(nearest)) {
            dir &= ~(BarrierNegativeX | BarrierPositiveX);
            current_x = x;
        }
        else if (barrier_is_horizontal(nearest)) {
            dir &= ~(BarrierNegativeY | BarrierPositiveY);
            current_y = y;
        }

        ev.flags = 0;
        ev.event_id = pbd->barrier_event_id;
        ev.barrierid = c->id;

        ev.dt = new_sequence ? 0 : ms - pbd->last_timestamp;
        ev.window = c->window;
        pbd->last_timestamp = ms;

        /* root x/y is filled in later */

        barrier_events->barrier_event = ev;
        barrier_events++;
        *nevents += 1;
    }

    xorg_list_for_each_entry(c, &cs->barriers, entry) {
        struct PointerBarrierDevice *pbd;
        int flags = 0;

        pbd = GetBarrierDevice(c, master->id);
        pbd->seen = FALSE;
        if (!pbd->hit)
            continue;

        if (barrier_inside_hit_box(&c->barrier, x, y))
            continue;

        pbd->hit = FALSE;

        ev.type = ET_BarrierLeave;

        if (pbd->barrier_event_id == pbd->release_event_id)
            flags |= XIBarrierPointerReleased;

        ev.flags = flags;
        ev.event_id = pbd->barrier_event_id;
        ev.barrierid = c->id;

        ev.dt = ms - pbd->last_timestamp;
        ev.window = c->window;
        pbd->last_timestamp = ms;

        /* root x/y is filled in later */

        barrier_events->barrier_event = ev;
        barrier_events++;
        *nevents += 1;

        /* If we've left the hit box, this is the
         * start of a new event ID. */
        pbd->barrier_event_id++;
    }

 out:
    *out_x = x;
    *out_y = y;
}

kick_delayed_job(tube t)
{
    int r;
    job j;
    size_t z;

    j = pq_take(&t->delay);
    if (!j) return 0;

    z = binlog_reserve_space_update(j);
    if (!z) return pq_give(&t->delay, j), 0; /* put it back */
    j->reserved_binlog_space += z;

    j->kick_ct++;
    r = enqueue_job(j, 0, 1);
    if (r == 1) return 1;

    /* ready queue is full, so delay it again */
    r = enqueue_job(j, j->delay, 0);
    if (r == 1) return 0;

    /* last resort */
    bury_job(j, 0);
    return 0;
}

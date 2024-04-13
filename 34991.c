kick_buried_job(tube t)
{
    int r;
    job j;
    size_t z;

    if (!buried_job_p(t)) return 0;
    j = remove_buried_job(t->buried.next);

    z = binlog_reserve_space_update(j);
    if (!z) return pq_give(&t->delay, j), 0; /* put it back */
    j->reserved_binlog_space += z;

    j->kick_ct++;
    r = enqueue_job(j, 0, 1);
    if (r == 1) return 1;

    /* ready queue is full, so bury it */
    bury_job(j, 0);
    return 0;
}

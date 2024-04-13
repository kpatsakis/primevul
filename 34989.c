h_conn_timeout(conn c)
{
    int r, should_timeout = 0;
    job j;

    /* Check if the client was trying to reserve a job. */
    if (conn_waiting(c) && conn_has_close_deadline(c)) should_timeout = 1;

    /* Check if any reserved jobs have run out of time. We should do this
     * whether or not the client is waiting for a new reservation. */
    while ((j = soonest_job(c))) {
        if (j->deadline_at >= now_usec()) break;

        /* This job is in the middle of being written out. If we return it to
         * the ready queue, someone might free it before we finish writing it
         * out to the socket. So we'll copy it here and free the copy when it's
         * done sending. */
        if (j == c->out_job) {
            c->out_job = job_copy(c->out_job);
        }

        timeout_ct++; /* stats */
        j->timeout_ct++;
        r = enqueue_job(remove_this_reserved_job(c, j), 0, 0);
        if (r < 1) bury_job(j, 0); /* out of memory, so bury it */
        r = conn_update_evq(c, c->evq.ev_events);
        if (r == -1) return twarnx("conn_update_evq() failed"), conn_close(c);
    }

    if (should_timeout) {
        dprintf("conn_waiting(%p) = %d\n", c, conn_waiting(c));
        return reply_msg(remove_waiting_conn(c), MSG_DEADLINE_SOON);
    } else if (conn_waiting(c) && c->pending_timeout >= 0) {
        dprintf("conn_waiting(%p) = %d\n", c, conn_waiting(c));
        c->pending_timeout = -1;
        return reply_msg(remove_waiting_conn(c), MSG_TIMED_OUT);
    }
}

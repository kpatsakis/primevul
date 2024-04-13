enqueue_incoming_job(conn c)
{
    int r;
    job j = c->in_job;

    c->in_job = NULL; /* the connection no longer owns this job */
    c->in_job_read = 0;

    /* check if the trailer is present and correct */
    if (memcmp(j->body + j->body_size - 2, "\r\n", 2)) {
        job_free(j);
        return reply_msg(c, MSG_EXPECTED_CRLF);
    }

    if (drain_mode) {
        job_free(j);
        return reply_serr(c, MSG_DRAINING);
    }

    if (j->reserved_binlog_space) return reply_serr(c, MSG_INTERNAL_ERROR);
    j->reserved_binlog_space = binlog_reserve_space_put(j);
    if (!j->reserved_binlog_space) return reply_serr(c, MSG_OUT_OF_MEMORY);

    /* we have a complete job, so let's stick it in the pqueue */
    r = enqueue_job(j, j->delay, 1);
    if (r < 0) return reply_serr(c, MSG_INTERNAL_ERROR);

    op_ct[OP_PUT]++; /* stats */
    global_stat.total_jobs_ct++;
    j->tube->stat.total_jobs_ct++;

    if (r == 1) return reply_line(c, STATE_SENDWORD, MSG_INSERTED_FMT, j->id);

    /* out of memory trying to grow the queue, so it gets buried */
    bury_job(j, 0);
    reply_line(c, STATE_SENDWORD, MSG_BURIED_FMT, j->id);
}

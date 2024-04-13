fmt_job_stats(char *buf, size_t size, job j)
{
    usec t;
    uint64_t time_left;

    t = now_usec();
    if (j->state == JOB_STATE_RESERVED || j->state == JOB_STATE_DELAYED) {
        time_left = (j->deadline_at - t) / 1000000;
    } else {
        time_left = 0;
    }
    return snprintf(buf, size, STATS_JOB_FMT,
            j->id,
            j->tube->name,
            job_state(j),
            j->pri,
            (t - j->created_at) / 1000000,
            j->delay / 1000000,
            j->ttr / 1000000,
            time_left,
            j->reserve_ct,
            j->timeout_ct,
            j->release_ct,
            j->bury_ct,
            j->kick_ct);
}

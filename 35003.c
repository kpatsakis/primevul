prot_replay_binlog(job binlog_jobs)
{
    job j, nj;
    usec delay;
    int r;

    for (j = binlog_jobs->next ; j != binlog_jobs ; j = nj) {
        nj = j->next;
        job_remove(j);
        binlog_reserve_space_update(j); /* reserve space for a delete */
        delay = 0;
        switch (j->state) {
        case JOB_STATE_BURIED:
            bury_job(j, 0);
            break;
        case JOB_STATE_DELAYED:
            if (started_at < j->deadline_at) {
                delay = j->deadline_at - started_at;
            }
            /* fall through */
        default:
            r = enqueue_job(j, delay, 0);
            if (r < 1) twarnx("error processing binlog job %llu", j->id);
        }
    }
}

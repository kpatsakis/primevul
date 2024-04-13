enqueue_reserved_jobs(conn c)
{
    int r;
    job j;

    while (job_list_any_p(&c->reserved_jobs)) {
        j = job_remove(c->reserved_jobs.next);
        r = enqueue_job(j, 0, 0);
        if (r < 1) bury_job(j, 0);
        global_stat.reserved_ct--;
        j->tube->stat.reserved_ct--;
        c->soonest_job = NULL;
        if (!job_list_any_p(&c->reserved_jobs)) conn_remove(c);
    }
}

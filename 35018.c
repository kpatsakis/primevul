touch_job(conn c, job j)
{
    j = find_reserved_job_in_conn(c, j);
    if (j) {
        j->deadline_at = now_usec() + j->ttr;
        c->soonest_job = NULL;
    }
    return j;
}

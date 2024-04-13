find_reserved_job_in_conn(conn c, job j)
{
    return (j && j->reserver == c && j->state == JOB_STATE_RESERVED) ? j : NULL;
}

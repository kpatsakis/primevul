remove_this_reserved_job(conn c, job j)
{
    j = job_remove(j);
    if (j) {
        global_stat.reserved_ct--;
        j->tube->stat.reserved_ct--;
        j->reserver = NULL;
    }
    c->soonest_job = NULL;
    if (!job_list_any_p(&c->reserved_jobs)) conn_remove(c);
    return j;
}

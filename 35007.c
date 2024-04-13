remove_buried_job(job j)
{
    if (!j || j->state != JOB_STATE_BURIED) return NULL;
    j = job_remove(j);
    if (j) {
        global_stat.buried_ct--;
        j->tube->stat.buried_ct--;
    }
    return j;
}

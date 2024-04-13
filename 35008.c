remove_ready_job(job j)
{
    if (!j || j->state != JOB_STATE_READY) return NULL;
    j = pq_remove(&j->tube->ready, j);
    if (j) {
        ready_ct--;
        if (j->pri < URGENT_THRESHOLD) {
            global_stat.urgent_ct--;
            j->tube->stat.urgent_ct--;
        }
    }
    return j;
}

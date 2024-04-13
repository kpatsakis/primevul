process_queue()
{
    job j;
    usec now = now_usec();

    dprintf("processing queue\n");
    while ((j = next_eligible_job(now))) {
        dprintf("got eligible job %llu in %s\n", j->id, j->tube->name);
        j = pq_take(&j->tube->ready);
        ready_ct--;
        if (j->pri < URGENT_THRESHOLD) {
            global_stat.urgent_ct--;
            j->tube->stat.urgent_ct--;
        }
        reserve_job(remove_waiting_conn(ms_take(&j->tube->waiting)), j);
    }
}

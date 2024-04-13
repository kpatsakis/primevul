enqueue_job(job j, usec delay, char update_store)
{
    int r;

    j->reserver = NULL;
    if (delay) {
        j->deadline_at = now_usec() + delay;
        r = pq_give(&j->tube->delay, j);
        if (!r) return 0;
        j->state = JOB_STATE_DELAYED;
        set_main_delay_timeout();
    } else {
        r = pq_give(&j->tube->ready, j);
        if (!r) return 0;
        j->state = JOB_STATE_READY;
        ready_ct++;
        if (j->pri < URGENT_THRESHOLD) {
            global_stat.urgent_ct++;
            j->tube->stat.urgent_ct++;
        }
    }

    if (update_store) {
        r = binlog_write_job(j);
        if (!r) return -1;
    }

    process_queue();
    return 1;
}

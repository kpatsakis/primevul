fmt_stats_tube(char *buf, size_t size, tube t)
{
    uint64_t time_left;

    if (t->pause > 0) {
        time_left = (t->deadline_at - now_usec()) / 1000000;
    } else {
        time_left = 0;
    }
    return snprintf(buf, size, STATS_TUBE_FMT,
            t->name,
            t->stat.urgent_ct,
            t->ready.used,
            t->stat.reserved_ct,
            t->delay.used,
            t->stat.buried_ct,
            t->stat.total_jobs_ct,
            t->using_ct,
            t->watching_ct,
            t->stat.waiting_ct,
            t->stat.pause_ct,
            t->pause / 1000000,
            time_left);
}

h_delay()
{
    int r;
    job j;
    usec now;
    int i;
    tube t;

    now = now_usec();
    while ((j = delay_q_peek())) {
        if (j->deadline_at > now) break;
        j = delay_q_take();
        r = enqueue_job(j, 0, 0);
        if (r < 1) bury_job(j, 0); /* out of memory, so bury it */
    }

    for (i = 0; i < tubes.used; i++) {
        t = tubes.items[i];

        dprintf("h_delay for %s t->waiting.used=%zu t->ready.used=%d t->pause=%" PRIu64 "\n",
                t->name, t->waiting.used, t->ready.used, t->pause);
        if (t->pause && t->deadline_at <= now) {
            t->pause = 0;
            process_queue();
        }
    }

    set_main_delay_timeout();
}

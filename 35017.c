set_main_delay_timeout()
{
    job j = delay_q_peek();
    tube t = pause_tube_peek();
    usec deadline_at = t ? t->deadline_at : 0;

    if (j && (!deadline_at || j->deadline_at < deadline_at)) deadline_at = j->deadline_at;

    dprintf("deadline_at=%" PRIu64 "\n", deadline_at);
    set_main_timeout(deadline_at);
}

delay_q_peek()
{
    int i;
    tube t;
    job j = NULL, nj;

    for (i = 0; i < tubes.used; i++) {
        t = tubes.items[i];
        nj = pq_peek(&t->delay);
        if (!nj) continue;
        if (!j || nj->deadline_at < j->deadline_at) j = nj;
    }

    return j;
}

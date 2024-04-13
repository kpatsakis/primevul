remove_waiting_conn(conn c)
{
    tube t;
    size_t i;

    if (!conn_waiting(c)) return NULL;

    c->type &= ~CONN_TYPE_WAITING;
    global_stat.waiting_ct--;
    for (i = 0; i < c->watch.used; i++) {
        t = c->watch.items[i];
        t->stat.waiting_ct--;
        ms_remove(&t->waiting, c);
    }
    return c;
}

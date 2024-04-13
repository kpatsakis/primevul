enqueue_waiting_conn(conn c)
{
    tube t;
    size_t i;

    global_stat.waiting_ct++;
    c->type |= CONN_TYPE_WAITING;
    for (i = 0; i < c->watch.used; i++) {
        t = c->watch.items[i];
        t->stat.waiting_ct++;
        ms_append(&t->waiting, c);
    }
}

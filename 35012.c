reply(conn c, const char *line, int len, int state)
{
    int r;

    if (!c) return;

    r = conn_update_evq(c, EV_WRITE | EV_PERSIST);
    if (r == -1) return twarnx("conn_update_evq() failed"), conn_close(c);

    c->reply = line;
    c->reply_len = len;
    c->reply_sent = 0;
    c->state = state;
    dprintf("sending reply: %.*s", len, line);
}

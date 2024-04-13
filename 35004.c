read_delay(usec *delay, const char *buf, char **end)
{
    int r;
    unsigned int delay_sec;

    r = read_pri(&delay_sec, buf, end);
    if (r) return r;
    *delay = ((usec) delay_sec) * 1000000;
    return 0;
}

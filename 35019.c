uptime()
{
    return (now_usec() - started_at) / 1000000;
}

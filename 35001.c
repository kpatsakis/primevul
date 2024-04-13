prot_init()
{
    started_at = now_usec();
    memset(op_ct, 0, sizeof(op_ct));

    ms_init(&tubes, NULL, NULL);

    TUBE_ASSIGN(default_tube, tube_find_or_make("default"));
    if (!default_tube) twarnx("Out of memory during startup!");
}

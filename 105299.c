static int parse_timeout(const char *target)
{
    QemuOptsList *list;
    QemuOpts *opts;
    const char *timeout;

    list = qemu_find_opts("iscsi");
    if (list) {
        opts = qemu_opts_find(list, target);
        if (!opts) {
            opts = QTAILQ_FIRST(&list->head);
        }
        if (opts) {
            timeout = qemu_opt_get(opts, "timeout");
            if (timeout) {
                return atoi(timeout);
            }
        }
    }

    return 0;
}

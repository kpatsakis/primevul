static void ga_disable_non_whitelisted(void)
{
    char **list_head, **list;
    bool whitelisted;
    int i;

    list_head = list = qmp_get_command_list();
    while (*list != NULL) {
        whitelisted = false;
        i = 0;
        while (ga_freeze_whitelist[i] != NULL) {
            if (strcmp(*list, ga_freeze_whitelist[i]) == 0) {
                whitelisted = true;
            }
            i++;
        }
        if (!whitelisted) {
            g_debug("disabling command: %s", *list);
            qmp_disable_command(*list);
        }
        g_free(*list);
        list++;
    }
    g_free(list_head);
}

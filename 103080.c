static void ga_enable_non_blacklisted(GList *blacklist)
{
    char **list_head, **list;

    list_head = list = qmp_get_command_list();
    while (*list != NULL) {
        if (g_list_find_custom(blacklist, *list, ga_strcmp) == NULL &&
            !qmp_command_is_enabled(*list)) {
            g_debug("enabling command: %s", *list);
            qmp_enable_command(*list);
        }
        g_free(*list);
        list++;
    }
    g_free(list_head);
}

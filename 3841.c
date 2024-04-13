irc_protocol_cap_print_cb (void *data,
                           struct t_hashtable *hashtable,
                           const char *key, const char *value)
{
    char **str_caps;

    /* make C compiler happy */
    (void) hashtable;

    str_caps = (char **)data;

    if (*str_caps[0])
        weechat_string_dyn_concat (str_caps, " ");
    weechat_string_dyn_concat (str_caps, key);
    if (value)
    {
        weechat_string_dyn_concat (str_caps, "=");
        weechat_string_dyn_concat (str_caps, value);
    }
}
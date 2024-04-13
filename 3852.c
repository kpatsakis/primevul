irc_protocol_get_message_tags (const char *tags)
{
    struct t_hashtable *hashtable;
    char **items, *pos, *key;
    int num_items, i;

    if (!tags || !tags[0])
        return NULL;

    hashtable = weechat_hashtable_new (32,
                                       WEECHAT_HASHTABLE_STRING,
                                       WEECHAT_HASHTABLE_STRING,
                                       NULL, NULL);
    if (!hashtable)
        return NULL;

    items = weechat_string_split (tags, ";", NULL,
                                  WEECHAT_STRING_SPLIT_STRIP_LEFT
                                  | WEECHAT_STRING_SPLIT_STRIP_RIGHT
                                  | WEECHAT_STRING_SPLIT_COLLAPSE_SEPS,
                                  0, &num_items);
    if (items)
    {
        for (i = 0; i < num_items; i++)
        {
            pos = strchr (items[i], '=');
            if (pos)
            {
                /* format: "tag=value" */
                key = weechat_strndup (items[i], pos - items[i]);
                if (key)
                {
                    weechat_hashtable_set (hashtable, key, pos + 1);
                    free (key);
                }
            }
            else
            {
                /* format: "tag" */
                weechat_hashtable_set (hashtable, items[i], NULL);
            }
        }
        weechat_string_free_split (items);
    }

    return hashtable;
}
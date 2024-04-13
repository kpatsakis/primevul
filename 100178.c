static void _addsubs(struct list_rock *rock)
{
    if (!rock->subs) return;
    if (!rock->last_mbentry) return;
    int i;
    const char *last_name = rock->last_mbentry->name;
    int namelen = strlen(last_name);
    for (i = 0; i < rock->subs->count; i++) {
        const char *name = strarray_nth(rock->subs, i);
        if (strncmp(last_name, name, namelen))
            continue;
        else if (!name[namelen]) {
            if ((rock->last_attributes & MBOX_ATTRIBUTE_NONEXISTENT))
                rock->last_attributes |= MBOX_ATTRIBUTE_CHILDINFO_SUBSCRIBED;
            else
                rock->last_attributes |= MBOX_ATTRIBUTE_SUBSCRIBED;
        }
        else if (name[namelen] == '.')
            rock->last_attributes |= MBOX_ATTRIBUTE_CHILDINFO_SUBSCRIBED;
    }
}

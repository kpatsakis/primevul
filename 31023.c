core_get_type(const xEvent *event)
{
    int type = event->u.u.type;

    return ((type & EXTENSION_EVENT_BASE) || type == GenericEvent) ? 0 : type;
}

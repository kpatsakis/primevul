static struct PointerBarrierDevice *AllocBarrierDevice(void)
{
    struct PointerBarrierDevice *pbd = NULL;

    pbd = malloc(sizeof(struct PointerBarrierDevice));
    if (!pbd)
        return NULL;

    pbd->deviceid = -1; /* must be set by caller */
    pbd->barrier_event_id = 1;
    pbd->release_event_id = 0;
    pbd->hit = FALSE;
    pbd->seen = FALSE;
    xorg_list_init(&pbd->entry);

    return pbd;
}

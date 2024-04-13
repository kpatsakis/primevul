XIDestroyPointerBarrier(ClientPtr client,
                        xXFixesDestroyPointerBarrierReq * stuff)
{
    int err;
    void *barrier;

    err = dixLookupResourceByType((void **) &barrier, stuff->barrier,
                                  PointerBarrierType, client, DixDestroyAccess);
    if (err != Success) {
        client->errorValue = stuff->barrier;
        return err;
    }

    if (CLIENT_ID(stuff->barrier) != client->index)
        return BadAccess;

    FreeResource(stuff->barrier, RT_NONE);
    return Success;
}

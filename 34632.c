ProcNoOperation(ClientPtr client)
{
    REQUEST_AT_LEAST_SIZE(xReq);

    /* noop -- don't do anything */
    return Success;
}

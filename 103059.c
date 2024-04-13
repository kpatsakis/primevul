static void lsi_skip_msgbytes(LSIState *s, unsigned int n)
{
    s->dnad += n;
    s->dbc  -= n;
}

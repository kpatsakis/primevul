static int ndis_set(USBNetState *s, uint32_t oid,
                uint8_t *inbuf, unsigned int inlen)
{
    switch (oid) {
    case OID_GEN_CURRENT_PACKET_FILTER:
        s->filter = le32_to_cpup((le32 *) inbuf);
        if (s->filter) {
            s->rndis_state = RNDIS_DATA_INITIALIZED;
        } else {
            s->rndis_state = RNDIS_INITIALIZED;
        }
        return 0;

    case OID_802_3_MULTICAST_LIST:
        return 0;
    }
    return -1;
}

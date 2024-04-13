 static int ndis_query(USBNetState *s, uint32_t oid,
                      size_t outlen)
{
    unsigned int i;

    switch (oid) {
    /* general oids (table 4-1) */
    /* mandatory */
    case OID_GEN_SUPPORTED_LIST:
        for (i = 0; i < ARRAY_SIZE(oid_supported_list); i++)
            ((le32 *) outbuf)[i] = cpu_to_le32(oid_supported_list[i]);
        return sizeof(oid_supported_list);

    /* mandatory */
    case OID_GEN_HARDWARE_STATUS:
        *((le32 *) outbuf) = cpu_to_le32(0);
        return sizeof(le32);

    /* mandatory */
    case OID_GEN_MEDIA_SUPPORTED:
        *((le32 *) outbuf) = cpu_to_le32(s->medium);
        return sizeof(le32);

    /* mandatory */
    case OID_GEN_MEDIA_IN_USE:
        *((le32 *) outbuf) = cpu_to_le32(s->medium);
        return sizeof(le32);

    /* mandatory */
    case OID_GEN_MAXIMUM_FRAME_SIZE:
        *((le32 *) outbuf) = cpu_to_le32(ETH_FRAME_LEN);
        return sizeof(le32);

    /* mandatory */
    case OID_GEN_LINK_SPEED:
        *((le32 *) outbuf) = cpu_to_le32(s->speed);
        return sizeof(le32);

    /* mandatory */
    case OID_GEN_TRANSMIT_BLOCK_SIZE:
        *((le32 *) outbuf) = cpu_to_le32(ETH_FRAME_LEN);
        return sizeof(le32);

    /* mandatory */
    case OID_GEN_RECEIVE_BLOCK_SIZE:
        *((le32 *) outbuf) = cpu_to_le32(ETH_FRAME_LEN);
        return sizeof(le32);

    /* mandatory */
    case OID_GEN_VENDOR_ID:
        *((le32 *) outbuf) = cpu_to_le32(s->vendorid);
        return sizeof(le32);

    /* mandatory */
    case OID_GEN_VENDOR_DESCRIPTION:
        pstrcpy((char *)outbuf, outlen, "QEMU USB RNDIS Net");
        return strlen((char *)outbuf) + 1;

    case OID_GEN_VENDOR_DRIVER_VERSION:
        *((le32 *) outbuf) = cpu_to_le32(1);
        return sizeof(le32);

    /* mandatory */
    case OID_GEN_CURRENT_PACKET_FILTER:
        *((le32 *) outbuf) = cpu_to_le32(s->filter);
        return sizeof(le32);

    /* mandatory */
    case OID_GEN_MAXIMUM_TOTAL_SIZE:
        *((le32 *) outbuf) = cpu_to_le32(RNDIS_MAX_TOTAL_SIZE);
        return sizeof(le32);

    /* mandatory */
    case OID_GEN_MEDIA_CONNECT_STATUS:
        *((le32 *) outbuf) = cpu_to_le32(s->media_state);
        return sizeof(le32);

    case OID_GEN_PHYSICAL_MEDIUM:
        *((le32 *) outbuf) = cpu_to_le32(0);
        return sizeof(le32);

    case OID_GEN_MAC_OPTIONS:
        *((le32 *) outbuf) = cpu_to_le32(
                        NDIS_MAC_OPTION_RECEIVE_SERIALIZED |
                        NDIS_MAC_OPTION_FULL_DUPLEX);
        return sizeof(le32);

    /* statistics OIDs (table 4-2) */
    /* mandatory */
    case OID_GEN_XMIT_OK:
        *((le32 *) outbuf) = cpu_to_le32(0);
        return sizeof(le32);

    /* mandatory */
    case OID_GEN_RCV_OK:
        *((le32 *) outbuf) = cpu_to_le32(0);
        return sizeof(le32);

    /* mandatory */
    case OID_GEN_XMIT_ERROR:
        *((le32 *) outbuf) = cpu_to_le32(0);
        return sizeof(le32);

    /* mandatory */
    case OID_GEN_RCV_ERROR:
        *((le32 *) outbuf) = cpu_to_le32(0);
        return sizeof(le32);

    /* mandatory */
    case OID_GEN_RCV_NO_BUFFER:
        *((le32 *) outbuf) = cpu_to_le32(0);
        return sizeof(le32);

    /* ieee802.3 OIDs (table 4-3) */
    /* mandatory */
    case OID_802_3_PERMANENT_ADDRESS:
        memcpy(outbuf, s->conf.macaddr.a, 6);
        return 6;

    /* mandatory */
    case OID_802_3_CURRENT_ADDRESS:
        memcpy(outbuf, s->conf.macaddr.a, 6);
        return 6;

    /* mandatory */
    case OID_802_3_MULTICAST_LIST:
        *((le32 *) outbuf) = cpu_to_le32(0xe0000000);
        return sizeof(le32);

    /* mandatory */
    case OID_802_3_MAXIMUM_LIST_SIZE:
        *((le32 *) outbuf) = cpu_to_le32(1);
        return sizeof(le32);

    case OID_802_3_MAC_OPTIONS:
        return 0;

    /* ieee802.3 statistics OIDs (table 4-4) */
    /* mandatory */
    case OID_802_3_RCV_ERROR_ALIGNMENT:
        *((le32 *) outbuf) = cpu_to_le32(0);
        return sizeof(le32);

    /* mandatory */
    case OID_802_3_XMIT_ONE_COLLISION:
        *((le32 *) outbuf) = cpu_to_le32(0);
        return sizeof(le32);

    /* mandatory */
    case OID_802_3_XMIT_MORE_COLLISIONS:
        *((le32 *) outbuf) = cpu_to_le32(0);
        return sizeof(le32);

    default:
        fprintf(stderr, "usbnet: unknown OID 0x%08x\n", oid);
        return 0;
    }
    return -1;
}

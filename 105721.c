static ssize_t usbnet_receive(NetClientState *nc, const uint8_t *buf, size_t size)
{
    USBNetState *s = qemu_get_nic_opaque(nc);
    uint8_t *in_buf = s->in_buf;
    size_t total_size = size;

    if (!s->dev.config) {
        return -1;
    }

    if (is_rndis(s)) {
        if (s->rndis_state != RNDIS_DATA_INITIALIZED) {
            return -1;
        }
        total_size += sizeof(struct rndis_packet_msg_type);
    }
    if (total_size > sizeof(s->in_buf)) {
        return -1;
    }

    /* Only accept packet if input buffer is empty */
    if (s->in_len > 0) {
        return 0;
    }

    if (is_rndis(s)) {
        struct rndis_packet_msg_type *msg;

        msg = (struct rndis_packet_msg_type *)in_buf;
        memset(msg, 0, sizeof(struct rndis_packet_msg_type));
        msg->MessageType = cpu_to_le32(RNDIS_PACKET_MSG);
        msg->MessageLength = cpu_to_le32(size + sizeof(*msg));
        msg->DataOffset = cpu_to_le32(sizeof(*msg) - 8);
        msg->DataLength = cpu_to_le32(size);
        /* msg->OOBDataOffset;
         * msg->OOBDataLength;
         * msg->NumOOBDataElements;
         * msg->PerPacketInfoOffset;
         * msg->PerPacketInfoLength;
         * msg->VcHandle;
         * msg->Reserved;
         */
        in_buf += sizeof(*msg);
    }

    memcpy(in_buf, buf, size);
    s->in_len = total_size;
    s->in_ptr = 0;
    return size;
}

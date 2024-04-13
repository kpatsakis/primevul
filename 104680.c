pvscsi_send_msg(PVSCSIState *s, SCSIDevice *dev, uint32_t msg_type)
{
    if (s->msg_ring_info_valid && pvscsi_ring_msg_has_room(&s->rings)) {
        PVSCSIMsgDescDevStatusChanged msg = {0};

        msg.type = msg_type;
        msg.bus = dev->channel;
        msg.target = dev->id;
        msg.lun[1] = dev->lun;

        pvscsi_msg_ring_put(s, (PVSCSIRingMsgDesc *)&msg);
        pvscsi_ring_flush_msg(&s->rings);
        pvscsi_raise_message_interrupt(s);
    }
}

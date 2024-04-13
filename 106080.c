vmxnet3_receive(NetClientState *nc, const uint8_t *buf, size_t size)
{
    VMXNET3State *s = qemu_get_nic_opaque(nc);
    size_t bytes_indicated;

    if (!vmxnet3_can_receive(nc)) {
        VMW_PKPRN("Cannot receive now");
        return -1;
    }

    if (s->peer_has_vhdr) {
        vmxnet_rx_pkt_set_vhdr(s->rx_pkt, (struct virtio_net_hdr *)buf);
        buf += sizeof(struct virtio_net_hdr);
        size -= sizeof(struct virtio_net_hdr);
    }

    vmxnet_rx_pkt_set_packet_type(s->rx_pkt,
        get_eth_packet_type(PKT_GET_ETH_HDR(buf)));

    if (vmxnet3_rx_filter_may_indicate(s, buf, size)) {
        vmxnet_rx_pkt_attach_data(s->rx_pkt, buf, size, s->rx_vlan_stripping);
        bytes_indicated = vmxnet3_indicate_packet(s) ? size : -1;
        if (bytes_indicated < size) {
            VMW_PKPRN("RX: %lu of %lu bytes indicated", bytes_indicated, size);
        }
    } else {
        VMW_PKPRN("Packet dropped by RX filter");
        bytes_indicated = size;
    }

    assert(size > 0);
    assert(bytes_indicated != 0);
    return bytes_indicated;
}

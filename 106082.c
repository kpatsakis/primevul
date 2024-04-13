static void vmxnet3_reset_mac(VMXNET3State *s)
{
    memcpy(&s->conf.macaddr.a, &s->perm_mac.a, sizeof(s->perm_mac.a));
    VMW_CFPRN("MAC address set to: " VMXNET_MF, VMXNET_MA(s->conf.macaddr.a));
}

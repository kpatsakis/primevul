uint32_t ide_status_read(void *opaque, uint32_t addr)
{
    IDEBus *bus = opaque;
    IDEState *s = idebus_active_if(bus);
    int ret;

    if ((!bus->ifs[0].blk && !bus->ifs[1].blk) ||
        (s != bus->ifs && !s->blk)) {
        ret = 0;
    } else {
        ret = s->status;
    }
#ifdef DEBUG_IDE
    printf("ide: read status addr=0x%x val=%02x\n", addr, ret);
#endif
    return ret;
}

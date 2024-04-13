static void vapic_realize(DeviceState *dev, Error **errp)
{
    SysBusDevice *sbd = SYS_BUS_DEVICE(dev);
    VAPICROMState *s = VAPIC(dev);

    memory_region_init_io(&s->io, OBJECT(s), &vapic_ops, s, "kvmvapic", 2);
    sysbus_add_io(sbd, VAPIC_IO_PORT, &s->io);
    sysbus_init_ioports(sbd, VAPIC_IO_PORT, 2);

    option_rom[nb_option_roms].name = "kvmvapic.bin";
    option_rom[nb_option_roms].bootindex = -1;
    nb_option_roms++;
}

static void virtser_bus_dev_print(Monitor *mon, DeviceState *qdev, int indent)
{
    VirtIOSerialPort *port = DO_UPCAST(VirtIOSerialPort, dev, qdev);

    monitor_printf(mon, "%*sport %d, guest %s, host %s, throttle %s\n",
                   indent, "", port->id,
                   port->guest_connected ? "on" : "off",
                   port->host_connected ? "on" : "off",
                   port->throttled ? "on" : "off");
}
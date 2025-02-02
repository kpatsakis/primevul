static void check_cmd(AHCIState *s, int port)
{
    AHCIPortRegs *pr = &s->dev[port].port_regs;
    uint8_t slot;

    if ((pr->cmd & PORT_CMD_START) && pr->cmd_issue) {
        for (slot = 0; (slot < 32) && pr->cmd_issue; slot++) {
            if ((pr->cmd_issue & (1U << slot)) &&
                !handle_cmd(s, port, slot)) {
                pr->cmd_issue &= ~(1U << slot);
            }
        }
    }
}

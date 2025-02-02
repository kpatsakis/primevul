static void vmxnet3_handle_command(VMXNET3State *s, uint64_t cmd)
{
    s->last_command = cmd;

    switch (cmd) {
    case VMXNET3_CMD_GET_PERM_MAC_HI:
        VMW_CBPRN("Set: Get upper part of permanent MAC");
        break;

    case VMXNET3_CMD_GET_PERM_MAC_LO:
        VMW_CBPRN("Set: Get lower part of permanent MAC");
        break;

    case VMXNET3_CMD_GET_STATS:
        VMW_CBPRN("Set: Get device statistics");
        vmxnet3_fill_stats(s);
        break;

    case VMXNET3_CMD_ACTIVATE_DEV:
        VMW_CBPRN("Set: Activating vmxnet3 device");
        vmxnet3_activate_device(s);
        break;

    case VMXNET3_CMD_UPDATE_RX_MODE:
        VMW_CBPRN("Set: Update rx mode");
        vmxnet3_update_rx_mode(s);
        break;

    case VMXNET3_CMD_UPDATE_VLAN_FILTERS:
        VMW_CBPRN("Set: Update VLAN filters");
        vmxnet3_update_vlan_filters(s);
        break;

    case VMXNET3_CMD_UPDATE_MAC_FILTERS:
        VMW_CBPRN("Set: Update MAC filters");
        vmxnet3_update_mcast_filters(s);
        break;

    case VMXNET3_CMD_UPDATE_FEATURE:
        VMW_CBPRN("Set: Update features");
        vmxnet3_update_features(s);
        break;

    case VMXNET3_CMD_UPDATE_PMCFG:
        VMW_CBPRN("Set: Update power management config");
        vmxnet3_update_pm_state(s);
        break;

    case VMXNET3_CMD_GET_LINK:
        VMW_CBPRN("Set: Get link");
        break;

    case VMXNET3_CMD_RESET_DEV:
        VMW_CBPRN("Set: Reset device");
        vmxnet3_reset(s);
        break;

    case VMXNET3_CMD_QUIESCE_DEV:
        VMW_CBPRN("Set: VMXNET3_CMD_QUIESCE_DEV - pause the device");
        vmxnet3_deactivate_device(s);
        break;

    case VMXNET3_CMD_GET_CONF_INTR:
        VMW_CBPRN("Set: VMXNET3_CMD_GET_CONF_INTR - interrupt configuration");
        break;

    default:
        VMW_CBPRN("Received unknown command: %" PRIx64, cmd);
        break;
    }
}

static int rio_set_settings(struct net_device *dev, struct ethtool_cmd *cmd)
{
	struct netdev_private *np = netdev_priv(dev);
	netif_carrier_off(dev);
	if (cmd->autoneg == AUTONEG_ENABLE) {
		if (np->an_enable)
			return 0;
		else {
			np->an_enable = 1;
			mii_set_media(dev);
			return 0;
		}
	} else {
		np->an_enable = 0;
		if (np->speed == 1000) {
			ethtool_cmd_speed_set(cmd, SPEED_100);
			cmd->duplex = DUPLEX_FULL;
			printk("Warning!! Can't disable Auto negotiation in 1000Mbps, change to Manual 100Mbps, Full duplex.\n");
		}
		switch (ethtool_cmd_speed(cmd)) {
		case SPEED_10:
			np->speed = 10;
			np->full_duplex = (cmd->duplex == DUPLEX_FULL);
			break;
		case SPEED_100:
			np->speed = 100;
			np->full_duplex = (cmd->duplex == DUPLEX_FULL);
			break;
		case SPEED_1000: /* not supported */
		default:
			return -EINVAL;
		}
		mii_set_media(dev);
	}
	return 0;
}

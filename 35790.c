static int rio_get_settings(struct net_device *dev, struct ethtool_cmd *cmd)
{
	struct netdev_private *np = netdev_priv(dev);
	if (np->phy_media) {
		/* fiber device */
		cmd->supported = SUPPORTED_Autoneg | SUPPORTED_FIBRE;
		cmd->advertising= ADVERTISED_Autoneg | ADVERTISED_FIBRE;
		cmd->port = PORT_FIBRE;
		cmd->transceiver = XCVR_INTERNAL;
	} else {
		/* copper device */
		cmd->supported = SUPPORTED_10baseT_Half |
			SUPPORTED_10baseT_Full | SUPPORTED_100baseT_Half
			| SUPPORTED_100baseT_Full | SUPPORTED_1000baseT_Full |
			SUPPORTED_Autoneg | SUPPORTED_MII;
		cmd->advertising = ADVERTISED_10baseT_Half |
			ADVERTISED_10baseT_Full | ADVERTISED_100baseT_Half |
			ADVERTISED_100baseT_Full | ADVERTISED_1000baseT_Full|
			ADVERTISED_Autoneg | ADVERTISED_MII;
		cmd->port = PORT_MII;
		cmd->transceiver = XCVR_INTERNAL;
	}
	if ( np->link_status ) {
		ethtool_cmd_speed_set(cmd, np->speed);
		cmd->duplex = np->full_duplex ? DUPLEX_FULL : DUPLEX_HALF;
	} else {
		ethtool_cmd_speed_set(cmd, -1);
		cmd->duplex = -1;
	}
	if ( np->an_enable)
		cmd->autoneg = AUTONEG_ENABLE;
	else
		cmd->autoneg = AUTONEG_DISABLE;

	cmd->phy_address = np->phy_addr;
	return 0;
}

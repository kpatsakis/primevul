void init_bgscan_params(struct rsi_common *common)
{
	memset((u8 *)&common->bgscan, 0, sizeof(struct rsi_bgscan_params));
	common->bgscan.bgscan_threshold = RSI_DEF_BGSCAN_THRLD;
	common->bgscan.roam_threshold = RSI_DEF_ROAM_THRLD;
	common->bgscan.bgscan_periodicity = RSI_BGSCAN_PERIODICITY;
	common->bgscan.num_bgscan_channels = 0;
	common->bgscan.two_probe = 1;
	common->bgscan.active_scan_duration = RSI_ACTIVE_SCAN_TIME;
	common->bgscan.passive_scan_duration = RSI_PASSIVE_SCAN_TIME;
}
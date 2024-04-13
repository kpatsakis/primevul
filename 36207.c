static void igmp_ifc_event(struct in_device *in_dev)
{
	if (IGMP_V1_SEEN(in_dev) || IGMP_V2_SEEN(in_dev))
		return;
	in_dev->mr_ifc_count = in_dev->mr_qrv ? in_dev->mr_qrv :
		IGMP_Unsolicited_Report_Count;
	igmp_ifc_start_timer(in_dev, 1);
}

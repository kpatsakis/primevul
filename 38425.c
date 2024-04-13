static inline int qeth_get_initial_mtu_for_card(struct qeth_card *card)
{
	switch (card->info.type) {
	case QETH_CARD_TYPE_UNKNOWN:
		return 1500;
	case QETH_CARD_TYPE_IQD:
		return card->info.max_mtu;
	case QETH_CARD_TYPE_OSD:
		switch (card->info.link_type) {
		case QETH_LINK_TYPE_HSTR:
		case QETH_LINK_TYPE_LANE_TR:
			return 2000;
		default:
			return card->options.layer2 ? 1500 : 1492;
		}
	case QETH_CARD_TYPE_OSM:
	case QETH_CARD_TYPE_OSX:
		return card->options.layer2 ? 1500 : 1492;
	default:
		return 1500;
	}
}

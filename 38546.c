static void *lbs_tlv_find(uint16_t tlv_type, const uint8_t *tlv, uint16_t size)
{
	struct mrvl_ie_header *tlv_h;
	uint16_t length;
	ssize_t pos = 0;

	while (pos < size) {
		tlv_h = (struct mrvl_ie_header *) tlv;
		if (!tlv_h->len)
			return NULL;
		if (tlv_h->type == cpu_to_le16(tlv_type))
			return tlv_h;
		length = le16_to_cpu(tlv_h->len) + sizeof(*tlv_h);
		pos += length;
		tlv += length;
	}
	return NULL;
}

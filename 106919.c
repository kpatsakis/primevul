bool ship_v2N(unsigned int np, u_int8_t critical,
	      u_int8_t protoid, chunk_t *spi,
	      u_int16_t type, chunk_t *n_data, pb_stream *rbody)
{
	struct ikev2_notify n;
	pb_stream n_pbs;

	DBG(DBG_CONTROLMORE,
	    DBG_log("Adding a v2N Payload"));
	n.isan_np =  np;
	n.isan_critical = critical;
	if (DBGP(IMPAIR_SEND_BOGUS_ISAKMP_FLAG)) {
		libreswan_log(
			" setting bogus ISAKMP_PAYLOAD_LIBRESWAN_BOGUS flag in ISAKMP payload");
		n.isan_critical |= ISAKMP_PAYLOAD_LIBRESWAN_BOGUS;
	}

	n.isan_protoid =  protoid;
	n.isan_spisize = spi->len;
	n.isan_type = type;

	if (!out_struct(&n, &ikev2_notify_desc, rbody, &n_pbs)) {
		libreswan_log(
			"error initializing notify payload for notify message");
		return FALSE;
	}

	if (spi->len > 0) {
		if (!out_raw(spi->ptr, spi->len, &n_pbs, "SPI ")) {
			libreswan_log("error writing SPI to notify payload");
			return FALSE;
		}
	}
	if (n_data != NULL) {
		if (!out_raw(n_data->ptr, n_data->len, &n_pbs,
			     "Notify data")) {
			libreswan_log(
				"error writing notify payload for notify message");
			return FALSE;
		}
	}

	close_output_pbs(&n_pbs);
	return TRUE;
}

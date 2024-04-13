int tls1_ec_curve_id2nid(int curve_id)
	{
	/* ECC curves from draft-ietf-tls-ecc-12.txt (Oct. 17, 2005) */
	if ((curve_id < 1) || ((unsigned int)curve_id >
				sizeof(nid_list)/sizeof(nid_list[0])))
		return 0;
	return nid_list[curve_id-1];
	}

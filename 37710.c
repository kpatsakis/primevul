static bool ldm_parse_vblk (const u8 *buf, int len, struct vblk *vb)
{
	bool result = false;
	int r_objid;

	BUG_ON (!buf || !vb);

	r_objid = ldm_relative (buf, len, 0x18, 0);
	if (r_objid < 0) {
		ldm_error ("VBLK header is corrupt.");
		return false;
	}

	vb->flags  = buf[0x12];
	vb->type   = buf[0x13];
	vb->obj_id = ldm_get_vnum (buf + 0x18);
	ldm_get_vstr (buf+0x18+r_objid, vb->name, sizeof (vb->name));

	switch (vb->type) {
		case VBLK_CMP3:  result = ldm_parse_cmp3 (buf, len, vb); break;
		case VBLK_DSK3:  result = ldm_parse_dsk3 (buf, len, vb); break;
		case VBLK_DSK4:  result = ldm_parse_dsk4 (buf, len, vb); break;
		case VBLK_DGR3:  result = ldm_parse_dgr3 (buf, len, vb); break;
		case VBLK_DGR4:  result = ldm_parse_dgr4 (buf, len, vb); break;
		case VBLK_PRT3:  result = ldm_parse_prt3 (buf, len, vb); break;
		case VBLK_VOL5:  result = ldm_parse_vol5 (buf, len, vb); break;
	}

	if (result)
		ldm_debug ("Parsed VBLK 0x%llx (type: 0x%02x) ok.",
			 (unsigned long long) vb->obj_id, vb->type);
	else
		ldm_error ("Failed to parse VBLK 0x%llx (type: 0x%02x).",
			(unsigned long long) vb->obj_id, vb->type);

	return result;
}

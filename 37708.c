static bool ldm_parse_prt3(const u8 *buffer, int buflen, struct vblk *vb)
{
	int r_objid, r_name, r_size, r_parent, r_diskid, r_index, len;
	struct vblk_part *part;

	BUG_ON(!buffer || !vb);
	r_objid = ldm_relative(buffer, buflen, 0x18, 0);
	if (r_objid < 0) {
		ldm_error("r_objid %d < 0", r_objid);
		return false;
	}
	r_name = ldm_relative(buffer, buflen, 0x18, r_objid);
	if (r_name < 0) {
		ldm_error("r_name %d < 0", r_name);
		return false;
	}
	r_size = ldm_relative(buffer, buflen, 0x34, r_name);
	if (r_size < 0) {
		ldm_error("r_size %d < 0", r_size);
		return false;
	}
	r_parent = ldm_relative(buffer, buflen, 0x34, r_size);
	if (r_parent < 0) {
		ldm_error("r_parent %d < 0", r_parent);
		return false;
	}
	r_diskid = ldm_relative(buffer, buflen, 0x34, r_parent);
	if (r_diskid < 0) {
		ldm_error("r_diskid %d < 0", r_diskid);
		return false;
	}
	if (buffer[0x12] & VBLK_FLAG_PART_INDEX) {
		r_index = ldm_relative(buffer, buflen, 0x34, r_diskid);
		if (r_index < 0) {
			ldm_error("r_index %d < 0", r_index);
			return false;
		}
		len = r_index;
	} else {
		r_index = 0;
		len = r_diskid;
	}
	if (len < 0) {
		ldm_error("len %d < 0", len);
		return false;
	}
	len += VBLK_SIZE_PRT3;
	if (len > get_unaligned_be32(buffer + 0x14)) {
		ldm_error("len %d > BE32(buffer + 0x14) %d", len,
				get_unaligned_be32(buffer + 0x14));
		return false;
	}
	part = &vb->vblk.part;
	part->start = get_unaligned_be64(buffer + 0x24 + r_name);
	part->volume_offset = get_unaligned_be64(buffer + 0x2C + r_name);
	part->size = ldm_get_vnum(buffer + 0x34 + r_name);
	part->parent_id = ldm_get_vnum(buffer + 0x34 + r_size);
	part->disk_id = ldm_get_vnum(buffer + 0x34 + r_parent);
	if (vb->flags & VBLK_FLAG_PART_INDEX)
		part->partnum = buffer[0x35 + r_diskid];
	else
		part->partnum = 0;
	return true;
}

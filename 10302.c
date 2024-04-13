static bool __ne_get_resources(r_bin_ne_obj_t *bin) {
	if (!bin->resources) {
		bin->resources = r_list_newf (__free_resource);
	}
	ut16 resoff = bin->ne_header->ResTableOffset + bin->header_offset;
	ut16 alignment = r_buf_read_le16_at (bin->buf, resoff);
	ut32 off = resoff + 2;
	while (true) {
		NE_image_typeinfo_entry ti = {0};
		r_ne_resource *res = R_NEW0 (r_ne_resource);
		if (!res) {
			break;
		}
		res->entry = r_list_newf (__free_resource_entry);
		if (!res->entry) {
			break;
		}
		r_buf_read_at (bin->buf, off, (ut8 *)&ti, sizeof (ti));
		if (!ti.rtTypeID) {
			break;
		} else if (ti.rtTypeID & 0x8000) {
			res->name = __resource_type_str (ti.rtTypeID & ~0x8000);
		} else {
			// Offset to resident name table
			res->name = __read_nonnull_str_at (bin->buf, (ut64)resoff + ti.rtTypeID);
		}
		off += sizeof (NE_image_typeinfo_entry);
		int i;
		for (i = 0; i < ti.rtResourceCount; i++) {
			NE_image_nameinfo_entry ni;
			r_ne_resource_entry *ren = R_NEW0 (r_ne_resource_entry);
			if (!ren) {
				break;
			}
			r_buf_read_at (bin->buf, off, (ut8 *)&ni, sizeof (NE_image_nameinfo_entry));
			ren->offset = ni.rnOffset << alignment;
			ren->size = ni.rnLength;
			if (ni.rnID & 0x8000) {
				ren->name = r_str_newf ("%d", ni.rnID & ~0x8000);
			} else {
				// Offset to resident name table
				ren->name = __read_nonnull_str_at (bin->buf, (ut64)resoff + ni.rnID);
			}
			r_list_append (res->entry, ren);
			off += sizeof (NE_image_nameinfo_entry);
		}
		r_list_append (bin->resources, res);
	}
	return true;
}
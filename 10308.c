r_bin_ne_obj_t *r_bin_ne_new_buf(RBuffer *buf, bool verbose) {
	r_bin_ne_obj_t *bin = R_NEW0 (r_bin_ne_obj_t);
	if (!bin) {
		return NULL;
	}
	__init(buf, bin);
	return bin;
}
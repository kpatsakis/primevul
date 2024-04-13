static inline zend_bool check_str(const char *chk_str, size_t chk_len, const char *sep_str, size_t sep_len) {
	return 0 < sep_len && chk_len >= sep_len && *chk_str == *sep_str && !memcmp(chk_str + 1, sep_str + 1, sep_len - 1);
}

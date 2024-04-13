char *nl_nlmsg_flags2str(int flags, char *buf, size_t len)
{
	memset(buf, 0, len);

#define PRINT_FLAG(f) \
	if (flags & NLM_F_##f) { \
		flags &= ~NLM_F_##f; \
		strncat(buf, #f, len - strlen(buf) - 1); \
		if (flags) \
			strncat(buf, ",", len - strlen(buf) - 1); \
	}
	
	PRINT_FLAG(REQUEST);
	PRINT_FLAG(MULTI);
	PRINT_FLAG(ACK);
	PRINT_FLAG(ECHO);
	PRINT_FLAG(ROOT);
	PRINT_FLAG(MATCH);
	PRINT_FLAG(ATOMIC);
	PRINT_FLAG(REPLACE);
	PRINT_FLAG(EXCL);
	PRINT_FLAG(CREATE);
	PRINT_FLAG(APPEND);

	if (flags) {
		char s[32];
		snprintf(s, sizeof(s), "0x%x", flags);
		strncat(buf, s, len - strlen(buf) - 1);
	}
#undef PRINT_FLAG

	return buf;
}

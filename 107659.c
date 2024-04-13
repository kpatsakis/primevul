char * rpmFFlagsString(uint32_t fflags, const char *pad)
{
    char *fmt = NULL;
    rasprintf(&fmt, "%s%s%s%s%s%s%s%s",
		(fflags & RPMFILE_DOC) ? "d" : pad,
		(fflags & RPMFILE_CONFIG) ? "c" : pad,
		(fflags & RPMFILE_SPECFILE) ? "s" : pad,
		(fflags & RPMFILE_MISSINGOK) ? "m" : pad,
		(fflags & RPMFILE_NOREPLACE) ? "n" : pad,
		(fflags & RPMFILE_GHOST) ? "g" : pad,
		(fflags & RPMFILE_LICENSE) ? "l" : pad,
		(fflags & RPMFILE_README) ? "r" : pad);
    return fmt;
}

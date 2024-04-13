static int match_tcon(struct cifs_tcon *tcon, const char *unc)
{
	if (tcon->tidStatus == CifsExiting)
		return 0;
	if (strncmp(tcon->treeName, unc, MAX_TREE_SIZE))
		return 0;
	return 1;
}

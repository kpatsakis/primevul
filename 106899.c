static int build_ike_version()
{
return ((IKEv2_MAJOR_VERSION + (DBGP(IMPAIR_MAJOR_VERSION_BUMP) ? 1 : 0))
	<< ISA_MAJ_SHIFT) | (IKEv2_MINOR_VERSION +
	(DBGP(IMPAIR_MINOR_VERSION_BUMP) ? 1 : 0));
}

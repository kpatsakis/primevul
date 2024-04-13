match_security(struct TCP_Server_Info *server, struct smb_vol *vol)
{
	unsigned int secFlags;

	if (vol->secFlg & (~(CIFSSEC_MUST_SIGN | CIFSSEC_MUST_SEAL)))
		secFlags = vol->secFlg;
	else
		secFlags = global_secflags | vol->secFlg;

	switch (server->secType) {
	case LANMAN:
		if (!(secFlags & (CIFSSEC_MAY_LANMAN|CIFSSEC_MAY_PLNTXT)))
			return false;
		break;
	case NTLMv2:
		if (!(secFlags & CIFSSEC_MAY_NTLMV2))
			return false;
		break;
	case NTLM:
		if (!(secFlags & CIFSSEC_MAY_NTLM))
			return false;
		break;
	case Kerberos:
		if (!(secFlags & CIFSSEC_MAY_KRB5))
			return false;
		break;
	case RawNTLMSSP:
		if (!(secFlags & CIFSSEC_MAY_NTLMSSP))
			return false;
		break;
	default:
		/* shouldn't happen */
		return false;
	}

	/* now check if signing mode is acceptable */
	if ((secFlags & CIFSSEC_MAY_SIGN) == 0 &&
	    (server->sec_mode & SECMODE_SIGN_REQUIRED))
			return false;
	else if (((secFlags & CIFSSEC_MUST_SIGN) == CIFSSEC_MUST_SIGN) &&
		 (server->sec_mode &
		  (SECMODE_SIGN_ENABLED|SECMODE_SIGN_REQUIRED)) == 0)
			return false;

	return true;
}

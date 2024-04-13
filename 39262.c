cifs_set_vol_auth(struct smb_vol *vol, struct cifs_ses *ses)
{
	switch (ses->server->secType) {
	case Kerberos:
		vol->secFlg = CIFSSEC_MUST_KRB5;
		return 0;
	case NTLMv2:
		vol->secFlg = CIFSSEC_MUST_NTLMV2;
		break;
	case NTLM:
		vol->secFlg = CIFSSEC_MUST_NTLM;
		break;
	case RawNTLMSSP:
		vol->secFlg = CIFSSEC_MUST_NTLMSSP;
		break;
	case LANMAN:
		vol->secFlg = CIFSSEC_MUST_LANMAN;
		break;
	}

	return cifs_set_cifscreds(vol, ses);
}

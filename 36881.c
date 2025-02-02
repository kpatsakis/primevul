cifs_find_smb_ses(struct TCP_Server_Info *server, struct smb_vol *vol)
{
	struct cifsSesInfo *ses;

	spin_lock(&cifs_tcp_ses_lock);
	list_for_each_entry(ses, &server->smb_ses_list, smb_ses_list) {
		switch (server->secType) {
		case Kerberos:
			if (vol->cred_uid != ses->cred_uid)
				continue;
			break;
		default:
			/* anything else takes username/password */
			if (ses->user_name == NULL)
				continue;
			if (strncmp(ses->user_name, vol->username,
				    MAX_USERNAME_SIZE))
				continue;
			if (strlen(vol->username) != 0 &&
			    ses->password != NULL &&
			    strncmp(ses->password,
				    vol->password ? vol->password : "",
				    MAX_PASSWORD_SIZE))
				continue;
		}
		++ses->ses_count;
		spin_unlock(&cifs_tcp_ses_lock);
		return ses;
	}
	spin_unlock(&cifs_tcp_ses_lock);
	return NULL;
}

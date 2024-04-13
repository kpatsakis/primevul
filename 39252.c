cifs_parse_mount_options(const char *mountdata, const char *devname,
			 struct smb_vol *vol)
{
	char *data, *end;
	char *mountdata_copy = NULL, *options;
	unsigned int  temp_len, i, j;
	char separator[2];
	short int override_uid = -1;
	short int override_gid = -1;
	bool uid_specified = false;
	bool gid_specified = false;
	bool sloppy = false;
	char *invalid = NULL;
	char *nodename = utsname()->nodename;
	char *string = NULL;
	char *tmp_end, *value;
	char delim;
	bool got_ip = false;
	unsigned short port = 0;
	struct sockaddr *dstaddr = (struct sockaddr *)&vol->dstaddr;

	separator[0] = ',';
	separator[1] = 0;
	delim = separator[0];

	/* ensure we always start with zeroed-out smb_vol */
	memset(vol, 0, sizeof(*vol));

	/*
	 * does not have to be perfect mapping since field is
	 * informational, only used for servers that do not support
	 * port 445 and it can be overridden at mount time
	 */
	memset(vol->source_rfc1001_name, 0x20, RFC1001_NAME_LEN);
	for (i = 0; i < strnlen(nodename, RFC1001_NAME_LEN); i++)
		vol->source_rfc1001_name[i] = toupper(nodename[i]);

	vol->source_rfc1001_name[RFC1001_NAME_LEN] = 0;
	/* null target name indicates to use *SMBSERVR default called name
	   if we end up sending RFC1001 session initialize */
	vol->target_rfc1001_name[0] = 0;
	vol->cred_uid = current_uid();
	vol->linux_uid = current_uid();
	vol->linux_gid = current_gid();

	/* default to only allowing write access to owner of the mount */
	vol->dir_mode = vol->file_mode = S_IRUGO | S_IXUGO | S_IWUSR;

	/* vol->retry default is 0 (i.e. "soft" limited retry not hard retry) */
	/* default is always to request posix paths. */
	vol->posix_paths = 1;
	/* default to using server inode numbers where available */
	vol->server_ino = 1;

	/* default is to use strict cifs caching semantics */
	vol->strict_io = true;

	vol->actimeo = CIFS_DEF_ACTIMEO;

	/* FIXME: add autonegotiation -- for now, SMB1 is default */
	vol->ops = &smb1_operations;
	vol->vals = &smb1_values;

	if (!mountdata)
		goto cifs_parse_mount_err;

	mountdata_copy = kstrndup(mountdata, PAGE_SIZE, GFP_KERNEL);
	if (!mountdata_copy)
		goto cifs_parse_mount_err;

	options = mountdata_copy;
	end = options + strlen(options);

	if (strncmp(options, "sep=", 4) == 0) {
		if (options[4] != 0) {
			separator[0] = options[4];
			options += 5;
		} else {
			cifs_dbg(FYI, "Null separator not allowed\n");
		}
	}
	vol->backupuid_specified = false; /* no backup intent for a user */
	vol->backupgid_specified = false; /* no backup intent for a group */

	switch (cifs_parse_devname(devname, vol)) {
	case 0:
		break;
	case -ENOMEM:
		cifs_dbg(VFS, "Unable to allocate memory for devname.\n");
		goto cifs_parse_mount_err;
	case -EINVAL:
		cifs_dbg(VFS, "Malformed UNC in devname.\n");
		goto cifs_parse_mount_err;
	default:
		cifs_dbg(VFS, "Unknown error parsing devname.\n");
		goto cifs_parse_mount_err;
	}

	while ((data = strsep(&options, separator)) != NULL) {
		substring_t args[MAX_OPT_ARGS];
		unsigned long option;
		int token;

		if (!*data)
			continue;

		token = match_token(data, cifs_mount_option_tokens, args);

		switch (token) {

		/* Ingnore the following */
		case Opt_ignore:
			break;

		/* Boolean values */
		case Opt_user_xattr:
			vol->no_xattr = 0;
			break;
		case Opt_nouser_xattr:
			vol->no_xattr = 1;
			break;
		case Opt_forceuid:
			override_uid = 1;
			break;
		case Opt_noforceuid:
			override_uid = 0;
			break;
		case Opt_forcegid:
			override_gid = 1;
			break;
		case Opt_noforcegid:
			override_gid = 0;
			break;
		case Opt_noblocksend:
			vol->noblocksnd = 1;
			break;
		case Opt_noautotune:
			vol->noautotune = 1;
			break;
		case Opt_hard:
			vol->retry = 1;
			break;
		case Opt_soft:
			vol->retry = 0;
			break;
		case Opt_perm:
			vol->noperm = 0;
			break;
		case Opt_noperm:
			vol->noperm = 1;
			break;
		case Opt_mapchars:
			vol->remap = 1;
			break;
		case Opt_nomapchars:
			vol->remap = 0;
			break;
		case Opt_sfu:
			vol->sfu_emul = 1;
			break;
		case Opt_nosfu:
			vol->sfu_emul = 0;
			break;
		case Opt_nodfs:
			vol->nodfs = 1;
			break;
		case Opt_posixpaths:
			vol->posix_paths = 1;
			break;
		case Opt_noposixpaths:
			vol->posix_paths = 0;
			break;
		case Opt_nounix:
			vol->no_linux_ext = 1;
			break;
		case Opt_nocase:
			vol->nocase = 1;
			break;
		case Opt_brl:
			vol->nobrl =  0;
			break;
		case Opt_nobrl:
			vol->nobrl =  1;
			/*
			 * turn off mandatory locking in mode
			 * if remote locking is turned off since the
			 * local vfs will do advisory
			 */
			if (vol->file_mode ==
				(S_IALLUGO & ~(S_ISUID | S_IXGRP)))
				vol->file_mode = S_IALLUGO;
			break;
		case Opt_forcemandatorylock:
			vol->mand_lock = 1;
			break;
		case Opt_setuids:
			vol->setuids = 1;
			break;
		case Opt_nosetuids:
			vol->setuids = 0;
			break;
		case Opt_dynperm:
			vol->dynperm = true;
			break;
		case Opt_nodynperm:
			vol->dynperm = false;
			break;
		case Opt_nohard:
			vol->retry = 0;
			break;
		case Opt_nosoft:
			vol->retry = 1;
			break;
		case Opt_nointr:
			vol->intr = 0;
			break;
		case Opt_intr:
			vol->intr = 1;
			break;
		case Opt_nostrictsync:
			vol->nostrictsync = 1;
			break;
		case Opt_strictsync:
			vol->nostrictsync = 0;
			break;
		case Opt_serverino:
			vol->server_ino = 1;
			break;
		case Opt_noserverino:
			vol->server_ino = 0;
			break;
		case Opt_rwpidforward:
			vol->rwpidforward = 1;
			break;
		case Opt_cifsacl:
			vol->cifs_acl = 1;
			break;
		case Opt_nocifsacl:
			vol->cifs_acl = 0;
			break;
		case Opt_acl:
			vol->no_psx_acl = 0;
			break;
		case Opt_noacl:
			vol->no_psx_acl = 1;
			break;
		case Opt_locallease:
			vol->local_lease = 1;
			break;
		case Opt_sign:
			vol->secFlg |= CIFSSEC_MUST_SIGN;
			break;
		case Opt_seal:
			/* we do not do the following in secFlags because seal
			 * is a per tree connection (mount) not a per socket
			 * or per-smb connection option in the protocol
			 * vol->secFlg |= CIFSSEC_MUST_SEAL;
			 */
			vol->seal = 1;
			break;
		case Opt_noac:
			printk(KERN_WARNING "CIFS: Mount option noac not "
				"supported. Instead set "
				"/proc/fs/cifs/LookupCacheEnabled to 0\n");
			break;
		case Opt_fsc:
#ifndef CONFIG_CIFS_FSCACHE
			cifs_dbg(VFS, "FS-Cache support needs CONFIG_CIFS_FSCACHE kernel config option set\n");
			goto cifs_parse_mount_err;
#endif
			vol->fsc = true;
			break;
		case Opt_mfsymlinks:
			vol->mfsymlinks = true;
			break;
		case Opt_multiuser:
			vol->multiuser = true;
			break;
		case Opt_sloppy:
			sloppy = true;
			break;

		/* Numeric Values */
		case Opt_backupuid:
			if (get_option_uid(args, &vol->backupuid)) {
				cifs_dbg(VFS, "%s: Invalid backupuid value\n",
					 __func__);
				goto cifs_parse_mount_err;
			}
			vol->backupuid_specified = true;
			break;
		case Opt_backupgid:
			if (get_option_gid(args, &vol->backupgid)) {
				cifs_dbg(VFS, "%s: Invalid backupgid value\n",
					 __func__);
				goto cifs_parse_mount_err;
			}
			vol->backupgid_specified = true;
			break;
		case Opt_uid:
			if (get_option_uid(args, &vol->linux_uid)) {
				cifs_dbg(VFS, "%s: Invalid uid value\n",
					 __func__);
				goto cifs_parse_mount_err;
			}
			uid_specified = true;
			break;
		case Opt_cruid:
			if (get_option_uid(args, &vol->cred_uid)) {
				cifs_dbg(VFS, "%s: Invalid cruid value\n",
					 __func__);
				goto cifs_parse_mount_err;
			}
			break;
		case Opt_gid:
			if (get_option_gid(args, &vol->linux_gid)) {
				cifs_dbg(VFS, "%s: Invalid gid value\n",
					 __func__);
				goto cifs_parse_mount_err;
			}
			gid_specified = true;
			break;
		case Opt_file_mode:
			if (get_option_ul(args, &option)) {
				cifs_dbg(VFS, "%s: Invalid file_mode value\n",
					 __func__);
				goto cifs_parse_mount_err;
			}
			vol->file_mode = option;
			break;
		case Opt_dirmode:
			if (get_option_ul(args, &option)) {
				cifs_dbg(VFS, "%s: Invalid dir_mode value\n",
					 __func__);
				goto cifs_parse_mount_err;
			}
			vol->dir_mode = option;
			break;
		case Opt_port:
			if (get_option_ul(args, &option) ||
			    option > USHRT_MAX) {
				cifs_dbg(VFS, "%s: Invalid port value\n",
					 __func__);
				goto cifs_parse_mount_err;
			}
			port = (unsigned short)option;
			break;
		case Opt_rsize:
			if (get_option_ul(args, &option)) {
				cifs_dbg(VFS, "%s: Invalid rsize value\n",
					 __func__);
				goto cifs_parse_mount_err;
			}
			vol->rsize = option;
			break;
		case Opt_wsize:
			if (get_option_ul(args, &option)) {
				cifs_dbg(VFS, "%s: Invalid wsize value\n",
					 __func__);
				goto cifs_parse_mount_err;
			}
			vol->wsize = option;
			break;
		case Opt_actimeo:
			if (get_option_ul(args, &option)) {
				cifs_dbg(VFS, "%s: Invalid actimeo value\n",
					 __func__);
				goto cifs_parse_mount_err;
			}
			vol->actimeo = HZ * option;
			if (vol->actimeo > CIFS_MAX_ACTIMEO) {
				cifs_dbg(VFS, "attribute cache timeout too large\n");
				goto cifs_parse_mount_err;
			}
			break;

		/* String Arguments */

		case Opt_blank_user:
			/* null user, ie. anonymous authentication */
			vol->nullauth = 1;
			vol->username = NULL;
			break;
		case Opt_user:
			string = match_strdup(args);
			if (string == NULL)
				goto out_nomem;

			if (strnlen(string, MAX_USERNAME_SIZE) >
							MAX_USERNAME_SIZE) {
				printk(KERN_WARNING "CIFS: username too long\n");
				goto cifs_parse_mount_err;
			}
			vol->username = kstrdup(string, GFP_KERNEL);
			if (!vol->username)
				goto cifs_parse_mount_err;
			break;
		case Opt_blank_pass:
			/* passwords have to be handled differently
			 * to allow the character used for deliminator
			 * to be passed within them
			 */

			/*
			 * Check if this is a case where the  password
			 * starts with a delimiter
			 */
			tmp_end = strchr(data, '=');
			tmp_end++;
			if (!(tmp_end < end && tmp_end[1] == delim)) {
				/* No it is not. Set the password to NULL */
				vol->password = NULL;
				break;
			}
			/* Yes it is. Drop down to Opt_pass below.*/
		case Opt_pass:
			/* Obtain the value string */
			value = strchr(data, '=');
			value++;

			/* Set tmp_end to end of the string */
			tmp_end = (char *) value + strlen(value);

			/* Check if following character is the deliminator
			 * If yes, we have encountered a double deliminator
			 * reset the NULL character to the deliminator
			 */
			if (tmp_end < end && tmp_end[1] == delim) {
				tmp_end[0] = delim;

				/* Keep iterating until we get to a single
				 * deliminator OR the end
				 */
				while ((tmp_end = strchr(tmp_end, delim))
					!= NULL && (tmp_end[1] == delim)) {
						tmp_end = (char *) &tmp_end[2];
				}

				/* Reset var options to point to next element */
				if (tmp_end) {
					tmp_end[0] = '\0';
					options = (char *) &tmp_end[1];
				} else
					/* Reached the end of the mount option
					 * string */
					options = end;
			}

			/* Now build new password string */
			temp_len = strlen(value);
			vol->password = kzalloc(temp_len+1, GFP_KERNEL);
			if (vol->password == NULL) {
				printk(KERN_WARNING "CIFS: no memory "
						    "for password\n");
				goto cifs_parse_mount_err;
			}

			for (i = 0, j = 0; i < temp_len; i++, j++) {
				vol->password[j] = value[i];
				if ((value[i] == delim) &&
				     value[i+1] == delim)
					/* skip the second deliminator */
					i++;
			}
			vol->password[j] = '\0';
			break;
		case Opt_blank_ip:
			/* FIXME: should this be an error instead? */
			got_ip = false;
			break;
		case Opt_ip:
			string = match_strdup(args);
			if (string == NULL)
				goto out_nomem;

			if (!cifs_convert_address(dstaddr, string,
					strlen(string))) {
				printk(KERN_ERR "CIFS: bad ip= option (%s).\n",
					string);
				goto cifs_parse_mount_err;
			}
			got_ip = true;
			break;
		case Opt_domain:
			string = match_strdup(args);
			if (string == NULL)
				goto out_nomem;

			if (strnlen(string, 256) == 256) {
				printk(KERN_WARNING "CIFS: domain name too"
						    " long\n");
				goto cifs_parse_mount_err;
			}

			vol->domainname = kstrdup(string, GFP_KERNEL);
			if (!vol->domainname) {
				printk(KERN_WARNING "CIFS: no memory "
						    "for domainname\n");
				goto cifs_parse_mount_err;
			}
			cifs_dbg(FYI, "Domain name set\n");
			break;
		case Opt_srcaddr:
			string = match_strdup(args);
			if (string == NULL)
				goto out_nomem;

			if (!cifs_convert_address(
					(struct sockaddr *)&vol->srcaddr,
					string, strlen(string))) {
				printk(KERN_WARNING "CIFS:  Could not parse"
						    " srcaddr: %s\n", string);
				goto cifs_parse_mount_err;
			}
			break;
		case Opt_iocharset:
			string = match_strdup(args);
			if (string == NULL)
				goto out_nomem;

			if (strnlen(string, 1024) >= 65) {
				printk(KERN_WARNING "CIFS: iocharset name "
						    "too long.\n");
				goto cifs_parse_mount_err;
			}

			 if (strnicmp(string, "default", 7) != 0) {
				vol->iocharset = kstrdup(string,
							 GFP_KERNEL);
				if (!vol->iocharset) {
					printk(KERN_WARNING "CIFS: no memory"
							    "for charset\n");
					goto cifs_parse_mount_err;
				}
			}
			/* if iocharset not set then load_nls_default
			 * is used by caller
			 */
			 cifs_dbg(FYI, "iocharset set to %s\n", string);
			break;
		case Opt_netbiosname:
			string = match_strdup(args);
			if (string == NULL)
				goto out_nomem;

			memset(vol->source_rfc1001_name, 0x20,
				RFC1001_NAME_LEN);
			/*
			 * FIXME: are there cases in which a comma can
			 * be valid in workstation netbios name (and
			 * need special handling)?
			 */
			for (i = 0; i < RFC1001_NAME_LEN; i++) {
				/* don't ucase netbiosname for user */
				if (string[i] == 0)
					break;
				vol->source_rfc1001_name[i] = string[i];
			}
			/* The string has 16th byte zero still from
			 * set at top of the function
			 */
			if (i == RFC1001_NAME_LEN && string[i] != 0)
				printk(KERN_WARNING "CIFS: netbiosname"
				       " longer than 15 truncated.\n");

			break;
		case Opt_servern:
			/* servernetbiosname specified override *SMBSERVER */
			string = match_strdup(args);
			if (string == NULL)
				goto out_nomem;

			/* last byte, type, is 0x20 for servr type */
			memset(vol->target_rfc1001_name, 0x20,
				RFC1001_NAME_LEN_WITH_NULL);

			/* BB are there cases in which a comma can be
			   valid in this workstation netbios name
			   (and need special handling)? */

			/* user or mount helper must uppercase the
			   netbios name */
			for (i = 0; i < 15; i++) {
				if (string[i] == 0)
					break;
				vol->target_rfc1001_name[i] = string[i];
			}
			/* The string has 16th byte zero still from
			   set at top of the function  */
			if (i == RFC1001_NAME_LEN && string[i] != 0)
				printk(KERN_WARNING "CIFS: server net"
				       "biosname longer than 15 truncated.\n");
			break;
		case Opt_ver:
			string = match_strdup(args);
			if (string == NULL)
				goto out_nomem;

			if (strnicmp(string, "1", 1) == 0) {
				/* This is the default */
				break;
			}
			/* For all other value, error */
			printk(KERN_WARNING "CIFS: Invalid version"
					    " specified\n");
			goto cifs_parse_mount_err;
		case Opt_vers:
			string = match_strdup(args);
			if (string == NULL)
				goto out_nomem;

			if (cifs_parse_smb_version(string, vol) != 0)
				goto cifs_parse_mount_err;
			break;
		case Opt_sec:
			string = match_strdup(args);
			if (string == NULL)
				goto out_nomem;

			if (cifs_parse_security_flavors(string, vol) != 0)
				goto cifs_parse_mount_err;
			break;
		case Opt_cache:
			string = match_strdup(args);
			if (string == NULL)
				goto out_nomem;

			if (cifs_parse_cache_flavor(string, vol) != 0)
				goto cifs_parse_mount_err;
			break;
		default:
			/*
			 * An option we don't recognize. Save it off for later
			 * if we haven't already found one
			 */
			if (!invalid)
				invalid = data;
			break;
		}
		/* Free up any allocated string */
		kfree(string);
		string = NULL;
	}

	if (!sloppy && invalid) {
		printk(KERN_ERR "CIFS: Unknown mount option \"%s\"\n", invalid);
		goto cifs_parse_mount_err;
	}

#ifndef CONFIG_KEYS
	/* Muliuser mounts require CONFIG_KEYS support */
	if (vol->multiuser) {
		cifs_dbg(VFS, "Multiuser mounts require kernels with CONFIG_KEYS enabled\n");
		goto cifs_parse_mount_err;
	}
#endif
	if (!vol->UNC) {
		cifs_dbg(VFS, "CIFS mount error: No usable UNC path provided in device string!\n");
		goto cifs_parse_mount_err;
	}

	/* make sure UNC has a share name */
	if (!strchr(vol->UNC + 3, '\\')) {
		cifs_dbg(VFS, "Malformed UNC. Unable to find share name.\n");
		goto cifs_parse_mount_err;
	}

	if (!got_ip) {
		/* No ip= option specified? Try to get it from UNC */
		if (!cifs_convert_address(dstaddr, &vol->UNC[2],
						strlen(&vol->UNC[2]))) {
			printk(KERN_ERR "Unable to determine destination "
					"address.\n");
			goto cifs_parse_mount_err;
		}
	}

	/* set the port that we got earlier */
	cifs_set_port(dstaddr, port);

	if (uid_specified)
		vol->override_uid = override_uid;
	else if (override_uid == 1)
		printk(KERN_NOTICE "CIFS: ignoring forceuid mount option "
				   "specified with no uid= option.\n");

	if (gid_specified)
		vol->override_gid = override_gid;
	else if (override_gid == 1)
		printk(KERN_NOTICE "CIFS: ignoring forcegid mount option "
				   "specified with no gid= option.\n");

	kfree(mountdata_copy);
	return 0;

out_nomem:
	printk(KERN_WARNING "Could not allocate temporary buffer\n");
cifs_parse_mount_err:
	kfree(string);
	kfree(mountdata_copy);
	return 1;
}

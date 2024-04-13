static int nagios_core_worker(const char *path)
{
	int sd, ret;
	char response[128];

	is_worker = 1;

	set_loadctl_defaults();

	sd = nsock_unix(path, NSOCK_TCP | NSOCK_CONNECT);
	if (sd < 0) {
		printf("Failed to connect to query socket '%s': %s: %s\n",
			   path, nsock_strerror(sd), strerror(errno));
		return 1;
	}

	ret = nsock_printf_nul(sd, "@wproc register name=Core Worker %ld;pid=%ld", (long)getpid(), (long)getpid());
	if (ret < 0) {
		printf("Failed to register as worker.\n");
		return 1;
	}

	ret = read(sd, response, 3);
	if (ret != 3) {
		printf("Failed to read response from wproc manager\n");
		return 1;
	}
	if (memcmp(response, "OK", 3)) {
		read(sd, response + 3, sizeof(response) - 4);
		response[sizeof(response) - 2] = 0;
		printf("Failed to register with wproc manager: %s\n", response);
		return 1;
	}

	enter_worker(sd, start_cmd);
	return 0;
}

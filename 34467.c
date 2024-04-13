launch_login(struct passwd *pw, const char *hostname)
{
	/* Launch login(1). */

	execl(LOGIN_PROGRAM, "login", "-h", hostname,
#ifdef xxxLOGIN_NEEDS_TERM
		    (s->term ? s->term : "unknown"),
#endif /* LOGIN_NEEDS_TERM */
#ifdef LOGIN_NO_ENDOPT
	    "-p", "-f", pw->pw_name, (char *)NULL);
#else
	    "-p", "-f", "--", pw->pw_name, (char *)NULL);
#endif

	/* Login couldn't be executed, die. */

	perror("login");
	exit(1);
}

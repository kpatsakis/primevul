void killchild(gpointer key, gpointer value, gpointer user_data) {
	pid_t *pid=value;
	int *parent=user_data;

	kill(*pid, SIGTERM);
	*parent=1;
}

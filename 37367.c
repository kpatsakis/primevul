cifs_echo_callback(struct mid_q_entry *mid)
{
	struct TCP_Server_Info *server = mid->callback_data;

	DeleteMidQEntry(mid);
	atomic_dec(&server->inFlight);
	wake_up(&server->request_q);
}

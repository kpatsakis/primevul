void stream_int_unregister_handler(struct stream_interface *si)
{
	si_detach(si);
}

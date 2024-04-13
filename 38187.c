static void portio_release(struct kobject *kobj)
{
	struct uio_portio *portio = to_portio(kobj);
	kfree(portio);
}

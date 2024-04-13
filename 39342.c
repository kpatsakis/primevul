static void userns_put(void *ns)
{
	put_user_ns(ns);
}

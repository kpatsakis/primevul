void nlmsg_get(struct nl_msg *msg)
{
	msg->nm_refcnt++;
	NL_DBG(4, "New reference to message %p, total %d\n",
	       msg, msg->nm_refcnt);
}

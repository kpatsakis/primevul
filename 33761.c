void nlmsg_free(struct nl_msg *msg)
{
	if (!msg)
		return;

	msg->nm_refcnt--;
	NL_DBG(4, "Returned message reference %p, %d remaining\n",
	       msg, msg->nm_refcnt);

	if (msg->nm_refcnt < 0)
		BUG();

	if (msg->nm_refcnt <= 0) {
		free(msg->nm_nlh);
		NL_DBG(2, "msg %p: Freed\n", msg);
		free(msg);
	}
}

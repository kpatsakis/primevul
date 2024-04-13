void nfs4_destroy_session(struct nfs4_session *session)
{
	struct rpc_xprt *xprt;

	nfs4_proc_destroy_session(session);

	rcu_read_lock();
	xprt = rcu_dereference(session->clp->cl_rpcclient->cl_xprt);
	rcu_read_unlock();
	dprintk("%s Destroy backchannel for xprt %p\n",
		__func__, xprt);
	xprt_destroy_backchannel(xprt, NFS41_BC_MIN_CALLBACKS);
	nfs4_destroy_slot_tables(session);
	kfree(session);
}

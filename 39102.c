void __init msg_init(void)
{
	msg_init_ns(&init_ipc_ns);

	printk(KERN_INFO "msgmni has been set to %d\n",
		init_ipc_ns.msg_ctlmni);

	ipc_init_proc_interface("sysvipc/msg",
				"       key      msqid perms      cbytes       qnum lspid lrpid   uid   gid  cuid  cgid      stime      rtime      ctime\n",
				IPC_MSG_IDS, sysvipc_msg_proc_show);
}

static int __init default_policy_setup(char *str)
{
	ima_use_tcb = 1;
	return 1;
}

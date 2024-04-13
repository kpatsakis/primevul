static int __init setup_slub_nomerge(char *str)
{
	slub_nomerge = 1;
	return 1;
}

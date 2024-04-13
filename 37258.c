static int __init setup_slub_max_order(char *str)
{
	get_option(&str, &slub_max_order);

	return 1;
}

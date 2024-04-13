static void lbs_debug_init(struct lbs_private *priv)
{
	int i;

	if (!priv->debugfs_dir)
		return;

	for (i = 0; i < num_of_items; i++)
		items[i].addr += (size_t) priv;

	priv->debugfs_debug = debugfs_create_file("debug", 0644,
						  priv->debugfs_dir, &items[0],
						  &lbs_debug_fops);
}

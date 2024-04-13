rdp_main_loop(RD_BOOL * deactivated, uint32 * ext_disc_reason)
{
	do
	{
		if (rdp_loop(deactivated, ext_disc_reason) == False)
		{
			g_exit_mainloop = True;
		}
	}
	while (g_exit_mainloop == False);
}

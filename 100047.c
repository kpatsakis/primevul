rdp_process_bitmap_caps(STREAM s)
{

	uint16 depth;

	logger(Protocol, Debug, "%s()", __func__);

	in_uint16_le(s, depth);
	in_uint8s(s, 6);

	in_uint16_le(s, g_session_width);
	in_uint16_le(s, g_session_height);

	logger(Protocol, Debug,
	       "rdp_process_bitmap_caps(), setting desktop size and depth to: %dx%dx%d",
	       g_session_width, g_session_height, depth);

	/* Detect if we can have dynamic session resize enabled, only once. */
	if (g_first_bitmap_caps == True && !(g_session_width == g_requested_session_width
					     && g_session_height == g_requested_session_height))
	{
		logger(Core, Notice, "Disabling dynamic session resize");
		g_dynamic_session_resize = False;
	}
	g_first_bitmap_caps = False;

	/*
	 * The server may limit depth and change the size of the desktop (for
	 * example when shadowing another session).
	 */
	if (g_server_depth != depth)
	{
		logger(Core, Verbose,
		       "Remote desktop does not support colour depth %d; falling back to %d",
		       g_server_depth, depth);
		g_server_depth = depth;
	}

	/* Resize window size to match session size, except when we're in
	   fullscreen, where we want the window to always cover the entire
	   screen. */

	if (g_fullscreen == True)
		return;

	/* If dynamic session resize is disabled, set window size hints to
	   fixed session size */
	if (g_dynamic_session_resize == False)
	{
		ui_update_window_sizehints(g_session_width, g_session_height);
		return;
	}

	ui_resize_window(g_session_width, g_session_height);
}

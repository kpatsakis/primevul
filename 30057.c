re_input (void)
{
    if (using_plan_a) {
      if (i_buffer)
	{
	  free (i_buffer);
	  i_buffer = 0;
	  free (i_ptr);
	}
    }
    else {
	if (tifd >= 0)
	  close (tifd);
	tifd = -1;
	if (tibuf[0])
	  {
	    free (tibuf[0]);
	    tibuf[0] = 0;
	  }
	tiline[0] = tiline[1] = -1;
	tireclen = 0;
    }
}

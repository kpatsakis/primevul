close_notify_handler (int fd, void *opaque)
{
  engine_uiserver_t uiserver = opaque;

  assert (fd != -1);
  if (uiserver->status_cb.fd == fd)
    {
      if (uiserver->status_cb.tag)
	(*uiserver->io_cbs.remove) (uiserver->status_cb.tag);
      uiserver->status_cb.fd = -1;
      uiserver->status_cb.tag = NULL;
    }
  else if (uiserver->input_cb.fd == fd)
    {
      if (uiserver->input_cb.tag)
	(*uiserver->io_cbs.remove) (uiserver->input_cb.tag);
      uiserver->input_cb.fd = -1;
      uiserver->input_cb.tag = NULL;
      if (uiserver->input_helper_data)
        {
          gpgme_data_release (uiserver->input_helper_data);
          uiserver->input_helper_data = NULL;
        }
      if (uiserver->input_helper_memory)
        {
          free (uiserver->input_helper_memory);
          uiserver->input_helper_memory = NULL;
        }
    }
  else if (uiserver->output_cb.fd == fd)
    {
      if (uiserver->output_cb.tag)
	(*uiserver->io_cbs.remove) (uiserver->output_cb.tag);
      uiserver->output_cb.fd = -1;
      uiserver->output_cb.tag = NULL;
    }
  else if (uiserver->message_cb.fd == fd)
    {
      if (uiserver->message_cb.tag)
	(*uiserver->io_cbs.remove) (uiserver->message_cb.tag);
      uiserver->message_cb.fd = -1;
      uiserver->message_cb.tag = NULL;
    }
}

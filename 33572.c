gpgsm_clear_fd (engine_gpgsm_t gpgsm, fd_type_t fd_type)
{
#if !USE_DESCRIPTOR_PASSING
  switch (fd_type)
    {
    case INPUT_FD:
      _gpgme_io_close (gpgsm->input_cb.fd);
      break;
    case OUTPUT_FD:
      _gpgme_io_close (gpgsm->output_cb.fd);
      break;
    case MESSAGE_FD:
      _gpgme_io_close (gpgsm->message_cb.fd);
      break;
    }
#endif
}

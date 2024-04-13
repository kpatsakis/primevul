int vrend_renderer_get_poll_fd(void)
{
   if (!vrend_state.inited)
      return -1;

   return vrend_state.eventfd;
}

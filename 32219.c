vrend_renderer_fini(void)
{
   if (!vrend_state.inited)
      return;

   vrend_free_sync_thread();
   if (vrend_state.eventfd != -1) {
      close(vrend_state.eventfd);
      vrend_state.eventfd = -1;
   }

   vrend_decode_reset(false);
   vrend_object_fini_resource_table();
   vrend_decode_reset(true);

   vrend_state.inited = false;
}

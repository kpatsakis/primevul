void vrend_renderer_reset(void)
{
   if (vrend_state.sync_thread) {
      vrend_free_sync_thread();
      vrend_state.stop_sync_thread = false;
   }
   vrend_reset_fences();
   vrend_decode_reset(false);
   vrend_object_fini_resource_table();
   vrend_decode_reset(true);
   vrend_object_init_resource_table();
   vrend_renderer_context_create_internal(0, 0, NULL);
}

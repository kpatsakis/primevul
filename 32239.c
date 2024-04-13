static void vrend_renderer_use_threaded_sync(void)
{
   struct virgl_gl_ctx_param ctx_params;

   if (getenv("VIRGL_DISABLE_MT"))
      return;

   ctx_params.shared = true;
   ctx_params.major_ver = vrend_state.gl_major_ver;
   ctx_params.minor_ver = vrend_state.gl_minor_ver;

   vrend_state.stop_sync_thread = false;

   vrend_state.sync_context = vrend_clicbs->create_gl_context(0, &ctx_params);
   if (vrend_state.sync_context == NULL) {
      fprintf(stderr, "failed to create sync opengl context\n");
      return;
   }

   vrend_state.eventfd = eventfd(0, EFD_CLOEXEC | EFD_NONBLOCK);
   if (vrend_state.eventfd == -1) {
      fprintf(stderr, "Failed to create eventfd\n");
      vrend_clicbs->destroy_gl_context(vrend_state.sync_context);
      return;
   }

   pipe_condvar_init(vrend_state.fence_cond);
   pipe_mutex_init(vrend_state.fence_mutex);

   vrend_state.sync_thread = pipe_thread_create(thread_sync, NULL);
   if (!vrend_state.sync_thread) {
      close(vrend_state.eventfd);
      vrend_state.eventfd = -1;
      vrend_clicbs->destroy_gl_context(vrend_state.sync_context);
      pipe_condvar_destroy(vrend_state.fence_cond);
      pipe_mutex_destroy(vrend_state.fence_mutex);
   }
}

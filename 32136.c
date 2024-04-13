static int thread_sync(void *arg)
{
   virgl_gl_context gl_context = vrend_state.sync_context;
   struct vrend_fence *fence, *stor;

   pipe_mutex_lock(vrend_state.fence_mutex);
   vrend_clicbs->make_current(0, gl_context);

   while (!vrend_state.stop_sync_thread) {
      if (LIST_IS_EMPTY(&vrend_state.fence_wait_list) &&
          pipe_condvar_wait(vrend_state.fence_cond, vrend_state.fence_mutex) != 0) {
         fprintf(stderr, "error while waiting on condition\n");
         break;
      }

      LIST_FOR_EACH_ENTRY_SAFE(fence, stor, &vrend_state.fence_wait_list, fences) {
         if (vrend_state.stop_sync_thread)
            break;
         list_del(&fence->fences);
         pipe_mutex_unlock(vrend_state.fence_mutex);
         wait_sync(fence);
         pipe_mutex_lock(vrend_state.fence_mutex);
      }
   }

   vrend_clicbs->make_current(0, 0);
   vrend_clicbs->destroy_gl_context(vrend_state.sync_context);
   pipe_mutex_unlock(vrend_state.fence_mutex);
   return 0;
}

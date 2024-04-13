static void vrend_free_sync_thread(void)
{
   if (!vrend_state.sync_thread)
      return;

   pipe_mutex_lock(vrend_state.fence_mutex);
   vrend_state.stop_sync_thread = true;
   pipe_mutex_unlock(vrend_state.fence_mutex);

   pipe_condvar_signal(vrend_state.fence_cond);
   pipe_thread_wait(vrend_state.sync_thread);
   vrend_state.sync_thread = 0;

   pipe_condvar_destroy(vrend_state.fence_cond);
   pipe_mutex_destroy(vrend_state.fence_mutex);
}

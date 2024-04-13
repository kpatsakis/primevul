static void vrend_reset_fences(void)
{
   struct vrend_fence *fence, *stor;

   if (vrend_state.sync_thread)
      pipe_mutex_lock(vrend_state.fence_mutex);

   LIST_FOR_EACH_ENTRY_SAFE(fence, stor, &vrend_state.fence_list, fences) {
      free_fence_locked(fence);
   }

   if (vrend_state.sync_thread)
      pipe_mutex_unlock(vrend_state.fence_mutex);
}

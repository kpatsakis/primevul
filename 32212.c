int vrend_renderer_create_fence(int client_fence_id, uint32_t ctx_id)
{
   struct vrend_fence *fence;

   fence = malloc(sizeof(struct vrend_fence));
   if (!fence)
      return ENOMEM;

   fence->ctx_id = ctx_id;
   fence->fence_id = client_fence_id;
   fence->syncobj = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);

   if (fence->syncobj == NULL)
      goto fail;

   if (vrend_state.sync_thread) {
      pipe_mutex_lock(vrend_state.fence_mutex);
      list_addtail(&fence->fences, &vrend_state.fence_wait_list);
      pipe_mutex_unlock(vrend_state.fence_mutex);
      pipe_condvar_signal(vrend_state.fence_cond);
   } else
      list_addtail(&fence->fences, &vrend_state.fence_list);
   return 0;

 fail:
   fprintf(stderr, "failed to create fence sync object\n");
   free(fence);
   return ENOMEM;
}

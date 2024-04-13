void vrend_set_clip_state(struct vrend_context *ctx, struct pipe_clip_state *ucp)
{
   if (vrend_state.use_core_profile) {
      ctx->sub->ucp_state = *ucp;
   } else {
      int i, j;
      GLdouble val[4];

      for (i = 0; i < 8; i++) {
         for (j = 0; j < 4; j++)
            val[j] = ucp->ucp[i][j];
         glClipPlane(GL_CLIP_PLANE0 + i, val);
      }
   }
}

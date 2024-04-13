void vrend_renderer_create_sub_ctx(struct vrend_context *ctx, int sub_ctx_id)
{
   struct vrend_sub_context *sub;
   struct virgl_gl_ctx_param ctx_params;

   LIST_FOR_EACH_ENTRY(sub, &ctx->sub_ctxs, head) {
      if (sub->sub_ctx_id == sub_ctx_id) {
         return;
      }
   }

   sub = CALLOC_STRUCT(vrend_sub_context);
   if (!sub)
      return;

   ctx_params.shared = (ctx->ctx_id == 0 && sub_ctx_id == 0) ? false : true;
   ctx_params.major_ver = vrend_state.gl_major_ver;
   ctx_params.minor_ver = vrend_state.gl_minor_ver;
   sub->gl_context = vrend_clicbs->create_gl_context(0, &ctx_params);
   vrend_clicbs->make_current(0, sub->gl_context);

   sub->sub_ctx_id = sub_ctx_id;

   if (!vrend_state.have_vertex_attrib_binding) {
      glGenVertexArrays(1, &sub->vaoid);
      glBindVertexArray(sub->vaoid);
   }

   glGenFramebuffers(1, &sub->fb_id);
   glGenFramebuffers(2, sub->blit_fb_ids);

   list_inithead(&sub->programs);
   list_inithead(&sub->streamout_list);

   sub->object_hash = vrend_object_init_ctx_table();

   ctx->sub = sub;
   list_add(&sub->head, &ctx->sub_ctxs);
   if (sub_ctx_id == 0)
      ctx->sub0 = sub;
}

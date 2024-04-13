struct vrend_context *vrend_create_context(int id, uint32_t nlen, const char *debug_name)
{
   struct vrend_context *grctx = CALLOC_STRUCT(vrend_context);

   if (!grctx)
      return NULL;

   if (nlen && debug_name) {
      strncpy(grctx->debug_name, debug_name, 64);
   }

   grctx->ctx_id = id;

   list_inithead(&grctx->sub_ctxs);
   list_inithead(&grctx->active_nontimer_query_list);

   grctx->res_hash = vrend_object_init_ctx_table();

   grctx->shader_cfg.use_core_profile = vrend_state.use_core_profile;
   grctx->shader_cfg.use_explicit_locations = vrend_state.use_explicit_locations;
   vrend_renderer_create_sub_ctx(grctx, 0);
   vrend_renderer_set_sub_ctx(grctx, 0);

   vrender_get_glsl_version(&grctx->shader_cfg.glsl_version);

   list_addtail(&grctx->ctx_entry, &vrend_state.active_ctx_list);
   return grctx;
}

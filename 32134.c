static inline bool should_invert_viewport(struct vrend_context *ctx)
{
   /* if we have a negative viewport then gallium wanted to invert it,
      however since we are rendering to GL FBOs we need to invert it
      again unless we are rendering upside down already
      - confused?
      so if gallium asks for a negative viewport */
   return !(ctx->sub->viewport_is_negative ^ ctx->sub->inverted_fbo_content);
}

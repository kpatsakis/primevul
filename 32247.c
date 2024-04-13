void vrend_set_blend_color(struct vrend_context *ctx,
                           struct pipe_blend_color *color)
{
   ctx->sub->blend_color = *color;
   glBlendColor(color->color[0], color->color[1], color->color[2],
                color->color[3]);
}

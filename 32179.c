static void vrend_draw_bind_vertex_legacy(struct vrend_context *ctx,
                                          struct vrend_vertex_element_array *va)
{
   uint32_t num_enable;
   uint32_t enable_bitmask;
   uint32_t disable_bitmask;
   int i;

   num_enable = va->count;
   enable_bitmask = 0;
   disable_bitmask = ~((1ull << num_enable) - 1);
   for (i = 0; i < va->count; i++) {
      struct vrend_vertex_element *ve = &va->elements[i];
      int vbo_index = ve->base.vertex_buffer_index;
      struct vrend_resource *res;
      GLint loc;

      if (i >= ctx->sub->prog->ss[PIPE_SHADER_VERTEX]->sel->sinfo.num_inputs) {
         /* XYZZY: debug this? */
         num_enable = ctx->sub->prog->ss[PIPE_SHADER_VERTEX]->sel->sinfo.num_inputs;
         break;
      }
      res = (struct vrend_resource *)ctx->sub->vbo[vbo_index].buffer;

      if (!res) {
         fprintf(stderr,"cannot find vbo buf %d %d %d\n", i, va->count, ctx->sub->prog->ss[PIPE_SHADER_VERTEX]->sel->sinfo.num_inputs);
         continue;
      }

      if (vrend_state.use_explicit_locations || vrend_state.have_vertex_attrib_binding) {
         loc = i;
      } else {
         if (ctx->sub->prog->attrib_locs) {
            loc = ctx->sub->prog->attrib_locs[i];
         } else loc = -1;

         if (loc == -1) {
            fprintf(stderr,"%s: cannot find loc %d %d %d\n", ctx->debug_name, i, va->count, ctx->sub->prog->ss[PIPE_SHADER_VERTEX]->sel->sinfo.num_inputs);
            num_enable--;
            if (i == 0) {
               fprintf(stderr,"%s: shader probably didn't compile - skipping rendering\n", ctx->debug_name);
               return;
            }
            continue;
         }
      }

      if (ve->type == GL_FALSE) {
         fprintf(stderr,"failed to translate vertex type - skipping render\n");
         return;
      }

      glBindBuffer(GL_ARRAY_BUFFER, res->id);

      if (ctx->sub->vbo[vbo_index].stride == 0) {
         void *data;
         /* for 0 stride we are kinda screwed */
         data = glMapBufferRange(GL_ARRAY_BUFFER, ctx->sub->vbo[vbo_index].buffer_offset, ve->nr_chan * sizeof(GLfloat), GL_MAP_READ_BIT);

         switch (ve->nr_chan) {
         case 1:
            glVertexAttrib1fv(loc, data);
            break;
         case 2:
            glVertexAttrib2fv(loc, data);
            break;
         case 3:
            glVertexAttrib3fv(loc, data);
            break;
         case 4:
         default:
            glVertexAttrib4fv(loc, data);
            break;
         }
         glUnmapBuffer(GL_ARRAY_BUFFER);
         disable_bitmask |= (1 << loc);
      } else {
         enable_bitmask |= (1 << loc);
         if (util_format_is_pure_integer(ve->base.src_format)) {
            glVertexAttribIPointer(loc, ve->nr_chan, ve->type, ctx->sub->vbo[vbo_index].stride, (void *)(unsigned long)(ve->base.src_offset + ctx->sub->vbo[vbo_index].buffer_offset));
         } else {
            glVertexAttribPointer(loc, ve->nr_chan, ve->type, ve->norm, ctx->sub->vbo[vbo_index].stride, (void *)(unsigned long)(ve->base.src_offset + ctx->sub->vbo[vbo_index].buffer_offset));
         }
         glVertexAttribDivisorARB(loc, ve->base.instance_divisor);
      }
   }
   if (ctx->sub->enabled_attribs_bitmask != enable_bitmask) {
      uint32_t mask = ctx->sub->enabled_attribs_bitmask & disable_bitmask;

      while (mask) {
         i = u_bit_scan(&mask);
         glDisableVertexAttribArray(i);
      }
      ctx->sub->enabled_attribs_bitmask &= ~disable_bitmask;

      mask = ctx->sub->enabled_attribs_bitmask ^ enable_bitmask;
      while (mask) {
         i = u_bit_scan(&mask);
         glEnableVertexAttribArray(i);
      }

      ctx->sub->enabled_attribs_bitmask = enable_bitmask;
   }
}

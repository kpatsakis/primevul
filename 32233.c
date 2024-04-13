void vrend_renderer_resource_destroy(struct vrend_resource *res, bool remove)
{
   if (res->readback_fb_id)
      glDeleteFramebuffers(1, &res->readback_fb_id);

   if (res->ptr)
      free(res->ptr);
   if (res->id) {
      if (res->target == GL_ELEMENT_ARRAY_BUFFER_ARB ||
          res->target == GL_ARRAY_BUFFER_ARB ||
          res->target == GL_UNIFORM_BUFFER||
          res->target == GL_TEXTURE_BUFFER||
          res->target == GL_TRANSFORM_FEEDBACK_BUFFER) {
         glDeleteBuffers(1, &res->id);
         if (res->target == GL_TEXTURE_BUFFER)
            glDeleteTextures(1, &res->tbo_tex_id);
      } else
         glDeleteTextures(1, &res->id);
   }

   if (res->handle && remove)
      vrend_resource_remove(res->handle);
   free(res);
}

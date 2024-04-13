static void vrend_use_program(struct vrend_context *ctx, GLuint program_id)
{
   if (ctx->sub->program_id != program_id) {
      glUseProgram(program_id);
      ctx->sub->program_id = program_id;
   }
}

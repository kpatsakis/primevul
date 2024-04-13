static inline const char *pipe_shader_to_prefix(int shader_type)
{
   switch (shader_type) {
   case PIPE_SHADER_VERTEX: return "vs";
   case PIPE_SHADER_FRAGMENT: return "fs";
   case PIPE_SHADER_GEOMETRY: return "gs";
   default:
      return NULL;
   };
}

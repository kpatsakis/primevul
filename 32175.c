static void vrend_destroy_vertex_elements_object(void *obj_ptr)
{
   struct vrend_vertex_element_array *v = obj_ptr;

   if (vrend_state.have_vertex_attrib_binding) {
      glDeleteVertexArrays(1, &v->id);
   }
   FREE(v);
}

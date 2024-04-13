static void dump_stream_out(struct pipe_stream_output_info *so)
{
   int i;
   if (!so)
      return;
   printf("streamout: %d\n", so->num_outputs);
   printf("strides: ");
   for (i = 0; i < 4; i++)
      printf("%d ", so->stride[i]);
   printf("\n");
   printf("outputs:\n");
   for (i = 0; i < so->num_outputs; i++) {
      printf("\t%d: reg: %d sc: %d, nc: %d ob: %d do: %d\n",
             i,
             so->output[i].register_index,
             so->output[i].start_component,
             so->output[i].num_components,
             so->output[i].output_buffer,
             so->output[i].dst_offset);
   }
}

void vrend_set_sample_mask(struct vrend_context *ctx, unsigned sample_mask)
{
   glSampleMaski(0, sample_mask);
}

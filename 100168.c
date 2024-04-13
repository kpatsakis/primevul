void gdImageWebpEx (gdImagePtr im, FILE * outFile, int quantization)
{
	gdIOCtx *out = gdNewFileCtx(outFile);
	gdImageWebpCtx(im, out, quantization);
	out->gd_free(out);
}

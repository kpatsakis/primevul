static void prefix_line(FILE *ofd, int prefix)
{
	int i;

	for (i = 0; i < prefix; i++)
		fprintf(ofd, "  ");
}

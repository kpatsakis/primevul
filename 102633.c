 int jas_seq2d_output(jas_matrix_t *matrix, FILE *out)
 {
 #define MAXLINELEN	80
	jas_matind_t i;
	jas_matind_t j;
 	jas_seqent_t x;
 	char buf[MAXLINELEN + 1];
 	char sbuf[MAXLINELEN + 1];
	int n;

	fprintf(out, "%"PRIiFAST32" %"PRIiFAST32"\n", jas_seq2d_xstart(matrix),
	  jas_seq2d_ystart(matrix));
	fprintf(out, "%"PRIiFAST32" %"PRIiFAST32"\n", jas_matrix_numcols(matrix),
	  jas_matrix_numrows(matrix));

	buf[0] = '\0';
	for (i = 0; i < jas_matrix_numrows(matrix); ++i) {
		for (j = 0; j < jas_matrix_numcols(matrix); ++j) {
			x = jas_matrix_get(matrix, i, j);
			sprintf(sbuf, "%s%4ld", (strlen(buf) > 0) ? " " : "",
			  JAS_CAST(long, x));
			n = JAS_CAST(int, strlen(buf));
			if (n + JAS_CAST(int, strlen(sbuf)) > MAXLINELEN) {
				fputs(buf, out);
				fputs("\n", out);
				buf[0] = '\0';
			}
			strcat(buf, sbuf);
			if (j == jas_matrix_numcols(matrix) - 1) {
				fputs(buf, out);
				fputs("\n", out);
				buf[0] = '\0';
			}
		}
	}
	fputs(buf, out);

	return 0;
}

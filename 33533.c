static void print_pem_cert(FILE *out, certificate_t *cert)
{
	chunk_t encoded;

	if (cert->get_encoding(cert, CERT_PEM, &encoded))
	{
		fprintf(out, "%.*s", (int)encoded.len, encoded.ptr);
		free(encoded.ptr);
	}
}

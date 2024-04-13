static void cmd_compress(char *tag, char *alg)
{
    if (imapd_compress_done) {
        prot_printf(imapd_out,
                    "%s BAD [COMPRESSIONACTIVE] DEFLATE active via COMPRESS\r\n",
                    tag);
    }
#if defined(HAVE_SSL) && (OPENSSL_VERSION_NUMBER >= 0x0090800fL)
    else if (imapd_tls_comp) {
        prot_printf(imapd_out,
                    "%s NO [COMPRESSIONACTIVE] %s active via TLS\r\n",
                    tag, SSL_COMP_get_name(imapd_tls_comp));
    }
#endif // defined(HAVE_SSL) && (OPENSSL_VERSION_NUMBER >= 0x0090800fL)
    else if (strcasecmp(alg, "DEFLATE")) {
        prot_printf(imapd_out,
                    "%s NO Unknown COMPRESS algorithm: %s\r\n", tag, alg);
    }
    else if (ZLIB_VERSION[0] != zlibVersion()[0]) {
        prot_printf(imapd_out,
                    "%s NO Error initializing %s (incompatible zlib version)\r\n",
                    tag, alg);
    }
    else {
        prot_printf(imapd_out,
                    "%s OK %s active\r\n", tag, alg);

        /* enable (de)compression for the prot layer */
        prot_setcompress(imapd_in);
        prot_setcompress(imapd_out);

        imapd_compress_done = 1;
    }
}

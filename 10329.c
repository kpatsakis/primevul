SWTPM_NVRAM_GetFilenameForName(char *filename,       /* output: filename */
                               size_t bufsize,
                               uint32_t tpm_number,
                               const char *name,     /* input: abstract name */
                               TPM_BOOL is_tempfile) /* input: is temporary file? */
{
    TPM_RESULT res = TPM_SUCCESS;
    int n;
    const char *suffix = "";

    TPM_DEBUG(" SWTPM_NVRAM_GetFilenameForName: For name %s\n", name);

    switch (tpmstate_get_version()) {
    case TPMLIB_TPM_VERSION_1_2:
        break;
    case TPMLIB_TPM_VERSION_2:
        suffix = "2";
        break;
    }

    if (is_tempfile) {
        n = snprintf(filename, bufsize, "TMP%s-%02lx.%s", suffix, (unsigned long)tpm_number, name);
    } else {
        n = snprintf(filename, bufsize, "tpm%s-%02lx.%s", suffix, (unsigned long)tpm_number, name);
    }
    if ((size_t)n > bufsize) {
        res = TPM_FAIL;
    }

    TPM_DEBUG("  SWTPM_NVRAM_GetFilenameForName: File name %s\n", filename);

    return res;
}
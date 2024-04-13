static int backend_version(struct backend *be)
{
    const char *minor;

    /* IMPORTANT:
     *
     * When adding checks for new versions, you must also backport these
     * checks to previous versions (especially 2.4 and 2.5).
     *
     * Otherwise, old versions will be unable to recognise the new version,
     * assume it is ancient, and downgrade the index to the oldest version
     * supported (version 6, prior to v2.3).
     */

    /* It's like looking in the mirror and not suffering from schizophrenia */
    if (strstr(be->banner, cyrus_version())) {
        return MAILBOX_MINOR_VERSION;
    }

    /* master branch? */
    if (strstr(be->banner, "Cyrus IMAP 3.0")) {
        return 13;
    }

    /* version 2.5 is 13 */
    if (strstr(be->banner, "Cyrus IMAP 2.5.")
     || strstr(be->banner, "Cyrus IMAP Murder 2.5.")
     || strstr(be->banner, "git2.5.")) {
        return 13;
    }

    /* version 2.4 was all 12 */
    if (strstr(be->banner, "v2.4.") || strstr(be->banner, "git2.4.")) {
        return 12;
    }

    minor = strstr(be->banner, "v2.3.");
    if (!minor) return 6;

    /* at least version 2.3.10 */
    if (minor[1] != ' ') {
        return 10;
    }
    /* single digit version, figure out which */
    switch (minor[0]) {
    case '0':
    case '1':
    case '2':
    case '3':
        return 7;
        break;

    case '4':
    case '5':
    case '6':
        return 8;
        break;

    case '7':
    case '8':
    case '9':
        return 9;
        break;
    }

    /* fallthrough, shouldn't happen */
    return 6;
}

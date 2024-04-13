static int append_catenate(FILE *f, const char *cur_name, unsigned *totalsize,
                           int *binary, const char **parseerr, const char **url)
{
    int c, r = 0;
    static struct buf arg;

    do {
        c = getword(imapd_in, &arg);
        if (c != ' ') {
            *parseerr = "Missing message part data in Append command";
            return IMAP_PROTOCOL_ERROR;
        }

        if (!strcasecmp(arg.s, "TEXT")) {
            int r1 = catenate_text(f, totalsize, binary, parseerr);
            if (r1) return r1;

            /* if we see a SP, we're trying to catenate more than one part */

            /* Parse newline terminating command */
            c = prot_getc(imapd_in);
        }
        else if (!strcasecmp(arg.s, "URL")) {
            c = getastring(imapd_in, imapd_out, &arg);
            if (c != ' ' && c != ')') {
                *parseerr = "Missing URL in Append command";
                return IMAP_PROTOCOL_ERROR;
            }

            if (!r) {
                r = catenate_url(arg.s, cur_name, f, totalsize, parseerr);
                if (r) {
                    *url = arg.s;
                    return r;
                }
            }
        }
        else {
            *parseerr = "Invalid message part type in Append command";
            return IMAP_PROTOCOL_ERROR;
        }

        fflush(f);
    } while (c == ' ');

    if (c != ')') {
        *parseerr = "Missing space or ) after catenate list in Append command";
        return IMAP_PROTOCOL_ERROR;
    }

    if (ferror(f) || fsync(fileno(f))) {
        syslog(LOG_ERR, "IOERROR: writing message: %m");
        return IMAP_IOERROR;
    }

    return r;
}

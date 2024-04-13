parse_cmd_line(const char *ctl_line, GString *result) {
    size_t len=0;
    gchar *ctlstrip = NULL;
    gchar *exp_line = NULL;
    gchar *work_string = NULL;

    work_string = g_strdup(ctl_line);

    /* strip trailing newline */
    len = strlen(ctl_line);
    if (work_string[len - 1] == '\n')
        ctlstrip = g_strndup(work_string, len - 1);
    else
        ctlstrip = g_strdup(work_string);
    g_free(work_string);

    if( strcmp(g_strchug(ctlstrip), "") &&
        strcmp(exp_line = expand(ctlstrip, 0), "")
      ) {
            /* ignore comments */
            if((exp_line[0] == '#'))
                ;

            /* parse a command */
            else {
                gchar **tokens = NULL;

                tokens = g_strsplit(exp_line, " ", 2);
                parse_command(tokens[0], tokens[1], result);
                g_strfreev(tokens);
            }
        g_free(exp_line);
    }

    g_free(ctlstrip);
}

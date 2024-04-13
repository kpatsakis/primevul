run_handler (const gchar *act, const gchar *args) {
    /* Consider this code a temporary hack to make the handlers usable.
       In practice, all this splicing, injection, and reconstruction is
       inefficient, annoying and hard to manage.  Potential pitfalls arise
       when the handler specific args 1) are not quoted  (the handler
       callbacks should take care of this)  2) are quoted but interfere
       with the users' own quotation.  A more ideal solution is
       to refactor parse_command so that it doesn't just take a string
       and execute it; rather than that, we should have a function which
       returns the argument vector parsed from the string.  This vector
       could be modified (e.g. insert additional args into it) before
       passing it to the next function that actually executes it.  Though
       it still isn't perfect for chain actions..  will reconsider & re-
       factor when I have the time. -duc */

    if (!act) return;
    char **parts = g_strsplit(act, " ", 2);
    if (!parts || !parts[0]) return;
    if (g_strcmp0(parts[0], "chain") == 0) {
        GString *newargs = g_string_new("");
        gchar **chainparts = split_quoted(parts[1], FALSE);

        /* for every argument in the chain, inject the handler args
           and make sure the new parts are wrapped in quotes */
        gchar **cp = chainparts;
        gchar quot = '\'';
        gchar *quotless = NULL;
        gchar **spliced_quotless = NULL; // sigh -_-;
        gchar **inpart = NULL;

        while (*cp) {
            if ((**cp == '\'') || (**cp == '\"')) { /* strip old quotes */
                quot = **cp;
                quotless = g_strndup(&(*cp)[1], strlen(*cp) - 2);
            } else quotless = g_strdup(*cp);

            spliced_quotless = g_strsplit(quotless, " ", 2);
            inpart = inject_handler_args(spliced_quotless[0], spliced_quotless[1], args);
            g_strfreev(spliced_quotless);

            g_string_append_printf(newargs, " %c%s %s%c", quot, inpart[0], inpart[1], quot);
            g_free(quotless);
            g_strfreev(inpart);
            cp++;
        }

        parse_command(parts[0], &(newargs->str[1]), NULL);
        g_string_free(newargs, TRUE);
        g_strfreev(chainparts);

    } else {
        gchar **inparts;
        gchar *inparts_[2];
        if (parts[1]) {
            /* expand the user-specified arguments */
            gchar* expanded = expand(parts[1], 0);
            inparts = inject_handler_args(parts[0], expanded, args);
            g_free(expanded);
        } else {
            inparts_[0] = parts[0];
            inparts_[1] = g_strdup(args);
            inparts = inparts_;
        }

        parse_command(inparts[0], inparts[1], NULL);

        if (inparts != inparts_) {
            g_free(inparts[0]);
            g_free(inparts[1]);
        } else
            g_free(inparts[1]);
    }
    g_strfreev(parts);
}

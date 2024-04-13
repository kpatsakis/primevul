static void pass_args_to_existing_instance(const GOptionEntry* opt_entries, int screen_num, int sock)
{
    const GOptionEntry* ent;
    FILE* f = fdopen(sock, "w");
    char* escaped;

    /* pass cwd */
    char* cwd = g_get_current_dir();
    escaped = g_strescape(cwd, NULL);
    fprintf(f, "%s\n", escaped);
    g_free(cwd);
    cwd = escaped;

    /* pass screen number */
    fprintf(f, "%d\n", screen_num);

    for(ent = opt_entries; ent->long_name; ++ent)
    {
        switch(ent->arg)
        {
        case G_OPTION_ARG_NONE:
            if(*(gboolean*)ent->arg_data)
                fprintf(f, "--%s\n", ent->long_name);
            break;
        case G_OPTION_ARG_STRING:
        case G_OPTION_ARG_FILENAME:
        {
            char* str = *(char**)ent->arg_data;
            if(str && *str)
            {
                fprintf(f, "--%s\n", ent->long_name);
                if(g_str_has_prefix(str, "--")) /* strings begining with -- */
                    fprintf(f, "--\n"); /* prepend a -- to it */
                escaped = g_strescape(str, NULL);
                fprintf(f, "%s\n", escaped);
                g_free(escaped);
            }
            break;
        }
        case G_OPTION_ARG_INT:
        {
            gint value = *(gint*)ent->arg_data;
            if(value >= 0)
            {
                fprintf(f, "--%s\n%d\n", ent->long_name, value);
            }
            break;
        }
        case G_OPTION_ARG_STRING_ARRAY:
        case G_OPTION_ARG_FILENAME_ARRAY:
        {
            char** strv = *(char***)ent->arg_data;
            if(strv && *strv)
            {
                if(*ent->long_name) /* G_OPTION_REMAINING = "" */
                    fprintf(f, "--%s\n", ent->long_name);
                for(; *strv; ++strv)
                {
                    char* str = *strv;
                    /* if not absolute path and not URI then prepend cwd or $HOME */
                    if(str[0] == '~' && str[1] == '\0') ; /* pass "~" as is */
                    else if(str[0] == '~' && str[1] == '/')
                    {
                        const char *envvar = g_getenv("HOME");
                        if(envvar)
                        {
                            escaped = g_strescape(envvar, NULL);
                            fprintf(f, "%s", escaped);
                            g_free(escaped);
                            str++;
                        }
                    }
                    else if ((escaped = g_uri_parse_scheme(str))) /* a valid URI */
                        g_free(escaped);
                    else if(str[0] != '/')
                        fprintf(f, "%s/", cwd);
                    escaped = g_strescape(str, NULL);
                    fprintf(f, "%s\n", escaped);
                    g_free(escaped);
                }
            }
            break;
        }
        case G_OPTION_ARG_DOUBLE:
            fprintf(f, "--%s\n%lf\n", ent->long_name, *(gdouble*)ent->arg_data);
            break;
        case G_OPTION_ARG_INT64:
            fprintf(f, "--%s\n%lld\n", ent->long_name, (long long int)*(gint64*)ent->arg_data);
            break;
        case G_OPTION_ARG_CALLBACK:
            /* Not supported */
            break;
        }
    }
    fclose(f);
    g_free(cwd);
}

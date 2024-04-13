set_var_value(const gchar *name, gchar *val) {
    uzbl_cmdprop *c = NULL;
    char *endp = NULL;
    char *buf = NULL;
    char *invalid_chars = "^°!\"§$%&/()=?'`'+~*'#-.:,;@<>| \\{}[]¹²³¼½";
    GString *msg;

    if( (c = g_hash_table_lookup(uzbl.comm.proto_var, name)) ) {
        if(!c->writeable) return FALSE;

        msg = g_string_new(name);

        /* check for the variable type */
        if (c->type == TYPE_STR) {
            buf = g_strdup(val);
            g_free(*c->ptr.s);
            *c->ptr.s = buf;
            g_string_append_printf(msg, " str %s", buf);

        } else if(c->type == TYPE_INT) {
            *c->ptr.i = (int)strtoul(val, &endp, 10);
            g_string_append_printf(msg, " int %d", *c->ptr.i);

        } else if (c->type == TYPE_FLOAT) {
            *c->ptr.f = strtod(val, &endp);
            g_string_append_printf(msg, " float %f", *c->ptr.f);
        }

        send_event(VARIABLE_SET, msg->str, NULL);
        g_string_free(msg,TRUE);

        /* invoke a command specific function */
        if(c->func) c->func();
    } else {
        /* check wether name violates our naming scheme */
        if(strpbrk(name, invalid_chars)) {
            if (uzbl.state.verbose)
                printf("Invalid variable name\n");
            return FALSE;
        }

        /* custom vars */
        c = g_malloc(sizeof(uzbl_cmdprop));
        c->type = TYPE_STR;
        c->dump = 0;
        c->func = NULL;
        c->writeable = 1;
        buf = g_strdup(val);
        c->ptr.s = g_malloc(sizeof(char *));
        *c->ptr.s = buf;
        g_hash_table_insert(uzbl.comm.proto_var,
                g_strdup(name), (gpointer) c);

        msg = g_string_new(name);
        g_string_append_printf(msg, " str %s", buf);
        send_event(VARIABLE_SET, msg->str, NULL);
        g_string_free(msg,TRUE);
    }
    update_title();
    return TRUE;
}

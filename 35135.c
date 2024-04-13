dump_var_hash_as_event(gpointer k, gpointer v, gpointer ud) {
    (void) ud;
    uzbl_cmdprop *c = v;
    GString *msg;

    if(!c->dump)
        return;

    /* check for the variable type */
    msg = g_string_new((char *)k);
    if (c->type == TYPE_STR) {
        g_string_append_printf(msg, " str %s", *c->ptr.s ? *c->ptr.s : " ");
    } else if(c->type == TYPE_INT) {
        g_string_append_printf(msg, " int %d", *c->ptr.i);
    } else if (c->type == TYPE_FLOAT) {
        g_string_append_printf(msg, " float %f", *c->ptr.f);
    }

    send_event(VARIABLE_SET, msg->str, NULL);
    g_string_free(msg, TRUE);
}

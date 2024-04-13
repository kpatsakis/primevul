inline int web_client_api_request_v1(RRDHOST *host, struct web_client *w, char *url) {
    static int initialized = 0;
    int i;

    if(unlikely(initialized == 0)) {
        initialized = 1;

        for(i = 0; api_commands[i].command ; i++)
            api_commands[i].hash = simple_hash(api_commands[i].command);
    }

    char *tok = mystrsep(&url, "/?&");
    if(tok && *tok) {
        debug(D_WEB_CLIENT, "%llu: Searching for API v1 command '%s'.", w->id, tok);
        uint32_t hash = simple_hash(tok);

        for(i = 0; api_commands[i].command ;i++) {
            if(unlikely(hash == api_commands[i].hash && !strcmp(tok, api_commands[i].command))) {
                if(unlikely(api_commands[i].acl != WEB_CLIENT_ACL_NOCHECK) && !(w->acl & api_commands[i].acl))
                    return web_client_permission_denied(w);

                return api_commands[i].callback(host, w, url);
            }
        }

        buffer_flush(w->response.data);
        buffer_strcat(w->response.data, "Unsupported v1 API command: ");
        buffer_strcat_htmlescape(w->response.data, tok);
        return 404;
    }
    else {
        buffer_flush(w->response.data);
        buffer_sprintf(w->response.data, "Which API v1 command?");
        return 400;
    }
}

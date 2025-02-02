long vnc_client_write_buf(VncState *vs, const uint8_t *data, size_t datalen)
{
    long ret;
#ifdef CONFIG_VNC_TLS
    if (vs->tls.session) {
        ret = vnc_client_write_tls(&vs->tls.session, data, datalen);
    } else {
#ifdef CONFIG_VNC_WS
        if (vs->ws_tls.session) {
            ret = vnc_client_write_tls(&vs->ws_tls.session, data, datalen);
        } else
#endif /* CONFIG_VNC_WS */
#endif /* CONFIG_VNC_TLS */
        {
            ret = send(vs->csock, (const void *)data, datalen, 0);
        }
#ifdef CONFIG_VNC_TLS
    }
#endif /* CONFIG_VNC_TLS */
    VNC_DEBUG("Wrote wire %p %zd -> %ld\n", data, datalen, ret);
    return vnc_client_io_error(vs, ret, socket_error());
}

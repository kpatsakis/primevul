static void clear_id() {
    if (imapd_id.params) {
        freeattvalues(imapd_id.params);
    }
    memset(&imapd_id, 0, sizeof(struct id_data));
}

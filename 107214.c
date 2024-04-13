void preventCommandPropagation(client *c) {
    c->flags |= CLIENT_PREVENT_PROP;
}

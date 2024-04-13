void echoCommand(client *c) {
    addReplyBulk(c,c->argv[1]);
}

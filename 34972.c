cmd_len(conn c)
{
    return scan_line_end(c->cmd, c->cmd_read);
}

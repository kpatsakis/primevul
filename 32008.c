static void bomb_out(Ssh ssh, char *text)
{
    ssh_do_close(ssh, FALSE);
    logevent(text);
    connection_fatal(ssh->frontend, "%s", text);
    sfree(text);
}

do_cmd(conn c)
{
    dispatch_cmd(c);
    fill_extra_data(c);
}

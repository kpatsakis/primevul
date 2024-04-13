remove_reserved_job(conn c, job j)
{
    return remove_this_reserved_job(c, find_reserved_job_in_conn(c, j));
}

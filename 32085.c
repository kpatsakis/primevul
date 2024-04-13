static void ssh_throttle_conn(Ssh ssh, int adjust)
{
    int old_count = ssh->conn_throttle_count;
    ssh->conn_throttle_count += adjust;
    assert(ssh->conn_throttle_count >= 0);
    if (ssh->conn_throttle_count && !old_count) {
     }
 }

static int sockfs_setattr(struct dentry *dentry, struct iattr *iattr)
{
	int err = simple_setattr(dentry, iattr);

 	if (!err && (iattr->ia_valid & ATTR_UID)) {
 		struct socket *sock = SOCKET_I(d_inode(dentry));
 
		if (sock->sk)
			sock->sk->sk_uid = iattr->ia_uid;
		else
			err = -ENOENT;
 	}
 
 	return err;
}

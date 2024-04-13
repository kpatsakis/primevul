static int move_addr_to_user(struct sockaddr *kaddr, int klen,
			     void __user *uaddr, int __user *ulen)
{
	int err;
	int len;

	err = get_user(len, ulen);
	if (err)
		return err;
	if (len > klen)
		len = klen;
	if (len < 0 || len > sizeof(struct sockaddr_storage))
		return -EINVAL;
	if (len) {
		if (audit_sockaddr(klen, kaddr))
			return -ENOMEM;
		if (copy_to_user(uaddr, kaddr, len))
			return -EFAULT;
	}
	/*
	 *      "fromlen shall refer to the value before truncation.."
	 *                      1003.1g
	 */
	return __put_user(klen, ulen);
}

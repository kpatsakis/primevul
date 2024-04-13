static long oz_cdev_ioctl(struct file *filp, unsigned int cmd,
			  unsigned long arg)
{
	int rc = 0;

	if (_IOC_TYPE(cmd) != OZ_IOCTL_MAGIC)
		return -ENOTTY;
	if (_IOC_NR(cmd) > OZ_IOCTL_MAX)
		return -ENOTTY;
	if (_IOC_DIR(cmd) & _IOC_READ)
		rc = !access_ok(VERIFY_WRITE, (void __user *)arg,
			_IOC_SIZE(cmd));
	else if (_IOC_DIR(cmd) & _IOC_WRITE)
		rc = !access_ok(VERIFY_READ, (void __user *)arg,
			_IOC_SIZE(cmd));
	if (rc)
		return -EFAULT;
	switch (cmd) {
	case OZ_IOCTL_GET_PD_LIST: {
			struct oz_pd_list list;
			oz_dbg(ON, "OZ_IOCTL_GET_PD_LIST\n");
			memset(&list, 0, sizeof(list));
			list.count = oz_get_pd_list(list.addr, OZ_MAX_PDS);
			if (copy_to_user((void __user *)arg, &list,
				sizeof(list)))
				return -EFAULT;
		}
		break;
	case OZ_IOCTL_SET_ACTIVE_PD: {
			u8 addr[ETH_ALEN];
			oz_dbg(ON, "OZ_IOCTL_SET_ACTIVE_PD\n");
			if (copy_from_user(addr, (void __user *)arg, ETH_ALEN))
				return -EFAULT;
			rc = oz_set_active_pd(addr);
		}
		break;
	case OZ_IOCTL_GET_ACTIVE_PD: {
			u8 addr[ETH_ALEN];
			oz_dbg(ON, "OZ_IOCTL_GET_ACTIVE_PD\n");
			spin_lock_bh(&g_cdev.lock);
			memcpy(addr, g_cdev.active_addr, ETH_ALEN);
			spin_unlock_bh(&g_cdev.lock);
			if (copy_to_user((void __user *)arg, addr, ETH_ALEN))
				return -EFAULT;
		}
		break;
	case OZ_IOCTL_ADD_BINDING:
	case OZ_IOCTL_REMOVE_BINDING: {
			struct oz_binding_info b;
			if (copy_from_user(&b, (void __user *)arg,
				sizeof(struct oz_binding_info))) {
				return -EFAULT;
			}
			/* Make sure name is null terminated. */
			b.name[OZ_MAX_BINDING_LEN-1] = 0;
			if (cmd == OZ_IOCTL_ADD_BINDING)
				oz_binding_add(b.name);
			else
				oz_binding_remove(b.name);
		}
		break;
	}
	return rc;
}

int wvlan_rts(struct rtsreq *rrq, __u32 io_base)
{
	int ioctl_ret = 0;
	/*------------------------------------------------------------------------*/


	DBG_FUNC("wvlan_rts");
	DBG_ENTER(DbgInfo);


	DBG_PRINT("io_base: 0x%08x\n", io_base);

	switch (rrq->typ) {
	case WL_IOCTL_RTS_READ:
		DBG_TRACE(DbgInfo, "IOCTL: WVLAN2_IOCTL_RTS -- WL_IOCTL_RTS_READ\n");
		rrq->data[0] = IN_PORT_WORD(io_base + rrq->reg);
		DBG_TRACE(DbgInfo, "  reg 0x%04x ==> 0x%04x\n", rrq->reg, CNV_LITTLE_TO_SHORT(rrq->data[0]));
		break;
	case WL_IOCTL_RTS_WRITE:
		DBG_TRACE(DbgInfo, "IOCTL: WVLAN2_IOCTL_RTS -- WL_IOCTL_RTS_WRITE\n");
		OUT_PORT_WORD(io_base + rrq->reg, rrq->data[0]);
		DBG_TRACE(DbgInfo, "  reg 0x%04x <== 0x%04x\n", rrq->reg, CNV_LITTLE_TO_SHORT(rrq->data[0]));
		break;
	case WL_IOCTL_RTS_BATCH_READ:
		DBG_TRACE(DbgInfo, "IOCTL: WVLAN2_IOCTL_RTS -- WL_IOCTL_RTS_BATCH_READ\n");
		IN_PORT_STRING_16(io_base + rrq->reg, rrq->data, rrq->len);
		DBG_TRACE(DbgInfo, "  reg 0x%04x ==> %d bytes\n", rrq->reg, rrq->len * sizeof(__u16));
		break;
	case WL_IOCTL_RTS_BATCH_WRITE:
		DBG_TRACE(DbgInfo, "IOCTL: WVLAN2_IOCTL_RTS -- WL_IOCTL_RTS_BATCH_WRITE\n");
		OUT_PORT_STRING_16(io_base + rrq->reg, rrq->data, rrq->len);
		DBG_TRACE(DbgInfo, "  reg 0x%04x <== %d bytes\n", rrq->reg, rrq->len * sizeof(__u16));
		break;
	default:

		DBG_TRACE(DbgInfo, "IOCTL: WVLAN2_IOCTL_RTS -- UNSUPPORTED RTS CODE: 0x%X", rrq->typ);
		ioctl_ret = -EOPNOTSUPP;
		break;
	}

	DBG_LEAVE(DbgInfo);
	return ioctl_ret;
} /* wvlan_rts */

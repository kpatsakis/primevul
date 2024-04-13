static int decode_read(struct xdr_stream *xdr, struct rpc_rqst *req, struct nfs_readres *res)
{
	struct kvec *iov = req->rq_rcv_buf.head;
	__be32 *p;
	uint32_t count, eof, recvd, hdrlen;
	int status;

	status = decode_op_hdr(xdr, OP_READ);
	if (status)
		return status;
	READ_BUF(8);
	READ32(eof);
	READ32(count);
	hdrlen = (u8 *) p - (u8 *) iov->iov_base;
	recvd = req->rq_rcv_buf.len - hdrlen;
	if (count > recvd) {
		dprintk("NFS: server cheating in read reply: "
				"count %u > recvd %u\n", count, recvd);
		count = recvd;
		eof = 0;
	}
	xdr_read_pages(xdr, count);
	res->eof = eof;
	res->count = count;
	return 0;
}

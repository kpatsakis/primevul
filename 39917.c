static int iucv_send_iprm(struct iucv_path *path, struct iucv_message *msg,
			  struct sk_buff *skb)
{
	u8 prmdata[8];

	memcpy(prmdata, (void *) skb->data, skb->len);
	prmdata[7] = 0xff - (u8) skb->len;
	return pr_iucv->message_send(path, msg, IUCV_IPRMDATA, 0,
				 (void *) prmdata, 8);
}

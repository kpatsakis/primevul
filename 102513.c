static int crypto_ablkcipher_report(struct sk_buff *skb, struct crypto_alg *alg)
 {
 	struct crypto_report_blkcipher rblkcipher;
 
	strncpy(rblkcipher.type, "ablkcipher", sizeof(rblkcipher.type));
	strncpy(rblkcipher.geniv, alg->cra_ablkcipher.geniv ?: "<default>",
		sizeof(rblkcipher.geniv));
 
 	rblkcipher.blocksize = alg->cra_blocksize;
 	rblkcipher.min_keysize = alg->cra_ablkcipher.min_keysize;
	rblkcipher.max_keysize = alg->cra_ablkcipher.max_keysize;
	rblkcipher.ivsize = alg->cra_ablkcipher.ivsize;

	if (nla_put(skb, CRYPTOCFGA_REPORT_BLKCIPHER,
		    sizeof(struct crypto_report_blkcipher), &rblkcipher))
		goto nla_put_failure;
	return 0;

nla_put_failure:
	return -EMSGSIZE;
}

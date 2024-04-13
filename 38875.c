static int ip_vs_genl_fill_stats(struct sk_buff *skb, int container_type,
				 struct ip_vs_stats *stats)
{
	struct nlattr *nl_stats = nla_nest_start(skb, container_type);
	if (!nl_stats)
		return -EMSGSIZE;

	spin_lock_bh(&stats->lock);

	NLA_PUT_U32(skb, IPVS_STATS_ATTR_CONNS, stats->ustats.conns);
	NLA_PUT_U32(skb, IPVS_STATS_ATTR_INPKTS, stats->ustats.inpkts);
	NLA_PUT_U32(skb, IPVS_STATS_ATTR_OUTPKTS, stats->ustats.outpkts);
	NLA_PUT_U64(skb, IPVS_STATS_ATTR_INBYTES, stats->ustats.inbytes);
	NLA_PUT_U64(skb, IPVS_STATS_ATTR_OUTBYTES, stats->ustats.outbytes);
	NLA_PUT_U32(skb, IPVS_STATS_ATTR_CPS, stats->ustats.cps);
	NLA_PUT_U32(skb, IPVS_STATS_ATTR_INPPS, stats->ustats.inpps);
	NLA_PUT_U32(skb, IPVS_STATS_ATTR_OUTPPS, stats->ustats.outpps);
	NLA_PUT_U32(skb, IPVS_STATS_ATTR_INBPS, stats->ustats.inbps);
	NLA_PUT_U32(skb, IPVS_STATS_ATTR_OUTBPS, stats->ustats.outbps);

	spin_unlock_bh(&stats->lock);

	nla_nest_end(skb, nl_stats);

	return 0;

nla_put_failure:
	spin_unlock_bh(&stats->lock);
	nla_nest_cancel(skb, nl_stats);
	return -EMSGSIZE;
}

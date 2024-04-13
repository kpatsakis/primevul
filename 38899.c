static int ip_vs_stats_show(struct seq_file *seq, void *v)
{

/*               01234567 01234567 01234567 0123456701234567 0123456701234567 */
	seq_puts(seq,
		 "   Total Incoming Outgoing         Incoming         Outgoing\n");
	seq_printf(seq,
		   "   Conns  Packets  Packets            Bytes            Bytes\n");

	spin_lock_bh(&ip_vs_stats.lock);
	seq_printf(seq, "%8X %8X %8X %16LX %16LX\n\n", ip_vs_stats.ustats.conns,
		   ip_vs_stats.ustats.inpkts, ip_vs_stats.ustats.outpkts,
		   (unsigned long long) ip_vs_stats.ustats.inbytes,
		   (unsigned long long) ip_vs_stats.ustats.outbytes);

/*                 01234567 01234567 01234567 0123456701234567 0123456701234567 */
	seq_puts(seq,
		   " Conns/s   Pkts/s   Pkts/s          Bytes/s          Bytes/s\n");
	seq_printf(seq,"%8X %8X %8X %16X %16X\n",
			ip_vs_stats.ustats.cps,
			ip_vs_stats.ustats.inpps,
			ip_vs_stats.ustats.outpps,
			ip_vs_stats.ustats.inbps,
			ip_vs_stats.ustats.outbps);
	spin_unlock_bh(&ip_vs_stats.lock);

	return 0;
}

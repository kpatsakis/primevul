clear_stats (struct net_device *dev)
{
	long ioaddr = dev->base_addr;
#ifdef MEM_MAPPING
	int i;
#endif

	/* All statistics registers need to be acknowledged,
	   else statistic overflow could cause problems */
	readl (ioaddr + FramesRcvOk);
	readl (ioaddr + FramesXmtOk);
	readl (ioaddr + OctetRcvOk);
	readl (ioaddr + OctetXmtOk);

	readl (ioaddr + McstFramesRcvdOk);
	readl (ioaddr + SingleColFrames);
	readl (ioaddr + MultiColFrames);
	readl (ioaddr + LateCollisions);
	/* detailed rx errors */
	readw (ioaddr + FrameTooLongErrors);
	readw (ioaddr + InRangeLengthErrors);
	readw (ioaddr + FramesCheckSeqErrors);
	readw (ioaddr + FramesLostRxErrors);

	/* detailed tx errors */
	readw (ioaddr + FramesAbortXSColls);
	readw (ioaddr + CarrierSenseErrors);

	/* Clear all other statistic register. */
	readl (ioaddr + McstOctetXmtOk);
	readw (ioaddr + BcstFramesXmtdOk);
	readl (ioaddr + McstFramesXmtdOk);
	readw (ioaddr + BcstFramesRcvdOk);
	readw (ioaddr + MacControlFramesRcvd);
	readl (ioaddr + McstOctetXmtOk);
	readl (ioaddr + BcstOctetXmtOk);
	readl (ioaddr + McstFramesXmtdOk);
	readl (ioaddr + FramesWDeferredXmt);
	readw (ioaddr + BcstFramesXmtdOk);
	readw (ioaddr + MacControlFramesXmtd);
	readw (ioaddr + FramesWEXDeferal);
#ifdef MEM_MAPPING
	for (i = 0x100; i <= 0x150; i += 4)
		readl (ioaddr + i);
#endif
	readw (ioaddr + TxJumboFrames);
	readw (ioaddr + RxJumboFrames);
	readw (ioaddr + TCPCheckSumErrors);
	readw (ioaddr + UDPCheckSumErrors);
	readw (ioaddr + IPCheckSumErrors);
	return 0;
}

void IDNSpoofChecker::RestoreTrieParamsForTesting() {
  g_trie_params = HuffmanTrieParams{
      kTopDomainsHuffmanTree, sizeof(kTopDomainsHuffmanTree), kTopDomainsTrie,
      kTopDomainsTrieBits, kTopDomainsRootPosition};
}

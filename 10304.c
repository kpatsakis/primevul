static int __find_symbol_by_paddr(const void *paddr, const void *sym) {
	return (int)!(*(ut64 *)paddr == ((RBinSymbol *)sym)->paddr);
}
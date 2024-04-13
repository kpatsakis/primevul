void op_addAvxCC(MCInst *MI, int v)
{
	if (MI->csh->detail) {
		MI->flat_insn->detail->x86.avx_cc = v;
	}
}

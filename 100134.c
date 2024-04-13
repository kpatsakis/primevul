void op_addXopCC(MCInst *MI, int v)
{
	if (MI->csh->detail) {
		MI->flat_insn->detail->x86.xop_cc = v;
	}
}

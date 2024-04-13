static bool opcode_matches(uint8_t *opcode, const TPRInstruction *instr)
{
    return opcode[0] == instr->opcode &&
        (!(instr->flags & TPR_INSTR_ABS_MODRM) || is_abs_modrm(opcode[1])) &&
        (!(instr->flags & TPR_INSTR_MATCH_MODRM_REG) ||
         modrm_reg(opcode[1]) == instr->modrm_reg);
}

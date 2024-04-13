static inline void apic_set_spiv(struct kvm_lapic *apic, u32 val)
{
	if ((kvm_apic_get_reg(apic, APIC_SPIV) ^ val) & APIC_SPIV_APIC_ENABLED) {
		if (val & APIC_SPIV_APIC_ENABLED)
			static_key_slow_dec_deferred(&apic_sw_disabled);
		else
			static_key_slow_inc(&apic_sw_disabled.key);
	}
	apic_set_reg(apic, APIC_SPIV, val);
}

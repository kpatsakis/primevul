static uint32_t vmsvga_value_read(void *opaque, uint32_t address)
{
    uint32_t caps;
    struct vmsvga_state_s *s = opaque;
    DisplaySurface *surface = qemu_console_surface(s->vga.con);
    PixelFormat pf;
    uint32_t ret;

    switch (s->index) {
    case SVGA_REG_ID:
        ret = s->svgaid;
        break;

    case SVGA_REG_ENABLE:
        ret = s->enable;
        break;

    case SVGA_REG_WIDTH:
        ret = s->new_width ? s->new_width : surface_width(surface);
        break;

    case SVGA_REG_HEIGHT:
        ret = s->new_height ? s->new_height : surface_height(surface);
        break;

    case SVGA_REG_MAX_WIDTH:
        ret = SVGA_MAX_WIDTH;
        break;

    case SVGA_REG_MAX_HEIGHT:
        ret = SVGA_MAX_HEIGHT;
        break;

    case SVGA_REG_DEPTH:
        ret = (s->new_depth == 32) ? 24 : s->new_depth;
        break;

    case SVGA_REG_BITS_PER_PIXEL:
    case SVGA_REG_HOST_BITS_PER_PIXEL:
        ret = s->new_depth;
        break;

    case SVGA_REG_PSEUDOCOLOR:
        ret = 0x0;
        break;

    case SVGA_REG_RED_MASK:
        pf = qemu_default_pixelformat(s->new_depth);
        ret = pf.rmask;
        break;

    case SVGA_REG_GREEN_MASK:
        pf = qemu_default_pixelformat(s->new_depth);
        ret = pf.gmask;
        break;

    case SVGA_REG_BLUE_MASK:
        pf = qemu_default_pixelformat(s->new_depth);
        ret = pf.bmask;
        break;

    case SVGA_REG_BYTES_PER_LINE:
        if (s->new_width) {
            ret = (s->new_depth * s->new_width) / 8;
        } else {
            ret = surface_stride(surface);
        }
        break;

    case SVGA_REG_FB_START: {
        struct pci_vmsvga_state_s *pci_vmsvga
            = container_of(s, struct pci_vmsvga_state_s, chip);
        ret = pci_get_bar_addr(PCI_DEVICE(pci_vmsvga), 1);
        break;
    }

    case SVGA_REG_FB_OFFSET:
        ret = 0x0;
        break;

    case SVGA_REG_VRAM_SIZE:
        ret = s->vga.vram_size; /* No physical VRAM besides the framebuffer */
        break;

    case SVGA_REG_FB_SIZE:
        ret = s->vga.vram_size;
        break;

    case SVGA_REG_CAPABILITIES:
        caps = SVGA_CAP_NONE;
#ifdef HW_RECT_ACCEL
        caps |= SVGA_CAP_RECT_COPY;
#endif
#ifdef HW_FILL_ACCEL
        caps |= SVGA_CAP_RECT_FILL;
#endif
#ifdef HW_MOUSE_ACCEL
        if (dpy_cursor_define_supported(s->vga.con)) {
            caps |= SVGA_CAP_CURSOR | SVGA_CAP_CURSOR_BYPASS_2 |
                    SVGA_CAP_CURSOR_BYPASS;
        }
#endif
        ret = caps;
        break;

    case SVGA_REG_MEM_START: {
        struct pci_vmsvga_state_s *pci_vmsvga
            = container_of(s, struct pci_vmsvga_state_s, chip);
        ret = pci_get_bar_addr(PCI_DEVICE(pci_vmsvga), 2);
        break;
    }

    case SVGA_REG_MEM_SIZE:
        ret = s->fifo_size;
        break;

    case SVGA_REG_CONFIG_DONE:
        ret = s->config;
        break;

    case SVGA_REG_SYNC:
    case SVGA_REG_BUSY:
        ret = s->syncing;
        break;

    case SVGA_REG_GUEST_ID:
        ret = s->guest;
        break;

    case SVGA_REG_CURSOR_ID:
        ret = s->cursor.id;
        break;

    case SVGA_REG_CURSOR_X:
        ret = s->cursor.x;
        break;

    case SVGA_REG_CURSOR_Y:
        ret = s->cursor.y;
        break;

    case SVGA_REG_CURSOR_ON:
        ret = s->cursor.on;
        break;

    case SVGA_REG_SCRATCH_SIZE:
        ret = s->scratch_size;
        break;

    case SVGA_REG_MEM_REGS:
    case SVGA_REG_NUM_DISPLAYS:
    case SVGA_REG_PITCHLOCK:
    case SVGA_PALETTE_BASE ... SVGA_PALETTE_END:
        ret = 0;
        break;

    default:
        if (s->index >= SVGA_SCRATCH_BASE &&
            s->index < SVGA_SCRATCH_BASE + s->scratch_size) {
            ret = s->scratch[s->index - SVGA_SCRATCH_BASE];
            break;
        }
        printf("%s: Bad register %02x\n", __func__, s->index);
        ret = 0;
        break;
    }

    if (s->index >= SVGA_SCRATCH_BASE) {
        trace_vmware_scratch_read(s->index, ret);
    } else if (s->index >= SVGA_PALETTE_BASE) {
        trace_vmware_palette_read(s->index, ret);
    } else {
        trace_vmware_value_read(s->index, ret);
    }
    return ret;
}

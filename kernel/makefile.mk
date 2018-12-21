
# Kernel objects
KERNEL_OBJECTS := \
	fs/bio.o\
	drivers/console.o\
	drivers/vesa.o\
	proc/exec.o\
	fs/file.o\
	fs/fs.o\
	drivers/ide.o\
	apic/ioapic.o\
	mm/kalloc.o\
	drivers/kbd.o\
	drivers/terminal.o\
	apic/lapic.o\
	main.o\
	drivers/mp.o\
	drivers/picirq.o\
	proc/pipe.o\
	proc/proc.o\
	drivers/spinlock.o\
	libc/string.o\
	proc/swtch.o\
	syscall/syscall.o\
	fs/sysfile.o\
	proc/sysproc.o\
	drivers/timer.o\
	trap/trapasm.o\
	trap/trap.o\
	drivers/uart.o\
	trap/vectors.o\
	mm/vm.o\


KERNEL_OBJECTS := $(addprefix kernel/, $(KERNEL_OBJECTS))

# Objects compiled in a special way for boot or alignment
KERNEL_SPECIAL_OBJECTS := \
	bootmain.o\
	bootasm.o\
	multiboot.o\
	data.o\
	bootother.o\
	initcode.o\
	bootgraphic.o\


KERNEL_SPECIAL_OBJECTS := $(addprefix kernel/boot/, $(KERNEL_SPECIAL_OBJECTS))

# Make dependency files
KERNEL_DEPS := \
	$(KERNEL_OBJECTS:.o=.d) \
	$(KERNEL_SPECIAL_OBJECTS:.o=.d)

# All generated files
KERNEL_CLEAN := \
	$(KERNEL_OBJECTS) \
	$(KERNEL_SPECIAL_OBJECTS) \
	$(KERNEL_DEPS) \
	kernel/trap/vectors.S\
	kernel/bootblock\
	kernel/bootblock.out\
	kernel/bootother.out\
	kernel/initcode.out\
	kernel/kernel\
	bootother\
	initcode\
	xv6.img

# add include dir to search path for headers
KERNEL_CPPFLAGS += -Iinclude
KERNEL_CPPFLAGS += -Iinclude/tinystl
KERNEL_CPPFLAGS += -Ikernel/include
# do not search standard system paths for headers
KERNEL_CPPFLAGS += -nostdinc
# disable PIC (position independent code)
KERNEL_CFLAGS += -fno-pic 
# do not use GCC builtin funtions (used to optimize common functions)
KERNEL_CFLAGS += -fno-builtin
# disable some optimizations that assume one object will not alias an object
# of a different type
KERNEL_CFLAGS += -fno-strict-aliasing
# disable stack smashing check
KERNEL_CFLAGS += -fno-stack-protector
# generate code for 32-bit environment
KERNEL_CFLAGS += -m32

KERNEL_ASFLAGS += $(KERNEL_CFLAGS)

# FreeBSD ld wants ``elf_i386_fbsd''
KERNEL_LDFLAGS += -m $(shell $(LD) -V | grep elf_i386 2>/dev/null)
# do not link with the host standard library files
KERNEL_LDFLAGS += -nostdlib
# use simple contiguous section layout and do not use dynamic linking
KERNEL_LDFLAGS += --omagic

# bootable disk image
xv6.img: kernel/bootblock kernel/kernel
	dd if=/dev/zero of=xv6.img count=10000
	dd if=kernel/bootblock of=xv6.img conv=notrunc
	dd if=kernel/kernel of=xv6.img seek=1 conv=notrunc

kernel/kernel: $(KERNEL_OBJECTS) kernel/boot/multiboot.o kernel/boot/data.o bootother initcode
	$(LD) $(LDFLAGS) $(KERNEL_LDFLAGS) \
		--section-start=.text=0x100000 --entry=main --output=kernel/kernel \
		kernel/boot/multiboot.o kernel/boot/data.o $(KERNEL_OBJECTS) \
		-b binary initcode bootother

# bootblock is optimized for space
kernel/boot/bootmain.o: kernel/boot/bootmain.c
	$(CC) $(CPPFLAGS) $(KERNEL_CPPFLAGS) $(CFLAGS) $(KERNEL_CFLAGS) \
		 -Os -c -o $@ $<

kernel/bootblock: kernel/boot/bootasm.o kernel/boot/bootmain.o kernel/boot/bootgraphic.o
	$(LD) $(LDFLAGS) $(KERNEL_LDFLAGS) \
		--entry=start --section-start=.text=0x7C00 \
		--output=kernel/bootblock.out kernel/boot/bootasm.o kernel/boot/bootmain.o kernel/boot/bootgraphic.o
	$(OBJCOPY) -S -O binary -j .text kernel/bootblock.out kernel/bootblock
	./kernel/sign.pl kernel/bootblock

bootother: kernel/boot/bootother.o
	$(LD) $(LDFLAGS) $(KERNEL_LDFLAGS) \
		--entry=start --section-start=.text=0x7000 \
		--output=kernel/bootother.out kernel/boot/bootother.o
	$(OBJCOPY) -S -O binary kernel/bootother.out $@

initcode: kernel/boot/initcode.o
	$(LD) $(LDFLAGS) $(KERNEL_LDFLAGS) \
		--entry=start --section-start=.text=0x0 \
		--output=kernel/initcode.out kernel/boot/initcode.o
	$(OBJCOPY) -S -O binary kernel/initcode.out $@

kernel/trap/vectors.S: kernel/trap/vectors.pl
	perl kernel/trap/vectors.pl > $@

# default recipe for object files
kernel/%.o: kernel/%.c
	$(CC) $(CPPFLAGS) $(KERNEL_CPPFLAGS) $(CFLAGS) $(KERNEL_CFLAGS) -c -o $@ $<

kernel/%.o: kernel/%.S
	$(AS) $(CPPFLAGS) $(KERNEL_CPPFLAGS) $(ASFLAGS) $(KERNEL_ASFLAGS) -c $< -o $@

kernel/%.o: kernel/%.cpp
	$(CXX) $(CPPFLAGS) $(KERNEL_CPPFLAGS) $(CFLAGS) $(CXX_FLAGS) $(KERNEL_CFLAGS) -c -o $@ $<

# default recipes for dependency files
kernel/%.d: kernel/%.c
	$(CC) $(CPPFLAGS) $(KERNEL_CPPFLAGS) $(CFLAGS) $(KERNEL_CFLAGS) \
		-M -MG $< -MF $@ -MT $@ -MT $(<:.c=.o)

kernel/%.d: kernel/%.cpp
	$(CXX) $(CPPFLAGS) $(KERNEL_CPPFLAGS) $(CFLAGS) $(CXX_FLAGS) $(KERNEL_CFLAGS) \
		-M -MG $< -MF $@ -MT $@ -MT $(<:.cpp=.o)

kernel/%.d: kernel/%.S
	$(AS) $(CPPFLAGS) $(KERNEL_CPPFLAGS) $(CFLAGS) $(KERNEL_CFLAGS) \
		-M -MG $< -MF $@ -MT $@ -MT $(<:.S=.o)


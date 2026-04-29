IMAGE_NAME = SnowOS

OVMFPATH = /usr/share/ovmf/x64/OVMF.4m.fd

QEMUFLAGS = --enable-kvm \
			-M q35 \
			-m 1G \
			-bios $(OVMFPATH)\
			-serial stdio

.PHONY: all kernel image iso

all:
	make image

image: kernel
	dd if=/dev/zero of=$(IMAGE_NAME).img bs=1048576 count=256
	sgdisk $(IMAGE_NAME).img -o --new=1 --typecode=1:ef00 -m 1
	./Limine/limine bios-install $(IMAGE_NAME).img
	sudo losetup /dev/loop0 $(IMAGE_NAME).img
	sudo mkfs.fat -F 32 /dev/loop0
	sudo mount /dev/loop0 /mnt/
	sudo mkdir /mnt/EFI
	sudo mkdir /mnt/EFI/BOOT
	sudo cp Limine/BOOTX64.EFI /mnt/EFI/BOOT/
	sudo cp yuki/limine.conf /mnt/EFI/BOOT/
	sudo cp Limine/limine-bios.sys /mnt/EFI/BOOT/
	sudo cp build/yuki/yuki.exe /mnt/
	sudo cp build/subprojects/hello/source/hello.exe /mnt/
	sudo cp initrd.tar.gz /mnt/
	sudo umount /mnt
	sudo losetup -d /dev/loop0
	@echo "Finished generating image file!"

iso: kernel
	rm -rf iso_root
	mkdir -p iso_root
	cp -v build/yuki/yuki.exe iso_root/
	cp -v build/subprojects/hello/source/hello.exe iso_root/
	mkdir -p iso_root/limine
	cp -v yuki/limine.conf iso_root/limine/
	mkdir -p iso_root/EFI/BOOT
	cp -v Limine/limine-bios.sys Limine/limine-bios-cd.bin Limine/limine-uefi-cd.bin iso_root/limine/
	cp -v Limine/BOOTX64.EFI iso_root/EFI/BOOT/
	xorriso -as mkisofs -R -r -J -b limine/limine-bios-cd.bin \
			-no-emul-boot -boot-load-size 4 -boot-info-table -hfsplus \
			-apm-block-size 2048 --efi-boot limine/limine-uefi-cd.bin \
			-efi-boot-part --efi-boot-image --protective-msdos-label \
			iso_root -o SnowOS.iso
	./Limine/limine bios-install SnowOS.iso

kernel:
	meson compile -C build

run:
	qemu-system-x86_64 $(QEMUFLAGS) -cdrom SnowOS.iso

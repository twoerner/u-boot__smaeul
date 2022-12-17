// SPDX-License-Identifier: GPL-2.0+

#include <dm.h>
#include <fdt_support.h>
#include <ram.h>
#include <spl.h>
#include <asm/spl.h>
#include <linux/types.h>

void board_debug_uart_init(void);

void board_init_f(ulong dummy)
{
	struct udevice *dev;

	board_debug_uart_init();
	spl_early_init();
	preloader_console_init();
	uclass_get_device(UCLASS_RAM, 0, &dev);
}

u32 spl_boot_device(void)
{
	return BOOT_DEVICE_MMC1;
}

void spl_perform_fixups(struct spl_image_info *spl_image)
{
	struct ram_info info;
	struct udevice *dev;
	int ret;

	ret = uclass_get_device(UCLASS_RAM, 0, &dev);
	if (ret)
		panic("No RAM device");

	ret = ram_get_info(dev, &info);
	if (ret)
		panic("No RAM info");

	ret = fdt_fixup_memory(spl_image->fdt_addr, info.base, info.size);
	if (ret)
		panic("Failed to update DTB");
}

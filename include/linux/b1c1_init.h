/* SPDX-License-Identifier: GPL-2.0
 *
 * Copyright (C) 2018-2019 Sultan Alsawaf <sultan@kerneltoast.com>.
 */
#ifndef _B1C1_INIT_H_
#define _B1C1_INIT_H_

#include <linux/init.h>

/* Selected and ordered according to /vendor/etc/init.insmod.cfg */
enum b1c1_init_id {
	B1C1_WLAN,
	B1C1_PINCTRL_WCD,
	B1C1_WCD_CORE,
	B1C1_WCD_DSP_GLINK,
	B1C1_SND_SOC_WCD9XXX,
	B1C1_SND_SOC_WCD934X,
	B1C1_SND_SOC_WCD_SPI,
	B1C1_SND_SOC_CS35L36,
	B1C1_SND_SOC_SDM845,
	B1C1_FTM5,
	B1C1_SEC_TOUCH,
	B1C1_INIT_MAX
};

#if defined(CONFIG_BOARD_B1C1) && !defined(CONFIG_MODULES)
int register_b1c1_init_fn(int (*fn)(void), enum b1c1_init_id id) __init;

/* The function passed to b1c1_init CANNOT be in the .init section! */
#define b1c1_init(fn, id) \
static int __init fn##_init(void)		\
{						\
	return register_b1c1_init_fn(fn, id);	\
}						\
early_initcall(fn##_init)
#else
#define b1c1_init(fn, id) module_init(fn)
#endif

#endif /* _B1C1_INIT_H_ */

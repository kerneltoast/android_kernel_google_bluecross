// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (C) 2018-2019 Sultan Alsawaf <sultan@kerneltoast.com>.
 */

#include <linux/b1c1_init.h>
#include <linux/capability.h>
#include <linux/syscalls.h>

static int (*b1c1_init_fns[B1C1_INIT_MAX])(void);
static atomic_t b1c1_init_done = ATOMIC_INIT(0);

int __init register_b1c1_init_fn(int (*fn)(void), enum b1c1_init_id id)
{
	WARN(b1c1_init_fns[id], "b1c1_init: Double register for id %d\n", id);
	b1c1_init_fns[id] = fn;
	return 0;
}

/* Hijack the absent init_module syscall to determine when to init */
SYSCALL_DEFINE3(init_module, void __user *, umod,
		unsigned long, len, const char __user *, uargs)
{
	if (capable(CAP_SYS_MODULE) && !atomic_cmpxchg(&b1c1_init_done, 0, 1)) {
		int i;

		for (i = 0; i < ARRAY_SIZE(b1c1_init_fns); i++) {
			if (!WARN_ON(!b1c1_init_fns[i]))
				b1c1_init_fns[i]();
		}
	}

	return -ENOSYS;
}

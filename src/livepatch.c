/* SPDX-License-Identifier: GPL-3.0-or-later */
/** @copyright 2022
 * @file proc.c
 */

#define CONFIG_TRACE_IRQFLAGS
#define CONFIG_IRQSOFF_TRACER
#define CONFIG_PREEMPT_TRACER

#include <asm/percpu.h>
#ifdef CONFIG_TRACE_IRQFLAGS
DEFINE_PER_CPU(int, hardirqs_enabled);
DEFINE_PER_CPU(int, hardirq_context);
// EXPORT_PER_CPU_SYMBOL_GPL(hardirqs_enabled);
// EXPORT_PER_CPU_SYMBOL_GPL(hardirq_context);
#endif

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/version.h>

#include <linux/livepatch.h>
#include <linux/seq_file.h>

static int livepatch_init(void);
static void livepatch_exit(void);

module_init(livepatch_init);
module_exit(livepatch_exit);
MODULE_LICENSE("GPL");
MODULE_INFO(livepatch, "Y");

static inline bool livepatch_lockdep_softirq_start(void)
{
	bool in_hardirq = false;

	if (lockdep_hardirq_context()) {
		in_hardirq = true;
#if LINUX_VERSION_CODE < KERNEL_VERSION(5, 7, 0) /* 2502ec37 */
		trace_hardirq_exit();
#else
		lockdep_hardirq_exit();
#endif
	}

	lockdep_softirq_enter();

	return in_hardirq;
}

static inline void livepatch_lockdep_softirq_end(bool in_hardirq)
{
	lockdep_softirq_exit();

	if (in_hardirq)
#if LINUX_VERSION_CODE < KERNEL_VERSION(5, 7, 0) /* 2502ec37 */
		trace_hardirq_enter();
#else
		lockdep_hardirq_enter();
#endif
}

static struct klp_func funcs[] = {
	{
		.old_name = "lockdep_softirq_start",
		.new_func = livepatch_lockdep_softirq_start,
	},
	{
		.old_name = "lockdep_softirq_end",
		.new_func = livepatch_lockdep_softirq_end,
	},
	{}
};
static struct klp_object objs[] = { {
					    .funcs = funcs,
				    },
				    {} };
static struct klp_patch patch = {
	.mod = THIS_MODULE,
	.objs = objs,
};
static int livepatch_init(void)
{
	int ret = 0;
	// ret = klp_register_patch(&patch); TODO find the documentation
	if (ret)
		return ret;
	ret = klp_enable_patch(&patch);
	if (ret) {
		// WARN_ON(klp_unregister_patch(&patch));
		return ret;
	}
	return 0;
}
static void livepatch_exit(void)
{
	// WARN_ON(klp_unregister_patch(&patch));
}

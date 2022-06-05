/* SPDX-License-Identifier: GPL-3.0-or-later */
/** @copyright 2022
 * @file proc.c
 */
// #define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

// necessary for linux module
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/version.h>
// basic memory management and data structure
#include <linux/list_sort.h>
#include <linux/mm.h>
// #include <linux/sched/mm.h>
// interrupt and timer utility
#include <linux/irq.h>
#include <linux/timer.h>
#include <linux/hrtimer.h>
// procfs utility
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
// mutex lock utility
#include <linux/mutex.h>

/*Linked List Node*/
struct probe_list {
	struct list_head list; //linux kernel list implementation
	int data;
};
static LIST_HEAD(probe_list_head);

static int variable;
static struct proc_dir_entry *gprocdir;

static int probe_module_init(void);
static void probe_module_exit(void);
static int probe_interrupt(struct inode *inode, struct file *file);
static ssize_t probe_echo_with_mm(struct file *filp, const char __user *ubuf,
				  size_t count, loff_t *off);

#if LINUX_VERSION_CODE <= KERNEL_VERSION(5, 5, 0) /* TODO */
static const struct file_operations proc_ops_interrupt = {
	.owner = THIS_MODULE,
	.open = probe_interrupt,
	.read = seq_read,
	.write = probe_echo_with_mm,
	.llseek = seq_lseek,
	.release = single_release,
};
#else
static const struct proc_ops proc_ops_interrupt = {
	.proc_open = probe_interrupt,
	.proc_read = seq_read,
	.proc_write = probe_echo_with_mm,
	.proc_lseek = seq_lseek,
	.proc_release = single_release,
}; /* https://git.kernel.org/pub/scm/linux/kernel/git/torvalds/linux.git/commit/?id=97a3253 */
#endif

module_init(probe_module_init); // or init_module()
module_exit(probe_module_exit); // or
module_param(variable, int, 0);
MODULE_PARM_DESC(variable, "replace (default=0)");
MODULE_AUTHOR("Author");
MODULE_VERSION("1:0.0.1");
MODULE_LICENSE("GPL");
MODULE_INFO(DKMS_Module, "Y");
DEFINE_MUTEX(mtx);

static int __init probe_module_init(void) // TODO the meaning and usage of __init
{
	struct proc_dir_entry *entry_file = NULL;
	// print_modules();
	/** procfs */
	gprocdir = proc_mkdir("probe", NULL);
	if (gprocdir == NULL) {
		goto procfs_error;
	}
	entry_file =
		proc_create("dkms_proc", 0766, gprocdir, &proc_ops_interrupt);
	if (entry_file == NULL) {
		goto procfs_error;
	}
	/** config and irq management */
	struct my_list *temp_node = NULL;
	pr_info("probe_list\n");
	temp_node = kmalloc(sizeof(struct probe_list),
			    GFP_KERNEL); // TODO GFP_KERNEL
	if (temp_node == NULL) {
		goto kmalloc_error;
	}
	pr_debug("probe init!");
	return 0;
error:
	//list_del();
kmalloc_error:
	kfree(temp_node);
procfs_error:
	remove_proc_subtree("probe", NULL);
error_return:
	return -(ENOMEM);
}

static void __exit probe_module_exit(void)
{
	remove_proc_subtree("probe", NULL);
	struct probe_list *cursor, *temp;
	list_for_each_entry_safe (cursor, temp, &probe_list_head, list) {
		list_del(&cursor->list);
		kfree(cursor);
	}
	pr_info("probe exit!");
}

static int probe_interrupt_handle(struct seq_file *seq, void *v)
{
	if (mutex_lock_interruptible(&mtx))
		return -ERESTARTSYS;
	seq_printf(seq, "interrput_handle\n");
	mutex_unlock(&mtx);
	return 0;
}

static int probe_interrupt(struct inode *inode, struct file *file)
{
	return single_open(file, probe_interrupt_handle, NULL);
}
static ssize_t probe_echo_with_mm(struct file *filp, const char __user *ubuf,
				  size_t count, loff_t *off)
{
	char buf[8]; /* ? */
	int ret = count;
	unsigned long configval = 0;

	if (mutex_lock_interruptible(&mtx))
		return -(ERESTARTSYS);
	if (count == 0 || count > sizeof(buf)) {
		ret = -EINVAL;
		goto out;
	}
	printk("ok1\n");
	if (copy_from_user(buf, ubuf, count)) {
		ret = -EFAULT;
		goto out;
	}
	printk("ok2\n");
	buf[count - 1] = '\0';
	pr_debug("user sent: buf = %s\n", buf);
	ret = kstrtoul(buf, 0, &configval);
	if (ret)
		goto out;
	ret = count;
out:
	mutex_unlock(&mtx);
	return ret;
}

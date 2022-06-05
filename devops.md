# devops

> [oscamp/proj154](https://github.com/oscomp/proj154-linux-realtime-probe-tool)
>
> compare to oscamp/proj122 oscomp/proj133 oscamp/proj118 oscamp/proj6

[TOC]

## General

According to the document, an user space client and a kernel module should be implemented and communicate with each other by socket API, where procfs is used in kernel space. This kernel module should be reliable (task 3), sensitive about every given IT (interrupt) and corresponding process.

Given that resources toward kernel framework, we think these knowledge are also important for further linux / operating system study, which is :

- kernel-RT
- dkms/kbuild system

We also have these concerns related to the project's topic:

- Rust language

## Goals

- [x] Week 1

  An dkms module with a functional procfs (or similar) implementation, works on kernel 5.15 (a LTS version).

  TODO: Test the function of dkms.

- [ ] Week 2

  Based on `kmalloc()` and `timer`, accomplish basic probe worker (at ms intervals) and file operation callback for irq probe. (Working)

  Try `hrtimer` with probe or livepatch for high accuracy probe worker.

- [ ] Week 3

  Out-of-tree module's loading at boot time (with `modprobe` or static load with `/etc/modules-load.d` and `mkinitramfs`), configuration loading.

  Configure a ci test with qemu environment in docker.

- [ ] N / A

  (Postponed) Implement a rust version

  _See <https://gitlab.eduxiji.net/Kaito/project788067-123436/-/blob/64eb4043/README.md> and <https://github.com/Rust-for-Linux/linux/blob/ec24f3d9/Documentation/rust/quick-start.rst>_

  Postponed for the configuration of build and development environment.

- [ ] CI for unit-test

  - to be implement.

- [ ] Misc

  - [ ] other modern interface for user-space interaction with the probe.

## Knowledge Bank

Diving into the manual for usage, into the source code for best practice.

<https://docs.kernel.org> is an essential for users and program developers that calls kernel's API, and any contributors interested in in-tree source code. However, for an out-of-tree kernel modules, kernel's documentation is seemed to

### Memory Management

- <https://www.kernel.org/doc/htmldocs/kernel-api/mm.html#id-1.6.2>
- <https://blog.cloudflare.com/diving-into-proc-pid-mem/>

#### Boot time memblock

#### Data Structure

_See <http://tele.sj.ifsc.edu.br/~msobral/prg2/kernel-ds.pdf>_

list. list_lru(cache) list_bl list_nulls list_sort

kfifo as queue in `<linux/kfifo.h>`.

idr as map(key, value) for UID.

rbtrees as binary tree.

### The legacy procfs

According to the kernel document:

> The new procfs behavior is more like other filesystems. Each procfs mount creates a new procfs instance. Mount options affect own procfs instance. It means that it became possible to have several procfs instances displaying tasks with different filtering options in one pid namespace.

From <https://www.kernel.org/doc/html/v5.15/filesystems/api-summary.html#the-proc-filesystem> and <https://www.kernel.org/doc/html/v5.15/filesystems/proc.html#>, it's so obvious that "There is no recent kernel documentation available for the proc API/ABI." (B means Binary)

From <https://www.kernel.org/doc/html/v5.15/filesystems/seq_file.html> which was wrote in 2003, it shows the measure to provide a virtual file in procfs.

Linux Kernel Procfs Guide by Erik (J.A.K.) Mouw shows api applied to kernel 2.4?

### Loading at boot time

_See <https://unix.stackexchange.com/questions/680689/load-kernel-module-at-boot-time-with-options>_

### Interrupt Model in Linux Kernel

term: NMI (Non Maskable Interrupt), SMP(Symmetric multiprocessing)

#### RTLinux with preemption

#### rt-tests

_See <https://wiki.linuxfoundation.org/realtime/documentation/howto/tools/rt-tests> and <https://wiki.archlinux.org/title/Realtime_kernel_patchset>_

### Probe / Tracepoint versus Livepatch

#### Probe and specific mechanic

_See <https://gitlab.eduxiji.net/202030445374/project788067-100200/-/blob/master/records/0525.md>, ch4 of book[^kpbook]_

#### Livepatch

_See <https://developer.ibm.com/tutorials/live-patching-the-linux-kernel/>, kernel's [`/lib/livepatch`](https://git.kernel.org/pub/scm/linux/kernel/git/torvalds/linux.git/tree/lib/livepatch?id=fc80c51)_

A prototype at [livepatch.c](./src/livepatch.c). Compiled successfully but with the execution error.

### Programming Prerequisites

- Meomory/Symbol

  - When shouldn't use `static`?

    The use of `static` is to prevent namespace pollution. Current kernel's namespace for symbols is available at `/proc/kallsyms`

- Portability

  - Kconfig difference may effect available ABI.

  Example: (`print_modules()` in `/module/main.c`)

```plain
ERROR: modpost: "print_modules" [**/oscomp-proj154/DKMS_Module.ko] undefined!
make[2]: *** [scripts/Makefile.modpost:150: **/oscomp-proj154/Module.symvers] Error 1
make[2]: *** Deleting file '**/oscomp-proj154/Module.symvers'
make[1]: *** [Makefile:1794: modules] Error 2
```

## Credits

- Linux Kernel Programming Part 2 : Char Device Drivers and Kernel Synchronization (2021) (9781801079518)

  [^kpbook] : _By Kaiwan N Billimoria_

- The Linux Kernel Module Programming Guide ver 2.6.4 (2007)

  _By Peter Jay Salzman, Michael Burian, Ori Pomerantz_

  [en:latest](https://tldp.org/LDP/lkmpg/2.6/html/index.html) [zh-tianjing:2.4](http://www.dirac.org/linux/writing/lkmpg/2.4/chinese/lkmpg_chs/index.html) [zh-tianjing:2.6](http://www.embeddedlinux.org.cn/linuxmod/)

- Linux Device Drivers 3rd Edition (2005) (9780596005900)

  _By Jonathan Corbet, Alessandro Rubini, Greg Kroah-Hartman_

> [oscamp/proj154](https://github.com/oscamp/proj154-linux-realtime-probe-tool)
> 
> compare to oscamp/proj122 oscomp/proj133 oscamp/proj118 oscamp/proj6

# General

According to the document, an user space client and a kernel module should be implemented and communicate with each other by socket API, where procfs is used in kernel space. This kernel module should be reliable (task 3), sensitive about every given IT (interrupt) and corresponding process.

Given that resources toward kernel framework, we think these knowledge are also important for further linux / operating system study, which is :

- kernel-RT
- dkms/kbuild system 

We also have these concerns related to the project's topic:

- Rust language 

# Goals

- [ ] **
  An dkms module with a functional procfs (or similar) implementation, works on kernel 5.15 (a LTS version).

- [ ] **Time**
  Implement a rust? version


# Knowledge Bank

## Memory Management

- <https://www.kernel.org/doc/htmldocs/kernel-api/mm.html#id-1.6.2>
- <https://blog.cloudflare.com/diving-into-proc-pid-mem/>

## The legacy procfs

According to the kernel document:

> The new procfs behavior is more like other filesystems. Each procfs mount creates a new procfs instance. Mount options affect own procfs instance. It means that it became possible to have several procfs instances displaying tasks with different filtering options in one pid namespace.

From <https://www.kernel.org/doc/html/v5.15/filesystems/api-summary.html#the-proc-filesystem> and <https://www.kernel.org/doc/html/v5.15/filesystems/proc.html#>, it's so obvious that "There is no recent kernel documentation available for the proc API/ABI." (B means Binary)

From <https://www.kernel.org/doc/html/v5.15/filesystems/seq_file.html> which was wrote in 2003, it shows the measure to provide a virtual file in procfs.

Linux Kernel Procfs Guide by Erik (J.A.K.) Mouw shows api applied to kernel 2.4?

## Programming Prerequisites

- Meomory/Symbol
  - When shouldn't use `static`?
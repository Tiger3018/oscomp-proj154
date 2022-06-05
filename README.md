# 全国大学生计算机系统能力大赛 OSCOMP proj154

> Original task on <https://github.com/oscomp/proj154-linux-realtime-probe-tool>.
>
> Original repository at <https://github.com/tiger3018/oscomp-proj154>.

See [devops.md](./devops.md) for infomation about project's analyses, design, plan, milestone, unit-test status and problem-solving.

See [background.md](./background.md) for infomation about the background and analyses of project's theme.

## Collaborators

@Tiger3018 :

@CrazyDogCN :

## Description

blank.

## Usage

### Kernel on Physical Computer

When developing, you can manually compile and re-insert module.

```shell
make && (sudo rmmod DKMS_Module; sudo insmod DKMS_Module.ko)
```

It's recommended to use the stable module with dkms.

```shell

```

### Virtual Machine on QEMU (for unit test, etc.)

## Credits

* [morrownr/8821cu-20210118](https://github.com/morrownr/8821cu-20210118)

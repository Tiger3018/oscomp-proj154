# Template dkms module

See [devops.md](./devops.md) for more infomation.

## Tips

- Priority of `Kbuild` is higher than `Makefile`. You should only have one of them in your folder, otherwise `Makefile` will be ignored.
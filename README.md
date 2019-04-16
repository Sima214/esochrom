# esochrom

A set of patches which make chromium's ui a bit more configurable.

Developed and used for [Esonov](https://github.com/Sima214/esonov-dotfiles) Arch Linux.

## Usage

Take the official PKGBUILD for [chromium](https://www.archlinux.org/packages/extra/x86_64/chromium/), copy the patches on the same folder as the PKGBUILD and finally modify the PKGBUILD so that the patches you want are applied on the `prepare` stage.
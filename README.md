# Gitty

<!-- Badges -->

![GitHub](https://img.shields.io/badge/OS-Linux%20%2F%20WSL-yellow)
[![Build Status](https://www.travis-ci.com/lianstuder/gitty.svg?branch=opt2_ncurses_lib)](https://www.travis-ci.com/lianstuder/gitty)

All features and more information on Gitty are available in the [the docs](https://github.com/lianstuder/gitty/blob/master/docs/README.md).

I paused developing this project for a while, since I am reorienting my self technologically and I started out with a completely wrong approach to develop this. I will plan this out a bit better and then I will start developing again, if I planned this properly.

## Build from source

Since gitty is written in Crystal, you will need to have the Crystal compiler installed. For now, this makes gitty only available on Linux or in a WSL. You can learn more on WSL [here](https://docs.microsoft.com/en-us/windows/wsl/install-win10).

To install Crystal, go to [the Crystal website](https://crystal-lang.org/install) and follow the instructions there.

You will also need to install Crystals dependency manager `shards`. You will need to do some research on how to install shards on your Linux distribution. Most of the times it will be included with Crystal directly, but not always (e.g. Gentoo Linux).

If you installed the Crystal compiler and - if necessary - shards, you can run the included `build.sh` script in the downloaded folder to build and install gitty. That's everything you need to do!

## License

This project is licensed under the [GNU GPLv3](https://github.com/lianstuder/gitty/blob/master/LICENSE)

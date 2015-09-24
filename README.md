# Green Electronics Repo
Libraries and code base for Stanford Green Electronics (EE155). This repository contains resources required for completing the labs and final project.

## Setup

Before you can program the STM32 development boards, you will need to set up the necessary toolchain on your machine. The toolchain has been tested on Ubuntu 14.04 which we suggest using for this toolchain. Other distributions/OSs will be left for you to setup and debug if you wish :).

Included in this repo is a script to install the necessary dependancies and setup the tools necessary for embedded ARM development. To run the script simply type `./setup.sh`. Note that you will be promped for your root password several times as it moves through the installation.

The tools will be installed in `~/ee155_tools` and you must run `source ~/ee155_tools/env.sh` before using the toolchain to put it on your path.

## Using the template

The template is adapted from mblythe86's F3 template: https://github.com/mblythe86/stm32f3-discovery-basic-template. This template provides an easy starter project that can take advantage of the standard libraries provided by STMicroelectronics and interface with a simple Makefile.

When starting a new project simply copy the template into a new folder with the following command `cp -r project-template my-new-project-name` and begin development. Depending on where you copy the project folder, you may need to modify the `EE155_LIB` parameter in the `Makefile`. Make sure that it points to the `libraries` folder.

The included Makefile will automatically detect all C source code in the `inc` and `src` directories. To compile the project run `make all`. Likewise to program the board, run `make program`. Additionally, the STM32F3 has the option to run GDB remotely on the board allowing you to interactively debug your code on the processor. Run `make debug` to program the board and launch GDB.

## Libraries

This repository includes a set of EE155 standard libraries used in this class. They are automatically linked against your code with the included Makefile. Under the `libraries` directory is also where you can install other custom libraries you find helpful.

Please take a look at the libraries to see what functionality is available to you.

## Datasheets

For convenience we have the most used datasheets when developing for the STM32F303 microprocessor. These are useful for figuring out pin mappings and registers especially when going past what the included libraries provide.

## Hardware

For reference the hardware designs used in this class are included.

## Labs

In the `labs` folder you will find starter code for the various labs.

## init_bitbucket

Included in the repository is an `init_bitbucket.sh` script. This file is included to automate the process of mapping the repository to a new, private repository hosted by bitbucket. Before running this script, you need to have created a bitbucket account. After execution, you will have initialized a new private repo and `git push` and `git pull` will be mapped there by default.

To pull the latest patches (which should be done at least once before starting a new lab) run `git pull upstream master`.
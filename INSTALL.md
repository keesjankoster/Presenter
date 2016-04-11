# INSTALL
Setup for Presenter running on a Raspberry Pi 2.

## Setup Raspbian Lite
- Download the latest Raspbian Lite image (https://www.raspberrypi.org/downloads/raspbian/)
- Use a tool like HDD Raw Copy Tool (http://hddguru.com/software/HDD-Raw-Copy-Tool/) to put the image on a SD Card
- Open the SD Card and edit config.txt in Notepad++ (https://notepad-plus-plus.org/) or similar, uncomment and change the following settings:

```
# Settings specific for 1024x768 projector

hdmi_group=2
hdmi_mode=16

hdmi_drive=2
```

- Boot the Raspberry Pi and run the Raspberry Pi Software Configuration Tool to set the following settings:

```
# sudo raspi-config
```

- Expand Filesystem
- Change User Password
- Boot Options: Console or Console Autologin
- Advanced Options / Hostname
- Reboot on finishing Raspi-Config
- Update software packages:

```
# sudo apt-get update
# sudo apt-get upgrade
# sudo apt-get dist-upgrade
```

## Install OpenFrameworks

- Download OpenFrameworks:

```
# cd ~
# curl -O http://openframeworks.cc/versions/v0.9.3/of_v0.9.3_linuxarmv6l_release.tar.gz
# mkdir openFrameworks
# tar vxfz of_v0.9.3_linuxarmv6l_release.tar.gz -C openFrameworks --strip-components 1
```

- Install packages and compile OpenFrameworks:

```
# cd ~/openFrameworks/scripts/linux/debian
# sudo ./install_dependencies.sh
# make Release -j4 -C ~/openFrameworks/libs/openFrameworksCompiled/project
```

## Install Presenter

- Install git:

```
# sudo apt-get install git
```

- Clone the ofxOMXPlayer git repository:

```
# cd ~/openFrameworks/addons/
# git clone https://github.com/jvcleave/ofxOMXPlayer.git
```

- Clone the Presenter git repository:

```
# cd ~/openFrameworks/apps/
# mkdir LUKETheatre
# cd LUKETheatre
# git clone https://github.com/keesjankoster/Presenter.git
```

- Compile Presenter:

```
# cd Presenter
# chmod 775 make.sh
# ./make.sh
```

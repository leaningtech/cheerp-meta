---
title: Ubuntu & Debian installation
---

Pre-built packages for Debian/Ubuntu are provided in our PPA

https://launchpad.net/~leaningtech-dev/+archive/ubuntu/cheerp-ppa

For other Linux distributions, please build from source

https://github.com/leaningtech/cheerp-meta/wiki/Linux-build-instructions

## Adding the repository
To add it to your system you have two options:

#### Manually edit sources.list (works on Debian testing / stretch)
add the following line to /etc/apt/sources.list
```
deb http://ppa.launchpad.net/leaningtech-dev/cheerp-ppa/ubuntu xenial main 
```
and import the key for apt with the command
```
sudo apt-key adv --keyserver keyserver.ubuntu.com --recv-keys 84540D4B9BF457D5
sudo apt-get update
```
or
#### Use add-apt-repository (Ubuntu only)
run the following command
```
sudo add-apt-repository ppa:leaningtech-dev/cheerp-ppa
sudo apt-get update
```

## Install packages

To install all cheerp components, run

```
apt-get install cheerp-core
```

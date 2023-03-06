---
title: RHEL8 and CentOS installation
--- 

We provide pre-built RPM packages for RHEL8 (and CentOS 8).

# Packages

We provide a repo with stable builds at https://rpm.leaningtech.com/stable and a repo with nightly builds at https://rpm.leaningtech.com/nightly.

To install Cheerp just add the repo configuration file (Stable in this case):

```
cat << EOF > /etc/yum.repos.d/cheerp.repo
[Cheerp-Stable]
name=Cheerp Stable
gpgcheck=1
gpgkey=https://rpm.leaningtech.com/RPM-GPG-KEY-leaningtech
enabled=1
baseurl=https://rpm.leaningtech.com/stable
EOF
```

And install the `cheerp-core` package:

```
yum install cheerp-core
```
Note that the nightly and stable repos are **NOT** compatible, so be sure to enable just one of them. Also, going from nightly to stable is always considered a downgrade by the package manager.

# Getting Started

If you are new to Cheerp, go [here](./Getting-Started) for more information on how to use it.

# Support

For support on Cheerp RPM packages or Cheerp in general, please refer to our two main channels:
1. [Cheerp Discord channel](https://discord.leaningtech.com)
2. [Cheerp GitHub Issue Tracking](https://github.com/leaningtech/cheerp-meta/issues)

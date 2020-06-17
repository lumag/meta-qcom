# Copyright (C) 2014-2019 Linaro
# Released under the MIT license (see COPYING.MIT for the terms)

DESCRIPTION = "Linaro Qualcomm Landing team 5.7 Kernel"
LIC_FILES_CHKSUM = "file://COPYING;md5=6bc538ed5bd9a7fc9398086aedcd7e46"

inherit python3native

require recipes-kernel/linux/linux-linaro-qcom.inc
require recipes-kernel/linux/linux-qcom-bootimg.inc

LOCALVERSION ?= "-linaro-lt-qcom"
SRCBRANCH ?= "release/qcomlt-5.7"
SRCREV ?= "a7ce3355155d92aeee3d17d11bceb6d8547045d8"

COMPATIBLE_MACHINE = "(apq8016|apq8096|sdm845)"

# Wifi firmware has a recognizable arch :( 
ERROR_QA_remove = "arch"

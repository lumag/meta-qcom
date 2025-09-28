FILESEXTRAPATHS:prepend := "${THISDIR}/${BPN}:"

SRC_URI += " \
    file://0001-freedreno-implement-get_compute_state_info-for-Adren.patch \
    file://0002-ir3-Implement-load_-store_global_ir3-for-A4xx-A5xx.patch \
    file://0003-freedreno-a5xx-skip-NULL-resources-in-fd5_launch_gri.patch \
"

# Enable freedreno driver
PACKAGECONFIG_FREEDRENO = "\
    freedreno \
    tools \
    ${@bb.utils.contains('BBFILE_COLLECTIONS', 'openembedded-layer', 'freedreno-fdperf', '', d)} \
"

PACKAGECONFIG:append:qcom = "${PACKAGECONFIG_FREEDRENO}"

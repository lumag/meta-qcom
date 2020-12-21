DEPENDS += "skales-native"

QCOM_BOOTIMG_ROOTFS ?= "undefined"
SD_QCOM_BOOTIMG_ROOTFS ?= "undefined"

# Define INITRAMS_IMAGE and set this to 1 to allow creating kernel+initramfs
# Android boot images
QCOM_BOOTIMG_BUNDLE_INITRAMFS ?= ""

python __anonymous () {
    image = d.getVar('QCOM_BOOTIMG_BUNDLE_INITRAMFS')
    if image:
        d.appendVarFlag('do_deploy', 'depends', ' ${INITRAMFS_IMAGE}:do_image_complete')
}

# set output file names
BOOT_IMAGE_BASE_NAME = "boot-${KERNEL_IMAGE_NAME}"
BOOT_IMAGE_SYMLINK_NAME = "boot-${KERNEL_IMAGE_LINK_NAME}"
SD_BOOT_IMAGE_BASE_NAME = "boot-sd${KERNEL_IMAGE_NAME}" 
SD_BOOT_IMAGE_SYMLINK_NAME = "boot-sd-${KERNEL_IMAGE_LINK_NAME}"
KERNEL_CMDLINE = "root=${1} rw rootwait console=${ttydev},${baudrate}n8"
KERNEL_CMDLINE_append_dragonboard-845c = " clk_ignore_unused pd_ignore_unused"
KERNEL_CMDLINE_append_qrb5165-rb5 = " pcie_pme=nomsi"

# param ${1} partition where rootfs is located
# param ${2} output boot image file name
# param ${3} initramfs image
priv_make_image() {
    ${STAGING_BINDIR_NATIVE}/skales/mkbootimg --kernel ${B}/arch/${ARCH}/boot/${KERNEL_IMAGETYPE} \
              --ramdisk ${3} \
              --output ${DEPLOYDIR}/${2}.img \
              --pagesize ${QCOM_BOOTIMG_PAGE_SIZE} \
              --base ${QCOM_BOOTIMG_KERNEL_BASE} \
              --cmdline "${KERNEL_CMDLINE}"
}

do_deploy_append() {

    tmp="${SERIAL_CONSOLES}"
    baudrate=`echo $tmp | sed 's/\;.*//'`
    ttydev=`echo $tmp | sed -e 's/^[0-9]*\;//' -e 's/\s.*//' -e 's/\;.*//'`

    # mkbootimg requires an initrd file, make fake one that will be ignored
    # during boot
    echo "This is not an initrd" > ${B}/initrd.img

    # don't build bootimg if rootfs partition is not defined
    if [ "${QCOM_BOOTIMG_ROOTFS}" = "undefined" ]; then
        bbfatal "Rootfs partition must be defined"
    fi

    priv_make_image ${QCOM_BOOTIMG_ROOTFS} ${BOOT_IMAGE_BASE_NAME} ${B}/initrd.img
    ln -sf ${BOOT_IMAGE_BASE_NAME}.img ${DEPLOYDIR}/${BOOT_IMAGE_SYMLINK_NAME}.img

    if [ -n "${QCOM_BOOTIMG_BUNDLE_INITRAMFS}" ] ; then
	for img in cpio cpio.gz cpio.lz4 cpio.lzo cpio.lzma cpio.xz; do
	    if [ -e "${DEPLOY_DIR_IMAGE}/${INITRAMFS_IMAGE_NAME}.$img" ]; then
		priv_make_image ${QCOM_BOOTIMG_ROOTFS} boot-initramfs-${KERNEL_IMAGE_NAME} ${DEPLOY_DIR_IMAGE}/${INITRAMFS_IMAGE_NAME}.$img
		ln -sf boot-initramfs-${KERNEL_IMAGE_NAME}.img ${DEPLOYDIR}/boot-initramfs-${KERNEL_IMAGE_LINK_NAME}.img
		break
	    fi
	done
	[ -f ${DEPLOYDIR}/boot-initramfs-${KERNEL_IMAGE_NAME}.img ] || die "Could not find initramfs image ${INITRAMFS_IMAGE} for bundling"
    fi

    # build sd boot image only for machines supporting it.
    if [ "${SD_QCOM_BOOTIMG_ROOTFS}" != "undefined" ]; then
        priv_make_image ${SD_QCOM_BOOTIMG_ROOTFS} ${SD_BOOT_IMAGE_BASE_NAME}
        ln -sf ${SD_BOOT_IMAGE_BASE_NAME}.img ${DEPLOYDIR}/${SD_BOOT_IMAGE_SYMLINK_NAME}.img
    fi
}

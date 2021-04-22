SUMMARY = "DIAG implements routing of diagnostics related messages between host and various subsystems."
HOMEPAGE = "https://github.com/andersson/diag"
LICENSE = "BSD-3-Clause"
LIC_FILES_CHKSUM = "file://LICENSE;md5=f6832ae4af693c6f31ffd931e25ef580"

SRC_URI = "git://github.com/andersson/diag.git;protocol=https \
           file://diag.service \
           file://diag-gadget-setup \
           file://diag-gadget-setup.machine \
           file://diag-gadget-start \
           file://diag-gadget-cleanup \
           "

PV = "0.0+git${SRCPV}"
SRCREV = "d06e599d197790c9e84ac41a51bf124a69768c4f"

S = "${WORKDIR}/git"

DEPENDS = "qrtr udev"

do_compile () {
	oe_runmake
}

do_install () {
	oe_runmake install 'DESTDIR=${D}'
	install -d ${D}${systemd_system_unitdir}
	install -m 0644 ${WORKDIR}/diag.service ${D}${systemd_system_unitdir}
	for file in diag-gadget-setup diag-gadget-setup.machine diag-gadget-start diag-gadget-cleanup ; do
		install -m 0755 ${WORKDIR}/$file ${D}${bindir}
	done
}

inherit systemd
PACKAGE_BEFORE_PN += "${PN}-systemd"
SYSTEMD_PACKAGES = "${PN}-systemd"
FILES_${PN}-systemd = "\
    ${systemd_system_unitdir} \
    ${bindir}/diag-gadget-* \
"

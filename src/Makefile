# Default Target
.PHONY: all
all: debug release

.PHONY: debug
debug: aries-debug njr-debug vedo-debug

.PHONY: release
release: aries njr vedo

.PHONY: aries-debug
aries-debug:
	(cd aries; ${MAKE} debug)

# Aries
.PHONY: aries
aries:
	(cd aries; ${MAKE} release)

.PHONY: njr-debug
njr-debug:
	(cd njr; ${MAKE} debug)

# NJR
.PHONY: njr
njr:
	(cd njr; ${MAKE} release)

.PHONY: vedo-debug
vedo-debug:
	(cd vedo; ${MAKE} debug)

# VEDO
.PHONY: vedo
vedo:
	(cd vedo; ${MAKE} release)

# Clean
.PHONY: clean
clean:
	(cd aries; ${MAKE} clean)
	(cd njr; ${MAKE} clean)
	(cd vedo; ${MAKE} clean)

PYTESTS = reverse_proxy_unittest.py
SHTESTS = reverse_proxy_tests.sh
noinst_SCRIPTS = $(SHTESTS)

EXTRA_DIST = $(PYTESTS) extension_test_module.py
EXTRA_DIST += api/hook.json
EXTRA_DIST += api/no-access.json
EXTRA_DIST += api/test.json
EXTRA_DIST += ca/0c7eedb9.0
EXTRA_DIST += ca/3071e5ff.0
EXTRA_DIST += ca/ad950210.0
EXTRA_DIST += ca/d3d11a5f.0
EXTRA_DIST += ca/doc
EXTRA_DIST += ca/ext-addr-conf.cnf
EXTRA_DIST += ca/ext-conf.cnf
EXTRA_DIST += ca/kea-ca.crt
EXTRA_DIST += ca/kea-ca.key
EXTRA_DIST += ca/kea-client.crt
EXTRA_DIST += ca/kea-client.csr
EXTRA_DIST += ca/kea-client.key
EXTRA_DIST += ca/kea-client.p12
EXTRA_DIST += ca/kea-server-addr.crt
EXTRA_DIST += ca/kea-server-addr.csr
EXTRA_DIST += ca/kea-server.crt
EXTRA_DIST += ca/kea-server.csr
EXTRA_DIST += ca/kea-server.key
EXTRA_DIST += ca/server-addr-conf.cnf
EXTRA_DIST += ca/server-conf.cnf
EXTRA_DIST += dir/dir.json

DISTCLEANFILES = reverse_proxy_tests.sh

# test using command-line arguments, so use check-local target instead of TESTS
check-local:  pythontests shtests

pythontests:
	@for pytest in $(PYTESTS) ; do \
	echo Running python test: $$pytest ; \
	PYTHONPATH=$(PYTHONPATH):$(abs_top_srcdir)/src/bin/reverse_proxy @PYTHON@ $(abs_srcdir)/$$pytest || exit ; \
	done

shtests:
	@for shtest in $(SHTESTS) ; do \
	echo Running shell test: $$shtest ; \
	export KEA_LOCKFILE_DIR=$(abs_top_builddir); \
	export KEA_PIDFILE_DIR=$(abs_top_builddir); \
	export PYTHONPATH=$(PYTHONPATH):$(abs_top_srcdir)/src/bin/secure_shell:$(abs_top_srcdir)/src/bin/reverse_proxy; \
	export TESTSRCDIR=@abs_top_srcdir@/src/bin/reverse_proxy/tests; \
	${SHELL} $(abs_builddir)/$$shtest || exit ; \
	done

CLEANDIRS = __pycache__

clean-local:
	rm -rf $(CLEANDIRS)

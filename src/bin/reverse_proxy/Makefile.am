SUBDIRS = . tests

pkgpython_PYTHON = kea_config.py kea_role.py kea_error_messages.py
pkgpython_PYTHON += kea_command_accept.py kea_response_filter.py

sbin_SCRIPTS = kea-reverse-proxy

CLEANFILES = *.pyc

CLEANDIRS = __pycache__

clean-local:
	rm -rf html
	rm -rf $(CLEANDIRS)

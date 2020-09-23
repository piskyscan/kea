# Copyright (C) 2020 Internet Systems Consortium, Inc. ("ISC")
#
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

"""
Kea Reverse Proxy extension test module.
"""

import ssl
import kea_config

class KeaReverseProxyExtension:
    """
    This class provides the extension.
    """
    saw_pre_parse = False
    saw_post_parse = False

def pre_parse(config, rp_cfg):
    """
    Pre parser hook.
    """
    if not isinstance(config, kea_config.KeaReverseProxyConfig):
        raise TypeError('not a config instance')
    if not isinstance(rp_cfg, dict):
        raise TypeError('bad syntax')
    if rp_cfg.get('extension-module-name') != 'extension_test_module':
        ValueError("can't find myself")
    KeaReverseProxyExtension.saw_pre_parse = True

def post_parse(config, rp_cfg):
    """
    Post parser hook.
    """
    if not isinstance(config, kea_config.KeaReverseProxyConfig):
        raise TypeError('not a config instance')
    if not isinstance(rp_cfg, dict):
        raise TypeError('bad syntax')
    if rp_cfg.get('extension-module-name') != 'extension_test_module':
        ValueError("can't find myself")
    KeaReverseProxyExtension.saw_post_parse = True

def tls_setup(config, ctx):
    """
    TLS setup hook.
    """
    if not isinstance(config, kea_config.KeaReverseProxyConfig):
        raise TypeError('not a config instance')
    if not isinstance(ctx, ssl.SSLContext):
        raise TypeError('not a SSLContext')
    ctx.options |= ssl.OP_SINGLE_DH_USE
    ctx.options |= ssl.OP_SINGLE_ECDH_USE

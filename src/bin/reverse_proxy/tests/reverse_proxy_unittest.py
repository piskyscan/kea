#!/usr/bin/env python3

# Copyright (C) 2020 Internet Systems Consortium, Inc. ("ISC")
#
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

"""
Kea reverse proxy unittest (python part).
"""

import os
import sys
import unittest
import tempfile
import json
from pathlib import Path
import ssl

# Specific imports

import kea_config
import kea_role
import kea_command_accept
import kea_response_filter

class ConfigTestCase(unittest.TestCase):
    """
    Test kea_config.
    """

    def test_comments(self):
        """
        This test verifies that # and // comments are removed.
        """
        with tempfile.TemporaryDirectory() as tempdir:
            filename = tempdir + 'test.conf'
            with open(filename, 'w') as file:
                file.write('# a comment\n')
                file.write('{ "ReverseProxy": { # another\n')
                file.write('// other style\n')
                file.write('"reverse-proxy-port": 44443, //////\n')
                file.write('"api-files": "test",//\n')
                file.write('"assign-role-by": "test",#\n')
                file.write('"assign-role-parameter": "test" ###\n')
                file.write('}}')
            config = kea_config.KeaReverseProxyConfig()
            config.load(filename)
            self.assertEqual(config.rp_port, 44443)

    def test_invalid_json(self):
        """
        This test verifies that invalid json is refused.
        """
        with tempfile.TemporaryDirectory() as tempdir:
            filename = tempdir + 'test.conf'
            with open(filename, 'w') as file:
                file.write('{ "ReverseProxy": { }')
            config = kea_config.KeaReverseProxyConfig()
            self.assertRaises(json.JSONDecodeError, config.load, filename)

    def test_config_map(self):
        """
        This test verifies that the configuration must be a map.
        """
        cfg = json.loads('"ReverseProxy"')
        config = kea_config.KeaReverseProxyConfig()
        msg = r'^configuration must be a map$'
        self.assertRaisesRegex(TypeError, msg, config.parse, cfg)

def config_suite():
    """
    Test suite for kea_config.
    """
    suite = unittest.TestSuite()
    suite.addTest(ConfigTestCase('test_comments'))
    suite.addTest(ConfigTestCase('test_invalid_json'))
    suite.addTest(ConfigTestCase('test_config_map'))
    return suite

class ConfigGlobalTestCase(unittest.TestCase):
    """
    Test kea_config (global parameters).
    """

    def test_reverse_proxy_entry(self):
        """
        This test verifies that the top entry must be ReverseProxy.
        """
        cfg = json.loads('{ "Foo": { } }')
        config = kea_config.KeaReverseProxyConfig()
        msg = r"^can't find 'ReverseProxy' toplevel entry$"
        self.assertRaisesRegex(ValueError, msg, config.parse, cfg)

    def test_reverse_proxy_map(self):
        """
        This test verifies that the ReverseProxy entry must be a map.
        """
        cfg = json.loads('{ "ReverseProxy": [ ] }')
        config = kea_config.KeaReverseProxyConfig()
        msg = r"^'ReverseProxy' entry must be a map$"
        self.assertRaisesRegex(TypeError, msg, config.parse, cfg)

    def test_host_string(self):
        """
        This test verifies that the host parameter must be a string.
        """
        cfg = json.loads('{ "ReverseProxy": { "host": 2130706433 } }')
        config = kea_config.KeaReverseProxyConfig()
        msg = r"^'host' parameter must be a string$"
        self.assertRaisesRegex(TypeError, msg, config.parse, cfg)

    def test_host_default(self):
        """
        This test verifies the host parameter default (127.0.0.1).
        """
        cfg = json.loads('{ "ReverseProxy": { ' +
                         '"api-files": "test", ' +
                         '"assign-role-by": "test", ' +
                         '"assign-role-parameter": "test" } }')
        config = kea_config.KeaReverseProxyConfig()
        config.parse(cfg)
        self.assertEqual(config.host, '127.0.0.1')

    def test_host(self):
        """
        This test verifies that the host parameter is parsed.
        """
        cfg = json.loads('{ "ReverseProxy": { ' +
                         '"host": "192.0.2.1", ' +
                         '"api-files": "test", ' +
                         '"assign-role-by": "test", ' +
                         '"assign-role-parameter": "test" } }')
        config = kea_config.KeaReverseProxyConfig()
        config.parse(cfg)
        self.assertEqual(config.host, '192.0.2.1')

    def test_reverse_proxy_port_int(self):
        """
        This test verifies that the reverse-proxy-port parameter must be an int.
        """
        cfg = json.loads('{ "ReverseProxy": { "reverse-proxy-port": "443" } }')
        config = kea_config.KeaReverseProxyConfig()
        msg = r"^'reverse-proxy-port' parameter must be an integer$"
        self.assertRaisesRegex(TypeError, msg, config.parse, cfg)

    def test_reverse_proxy_port_default(self):
        """
        This test verifies the reverse-proxy-port parameter default (443).
        """
        cfg = json.loads('{ "ReverseProxy": { ' +
                         '"api-files": "test", '
                         '"assign-role-by": "test", ' +
                         '"assign-role-by": "test", ' +
                         '"assign-role-parameter": "test" } }')
        config = kea_config.KeaReverseProxyConfig()
        config.parse(cfg)
        self.assertEqual(config.rp_port, 443)

    def test_reverse_proxy_port(self):
        """
        This test verifies that the reverse-proxy-port parameter is parsed.
        """
        cfg = json.loads('{ "ReverseProxy": { ' +
                         '"reverse-proxy-port": 44443, ' +
                         '"api-files": "test", ' +
                         '"assign-role-by": "test", ' +
                         '"assign-role-parameter": "test" } }')
        config = kea_config.KeaReverseProxyConfig()
        config.parse(cfg)
        self.assertEqual(config.rp_port, 44443)

    def test_control_agent_port_int(self):
        """
        This test verifies that the control-agent-port parameter must be an int.
        """
        cfg = json.loads('{ "ReverseProxy": { "control-agent-port": "8000" } }')
        config = kea_config.KeaReverseProxyConfig()
        msg = r"^'control-agent-port' parameter must be an integer$"
        self.assertRaisesRegex(TypeError, msg, config.parse, cfg)

    def test_control_agent_port_default(self):
        """
        This test verifies the control-agent-port parameter default (8000).
        """
        cfg = json.loads('{ "ReverseProxy": { ' +
                         '"api-files": "test", ' +
                         '"assign-role-by": "test", ' +
                         '"assign-role-parameter": "test" } }')
        config = kea_config.KeaReverseProxyConfig()
        config.parse(cfg)
        self.assertEqual(config.ca_port, 8000)

    def test_control_agent_port(self):
        """
        This test verifies that the control-agent-port parameter is parsed.
        """
        cfg = json.loads('{ "ReverseProxy": { ' +
                         '"control-agent-port": 8888, ' +
                         '"api-files": "test", ' +
                         '"assign-role-by": "test", ' +
                         '"assign-role-parameter": "test" } }')
        config = kea_config.KeaReverseProxyConfig()
        config.parse(cfg)
        self.assertEqual(config.ca_port, 8888)

    def test_connect_timeout_number(self):
        """
        This test verifies that the connect-timeout parameter must be a number.
        """
        cfg = json.loads('{ "ReverseProxy": { "connect-timeout": "on" } }')
        config = kea_config.KeaReverseProxyConfig()
        msg = r"^'connect-timeout' parameter must be a number$"
        self.assertRaisesRegex(TypeError, msg, config.parse, cfg)

    def test_connect_timeout(self):
        """
        This test verifies that the connect-timeout parameter is parsed.
        """
        cfg = json.loads('{ "ReverseProxy": { ' +
                         '"connect-timeout": 4, ' +
                         '"api-files": "test", ' +
                         '"assign-role-by": "test", ' +
                         '"assign-role-parameter": "test" } }')
        config = kea_config.KeaReverseProxyConfig()
        config.parse(cfg)
        self.assertEqual(config.timeouts, (4, 10))

    def test_read_timeout_number(self):
        """
        This test verifies that the read-timeout parameter must be a number.
        """
        cfg = json.loads('{ "ReverseProxy": { "read-timeout": "on" } }')
        config = kea_config.KeaReverseProxyConfig()
        msg = r"^'read-timeout' parameter must be a number$"
        self.assertRaisesRegex(TypeError, msg, config.parse, cfg)

    def test_read_timeout(self):
        """
        This test verifies that the read-timeout parameter is parsed.
        """
        cfg = json.loads('{ "ReverseProxy": { ' +
                         '"read-timeout": 64.0, ' +
                         '"api-files": "test", ' +
                         '"assign-role-by": "test", ' +
                         '"assign-role-parameter": "test" } }')
        config = kea_config.KeaReverseProxyConfig()
        config.parse(cfg)
        self.assertEqual(config.timeouts, (5, 64.))

    def test_api_files_mandatory(self):
        """
        This test verifies that the api-files parameter is required.
        """
        cfg = json.loads('{ "ReverseProxy": { } }')
        config = kea_config.KeaReverseProxyConfig()
        msg = r"'api-files' parameter is mandatory$"
        self.assertRaisesRegex(ValueError, msg, config.parse, cfg)

    def test_api_files_string(self):
        """
        This test verifies that the api-files parameter must be a string.
        """
        cfg = json.loads('{ "ReverseProxy": { "api-files": 123 } }')
        config = kea_config.KeaReverseProxyConfig()
        msg = r"^'api-files' parameter must be a string$"
        self.assertRaisesRegex(TypeError, msg, config.parse, cfg)

    def test_assign_role_by_mandatory(self):
        """
        This test verifies that the assign-role-by parameter is required.
        """
        cfg = json.loads('{ "ReverseProxy": { "api-files": "test" } }')
        config = kea_config.KeaReverseProxyConfig()
        msg = r"^'assign-role-by' parameter is mandatory$"
        self.assertRaisesRegex(ValueError, msg, config.parse, cfg)

    def test_assign_role_by_string(self):
        """
        This test verifies that the assign-role-by parameter must be a string.
        """
        cfg = json.loads('{ "ReverseProxy": { ' +
                         '"api-files": "test", ' +
                         '"assign-role-by": 234 } }')
        config = kea_config.KeaReverseProxyConfig()
        msg = r"^'assign-role-by' parameter must be a string$"
        self.assertRaisesRegex(TypeError, msg, config.parse, cfg)

    def test_assign_role_by_unsupported(self):
        """
        This test verifies that the assign-role-by parameter has a limited
        set of possible values.
        """
        cfg = json.loads('{ "ReverseProxy": { ' +
                         '"api-files": "test", ' +
                         '"assign-role-by": "foobar" } }')
        config = kea_config.KeaReverseProxyConfig()
        msg = r"^unsupported value 'foobar' for 'assign-role-by' parameter$"
        self.assertRaisesRegex(ValueError, msg, config.parse, cfg)

    def test_assign_role_by_issuer(self):
        """
        This test verifies that the assign-role-by issue is parsed.
        """
        cfg = json.loads('{ "ReverseProxy": { ' +
                         '"api-files": "test", ' +
                         '"assign-role-by": "issuer" } }')
        config = kea_config.KeaReverseProxyConfig()
        config.parse(cfg)
        self.assertIsNotNone(config.assign)
        self.assertEqual(config.assign.name(), 'issuer')

    def test_assign_role_by_subject(self):
        """
        This test verifies that the assign-role-by issue is parsed.
        """
        cfg = json.loads('{ "ReverseProxy": { ' +
                         '"api-files": "test", ' +
                         '"assign-role-by": "subject" } }')
        config = kea_config.KeaReverseProxyConfig()
        config.parse(cfg)
        self.assertIsNotNone(config.assign)
        self.assertEqual(config.assign.name(), 'subject')

    def test_assign_static_role(self):
        """
        This test verifies that the assign-role-by static is parsed.
        """
        cfg = json.loads('{ "ReverseProxy": { ' +
                         '"api-files": "test", ' +
                         '"assign-role-by": "static", ' +
                         '"assign-role-parameter": "foobar" } }')
        config = kea_config.KeaReverseProxyConfig()
        config.parse(cfg)
        self.assertIsNotNone(config.assign)
        self.assertEqual(config.assign.name(), 'static')
        self.assertEqual(config.assign.role, 'foobar')

    def test_default_role_string(self):
        """
        This test verifies that the default-role parameter must be a string.
        """
        cfg = json.loads('{ "ReverseProxy": { ' +
                         '"default-role": 2130706433, ' +
                         '"api-files": "test", ' +
                         '"assign-role-by": "test" } }')
        config = kea_config.KeaReverseProxyConfig()
        msg = r"^'default-role' parameter must be a string$"
        self.assertRaisesRegex(TypeError, msg, config.parse, cfg)

    def test_default_role_default(self):
        """
        This test verifies the default-role parameter default ('').
        """
        cfg = json.loads('{ "ReverseProxy": { ' +
                         '"api-files": "test", ' +
                         '"assign-role-by": "test", ' +
                         '"assign-role-parameter": "test" } }')
        config = kea_config.KeaReverseProxyConfig()
        config.parse(cfg)
        self.assertEqual(config.default, '')

    def test_default_role(self):
        """
        This test verifies that the default-role parameter is parsed.
        """
        cfg = json.loads('{ "ReverseProxy": { ' +
                         '"default-role": "read-only", ' +
                         '"api-files": "test", ' +
                         '"assign-role-by": "static"  } }')
        config = kea_config.KeaReverseProxyConfig()
        config.parse(cfg)
        self.assertEqual(config.default, 'read-only')
        self.assertEqual(config.assign.role, 'read-only')

def config_global_suite():
    """
    Test suite for kea_config (global parameters).
    """
    suite = unittest.TestSuite()
    suite.addTest(ConfigGlobalTestCase('test_reverse_proxy_entry'))
    suite.addTest(ConfigGlobalTestCase('test_reverse_proxy_map'))
    suite.addTest(ConfigGlobalTestCase('test_host_string'))
    suite.addTest(ConfigGlobalTestCase('test_host_default'))
    suite.addTest(ConfigGlobalTestCase('test_host'))
    suite.addTest(ConfigGlobalTestCase('test_reverse_proxy_port_int'))
    suite.addTest(ConfigGlobalTestCase('test_reverse_proxy_port_default'))
    suite.addTest(ConfigGlobalTestCase('test_reverse_proxy_port'))
    suite.addTest(ConfigGlobalTestCase('test_control_agent_port_int'))
    suite.addTest(ConfigGlobalTestCase('test_control_agent_port_default'))
    suite.addTest(ConfigGlobalTestCase('test_control_agent_port'))
    suite.addTest(ConfigGlobalTestCase('test_connect_timeout_number'))
    suite.addTest(ConfigGlobalTestCase('test_connect_timeout'))
    suite.addTest(ConfigGlobalTestCase('test_read_timeout_number'))
    suite.addTest(ConfigGlobalTestCase('test_read_timeout'))
    suite.addTest(ConfigGlobalTestCase('test_api_files_mandatory'))
    suite.addTest(ConfigGlobalTestCase('test_api_files_string'))
    suite.addTest(ConfigGlobalTestCase('test_assign_role_by_mandatory'))
    suite.addTest(ConfigGlobalTestCase('test_assign_role_by_string'))
    suite.addTest(ConfigGlobalTestCase('test_assign_role_by_unsupported'))
    suite.addTest(ConfigGlobalTestCase('test_assign_role_by_issuer'))
    suite.addTest(ConfigGlobalTestCase('test_assign_role_by_subject'))
    suite.addTest(ConfigGlobalTestCase('test_assign_static_role'))
    suite.addTest(ConfigGlobalTestCase('test_default_role_string'))
    suite.addTest(ConfigGlobalTestCase('test_default_role_default'))
    suite.addTest(ConfigGlobalTestCase('test_default_role'))
    return suite

class ApiFileTestCase(unittest.TestCase):
    """
    Test kea_config.load_api_file[s].
    """

    def test_load_api_files(self):
        """
        This test verifies that api files load as expected.
        """
        config = kea_config.KeaReverseProxyConfig()
        msg = r"^'does/not/exit' does not exist$"
        self.assertRaisesRegex(ValueError, msg, config.load_api_files,
                               'does/not/exit')
        msg = r"^'Makefile.am' must be a directory$"
        self.assertRaisesRegex(ValueError, msg, config.load_api_files,
                               'Makefile.am')
        msg = r"^'dir/dir.json' is not a file$"
        self.assertRaisesRegex(ValueError, msg, config.load_api_files,
                               'dir')

    def test_load_api_file(self):
        """
        This test verifies that an api file loads as expected.
        """
        config = kea_config.KeaReverseProxyConfig()
        self.assertRaises(json.JSONDecodeError, config.load_api_file,
                          'test', Path('Makefile.am'))
        msg = r"^api file 'api/test.json' does not describe command 'test'$"
        self.assertRaisesRegex(ValueError, msg, config.load_api_file,
                               'test', Path('api/test.json'))
        msg = r"^command 'list-commands' description has no access$"
        self.assertRaisesRegex(ValueError, msg, config.load_api_file,
                               'list-commands', Path('api/no-access.json'))
        config.load_api_file('list-commands', Path('api/test.json'))
        config.load_api_file('ha-sync', Path('api/hook.json'))
        self.assertEqual(config.accesses, {'read', 'write'})
        self.assertEqual(config.hooks, {'high_availability'})
        self.assertEqual(len(config.apis), 2)
        test = config.apis.get('list-commands')
        self.assertIsInstance(test, dict)
        self.assertEqual(test.get('access'), 'read')
        self.assertIsNone(test.get('hook'))
        ha_sync = config.apis.get('ha-sync')
        self.assertIsInstance(ha_sync, dict)
        self.assertEqual(ha_sync.get('access'), 'write')
        self.assertEqual(ha_sync.get('hook'), 'high_availability')

def api_file_suite():
    """
    Test suite for api file loading.
    """
    suite = unittest.TestSuite()
    suite.addTest(ApiFileTestCase('test_load_api_files'))
    suite.addTest(ApiFileTestCase('test_load_api_file'))
    return suite

class RoleTestCase(unittest.TestCase):
    """
    Test the role classes.
    """

    def test_issuer(self):
        """
        This test verifies that role assignment by issuer works as expected.
        """
        role_by_issuer = kea_role.RoleByIssuerCN()
        self.assertFalse(role_by_issuer.use_request_handler)
        self.assertEqual(role_by_issuer.name(), 'issuer')
        role_by_issuer.setup({'foo': 'bar'})
        cert0 = {}
        self.assertEqual(role_by_issuer.assign(cert0), None)
        issuer1 = ((('countryName', 'US'),),
                   (('organizationName', 'ISC Inc.'),))
        cert1 = {'issuer' : issuer1}
        self.assertEqual(role_by_issuer.assign(cert1), None)
        issuer2 = ((('countryName', 'US'),),
                   (('organizationName', 'ISC Inc.'),),
                   (('commonName', 'test'),))
        cert2 = {'issuer' : issuer2}
        self.assertEqual(role_by_issuer.assign(cert2), None)
        issuer3 = ((('countryName', 'US'),),
                   (('organizationName', 'ISC Inc.'),),
                   (('commonName', 'foo'),))
        cert3 = {'issuer' : issuer3}
        self.assertEqual(role_by_issuer.assign(cert3), 'bar')

    def test_subject(self):
        """
        This test verifies that role assignment by subject works as expected.
        """
        role_by_subject = kea_role.RoleBySubjectCN()
        self.assertFalse(role_by_subject.use_request_handler)
        self.assertEqual(role_by_subject.name(), 'subject')
        role_by_subject.setup({'foo': 'bar'})
        cert0 = {}
        self.assertEqual(role_by_subject.assign(cert0), None)
        subject1 = ((('countryName', 'US'),),
                    (('organizationName', 'ISC Inc.'),))
        cert1 = {'subject' : subject1}
        self.assertEqual(role_by_subject.assign(cert1), None)
        subject2 = ((('countryName', 'US'),),
                    (('organizationName', 'ISC Inc.'),),
                    (('commonName', 'test'),))
        cert2 = {'subject' : subject2}
        self.assertEqual(role_by_subject.assign(cert2), None)
        subject3 = ((('countryName', 'US'),),
                    (('organizationName', 'ISC Inc.'),),
                    (('commonName', 'foo'),))
        cert3 = {'subject' : subject3}
        self.assertEqual(role_by_subject.assign(cert3), 'bar')

    def test_static(self):
        """
        This test verifies that static role assignment works as expected.
        """
        role_static = kea_role.RoleStatic()
        self.assertFalse(role_static.use_request_handler)
        self.assertEqual(role_static.name(), 'static')
        role_static.setup('foo')
        cert = {}
        self.assertEqual(role_static.assign(cert), 'foo')

def role_suite():
    """
    Test suite for kea_role.
    """
    suite = unittest.TestSuite()
    suite.addTest(RoleTestCase('test_issuer'))
    suite.addTest(RoleTestCase('test_subject'))
    suite.addTest(RoleTestCase('test_static'))
    return suite

class CommandAcceptTestCase(unittest.TestCase):
    """
    Test the accept command classes.
    """

    def test_accept(self):
        """
        This test verifies that command accept/drop works as expected.
        """
        accepts = {'foo'}
        rejects = {'bar'}
        accept = kea_command_accept.KeaReverseProxyCommandAccept(accepts,
                                                                 rejects,
                                                                 False)
        self.assertTrue(accept.accept('foo'))
        self.assertFalse(accept.accept('bar'))
        self.assertFalse(accept.accept('xyz'))
        accept = kea_command_accept.KeaReverseProxyCommandAccept(accepts,
                                                                 rejects,
                                                                 True)
        self.assertTrue(accept.accept('xyz'))
        # accepts has the preference
        rejects.add('foo')
        self.assertTrue(accept.accept('foo'))

    def test_all(self):
        """
        This test verifies that the all filter works as expected.
        """
        cfilt = kea_command_accept.CommandFilterAll()
        self.assertEqual(cfilt.name(), 'all')
        self.assertTrue(cfilt.filter('foo'))
        self.assertTrue(cfilt.filter('bar'))

    def test_access(self):
        """
        This test verifies that the access filter works as expected.
        No access returns False.
        """
        cfilt = kea_command_accept.CommandFilterAccess('foo')
        self.assertEqual(cfilt.name(), 'access')
        self.assertFalse(cfilt.filter({}))
        self.assertFalse(cfilt.filter({'access': 'bar'}))
        self.assertTrue(cfilt.filter({'access': 'foo'}))

    def test_commands(self):
        """
        This test verifies that the commands filter works as expected.
        """
        cfilt = kea_command_accept.CommandFilterCommands(['foo'])
        self.assertEqual(cfilt.name(), 'commands')
        self.assertTrue(cfilt.filter({'name': 'foo'}))
        self.assertFalse(cfilt.filter({'name': 'bar'}))

    def test_hooks(self):
        """
        This test verifies that the hooks filter works as expected.
        """
        cfilt = kea_command_accept.CommandFilterHooks(['foo'])
        self.assertEqual(cfilt.name(), 'hooks')
        self.assertFalse(cfilt.filter({}))
        self.assertTrue(cfilt.filter({'hook': 'foo'}))
        self.assertFalse(cfilt.filter({'hook': 'bar'}))

    def test_not(self):
        """
        This test verifies that the not filter works as expected.
        Not all is none, not (not x) is x.
        """
        call = kea_command_accept.CommandFilterAll()
        cfilt = kea_command_accept.CommandFilterNot(call)
        self.assertEqual(cfilt.name(), 'not(all)')
        self.assertFalse(cfilt.filter('foo'))
        self.assertFalse(cfilt.filter('bar'))
        dfilt = kea_command_accept.CommandFilterNot(cfilt)
        self.assertEqual(dfilt.name(), 'not(not(all))')
        self.assertTrue(dfilt.filter('foo'))
        self.assertTrue(dfilt.filter('bar'))

    def test_and(self):
        """
        This test verifies that the and filter works as expected.
        """
        cfoo = kea_command_accept.CommandFilterCommands(['foo'])
        cbar = kea_command_accept.CommandFilterHooks(['bar'])
        cfilt = kea_command_accept.CommandFilterAnd([cfoo, cbar])
        self.assertEqual(cfilt.name(), 'and[commands, hooks]')
        self.assertFalse(cfilt.filter({'name': 'foo'}))
        self.assertFalse(cfilt.filter({'hook': 'bar'}))
        self.assertTrue(cfilt.filter({'name': 'foo', 'hook': 'bar'}))

    def test_or(self):
        """
        This test verifies that the or filter works as expected.
        """
        cfoo = kea_command_accept.CommandFilterCommands(['foo'])
        cbar = kea_command_accept.CommandFilterHooks(['bar'])
        cfilt = kea_command_accept.CommandFilterOr([cfoo, cbar])
        self.assertEqual(cfilt.name(), 'or[commands, hooks]')
        self.assertFalse(cfilt.filter({'name': 'xyz'}))
        self.assertFalse(cfilt.filter({'hook': 'xyz'}))
        self.assertTrue(cfilt.filter({'name': 'foo', 'hook': 'xyz'}))
        self.assertTrue(cfilt.filter({'name': 'xyz', 'hook': 'bar'}))
        self.assertFalse(cfilt.filter({'name': 'bar', 'hook': 'foo'}))

    def test_apply(self):
        """
        This test verifies that application of a filter works as expected.
        """
        apis = {'foo': {'name' : 'foo'}, 'bar': {'name': 'bar'}}
        cfilt = kea_command_accept.CommandFilterCommands(['foo', 'bar'])
        result = set()
        cfilt.apply(apis, result)
        self.assertTrue(result, {'foo', 'bar'})

class SetupRoleTestCase(unittest.TestCase):
    """
    Test kea_config.setup_assign_dict and kea_config.setup_assign_default.
    """

    def test_setup_dict(self):
        """
        This test verifies that setup of assign role object by dictionary
        works as expected without a parameter.
        """
        config = kea_config.KeaReverseProxyConfig()
        config.assign = kea_role.RoleByIssuerCN()
        config.roles['foo'] = 1
        config.roles['bar'] = 2
        config.setup_assign_dict('issuer', None)
        self.assertEqual(config.assign.issuers, {'foo': 'foo', 'bar': 'bar'})

    def test_setup_dict_param(self):
        """
        This test verifies that setup of assign role object by dictionary
        works as expected with a parameter.
        """
        config = kea_config.KeaReverseProxyConfig()
        config.assign = kea_role.RoleBySubjectCN()
        msg = r"^'assign-role-parameter' parameter of 'foo' must be a map$"
        self.assertRaisesRegex(TypeError, msg, config.setup_assign_dict,
                               'foo', ['bar'])
        msg = r"^'assign-role-parameter' value for 'bar' key must be "
        msg += r"a string$"
        self.assertRaisesRegex(TypeError, msg, config.setup_assign_dict,
                               'foo', {'bar': 1})
        param = {'foo': 'a', 'bar': 'b'}
        config.setup_assign_dict('subject', param)
        self.assertEqual(config.assign.subjects, param)

    def test_setup_default(self):
        """
        This test verifies that setup of assign role object by default role
        works as expected without a parameter.
        """
        config = kea_config.KeaReverseProxyConfig()
        config.assign = kea_role.RoleStatic()
        msg = r"'test' requires an 'assign-role-parameter' or "
        msg += r"'default-role' parameter$"
        self.assertRaisesRegex(ValueError, msg, config.setup_assign_default,
                               'test', None)
        config.default = 'foo'
        config.setup_assign_default('static', None)
        self.assertEqual(config.assign.role, 'foo')

    def test_setup_default_param(self):
        """
        This test verifies that setup of assign role object by default role
        works as expected with a parameter.
        """
        config = kea_config.KeaReverseProxyConfig()
        config.assign = kea_role.RoleStatic()
        msg = r"^'assign-role-parameter' parameter of 'foo' must be "
        msg += r"a string$"
        self.assertRaisesRegex(TypeError, msg, config.setup_assign_default,
                               'foo', {})
        config.setup_assign_default('static', 'foo')
        self.assertEqual(config.assign.role, 'foo')

def setup_role_suite():
    """
    Test suite for assign class setup.
    """
    suite = unittest.TestSuite()
    suite.addTest(SetupRoleTestCase('test_setup_dict'))
    suite.addTest(SetupRoleTestCase('test_setup_dict_param'))
    suite.addTest(SetupRoleTestCase('test_setup_default'))
    suite.addTest(SetupRoleTestCase('test_setup_default_param'))
    return suite

def command_accept_suite():
    """
    Test suite for kea_command_accept.
    """
    suite = unittest.TestSuite()
    suite.addTest(CommandAcceptTestCase('test_accept'))
    suite.addTest(CommandAcceptTestCase('test_all'))
    suite.addTest(CommandAcceptTestCase('test_access'))
    suite.addTest(CommandAcceptTestCase('test_commands'))
    suite.addTest(CommandAcceptTestCase('test_hooks'))
    suite.addTest(CommandAcceptTestCase('test_not'))
    suite.addTest(CommandAcceptTestCase('test_and'))
    suite.addTest(CommandAcceptTestCase('test_or'))
    suite.addTest(CommandAcceptTestCase('test_apply'))
    return suite

class ConfigCommandAcceptTestCase(unittest.TestCase):
    """
    Test kea_config.parse_command[s].
    """

    def test_parse_unknown(self):
        """
        This test verifies that unknown filters do not parse.
        """
        config = kea_config.KeaReverseProxyConfig()
        commands = set()
        msg = r"^unknown entry 'foobar' in context$"
        self.assertRaisesRegex(ValueError, msg, config.parse_commands,
                               'foobar', commands, ' in context')
        self.assertRaisesRegex(ValueError, msg, config.parse_commands,
                               {'foobar': 1}, commands, ' in context')
        msg = r'^malformed entry in context: \{"bar": 2, "foo": 1\}$'
        self.assertRaisesRegex(ValueError, msg, config.parse_commands,
                               {'foo': 1, 'bar': 2}, commands, ' in context')
        msg = r'^malformed entry in context: \["foobar"\]$'
        self.assertRaisesRegex(ValueError, msg, config.parse_commands,
                               ['foobar'], commands, ' in context')

    def test_parse_all(self):
        """
        This test verifies that the all filter can be parsed.
        """
        config = kea_config.KeaReverseProxyConfig()
        config.apis['foo'] = 1
        config.apis['bar'] = 2
        commands = set()
        msg = r"^malformed entry 'all' in context$"
        self.assertRaisesRegex(TypeError, msg, config.parse_commands,
                               {'all': True}, commands, ' in context')
        config.parse_commands('all', commands, ' in context')
        self.assertEqual(commands, {'foo', 'bar'})

    def test_parse_access(self):
        """
        This test verifies that the access filter can be parsed.
        """
        config = kea_config.KeaReverseProxyConfig()
        config.apis['foo'] = {'access': 'read'}
        config.apis['bar'] = {'access': 'write'}
        config.accesses = {'read', 'write'}
        commands = set()
        msg = r"^malformed entry 'access' in context$"
        self.assertRaisesRegex(TypeError, msg, config.parse_commands,
                               'access', commands, ' in context')
        msg = r"^unknown argument \(foobar\) of 'access' in context$"
        self.assertRaisesRegex(ValueError, msg, config.parse_commands,
                               {'access': 'foobar'}, commands, ' in context')
        config.parse_commands({'access': 'read'}, commands, ' in context')
        self.assertEqual(commands, {'foo'})
        commands = set()
        config.parse_commands({'access': 'write'}, commands, ' in context')
        self.assertEqual(commands, {'bar'})

    def test_parse_commands(self):
        """
        This test verifies that the commands filter can be parsed.
        """
        config = kea_config.KeaReverseProxyConfig()
        config.apis['foo'] = {'name': 'foo'}
        config.apis['bar'] = {'name': 'bar'}
        commands = set()
        msg = r"^malformed entry 'commands' in context$"
        self.assertRaisesRegex(TypeError, msg, config.parse_commands,
                               'commands', commands, ' in context')
        msg = r"^malformed entry 'commands' in context: "
        msg += r'\{"commands": \{\}\}$'
        self.assertRaisesRegex(TypeError, msg, config.parse_commands,
                               {'commands': {}}, commands, ' in context')
        msg = r"^unknown command \(foobar\) of 'commands' in context$"
        self.assertRaisesRegex(ValueError, msg, config.parse_commands,
                               {'commands': ['foobar']},
                               commands, ' in context')
        config.parse_commands({'commands': ['foo']}, commands, ' in context')
        self.assertEqual(commands, {'foo'})
        commands = set()
        config.parse_commands({'commands': ['foo', 'bar']},
                              commands, ' in context')
        self.assertEqual(commands, {'foo', 'bar'})

    def test_parse_hooks(self):
        """
        This test verifies that the hooks filter can be parsed.
        """
        config = kea_config.KeaReverseProxyConfig()
        config.apis['foo'] = {'hook': 'hook1'}
        config.apis['bar'] = {'hook': 'hook2'}
        config.apis['xyz'] = {'foo': 'bar'}
        config.hooks = {'hook1', 'hook2'}
        commands = set()
        msg = r"^malformed entry 'hooks' in context$"
        self.assertRaisesRegex(TypeError, msg, config.parse_commands,
                               'hooks', commands, ' in context')
        msg = r"^malformed entry 'hooks' in context: "
        msg += r'\{"hooks": \{\}\}$'
        self.assertRaisesRegex(TypeError, msg, config.parse_commands,
                               {'hooks': {}}, commands, ' in context')
        msg = r"^unknown hook \(foobar\) of 'hooks' in context$"
        self.assertRaisesRegex(ValueError, msg, config.parse_commands,
                               {'hooks': ['foobar']},
                               commands, ' in context')
        config.parse_commands({'hooks': ['hook1']}, commands, ' in context')
        self.assertEqual(commands, {'foo'})
        commands = set()
        config.parse_commands({'hooks': ['hook2']}, commands, ' in context')
        self.assertEqual(commands, {'bar'})
        commands = set()
        config.parse_commands({'hooks': ['hook1', 'hook2']},
                              commands, ' in context')
        self.assertEqual(commands, {'foo', 'bar'})

    def test_parse_not(self):
        """
        This test verifies that the not filter can be parsed.
        """
        config = kea_config.KeaReverseProxyConfig()
        config.apis['foo'] = {'name': 'foo'}
        config.apis['bar'] = {'name': 'bar'}
        config.apis['xyz'] = {'name': 'xyz'}
        commands = set()
        msg = r"^malformed entry 'not' in context$"
        self.assertRaisesRegex(TypeError, msg, config.parse_commands,
                               'not', commands, ' in context')
        msg = r"^malformed entry 'not' in context: "
        msg += r'\{"not": \["foo"\]\}$'
        self.assertRaisesRegex(TypeError, msg, config.parse_commands,
                               {'not': ['foo']}, commands, ' in context')
        config.parse_commands({'not': 'all'}, commands, ' in context')
        self.assertEqual(len(commands), 0)
        commands = set()
        config.parse_commands({'commands': ['foo', 'bar']},
                              commands, ' in context')
        self.assertEqual(commands, {'foo', 'bar'})
        commands = set()
        config.parse_commands({'not': {'commands': ['foo', 'bar']}},
                              commands, ' in context')
        self.assertEqual(commands, {'xyz'})

    def test_parse_and(self):
        """
        This test verifies that the and filter can be parsed.
        """
        config = kea_config.KeaReverseProxyConfig()
        config.apis['foo'] = {'name': 'foo', 'hook': 'hook1'}
        config.apis['bar'] = {'name': 'bar', 'hook': 'hook2'}
        config.apis['xyz'] = {'name': 'xyz', 'hook': 'hook1'}
        config.hooks = {'hook1', 'hook2'}
        commands = set()
        msg = r"^malformed entry 'and' in context$"
        self.assertRaisesRegex(TypeError, msg, config.parse_commands,
                               'and', commands, ' in context')
        msg = r"^malformed entry 'and' in context: "
        msg += r'\{"and": \{\}\}$'
        self.assertRaisesRegex(TypeError, msg, config.parse_commands,
                               {'and': {}}, commands, ' in context')
        commands1 = set()
        config.parse_commands({'commands': ['foo', 'bar']},
                              commands1, ' in context')
        self.assertEqual(commands1, {'foo', 'bar'})
        commands2 = set()
        config.parse_commands({'hooks': ['hook1']}, commands2, ' in context')
        self.assertEqual(commands2, {'foo', 'xyz'})
        config.parse_commands({'and': [{'commands': ['foo', 'bar']},
                                       {'hooks': ['hook1']}]},
                              commands, ' in context')
        self.assertEqual(commands, {'foo'})
        self.assertEqual(commands, commands1.intersection(commands2))

    def test_parse_or(self):
        """
        This test verifies that the or filter can be parsed.
        """
        config = kea_config.KeaReverseProxyConfig()
        config.apis['foo'] = {'name': 'foo', 'hook': 'hook1'}
        config.apis['bar'] = {'name': 'bar', 'hook': 'hook2'}
        config.apis['xyz'] = {'name': 'xyz', 'hook': 'hook1'}
        config.hooks = {'hook1', 'hook2'}
        commands = set()
        msg = r"^malformed entry 'or' in context$"
        self.assertRaisesRegex(TypeError, msg, config.parse_commands,
                               'or', commands, ' in context')
        msg = r"^malformed entry 'or' in context: "
        msg += r'\{"or": \{\}\}$'
        self.assertRaisesRegex(TypeError, msg, config.parse_commands,
                               {'or': {}}, commands, ' in context')
        commands1 = set()
        config.parse_commands({'commands': ['foo', 'bar']},
                              commands1, ' in context')
        self.assertEqual(commands1, {'foo', 'bar'})
        commands2 = set()
        config.parse_commands({'hooks': ['hook1']}, commands2, ' in context')
        self.assertEqual(commands2, {'foo', 'xyz'})
        config.parse_commands({'or': [{'commands': ['foo', 'bar']},
                                      {'hooks': ['hook1']}]},
                              commands, ' in context')
        self.assertEqual(commands, {'foo', 'bar', 'xyz'})
        self.assertEqual(commands, commands1.union(commands2))

    def test_command_accept(self):
        """
        This test verifies that command_accept function is usable.
        """
        config = kea_config.KeaReverseProxyConfig()
        role = kea_config.Role('user')
        config.apis['foo'] = {'name': 'foo', 'access': 'read'}
        config.apis['bar'] = {'name': 'bar', 'access': 'read'}
        config.apis['xyz'] = {'name': 'xyz', 'access': 'write'}
        config.accesses = {'read', 'write'}
        config.parse_commands({'access': 'read'}, role.accepts, '')
        self.assertEqual(role.accepts, {'foo', 'bar'})
        self.assertTrue(kea_config.command_accept(role, 'foo'))
        self.assertTrue(kea_config.command_accept(role, 'bar'))
        self.assertFalse(kea_config.command_accept(role, 'xyz'))
        self.assertFalse(kea_config.command_accept(role, 'a-command'))

def config_command_accept_suite():
    """
    Test suite for command accept parsing.
    """
    suite = unittest.TestSuite()
    suite.addTest(ConfigCommandAcceptTestCase('test_parse_unknown'))
    suite.addTest(ConfigCommandAcceptTestCase('test_parse_all'))
    suite.addTest(ConfigCommandAcceptTestCase('test_parse_access'))
    suite.addTest(ConfigCommandAcceptTestCase('test_parse_commands'))
    suite.addTest(ConfigCommandAcceptTestCase('test_parse_hooks'))
    suite.addTest(ConfigCommandAcceptTestCase('test_parse_not'))
    suite.addTest(ConfigCommandAcceptTestCase('test_parse_and'))
    suite.addTest(ConfigCommandAcceptTestCase('test_parse_or'))
    suite.addTest(ConfigCommandAcceptTestCase('test_command_accept'))
    return suite

class ResponseFilterTestCase(unittest.TestCase):
    """
    Test the response filter classses.
    """

    def test_list_commands(self):
        """
        This test verifies that the list-commands response filter works
        as expected.
        """
        accepts = {'foo'}
        rejects = {'bar'}
        rfilt = kea_response_filter.ResponseFilterListCommands(accepts,
                                                               rejects,
                                                               False)
        self.assertEqual(rfilt.name(), 'list-commands')
        body = 'xyz'
        self.assertEqual(rfilt.filter('list-commands', body), body)
        body = []
        self.assertEqual(rfilt.filter('list-commands', body), body)
        body = [{}]
        self.assertEqual(rfilt.filter('list-commands', body), body)
        body = [{'arguments': 'xyz'}]
        self.assertEqual(rfilt.filter('list-commands', body), body)
        body = [{'arguments': ['foo']}]
        self.assertEqual(rfilt.filter('list-commands', body), body)
        body = [{'arguments': ['bar']}]
        self.assertEqual(rfilt.filter('a-command', body), body)
        expected = [{'arguments': []}]
        self.assertEqual(rfilt.filter('list-commands', body), expected)
        body = [{'arguments': ['xyz']}]
        self.assertEqual(rfilt.filter('list-commands', body), expected)
        self.assertEqual(rfilt.filter('a-command', body), body)
        rfilt = kea_response_filter.ResponseFilterListCommands(accepts,
                                                               rejects,
                                                               True)
        self.assertEqual(rfilt.filter('list-commands', body), body)
        # accepts has the preference
        rejects.add('foo')
        body = [{'arguments': ['foo', 'xyz']}]
        self.assertEqual(rfilt.filter('list-commands', body), body)
        # try multiple servers
        body = [{'arguments': ['foo']}, {'arguments': ['bar']}]
        expected = [{'arguments': ['foo']}, {'arguments': []}]
        self.assertEqual(rfilt.filter('list-commands', body), expected)

    def test_list_commands_json(self):
        """
        This test verifies that the list-commands response filter works
        on the control agent response.
        """
        accepts = {'build-report', 'list-commands', 'version-get'}
        rejects = {'config-reload', 'config-set', 'shutdown'}
        rfilt = kea_response_filter.ResponseFilterListCommands(accepts,
                                                               rejects,
                                                               False)
        txt = '[ { "arguments": [ "build-report", "config-get", '
        txt += '"config-reload", "config-set", "config-test", "config-write", '
        txt += '"list-commands", "shutdown", "status-get", "version-get" ], '
        txt += '"result": 0 } ]'
        body = json.loads(txt)
        result = rfilt.filter('list-commands', body)
        expected = '[{"arguments": ["build-report", "list-commands", '
        expected += '"version-get"], "result": 0}]'
        self.assertEqual(expected, json.dumps(result, sort_keys=True))
        # try now with others set to True
        rfilt = kea_response_filter.ResponseFilterListCommands(accepts,
                                                               rejects,
                                                               True)
        body = json.loads(txt)
        result = rfilt.filter('list-commands', body)
        expected = '[{"arguments": ["build-report", "config-get", '
        expected += '"config-test", "config-write", "list-commands", '
        expected += '"status-get", "version-get"], "result": 0}]'
        self.assertEqual(expected, json.dumps(result, sort_keys=True))

    def test_config_control(self):
        """
        This test verifies that the config-control response filter works
        as expected.
        """
        rfilt = kea_response_filter.ResponseFilterConfigControl()
        self.assertEqual(rfilt.name(), 'config-control')
        body = 'xyz'
        self.assertEqual(rfilt.filter('config-get', body), body)
        body = []
        self.assertEqual(rfilt.filter('config-get', body), body)
        body = [{}]
        self.assertEqual(rfilt.filter('config-get', body), body)
        body = [{'arguments': 'xyz'}]
        self.assertEqual(rfilt.filter('config-get', body), body)
        body = [{'arguments': {'FooBar': {'config-control': 1}}}]
        self.assertEqual(rfilt.filter('config-get', body), body)
        body = [{'arguments': {'Dhcp4': {'foo': 'bar'}}}]
        self.assertEqual(rfilt.filter('config-get', body), body)
        body = [{'arguments': {'Dhcp6': {'foo': 'bar'}}}]
        self.assertEqual(rfilt.filter('config-get', body), body)
        body = [{'arguments': {'Dhcp4': {'config-control': 1}}}]
        expected = [{'arguments': {'Dhcp4': {}}}]
        self.assertEqual(rfilt.filter('config-get', body), expected)
        self.assertEqual(rfilt.filter('a-command', body), body)
        body = [{'arguments': {'Dhcp6': {'config-control': 1}}}]
        expected = [{'arguments': {'Dhcp6': {}}}]
        self.assertEqual(rfilt.filter('config-get', body), expected)
        self.assertEqual(rfilt.filter('a-command', body), body)
        # works also with both.
        body = [{'arguments': {'Dhcp4': {'config-control': 1},
                               'Dhcp6': {'config-control': 2}}}]
        expected = [{'arguments': {'Dhcp4': {}, 'Dhcp6': {}}}]
        self.assertEqual(rfilt.filter('config-get', body), expected)
        self.assertEqual(rfilt.filter('a-command', body), body)
        # works with multiple answers too.
        body = [{'arguments': {'Dhcp4': {'config-control': 1}}},
                {'arguments': {'Dhcp6': {'config-control': 2}}}]
        expected = [{'arguments': {'Dhcp4': {}}}, {'arguments': {'Dhcp6': {}}}]
        self.assertEqual(rfilt.filter('config-get', body), expected)
        self.assertEqual(rfilt.filter('a-command', body), body)

    def test_config_control_json(self):
        """
        This test verifies that the config-control response filter works
        on the kea6 config-backend.json example.
        """
        rfilt = kea_response_filter.ResponseFilterConfigControl()
        examples_kea6_config_backend = '''
[ { "result": 0, "arguments":
{ "Dhcp6":

{
    "server-tag": "server2",

    "interfaces-config": {
        "interfaces": [ "ethX" ]
    },

    "lease-database": {
        "type": "memfile",
        "lfc-interval": 3600
    },

    "config-control": {
        "config-databases": [
            {
                "type": "mysql",
                "name": "kea",
                "user": "kea",
                "password": "kea",
                "host": "localhost",
                "port": 3306
            }
        ],
        "config-fetch-wait-time": 20
    },

    "control-socket": {
        "socket-type": "unix",
        "socket-name": "/tmp/kea6-ctrl-socket"
    },

    "hooks-libraries": [
        {
            "library": "/usr/local/lib/kea/hooks/libdhcp_mysql_cb.so"
        }
    ],

    "loggers": [
        {
            "name": "kea-dhcp6",
            "output_options": [
                {
                    "output": "stdout"
                }
            ],
            "debuglevel": 0,
            "severity": "INFO"
        }
    ]
}

}
} ]
'''
        body = json.loads(examples_kea6_config_backend)
        result = rfilt.filter('config-get', body)
        expected = '''
[ { "result": 0, "arguments":
{ "Dhcp6":

{
    "server-tag": "server2",

    "interfaces-config": {
        "interfaces": [ "ethX" ]
    },

    "lease-database": {
        "type": "memfile",
        "lfc-interval": 3600
    },

    "control-socket": {
        "socket-type": "unix",
        "socket-name": "/tmp/kea6-ctrl-socket"
    },

    "hooks-libraries": [
        {
            "library": "/usr/local/lib/kea/hooks/libdhcp_mysql_cb.so"
        }
    ],

    "loggers": [
        {
            "name": "kea-dhcp6",
            "output_options": [
                {
                    "output": "stdout"
                }
            ],
            "debuglevel": 0,
            "severity": "INFO"
        }
    ]
}

}
} ]
'''
        self.assertEqual(json.loads(expected), result)

    def test_redact_password(self):
        """
        This test verifies that the redact-password response filter work
        as expected.
        """
        rfilt = kea_response_filter.ResponseFilterRedactPassword()
        self.assertEqual(rfilt.name(), 'redact-password')
        body = {'foo': [1, 2.3], 'bar': 'password', 'xyz': {'abc': True}}
        self.assertEqual(rfilt.filter('a-command', body), body)
        body = {'foo': ['password'], 'bar': 'password',
                'xyz': {'password': True}, 'abc': {'password': 'change-me'}}
        expected = {'foo': ['password'], 'bar': 'password',
                    'xyz': {'password': True}, 'abc': {'password': '*****'}}
        self.assertEqual(rfilt.filter('a-command', body), expected)
        body = {'foo': [[{'password': 'one'}]], 'bar': {'password': 'two'},
                'password': 'three', 'xyz': {'password': 'four'}}
        expected = {'foo': [[{'password': '*****'}]],
                    'bar': {'password': '*****'},
                    'password': '*****',
                    'xyz': {'password': '*****'}}
        self.assertEqual(rfilt.filter('a-command', body), expected)
        body = {'foo': [[{'Password': 'one'}]], 'bar': {'passWord': 'two'},
                'PASSWORD': 'three', 'xyz': {'abc': {'passwords': 'four'}}}
        self.assertEqual(rfilt.filter('a-command', body), body)


    def test_redact_password_json(self):
        """
        This test verifies that the redact-password response filter works
        on a config-get partial response.
        """
        rfilt = kea_response_filter.ResponseFilterRedactPassword()
        txt = '[ { "arguments": { "Dhcp4": {'
        txt += '"hosts-database": { "type": "mysql", "name": "kea", '
        txt += '"user": "kea", "password": "kea", "host": "localhost", '
        txt += '"port": 3306 }, "user-context": { "password": "test" } '
        txt += ' } }, "result": 0 } ]'
        body = json.loads(txt)
        result = rfilt.filter('config-get', body)
        expected = '[{"arguments": {"Dhcp4": {"hosts-database": {'
        expected += '"host": "localhost", "name": "kea", '
        expected += '"password": "*****", "port": 3306, '
        expected += '"type": "mysql", "user": "kea"}, '
        expected += '"user-context": {"password": "*****"}}}, '
        expected += '"result": 0}]'
        self.assertEqual(expected, json.dumps(result, sort_keys=True))

def response_filter_suite():
    """
    Test suite for kea_response_filter.
    """
    suite = unittest.TestSuite()
    suite.addTest(ResponseFilterTestCase('test_list_commands'))
    suite.addTest(ResponseFilterTestCase('test_list_commands_json'))
    suite.addTest(ResponseFilterTestCase('test_config_control'))
    suite.addTest(ResponseFilterTestCase('test_config_control_json'))
    suite.addTest(ResponseFilterTestCase('test_redact_password'))
    suite.addTest(ResponseFilterTestCase('test_redact_password_json'))
    return suite

class ConfigResponseFilterTestCase(unittest.TestCase):
    """
    Test kea_config.parse_response_filters.
    """

    def test_response_filters(self):
        """
        This test verifies that response filters are parsed.
        """
        config = kea_config.KeaReverseProxyConfig()
        role = kea_config.Role('foo')
        role.others = True
        config.parse_response_filters(role, None)
        self.assertEqual(role.filters, [])
        msg = r"^'response-filters' in role 'foo' must be a list$"
        self.assertRaisesRegex(TypeError, msg, config.parse_response_filters,
                               role, {'bar'})
        msg = r'^a response filter \(\{"bar": 1\}\) must be a string$'
        self.assertRaisesRegex(TypeError, msg, config.parse_response_filters,
                               role, [{'bar': 1}])
        msg = r"^unsupported value 'bar' for a response filter$"
        self.assertRaisesRegex(ValueError, msg, config.parse_response_filters,
                               role, ['bar'])
        config.parse_response_filters(role,
                                      ['list-commands',
                                       'config-control',
                                       'redact-password'])
        self.assertEqual(len(role.filters), 3)
        self.assertEqual(role.filters[0].name(), 'list-commands')
        self.assertEqual(role.filters[1].name(), 'config-control')
        self.assertEqual(role.filters[2].name(), 'redact-password')

    def test_response_filter(self):
        """
        This test verifies that response_filter function is usable.
        """
        config = kea_config.KeaReverseProxyConfig()
        role = kea_config.Role('user')
        config.apis['list-commands'] = {'name': 'list-commands',
                                        'access': 'read'}
        config.apis['foo'] = {'name': 'foo', 'access': 'read'}
        config.apis['bar'] = {'name': 'bar', 'access': 'read'}
        config.apis['xyz'] = {'name': 'xyz', 'access': 'write'}
        config.accesses = {'read', 'write'}
        config.parse_commands({'access': 'read'}, role.accepts, '')
        self.assertEqual(role.accepts, {'list-commands', 'foo', 'bar'})
        config.parse_response_filters(role, ['list-commands'])
        resp_body = r'[{"arguments": ["a-command", "bar", "foo", '
        resp_body += r'"list-commands", "xyz"], "result": 0}]'
        response = json.loads(resp_body)
        response = kea_config.response_filter(role, 'list-commands', response)
        expected = r'[{"arguments": ["bar", "foo", "list-commands"], '
        expected += r'"result": 0}]'
        self.assertEqual(json.dumps(response), expected)

def config_response_filter_suite():
    """
    Test suite for response filter parsing.
    """
    suite = unittest.TestSuite()
    suite.addTest(ConfigResponseFilterTestCase('test_response_filters'))
    suite.addTest(ConfigResponseFilterTestCase('test_response_filter'))
    return suite

class ConfigRoleTestCase(unittest.TestCase):
    """
    Test kea_config.parse_role[s].
    """

    def test_roles(self):
        """
        This test verifies that roles can be parsed.
        """
        config = kea_config.KeaReverseProxyConfig()
        config.parse_roles(dict())
        msg = r"^'roles' parameter must be a list$"
        self.assertRaisesRegex(TypeError, msg, config.parse_roles,
                               {'roles': {}})
        config.parse_roles({'roles': []})
        config.parse_roles({'roles': [{'name': 'foo'}, {'name': 'bar'}]})
        foo = config.roles.get('foo')
        self.assertIsNotNone(foo)
        self.assertEqual(foo.name, 'foo')
        bar = config.roles.get('bar')
        self.assertIsNotNone(bar)
        self.assertEqual(bar.name, 'bar')

    def test_role_map(self):
        """
        This test verifies that a role must be a map.
        """
        config = kea_config.KeaReverseProxyConfig()
        msg = r'^a role must be a map$'
        self.assertRaisesRegex(TypeError, msg, config.parse_role, [])

    def test_role_name_mandatory(self):
        """
        This test verifies that the name role parameter is required.
        """
        config = kea_config.KeaReverseProxyConfig()
        msg = r"^'name' parameter is mandantory in a role$"
        self.assertRaisesRegex(ValueError, msg, config.parse_role, dict())

    def test_role_name_string(self):
        """
        This test verifies that the name role parameter must be a string.
        """
        config = kea_config.KeaReverseProxyConfig()
        msg = r"^'name' parameter in a role must be a string$"
        self.assertRaisesRegex(TypeError, msg, config.parse_role, {'name': 1})

    def test_role_name_duplicate(self):
        """
        This test verifies that the name role parameter must not be duplicated.
        """
        config = kea_config.KeaReverseProxyConfig()
        config.parse_role({'name': 'foo'})
        msg = r"^duplicate roles named 'foo'$"
        self.assertRaisesRegex(ValueError, msg, config.parse_role,
                               {'name': 'foo'})
        config.parse_role({'name': 'bar'})
        self.assertEqual(len(config.roles), 2)
        foo = config.roles.get('foo')
        self.assertIsNotNone(foo)
        self.assertEqual(foo.name, 'foo')
        bar = config.roles.get('bar')
        self.assertIsNotNone(bar)
        self.assertEqual(bar.name, 'bar')

    def test_role_accept_commands(self):
        """
        This test verifies that the accept-commands role parameter can
        be parsed.
        """
        config = kea_config.KeaReverseProxyConfig()
        config.parse_role({'name': 'foo', 'accept-commands': 'all'})
        self.assertIsNotNone(config.roles.get('foo'))

    def test_role_reject_commands(self):
        """
        This test verifies that the reject-commands role parameter can
        be parsed.
        """
        config = kea_config.KeaReverseProxyConfig()
        config.parse_role({'name': 'foo', 'reject-commands': {'not': 'all'}})
        self.assertIsNotNone(config.roles.get('foo'))

    def test_role_others_commands_boolean(self):
        """
        This test verifies that the others-commands role parameter must
        be a boolean.
        """
        config = kea_config.KeaReverseProxyConfig()
        msg = r"^'others-commands' in role 'foo' must be a boolean$"
        self.assertRaisesRegex(TypeError, msg, config.parse_role,
                               {'name': 'foo', 'others-commands': 'on'})

    def test_role_others_commands_default(self):
        """
        This test verifies that the others-commands role parameter default
        value is False.
        """
        config = kea_config.KeaReverseProxyConfig()
        config.parse_role({'name': 'foo'})
        foo = config.roles.get('foo')
        self.assertIsNotNone(foo)
        self.assertFalse(foo.others)

    def test_role_others_commands(self):
        """
        This test verifies that the others-commands role parameter can
        be parsed.
        """
        config = kea_config.KeaReverseProxyConfig()
        config.parse_role({'name': 'foo', 'others-commands': True})
        foo = config.roles.get('foo')
        self.assertIsNotNone(foo)
        self.assertTrue(foo.others)

    def test_role_preference_string(self):
        """
        This test verifies that the preference role parameter must be a string.
        """
        config = kea_config.KeaReverseProxyConfig()
        msg = r"^'preference' in role 'foo' must be a string$"
        self.assertRaisesRegex(TypeError, msg, config.parse_role,
                               {'name': 'foo', 'preference': True})

    def test_role_preference_unsupported(self):
        """
        This test verifies that the preference role parameter values can be
        only 'accept' and 'reject'.
        """
        config = kea_config.KeaReverseProxyConfig()
        msg = r"^unknown 'preference' \(foobar\) in role 'foo': "
        msg += r"use 'accept' or 'reject'$"
        self.assertRaisesRegex(ValueError, msg, config.parse_role,
                               {'name': 'foo', 'preference': 'foobar'})
        config.parse_role({'name': 'foo', 'preference': 'accept'})
        config.parse_role({'name': 'bar', 'preference': 'reject'})
        self.assertEqual(set(config.roles.keys()), {'foo', 'bar'})

    def test_role_preference_default(self):
        """
        This test verifies that the preference role parameter default value
        is 'accept'.
        """
        config = kea_config.KeaReverseProxyConfig()
        config.apis['foo'] = {'name': 'foo'}
        config.parse_role({'name': 'foo', 'accept-commands': 'all',
                           'reject-commands': 'all'})
        foo = config.roles.get('foo')
        self.assertIsNotNone(foo)
        self.assertEqual(foo.accepts, {'foo'})
        self.assertEqual(len(foo.rejects), 0)

    def test_role_preference(self):
        """
        This test verifies that the preference role parameter can be parsed.
        """
        config = kea_config.KeaReverseProxyConfig()
        config.apis['foo'] = {'name': 'foo'}
        config.parse_role({'name': 'foo', 'preference': 'reject',
                           'accept-commands': 'all', 'reject-commands': 'all'})
        foo = config.roles.get('foo')
        self.assertIsNotNone(foo)
        self.assertEqual(len(foo.accepts), 0)
        self.assertEqual(foo.rejects, {'foo'})

    def test_role_filters(self):
        """
        This test verifies that the response-filters role parameter can
        be parsed.
        """
        config = kea_config.KeaReverseProxyConfig()
        config.parse_role({'name': 'foo',
                           'response-filters': ['list-commands',
                                                'config-control']})
        foo = config.roles.get('foo')
        self.assertIsNotNone(foo)
        self.assertEqual(len(foo.filters), 2)
        self.assertEqual(foo.filters[0].name(), 'list-commands')
        self.assertEqual(foo.filters[1].name(), 'config-control')

    def test_role_user_string(self):
        """
        This test verifies that the basic-auth-user role parameter must be
        a string.
        """
        config = kea_config.KeaReverseProxyConfig()
        msg = r"^'user' parameter in role 'foo' must be a string$"
        self.assertRaisesRegex(TypeError, msg, config.parse_role,
                               {'name': 'foo', 'basic-auth-user': False})

    def test_role_user_default(self):
        """
        This test verifies that the basic-auth-user role parameter default
        value is the role name.
        """
        config = kea_config.KeaReverseProxyConfig()
        config.parse_role({'name': 'foo'})
        foo = config.roles.get('foo')
        self.assertIsNotNone(foo)
        self.assertEqual(foo.user, foo.name)
        self.assertEqual(foo.user, 'foo')

    def test_role_user(self):
        """
        This test verifies that the basic-auth-user role parameter can
        be parsed.
        """
        config = kea_config.KeaReverseProxyConfig()
        config.parse_role({'name': 'foo', 'basic-auth-user': 'bar'})
        foo = config.roles.get('foo')
        self.assertIsNotNone(foo)
        self.assertEqual(foo.user, 'bar')

    def test_role_password_string(self):
        """
        This test verifies that the basic-auth-password role parameter must be
        a string.
        """
        config = kea_config.KeaReverseProxyConfig()
        msg = r"^'password' parameter in role 'foo' must be a string$"
        self.assertRaisesRegex(TypeError, msg, config.parse_role,
                               {'name': 'foo', 'basic-auth-password': False})

    def test_role_password(self):
        """
        This test verifies that the basic-auth-password role parameter can
        be parsed.
        """
        config = kea_config.KeaReverseProxyConfig()
        config.parse_role({'name': 'foo', 'basic-auth-password': 'bar'})
        foo = config.roles.get('foo')
        self.assertIsNotNone(foo)
        self.assertEqual(foo.password, 'bar')

def config_role_suite():
    """
    Test suite for role parsing.
    """
    suite = unittest.TestSuite()
    suite.addTest(ConfigRoleTestCase('test_roles'))
    suite.addTest(ConfigRoleTestCase('test_role_map'))
    suite.addTest(ConfigRoleTestCase('test_role_name_mandatory'))
    suite.addTest(ConfigRoleTestCase('test_role_name_string'))
    suite.addTest(ConfigRoleTestCase('test_role_name_duplicate'))
    suite.addTest(ConfigRoleTestCase('test_role_accept_commands'))
    suite.addTest(ConfigRoleTestCase('test_role_reject_commands'))
    suite.addTest(ConfigRoleTestCase('test_role_others_commands_boolean'))
    suite.addTest(ConfigRoleTestCase('test_role_others_commands_default'))
    suite.addTest(ConfigRoleTestCase('test_role_others_commands'))
    suite.addTest(ConfigRoleTestCase('test_role_preference_string'))
    suite.addTest(ConfigRoleTestCase('test_role_preference_unsupported'))
    suite.addTest(ConfigRoleTestCase('test_role_preference_default'))
    suite.addTest(ConfigRoleTestCase('test_role_preference'))
    suite.addTest(ConfigRoleTestCase('test_role_filters'))
    suite.addTest(ConfigRoleTestCase('test_role_user_string'))
    suite.addTest(ConfigRoleTestCase('test_role_user_default'))
    suite.addTest(ConfigRoleTestCase('test_role_user'))
    suite.addTest(ConfigRoleTestCase('test_role_password_string'))
    suite.addTest(ConfigRoleTestCase('test_role_password'))
    return suite

class ConfigTlsTestCase(unittest.TestCase):
    """
    Test kea_config.parse_tls.
    """

    def test_ca(self):
        """
        This test verifies that trust-anchor parameter can be parsed.
        """
        config = kea_config.KeaReverseProxyConfig()
        msg = r"^'trust-anchor' parameter must be a string$"
        self.assertRaisesRegex(TypeError, msg, config.parse_tls,
                               {'trust-anchor': ['foo']})
        msg = r"^'does/not/exit' does not exist$"
        self.assertRaisesRegex(ValueError, msg, config.parse_tls,
                               {'trust-anchor': 'does/not/exit'})
        msg = r"^'" + os.path.devnull + "' is not a file or a directory$"
        self.assertRaisesRegex(ValueError, msg, config.parse_tls,
                               {'trust-anchor': os.path.devnull})
        config.parse_tls({'trust-anchor': 'ca/kea-ca.crt'})
        self.assertEqual(config.tls.ca_file, 'ca/kea-ca.crt')
        self.assertIsNone(config.tls.ca_path)
        config = kea_config.KeaReverseProxyConfig()
        config.parse_tls({'trust-anchor': 'ca'})
        self.assertIsNone(config.tls.ca_file)
        self.assertEqual(config.tls.ca_path, 'ca')

    def test_cert_file(self):
        """
        This test verifies that cert-file parameter can be parsed.
        """
        config = kea_config.KeaReverseProxyConfig()
        msg = r"^'cert-file' is useless without a 'trust-anchor'$"
        self.assertRaisesRegex(ValueError, msg, config.parse_tls,
                               {'cert-file': 'foo'})
        msg = r"^'cert-file' parameter must be a string$"
        self.assertRaisesRegex(TypeError, msg, config.parse_tls,
                               {'trust-anchor': 'ca/kea-ca.crt',
                                'cert-file': ['foo']})
        msg = r"^'does/not/exit' does not exist$"
        self.assertRaisesRegex(ValueError, msg, config.parse_tls,
                               {'trust-anchor': 'ca/kea-ca.crt',
                                'cert-file': 'does/not/exit'})
        msg = r"^'ca' is not a file$"
        self.assertRaisesRegex(ValueError, msg, config.parse_tls,
                               {'trust-anchor': 'ca/kea-ca.crt',
                                'cert-file': 'ca'})
        config.parse_tls({'trust-anchor': 'ca/kea-ca.crt',
                          'cert-file': 'ca/kea-server.crt'})
        self.assertEqual(config.tls.cert_file, 'ca/kea-server.crt')

    def test_key_file(self):
        """
        This test verifies that key-file parameter can be parsed.
        """
        config = kea_config.KeaReverseProxyConfig()
        msg = r"^'key-file' is useless without a 'trust-anchor'$"
        self.assertRaisesRegex(ValueError, msg, config.parse_tls,
                               {'key-file': 'foo'})
        msg = r"^'key-file' is useless without a 'cert-file'$"
        self.assertRaisesRegex(ValueError, msg, config.parse_tls,
                               {'trust-anchor': 'ca/kea-ca.crt',
                                'key-file': 'ca/kea-server.key'})
        msg = r"^'key-file' parameter must be a string$"
        self.assertRaisesRegex(TypeError, msg, config.parse_tls,
                               {'trust-anchor': 'ca/kea-ca.crt',
                                'cert-file': 'ca/kea-server.crt',
                                'key-file': ['foo']})
        msg = r"^'does/not/exit' does not exist$"
        self.assertRaisesRegex(ValueError, msg, config.parse_tls,
                               {'trust-anchor': 'ca/kea-ca.crt',
                                'cert-file': 'ca/kea-server.crt',
                                'key-file': 'does/not/exit'})
        msg = r"^'ca' is not a file$"
        self.assertRaisesRegex(ValueError, msg, config.parse_tls,
                               {'trust-anchor': 'ca/kea-ca.crt',
                                'cert-file': 'ca/kea-server.crt',
                                'key-file': 'ca'})
        config.parse_tls({'trust-anchor': 'ca/kea-ca.crt',
                          'cert-file': 'ca/kea-server.crt',
                          'key-file': 'ca/kea-server.key'})
        self.assertEqual(config.tls.key_file, 'ca/kea-server.key')

    def test_cert_required(self):
        """
        This test verifies that cert-required parameter can be parsed.
        """
        config = kea_config.KeaReverseProxyConfig()
        msg = r"^'cert-required' is useless without a 'trust-anchor'$"
        self.assertRaisesRegex(ValueError, msg, config.parse_tls,
                               {'cert-required': True})
        msg = r"^'cert-required' parameter must be a boolean$"
        self.assertRaisesRegex(TypeError, msg, config.parse_tls,
                               {'trust-anchor': 'ca/kea-ca.crt',
                                'cert-required': 1})
        self.assertTrue(config.tls.cert_required)
        config.parse_tls({'trust-anchor': 'ca/kea-ca.crt',
                          'cert-required': False})
        self.assertFalse(config.tls.cert_required)

    def test_tls_setup(self):
        """
        This test verifies that TLS complete setup works.
        """
        config = kea_config.KeaReverseProxyConfig()
        config.parse_tls({'trust-anchor': 'ca/kea-ca.crt',
                          'cert-file': 'ca/kea-server.crt',
                          'key-file': 'ca/kea-server.key'})
        ctx = ssl.create_default_context(ssl.Purpose.CLIENT_AUTH,
                                         cafile=config.tls.ca_file,
                                         capath=config.tls.ca_path)
        self.assertIsNotNone(ctx)
        ctx.verify_mode = ssl.CERT_OPTIONAL
        ctx.verify_mode = ssl.CERT_REQUIRED
        ctx.load_cert_chain(config.tls.cert_file, config.tls.key_file)

    def test_tls_setup_dir(self):
        """
        This test verifies that TLS complete setup using a directory works.
        """
        config = kea_config.KeaReverseProxyConfig()
        config.parse_tls({'trust-anchor': 'ca',
                          'cert-file': 'ca/kea-server.crt',
                          'key-file': 'ca/kea-server.key'})
        ctx = ssl.create_default_context(ssl.Purpose.CLIENT_AUTH,
                                         cafile=config.tls.ca_file,
                                         capath=config.tls.ca_path)
        self.assertIsNotNone(ctx)
        ctx.verify_mode = ssl.CERT_OPTIONAL
        ctx.verify_mode = ssl.CERT_REQUIRED
        ctx.load_cert_chain(config.tls.cert_file, config.tls.key_file)

def config_tls_suite():
    """
    Test suite for TLS setting.
    """
    suite = unittest.TestSuite()
    suite.addTest(ConfigTlsTestCase('test_ca'))
    suite.addTest(ConfigTlsTestCase('test_cert_file'))
    suite.addTest(ConfigTlsTestCase('test_key_file'))
    suite.addTest(ConfigTlsTestCase('test_cert_required'))
    suite.addTest(ConfigTlsTestCase('test_tls_setup'))
    suite.addTest(ConfigTlsTestCase('test_tls_setup_dir'))
    return suite

class ConfigExtensionTestCase(unittest.TestCase):
    """
    Test kea_config.parse_extension.
    """

    def test_search_path(self):
        """
        This test verifies that extension-search-path can be parsed.
        """
        config = kea_config.KeaReverseProxyConfig()
        msg = r"^'extension-search-path' parameter must be a string$"
        self.assertRaisesRegex(TypeError, msg, config.parse_extension,
                               {'extension-search-path': ['foo']})
        before = len(sys.path)
        config.parse_extension({'extension-search-path': '/tmp'})
        after = len(sys.path)
        last = sys.path.pop()
        self.assertEqual(after, before + 1)
        self.assertEqual(last, '/tmp')

    def test_module(self):
        """
        This test verifies that 'extension-module-name' can be parsed.
        """
        config = kea_config.KeaReverseProxyConfig()
        msg = r"^'extension-module-name' parameter must be a string$"
        self.assertRaisesRegex(TypeError, msg, config.parse_extension,
                               {'extension-module-name': ['foo']})
        config.parse_extension({'extension-module-name': 'stat'})
        self.assertRegex(repr(config.extension), r"^<module 'stat' from .*")

    def test_module_package(self):
        """
        This test verifies that 'extension-package-name' can be parsed.
        """
        config = kea_config.KeaReverseProxyConfig()
        msg = r"^'extension-package-name' parameter requires "
        msg += r"a 'extension-module-name' one$"
        self.assertRaisesRegex(ValueError, msg, config.parse_extension,
                               {'extension-package-name': 'foo'})
        msg = r"^'extension-package-name' parameter must be a string$"
        self.assertRaisesRegex(TypeError, msg, config.parse_extension,
                               {'extension-module-name': 'foo',
                                'extension-package-name': ['bar']})
        config.parse_extension({'extension-module-name': '.config',
                                'extension-package-name': 'logging'})
        self.assertRegex(repr(config.extension),
                         r"^<module 'logging.config' from .*")

    def test_extension_test_module(self):
        """
        This test verifies that the extension test module works as expected.
        """
        config = kea_config.KeaReverseProxyConfig()
        cfg_txt = '''
{
    "ReverseProxy":
    {
        "extension-module-name": "extension_test_module",
        "host": "localhost",
        "reverse-proxy-port": 4563,
        "control-agent-port": 8081,
        "api-files": "../../../share/api",
        "assign-role-by": "issuer",
        "assign-role-parameter": { "kea-ca": "admin" },
        "default-role": "user",
        "roles": [
        {
            "name": "admin",
            "accept-commands": "all"
        },{
            "name": "user",
            "accept-commands": { "access": "read" },
            "response-filters": [ "list-commands" ]
        } ],
        "trust-anchor": "ca/kea-ca.crt",
        "cert-file": "ca/kea-server.crt",
        "key-file": "ca/kea-server.key",
        "cert-required": false
    }
}
'''
        cfg_json = json.loads(cfg_txt)
        config.parse(cfg_json)
        self.assertIsNotNone(config.extension)
        obj = config.extension.KeaReverseProxyExtension()
        self.assertIsNotNone(obj)
        self.assertTrue(obj.saw_pre_parse)
        self.assertTrue(obj.saw_post_parse)

def config_extension_suite():
    """
    Test suite for extension configuration.
    """
    suite = unittest.TestSuite()
    suite.addTest(ConfigExtensionTestCase('test_search_path'))
    suite.addTest(ConfigExtensionTestCase('test_module'))
    suite.addTest(ConfigExtensionTestCase('test_module_package'))
    suite.addTest(ConfigExtensionTestCase('test_extension_test_module'))
    return suite

def all_suites():
    """
    Test suite for everything.
    """
    suites = unittest.TestSuite()
    suites.addTest(config_suite())
    suites.addTest(config_global_suite())
    suites.addTest(api_file_suite())
    suites.addTest(role_suite())
    suites.addTest(setup_role_suite())
    suites.addTest(command_accept_suite())
    suites.addTest(config_command_accept_suite())
    suites.addTest(response_filter_suite())
    suites.addTest(config_response_filter_suite())
    suites.addTest(config_role_suite())
    suites.addTest(config_tls_suite())
    suites.addTest(config_extension_suite())
    return suites

if __name__ == '__main__':
    testsrcdir = os.path.dirname(os.path.abspath(__file__))
    print('change directory', testsrcdir)
    os.chdir(testsrcdir)
    runner = unittest.TextTestRunner(verbosity=2)
    testresult = runner.run(all_suites())
    if testresult.wasSuccessful():
        sys.exit(0)
    else:
        sys.exit(1)

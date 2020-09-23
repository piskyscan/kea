# Copyright (C) 2020 Internet Systems Consortium, Inc. ("ISC")
#
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

"""
This file contains classes used for Reverse Proxy configuration.

The configuration is a JSON map with # and // comments.
Here is an example with default values.

{
    // Main entry (required and only entry at this level).
    "ReverseProxy": {
        // Extension path.
        "extension-search-path": "/tmp",
        // Extension module name.
        "extension-module-name": "foo",
        // Extension package name (requires a module name).
        "extension-package-name": "bar",
        // Hostname of the reverse proxy and the control agent.
        "host": "127.0.0.1",
        // TCP port where the reverse proxy listen on.
        "reverse-proxy-port": 443,
        // TCP port where the control agent listen on and
        // the reverse proxy connect to.
        "control-agent-port": 8000,
        // Connect timeout (in seconds).
        "connect-timeout": 5,
        // Read timeout (in seconds).
        "read-timeout": 10,
        // Where to find api files (mandatory).
        "api-files": "../../share/api",
        // Role assignment method (mandatory, value issuer or subject).
        "assign-role-by": "issuer",
        // Assignment method optional parameter.
        "assign-role-parameter": "foo",
        // Default role (reject everything if empty).
        "default-role": "",
        // Role list.
        "roles": [
            {
                // Role name (mandatory).
                "name": "admin",
                // Accept command filter.
                "accept-commands": {
                    // And: all filters must match.
                    "and": [
                        // Accepts all known commands.
                        "all",
                        {
                            // Not: negate filter.
                            "not": {
                                // Match cb_cmds hook.
                                "hooks": [ "cb_cmds" ]
                            }
                        }
                    ]
                },
                // Reject command filter.
                "reject-commands": {
                    // Or: a filter must match.
                    "or": [
                        {
                            // Match hooks.
                            "hooks": [ "cb_cmds" ]
                        },
                        {
                            // Match commands list.
                            "commands": [ "foo", "bar" ]
                        },
                        {
                            // Match access.
                            "access": "write"
                        }
                    ]
                },
                // Accept anything not already accepted or rejected.
                "others-commands": false,
                // Preference for commands of both accept and reject lists.
                "preference": "accept",
                // Filter responses.
                "response-filters": [
                    // Remove rejected commands from list-commands response.
                    "list-commands",
                    // Remove config-control from config-get responses.
                    "config-control"
                ],
                // Basic HTTP authentication user (default role name).
                "basic-auth-user": "kea-admin",
                // Basic HTTP authentication password.
                // No password means the reverse proxy does not add
                // basic HTTP authentication header.
                "basic-auth-password": "nimda-aek"
           }
        ],
        // Security aka TLS setting.
        // Trust anchor aka CA (can be a file or a directory).
        "trust-anchor": "/tmp/ca/ca.crt",
        // Server certificate file (please use accepted subjetAltName).
        "cert-file": "/tmp/ca/kea-reverse-proxy.crt",
        // Server key file (not ciphered).
        "key-file": "/tmp/ca/kea-reverse-proxy.key",
        // Client certificates are required or optional.
        "cert-required": true
    }
}
"""

import json
import sys
import importlib
from pathlib import Path
from requests.auth import HTTPBasicAuth
import kea_role
import kea_command_accept
import kea_response_filter

class Role:
    """
    Reverse Proxy role class.
    """

    __slots__ = ['name', 'accepts', 'rejects', 'others', 'filters',
                 'user', 'password']

    def __init__(self, name):
        """
        Initialize an instance.
         - name: role name.
         - accepts: set of accepted commands.
         - rejects: set of rejected commands.
         - others: True to accept commands not in accepts and rejects,
                   False otherwise.
         - filters: list of response filters.
         - user: basic HTTP authentication user.
         - password: basic HTTP authentication password.
        """
        self.name = name
        self.accepts = set()
        self.rejects = set()
        self.others = False
        self.filters = []
        self.user = name
        self.password = None

class Tls:
    """
    TLS setting class.
    """

    __slots__ = ['ca_file', 'ca_path',
                 'cert_file', 'key_file',
                 'cert_required']

    def __init__(self):
        """
        Initialize an instance.
         - ca_file: CA (aka trust anchor) certificate file.
         - ca_path: CA (aka trust anchor) directory path.
         - cert_file: server certificate file.
         - key_file: server key file.
         - cert_required: True if client certificates are requires,
                          False if they are optional (in this case
                          a default role is needed).
        CA can be file or path. Cert and key files come together.
        """
        self.ca_file = None
        self.ca_path = None
        self.cert_file = None
        self.key_file = None
        self.cert_required = True

class KeaReverseProxyConfig:
    """
    Reverse Proxy configuration class:
     - load the configuration file
     - load api files with per command attributes
     - build role assign, command and response filters
    """

    __slots__ = ['extension',
                 'host', 'rp_port', 'ca_port', 'timeouts', 'tls',
                 'apis', 'accesses', 'hooks',
                 'assign', 'default', 'roles',
                 'assign_creates', 'assign_setups',
                 'parsers', 'filters']

    def __init__(self):
        """
        Initialize an instance.
         - extension: extension module.
         - host: reverse proxy and control agent host name or address.
         - rp_port: reverse proxy port.
         - ca_port: control agent port.
         - timeouts: connect and read timeouts.
         - tls: TLS setting.
         - apis: command description dictionary.
         - accesses: set of possible api accesses.
         - hooks: set of possible api hooks.
         - assign: assign the role from the peer certificate.
         - default: default role (reject everything when empty_.
         - roles: roles dictionary.
         - assign_creates: factory for the assign member.
         - assign_setups: setup for the assign member.
         - parsers: parsers for the roles elements.
         - filters: factory for response filters.
        """
        self.extension = None
        self.host = '127.0.0.1'
        self.rp_port = 443
        self.ca_port = 8000
        self.timeouts = (5, 10)
        self.tls = Tls()
        self.apis = dict()
        self.accesses = set()
        self.hooks = set()
        self.assign = None
        self.default = ''
        self.roles = dict()
        self.assign_creates = dict()
        self.assign_creates['issuer'] = create_assign_issuer
        self.assign_creates['subject'] = create_assign_subject
        self.assign_creates['static'] = create_assign_static
        self.assign_creates['test'] = create_assign_static
        self.assign_setups = dict()
        self.assign_setups['issuer'] = self.setup_assign_dict
        self.assign_setups['subject'] = self.setup_assign_dict
        self.assign_setups['static'] = self.setup_assign_default
        self.assign_setups['test'] = self.setup_assign_default
        self.parsers = dict()
        self.parsers['all'] = parse_filter_all
        self.parsers['access'] = self.parse_filter_access
        self.parsers['commands'] = self.parse_filter_commands
        self.parsers['hooks'] = self.parse_filter_hooks
        self.parsers['not'] = self.parse_filter_not
        self.parsers['and'] = self.parse_filter_and
        self.parsers['or'] = self.parse_filter_or
        self.filters = dict()
        self.filters['list-commands'] = create_filter_list_commands
        self.filters['config-control'] = create_filter_config_control
        self.filters['redact-password'] = create_filter_redact_password

    def load(self, config_file):
        """
        Loads the configuration file:
         - remove comments
         - translate to JSON
         - parse the configuration
        """
        text_cfg = ''
        with open(config_file) as file:
            for line in file:
                comment = line.find("#")
                if comment >= 0:
                    line = line[: comment] + '\n'
                comment = line.find("//")
                if comment >= 0:
                    line = line[: comment] + '\n'
                text_cfg += line
        json_cfg = json.loads(text_cfg)
        self.parse(json_cfg)

    def parse(self, json_cfg):
        """
        Parse the JSON configuration
        """
        if not isinstance(json_cfg, dict):
            raise TypeError('configuration must be a map')
        rp_cfg = json_cfg.get('ReverseProxy')
        if rp_cfg is None:
            raise ValueError("can't find 'ReverseProxy' toplevel entry")
        if not isinstance(rp_cfg, dict):
            raise TypeError("'ReverseProxy' entry must be a map")
        self.parse_extension(rp_cfg)
        if self.extension is not None:
            try:
                self.extension.pre_parse(self, rp_cfg)
            except AttributeError:
                pass
        self.parse_globals(rp_cfg)
        self.parse_roles(rp_cfg)
        if self.default != '':
            if self.default not in self.roles:
                print("default role '" + self.default + "' is not configured")
        # todo: add sanity checks
        if self.assign is None:
            raise RuntimeError('no way to assign role')
        name = self.assign.name()
        setup = self.assign_setups.get(name)
        setup(name, rp_cfg.get('assign-role-parameter'))
        self.parse_tls(rp_cfg)
        if self.extension is not None:
            try:
                self.extension.post_parse(self, rp_cfg)
            except AttributeError:
                pass

    def parse_extension(self, rp_cfg):
        """
        Parse extension parameters.
        """
        search_path = rp_cfg.get('extension-search-path')
        if search_path is not None:
            if not isinstance(search_path, str):
                raise TypeError("'extension-search-path' parameter " +
                                "must be a string")
            sys.path.append(search_path)
        mod_name = rp_cfg.get('extension-module-name')
        if mod_name is not None:
            if not isinstance(mod_name, str):
                raise TypeError("'extension-module-name' parameter " +
                                "must be a string")
        pkg_name = rp_cfg.get('extension-package-name')
        if pkg_name is not None:
            if mod_name is None:
                raise ValueError("'extension-package-name' parameter " +
                                 "requires a 'extension-module-name' one")
            if not isinstance(pkg_name, str):
                raise TypeError("'extension-package-name' parameter " +
                                "must be a string")
        if mod_name is not None:
            self.extension = importlib.import_module(mod_name, pkg_name)

    def parse_globals(self, rp_cfg):
        """
        Parse global parameters.
        """
        host_cfg = rp_cfg.get('host')
        if host_cfg is not None:
            if not isinstance(host_cfg, str):
                raise TypeError("'host' parameter must be a string")
            self.host = host_cfg
        rp_port_cfg = rp_cfg.get('reverse-proxy-port')
        if rp_port_cfg is not None:
            if not isinstance(rp_port_cfg, int):
                raise TypeError("'reverse-proxy-port' parameter " +
                                "must be an integer")
            self.rp_port = rp_port_cfg
        ca_port_cfg = rp_cfg.get('control-agent-port')
        if ca_port_cfg is not None:
            if not isinstance(ca_port_cfg, int):
                raise TypeError("'control-agent-port' parameter " +
                                "must be an integer")
            self.ca_port = ca_port_cfg
        connect_timeout, read_timeout = self.timeouts
        connect_timeout_cfg = rp_cfg.get('connect-timeout')
        if connect_timeout_cfg is not None:
            if not isinstance(connect_timeout_cfg, (int, float)):
                raise TypeError("'connect-timeout' parameter must be a number")
            connect_timeout = connect_timeout_cfg
        read_timeout_cfg = rp_cfg.get('read-timeout')
        if read_timeout_cfg is not None:
            if not isinstance(read_timeout_cfg, (int, float)):
                raise TypeError("'read-timeout' parameter must be a number")
            read_timeout = read_timeout_cfg
        self.timeouts = (connect_timeout, read_timeout)
        api_dir = rp_cfg.get('api-files')
        if api_dir is None:
            raise ValueError("'api-files' parameter is mandatory")
        if not isinstance(api_dir, str):
            raise TypeError("'api-files' parameter must be a string")
        if api_dir != 'test':
            self.load_api_files(api_dir)
        assign_role_by = rp_cfg.get('assign-role-by')
        if assign_role_by is None:
            raise ValueError("'assign-role-by' parameter is mandatory")
        if not isinstance(assign_role_by, str):
            raise TypeError("'assign-role-by' parameter must be a string")
        factory = self.assign_creates.get(assign_role_by)
        if factory is None:
            raise ValueError("unsupported value '" + assign_role_by +
                             "' for 'assign-role-by' parameter")
        self.assign = factory()
        default_cfg = rp_cfg.get('default-role')
        if default_cfg is not None:
            if not isinstance(default_cfg, str):
                raise TypeError("'default-role' parameter must be a string")
            self.default = default_cfg

    def load_api_files(self, api_dir):
        """
        Loads api files from api_dir (string).
        """
        api_path = Path(api_dir)
        if not api_path.exists():
            raise ValueError("'" + api_dir + "' does not exist")
        if not api_path.is_dir():
            raise ValueError("'" + api_dir + "' must be a directory")
        files = api_path.glob('*.json')
        for api_file in files:
            command = api_file.stem
            # skip _template
            if command.startswith('_'):
                continue
            if not api_file.is_file():
                raise ValueError("'" + str(api_file) + "' is not a file")
            self.load_api_file(command, api_file)

    def load_api_file(self, command, api_file):
        """
        Loads an api file (api_file Path) for a command (string).
        """
        try:
            descr = json.loads(api_file.read_text())
        except:
            print("Error while loading api file '" + str(api_file) + "'")
            raise
        if command != descr.get('name'):
            raise ValueError("api file '" + str(api_file) + "' does not " +
                             "describe command '" + command + "'")
        access = descr.get('access')
        if access is None:
            raise ValueError("command '" + command + "' description " +
                             "has no access")
        self.accesses.add(access)
        hook = descr.get('hook')
        if hook is not None:
            self.hooks.add(hook)
        self.apis[command] = descr

    def parse_tls(self, rp_cfg):
        """
        Parse TLS setting.
        """
        ta_cfg = rp_cfg.get('trust-anchor')
        if ta_cfg is not None:
            if not isinstance(ta_cfg, str):
                raise TypeError("'trust-anchor' parameter must be a string")
            ta_path = Path(ta_cfg)
            if not ta_path.exists():
                raise ValueError("'" + ta_cfg + "' does not exist")
            if ta_path.is_file():
                self.tls.ca_file = ta_cfg
            elif ta_path.is_dir():
                self.tls.ca_path = ta_cfg
            else:
                raise ValueError("'" + ta_cfg + "' is not a file or "
                                 "a directory")
        cert_cfg = rp_cfg.get('cert-file')
        if cert_cfg is not None:
            if ta_cfg is None:
                raise ValueError("'cert-file' is useless without a "
                                 "'trust-anchor'")
            if not isinstance(cert_cfg, str):
                raise TypeError("'cert-file' parameter must be a string")
            cert_path = Path(cert_cfg)
            if not cert_path.exists():
                raise ValueError("'" + cert_cfg + "' does not exist")
            if not cert_path.is_file():
                raise ValueError("'" + cert_cfg + "' is not a file")
            self.tls.cert_file = cert_cfg
        key_cfg = rp_cfg.get('key-file')
        if key_cfg is not None:
            if ta_cfg is None:
                raise ValueError("'key-file' is useless without a "
                                 "'trust-anchor'")
            if cert_cfg is None:
                raise ValueError("'key-file' is useless without a "
                                 "'cert-file'")
            if not isinstance(key_cfg, str):
                raise TypeError("'key-file' parameter must be a string")
            key_path = Path(key_cfg)
            if not key_path.exists():
                raise ValueError("'" + key_cfg + "' does not exist")
            if not key_path.is_file():
                raise ValueError("'" + key_cfg  + "' is not a file")
            self.tls.key_file = key_cfg
        req_cfg = rp_cfg.get('cert-required')
        if req_cfg is not None:
            if ta_cfg is None:
                raise ValueError("'cert-required' is useless without a "
                                 "'trust-anchor'")
            if not isinstance(req_cfg, bool):
                raise TypeError("'cert-required' parameter must be a boolean")
            self.tls.cert_required = req_cfg

    def parse_roles(self, rp_cfg):
        """
        Parse roles.
        """
        roles_cfg = rp_cfg.get("roles")
        if roles_cfg is None:
            print('no roles are configured')
            return
        if not isinstance(roles_cfg, list):
            raise TypeError("'roles' parameter must be a list")
        for role_cfg in roles_cfg:
            self.parse_role(role_cfg)

    def parse_role(self, role_cfg):
        """
        Parse a role.
        """
        if not isinstance(role_cfg, dict):
            raise TypeError('a role must be a map')
        name = role_cfg.get('name')
        if name is None:
            raise ValueError("'name' parameter is mandantory in a role")
        if not isinstance(name, str):
            raise TypeError("'name' parameter in a role must be a string")
        if name in self.roles:
            raise ValueError("duplicate roles named '" + name + "'")
        role = Role(name)
        accept_cfg = role_cfg.get('accept-commands')
        if accept_cfg is not None:
            ctx = " in 'accept-commands' of role '" + name + "'"
            self.parse_commands(accept_cfg, role.accepts, ctx)
        role.rejects = set()
        reject_cfg = role_cfg.get('reject-commands')
        if reject_cfg is not None:
            ctx = " in 'reject-commands' of role '" + name + "'"
            self.parse_commands(reject_cfg, role.rejects, ctx)
        others_cfg = role_cfg.get('others-commands')
        if others_cfg is not None:
            if not isinstance(others_cfg, bool):
                raise TypeError("'others-commands' in role '" + name +
                                "' must be a boolean")
            role.others = others_cfg
        preference = 'accept'
        preference_cfg = role_cfg.get('preference')
        if preference_cfg is not None:
            if not isinstance(preference_cfg, str):
                raise TypeError("'preference' in role '" + name +
                                "' must be a string")
            if preference_cfg in ('accept', 'reject'):
                preference = preference_cfg
            else:
                raise ValueError("unknown 'preference' (" + preference_cfg +
                                 ") in role '" + name + "': use 'accept' " +
                                 "or 'reject'")
        if preference == 'accept':
            for command in role.accepts:
                role.rejects.discard(command)
        else:
            for command in role.rejects:
                role.accepts.discard(command)
        filters_cfg = role_cfg.get('response-filters')
        self.parse_response_filters(role, filters_cfg, )
        user_cfg = role_cfg.get('basic-auth-user')
        if user_cfg is not None:
            if not isinstance(user_cfg, str):
                raise TypeError("'user' parameter in role '" + name +
                                "' must be a string")
            role.user = user_cfg
        password_cfg = role_cfg.get('basic-auth-password')
        if password_cfg is not None:
            if not isinstance(password_cfg, str):
                raise TypeError("'password' parameter in role '" + name +
                                "' must be a string")
            role.password = password_cfg
        self.roles[name] = role

    def parse_commands(self, cf_cfg, commands, ctx):
        """
        Parse command accept/reject configurations.
         - cf_cfg: what to parse.
         - commands: where to put result.
         - ctx: ctx for error messages.
        """
        parser = self.parse_command(cf_cfg, ctx)
        parser.apply(self.apis, commands)

    def parse_command(self, cf_cfg, ctx):
        """
        Parse a command accept/reject configuration.
        """
        if isinstance(cf_cfg, str):
            name = cf_cfg
        elif isinstance(cf_cfg, dict):
            if len(cf_cfg) != 1:
                raise ValueError("malformed entry" + ctx + ": " +
                                 json.dumps(cf_cfg, sort_keys=True))
            name = list(cf_cfg.keys())[0]
        else:
            raise ValueError("malformed entry" + ctx +
                             ": " + json.dumps(cf_cfg))
        parser = self.parsers.get(name)
        if parser is None:
            raise ValueError("unknown entry '" + name + "'" + ctx)
        return parser(cf_cfg, ctx)

    def parse_filter_access(self, cf_cfg, ctx):
        """
        Parse the access command filter.
        Format: singleton map with a member of accesses.
        """
        if not isinstance(cf_cfg, dict):
            raise TypeError("malformed entry 'access'" + ctx)
        access = cf_cfg['access']
        if access not in self.accesses:
            raise ValueError("unknown argument (" + access +
                             ") of 'access'" + ctx)
        return kea_command_accept.CommandFilterAccess(access)

    def parse_filter_commands(self, cf_cfg, ctx):
        """
        Parse the commands command filter.
        Format: singleton map with a list of members of apis.
        """
        if not isinstance(cf_cfg, dict):
            raise TypeError("malformed entry 'commands'" + ctx)
        commands = cf_cfg['commands']
        if not isinstance(commands, list):
            raise TypeError("malformed entry 'commands'" + ctx +
                            ": " + json.dumps(cf_cfg))
        for command in commands:
            if command not in self.apis:
                raise ValueError("unknown command (" + command +
                                 ") of 'commands'" + ctx)
        return kea_command_accept.CommandFilterCommands(commands)

    def parse_filter_hooks(self, cf_cfg, ctx):
        """
        Parse the hooks command filter.
        Format: singleton map with a list of members of hooks.
        """
        if not isinstance(cf_cfg, dict):
            raise TypeError("malformed entry 'hooks'" + ctx)
        hooks = cf_cfg['hooks']
        if not isinstance(hooks, list):
            raise TypeError("malformed entry 'hooks'" + ctx +
                            ": " + json.dumps(cf_cfg))
        for hook in hooks:
            if hook not in self.hooks:
                raise ValueError("unknown hook (" + hook + ") of " +
                                 "'hooks'" + ctx)
        return kea_command_accept.CommandFilterHooks(hooks)

    def parse_filter_not(self, cf_cfg, ctx):
        """
        Parse the not command filter.
        Format: singleton map with a filter.
        """
        if not isinstance(cf_cfg, dict):
            raise TypeError("malformed entry 'not'" + ctx)
        arg = cf_cfg['not']
        if not isinstance(arg, (str, dict)):
            raise TypeError("malformed entry 'not'" + ctx +
                            ": " + json.dumps(cf_cfg))
        parser = self.parse_command(arg, ctx)
        return kea_command_accept.CommandFilterNot(parser)

    def parse_filter_and(self, cf_cfg, ctx):
        """
        Parse the and command filter.
        Format: singleton map with a list of filters.
        """
        if not isinstance(cf_cfg, dict):
            raise TypeError("malformed entry 'and'" + ctx)
        args = cf_cfg['and']
        if not isinstance(args, list):
            raise TypeError("malformed entry 'and'" + ctx +
                            ": " + json.dumps(cf_cfg))
        branches = []
        for arg in args:
            branches.append(self.parse_command(arg, ctx))
        return kea_command_accept.CommandFilterAnd(branches)

    def parse_filter_or(self, cf_cfg, ctx):
        """
        Parse the or command filter.
        Format: singleton map with a list of filters.
        """
        if not isinstance(cf_cfg, dict):
            raise TypeError("malformed entry 'or'" + ctx)
        args = cf_cfg['or']
        if not isinstance(args, list):
            raise TypeError("malformed entry 'or'" + ctx +
                            ": " + json.dumps(cf_cfg))
        branches = []
        for arg in args:
            branches.append(self.parse_command(arg, ctx))
        return kea_command_accept.CommandFilterOr(branches)

    def parse_response_filters(self, role, filters_cfg):
        """
        Parse response filter configurations.
         - role: the role.
         - filters_cfg: what to parse (None or a list of filters).
        """
        if filters_cfg is not None:
            if not isinstance(filters_cfg, list):
                raise TypeError("'response-filters' in role '" + role.name +
                                "' must be a list")
            for filter_cfg in filters_cfg:
                if not isinstance(filter_cfg, str):
                    raise TypeError('a response filter (' +
                                    json.dumps(filter_cfg) +
                                    ') must be a string')
                factory = self.filters.get(filter_cfg)
                if factory is None:
                    raise ValueError("unsupported value '" + filter_cfg +
                                     "' for a response filter")
                role.filters.append(factory(role))

    def setup_assign_dict(self, name, parameter):
        """
        Setup an assign role object using a dictionary.
        If the parameter was given use it after check.
        Without a parameter use a {<role-name>, <role-name>} map.
        """
        if parameter is None:
            result = dict()
            for role in self.roles:
                result[role] = role
            self.assign.setup(result)
            return
        if not isinstance(parameter, dict):
            raise TypeError("'assign-role-parameter' parameter of '" + name +
                            "' must be a map")
        for key, value in parameter.items():
            if not isinstance(value, str):
                raise TypeError("'assign-role-parameter' value for '" +
                                key + "' key must be a string")
        self.assign.setup(parameter)

    def setup_assign_default(self, name, parameter):
        """
        Setup an assign role object using a name.
        If the parameter was given use it after check.
        Without a parameter use the default role.
        """
        if parameter is None:
            if self.default == '':
                raise ValueError("'" + name + "' requires " +
                                 "an 'assign-role-parameter' or " +
                                 "'default-role' parameter")
            self.assign.setup(self.default)
            return
        if not isinstance(parameter, str):
            raise TypeError("'assign-role-parameter' parameter of '" + name +
                            "' must be a string")
        self.assign.setup(parameter)

def create_assign_issuer():
    """
    Create the assign by issuer parameter.
    """
    return kea_role.RoleByIssuerCN()

def create_assign_subject():
    """
    Create the assign by subject parameter.
    """
    return kea_role.RoleBySubjectCN()

def create_assign_static():
    """
    Create the static assign parameter.
    """
    return kea_role.RoleStatic()

def parse_filter_all(cf_cfg, ctx):
    """
    Parse the all command filter.
    Format: string.
    """
    if not isinstance(cf_cfg, str):
        raise TypeError("malformed entry 'all'" + ctx)
    return kea_command_accept.CommandFilterAll()

def create_filter_list_commands(role):
    """
    Create the list-commands response filter.
    """
    return kea_response_filter.ResponseFilterListCommands(role.accepts,
                                                          role.rejects,
                                                          role.others)

def create_filter_config_control(*_):
    """
    Create the config-control response filter.
    """
    return kea_response_filter.ResponseFilterConfigControl()

def create_filter_redact_password(*_):
    """
    Create the redact-password response filter.
    """
    return kea_response_filter.ResponseFilterRedactPassword()

def command_accept(role, command):
    """
    Accept or reject a command.
    """
    filt = kea_command_accept.KeaReverseProxyCommandAccept(role.accepts,
                                                           role.rejects,
                                                           role.others)
    return filt.accept(command)

def response_filter(role, command, body):
    """
    Apply filters to a response.
    """
    for filt in role.filters:
        body = filt.filter(command, body)
    return body

def basic_auth(role):
    """
    Create basic HTTP authentication.
    """
    if role.password is not None:
        user = role.user.encode('utf-8')
        password = role.password.encode('utf-8')
        return HTTPBasicAuth(user, password)
    return None

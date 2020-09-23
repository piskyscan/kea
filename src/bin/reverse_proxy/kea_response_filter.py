# Copyright (C) 2020 Internet Systems Consortium, Inc. ("ISC")
#
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

"""
This file contains classes used for Reverse Proxy response filters.
"""

class KeaReverseProxyResponseFilter:
    """
    This class provides response filter.
    """

    @classmethod
    def name(cls):
        """
        Return response filter name.
        """
        return 'base'

    def filter(self, command, body):
        """
        Filter the response body to the command.
        """
        raise NotImplementedError()

class ResponseFilterListCommands(KeaReverseProxyResponseFilter):
    """
    Filter the response body to the list-commands command.
    """

    __slots__ = ['accepts', 'rejects', 'others']

    def __init__(self, accepts, rejects, others):
        """
        Initialize an instance.
         - accepts: set of accepted commands.
         - rejects: set of rejected commands.
         - others: True to accept commands not in accepts and rejects,
                   False otherwise.
         Don't check is accepts and rejects are disjoint.
        """
        self.accepts = accepts
        self.rejects = rejects
        self.others = others

    def name(self):
        """
        Return response filter name.
        """
        return 'list-commands'

    def filter(self, command, body):
        """
        Filter the response body to the list-commands command.
        Accept has the preference.
        """
        if command != 'list-commands':
            return body
        if not isinstance(body, list):
            return body
        if len(body) == 0:
            return body
        for elem in body:
            if not isinstance(elem, dict):
                continue
            arguments = elem.get('arguments')
            if not isinstance(arguments, list):
                continue
            keep = []
            for cmd_name in arguments:
                if not isinstance(cmd_name, str):
                    keep.append(cmd_name)
                    continue
                if cmd_name in self.accepts:
                    keep.append(cmd_name)
                    continue
                if cmd_name in self.rejects:
                    continue
                if self.others:
                    keep.append(cmd_name)
            elem['arguments'] = keep
        return body

class ResponseFilterConfigControl(KeaReverseProxyResponseFilter):
    """
    Remove config-control from a response with a configuration.
    """

    def name(self):
        """
        Return response filter name.
        """
        return 'config-control'

    def filter(self, command, body):
        """
        Remove config-control from a response with a DHCP server configuration.
        Do nothing when the command is not config-get.
        Handle configurations with both Dhcp4 and Dhcp6.
        """
        if command != 'config-get':
            return body
        if not isinstance(body, list):
            return body
        if len(body) == 0:
            return body
        for elem in body:
            if not isinstance(elem, dict):
                continue
            arguments = elem.get('arguments')
            if not isinstance(arguments, dict):
                continue
            dhcp4 = arguments.get('Dhcp4')
            if isinstance(dhcp4, dict):
                if 'config-control' in dhcp4:
                    del dhcp4['config-control']
            dhcp6 = arguments.get('Dhcp6')
            if isinstance(dhcp6, dict):
                if 'config-control' in dhcp6:
                    del dhcp6['config-control']
        return body

class ResponseFilterRedactPassword(KeaReverseProxyResponseFilter):
    """
    Redact all password entries in responses.
    """

    def name(self):
        """
        Return response filter name.
        """
        return 'redact-password'

    def filter(self, command, body):
        """
        Redact all password entries in a response.
        Ignore the command name.
        """
        name = 'password'
        replace = '*****'

        def chase(json):
            """
            Recursively chase passwords entries and replaces them.
            """
            if isinstance(json, list):
                for elem in json:
                    chase(elem)
            if not isinstance(json, dict):
                return
            candidate = json.get(name)
            if isinstance(candidate, str):
                json[name] = replace
            for elem  in json.values():
                chase(elem)

        chase(body)
        return body

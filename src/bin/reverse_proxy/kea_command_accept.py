# Copyright (C) 2020 Internet Systems Consortium, Inc. ("ISC")
#
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

"""
This file contains classes used for Reverse Proxy command accept/reject.
"""

class KeaReverseProxyCommandAccept:
    """
    This class provides command accept/reject.
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

    def accept(self, command):
        """
        Accept or reject a command. Accept has the preference.
        """
        if command in self.accepts:
            return True
        if command in self.rejects:
            return False
        return self.others

class CommandFilter:
    """
    Base class of command filters.
    """

    @classmethod
    def name(cls):
        """
        Return filter name.
        """
        return 'base'

    def filter(self, descr):
        """
        Return True if command matches, False otherwise.
        """
        raise NotImplementedError()

    def apply(self, apis, result):
        """
        Build the result set for apis.
        """
        for command, descr in apis.items():
            if self.filter(descr):
                result.add(command)

class CommandFilterAll(CommandFilter):
    """
    Command filter which matches all commands.
    """

    def name(self):
        return 'all'

    def filter(self, descr):
        """
        Return True for all commands.
        """
        return True

class CommandFilterAccess(CommandFilter):
    """
    Command filter which matches a given access.
    """

    __slots__ = ['access']

    def __init__(self, access):
        """
        Initialize an instance.
         - access: the access to match.
        """
        self.access = access

    def name(self):
        return 'access'

    def filter(self, descr):
        """
        Return True when access matches, False otherwise.
        """
        return descr.get('access') == self.access

class CommandFilterCommands(CommandFilter):
    """
    Command filter which matches a command list.
    """

    __slots__ = ['commands']

    def __init__(self, commands):
        """
        Initialize an instance.
         - commands: the list of commands to match.
        """
        self.commands = commands

    def name(self):
        return 'commands'

    def filter(self, descr):
        """
        Return True when command matches, False otherwise.
        """
        return descr.get('name') in self.commands

class CommandFilterHooks(CommandFilter):
    """
    Command filter which matches a given hook list.
    """

    __slots__ = ['hooks']

    def __init__(self, hooks):
        """
        Initialize an instance.
         - hooks: the list of hooks to match.
        """
        self.hooks = hooks

    def name(self):
        return 'hooks'

    def filter(self, descr):
        """
        Return True when hook matches, False otherwise.
        """
        hook = descr.get('hook')
        if hook is None:
            return False
        return hook in self.hooks

class CommandFilterNot(CommandFilter):
    """
    Command filter which negates a given filter.
    """

    __slots__ = ['arg']

    def __init__(self, arg):
        """
        Initialize an instance.
         - arg: the filter to negate.
        """
        self.arg = arg

    def name(self):
        return 'not(' + self.arg.name() + ')'

    def filter(self, descr):
        """
        Return True when arg does not matches, False otherwise.
        """
        return not self.arg.filter(descr)

class CommandFilterAnd(CommandFilter):
    """
    Command filter which matches a conjunction of filters.
    """

    __slots__ = ['args']

    def __init__(self, args):
        """
        Initialize an instance.
         - args: the filters to conjugate.
        """
        self.args = args

    def name(self):
        names = []
        for arg in self.args:
            names.append(arg.name())
        return 'and[' + ', '.join(names) + ']'

    def filter(self, descr):
        """
        Return True when all members of args match, False otherwise.
        """
        for arg in self.args:
            if not arg.filter(descr):
                return False
        return True

class CommandFilterOr(CommandFilter):
    """
    Command filter which matches a disjunction of filters.
    """

    __slots__ = ['args']

    def __init__(self, args):
        """
        Initialize an instance.
         - args: the filters to merge.
        """
        self.args = args

    def name(self):
        names = []
        for arg in self.args:
            names.append(arg.name())
        return 'or[' + ', '.join(names) + ']'

    def filter(self, descr):
        """
        Return True when a member of args matches, False otherwise.
        """
        for arg in self.args:
            if arg.filter(descr):
                return True
        return False

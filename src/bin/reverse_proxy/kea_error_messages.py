# Copyright (C) 2020 Internet Systems Consortium, Inc. ("ISC")
#
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

"""
This file contains the classes used for HTTP error messages.
"""

class HttpErrorMessageByCode:
    """
    This class provides a HTTP error message by code.
    """

    # class variable with the dictionary of HTTP error messages by code.
    messages_by_code = dict()

    __slots__ = ['code', 'message']

    def __init__(self, code, message):
        """
        Initialize an instance. Register the code and message.
        """
        self.code = code
        self.message = message
        HttpErrorMessageByCode.register(code, message)

    @classmethod
    def get(cls, code):
        """
        Class method returning the message from a code.
        """
        msg = cls.messages_by_code.get(code)
        if msg is None:
            msg = 'Unspecified Error'
        return msg

    @classmethod
    def register(cls, code, message):
        """
        Class method registering the message for a code.
        """
        cls.messages_by_code[code] = message

# Define them here so we can easily overwrite them for the application.

BAD_REQUEST = 400
FORBIDDEN = 403
REQUEST_TIMEOUT = 408
LENGTH_REQUIRED = 411
INTERNAL_SERVER_ERROR = 500
BAD_GATEWAY = 502
GATEWAY_TIMEOUT = 504
# not very standard
CONTROL_AGENT_DOWN = 521

# Register messages by code.

HttpErrorMessageByCode(BAD_REQUEST, 'Bad Request')
# 401 Unauthorized
# 402 Payment Required
HttpErrorMessageByCode(FORBIDDEN, 'Forbidden')
# 404 Not Found
# 405 Method Not Allowed
# 406 Not Acceptable
# 407 Proxy Authentication Required
HttpErrorMessageByCode(REQUEST_TIMEOUT, 'Request Timeout')
# 409 Conflict
# 410 Gone
HttpErrorMessageByCode(LENGTH_REQUIRED, 'Length Required')

HttpErrorMessageByCode(INTERNAL_SERVER_ERROR, 'Internal Server Error')
# 501 Not Implemented
HttpErrorMessageByCode(BAD_GATEWAY, 'Bad Gateway')
# 503 Service Unavailable
HttpErrorMessageByCode(GATEWAY_TIMEOUT, 'Gateway Timeout')
HttpErrorMessageByCode(CONTROL_AGENT_DOWN, 'Control Agent Down')

class HttpErrorMessage:
    """
    This class provides a HTTP error message.
    """

    # Default error
    default = BAD_REQUEST

    # class variable with the dictionary of HTTP error messages.
    messages = dict()

    __slots__ = ['error', 'code', 'message']

    def __init__(self, error, code):
        """
        Initialize an instance. Register the error, code and message.
        """
        self.error = error
        self.code = code
        HttpErrorMessage.register(error, code)

    @classmethod
    def get(cls, error):
        """
        Class method returning the code and message pair from an error.
        """
        pair = cls.messages.get(error)
        if pair is None:
            default = HttpErrorMessage.default
            pair = (default, HttpErrorMessageByCode.get(default))
        return pair

    @classmethod
    def register(cls, error, code):
        """
        Class method registering the code and message pair for an error.
        """
        cls.messages[error] = (code, HttpErrorMessageByCode.get(code))

# Register code and message pair by error.

HttpErrorMessage('drop on no role', FORBIDDEN)
HttpErrorMessage('drop on unknown role', FORBIDDEN)
HttpErrorMessage('no content-type', BAD_REQUEST)
HttpErrorMessage('bad content-type', BAD_REQUEST)
HttpErrorMessage('no Content-Length', LENGTH_REQUIRED)
HttpErrorMessage('bad request body', BAD_REQUEST)
HttpErrorMessage('query is not a dict', BAD_REQUEST)
HttpErrorMessage('no command in query', BAD_REQUEST)
HttpErrorMessage('command filtered out', FORBIDDEN)
HttpErrorMessage('request timeout', GATEWAY_TIMEOUT)
HttpErrorMessage('connection error', CONTROL_AGENT_DOWN)
HttpErrorMessage('bad response body', BAD_GATEWAY)
HttpErrorMessage('bad response', BAD_GATEWAY)

# Copyright (C) 2020 Internet Systems Consortium, Inc. ("ISC")
#
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

"""
This file contains classes used for Reverse Proxy roles.

Relies on _ssl.c _decode_certificate(X509 *certificate):
PyDict_New()
 - subject <- _create_tuple_for_X509_NAME(X509_get_subject_name(certificate))
 - issuer <- _create_tuple_for_X509_NAME(X509_get_issuer_name(certificate))
 - version <- X509_get_version(certificate) + 1
 - serialNumber <- X509_get_serialNumber(certificate)
 - notBefore <- X509_get_notBefore(certificate)
 - notAfter <- X509_get_notAfter(certificate)
 - subjectAltName <- _get_peer_alt_names(certificate)
 - OCSP <- _get_aia_uri(certificate, NID_ad_OCSP)
 - caIssuers <- _get_aia_uri(certificate, NID_ad_ca_issuers)
 - crlDistributionPoints <- _get_crl_dp(certificate)

with _create_tuple_for_X509_NAME(X509_NAME *xname):
PyList_New(0) (DN aka distinguished name)
 PyList_New(0)  PyList_AsTuple (RDN aka relative distinguished name)
  + _create_tuple_for_attribute(X509_NAME_ENTRY_get_object(entry),
                                X509_NAME_ENTRY_get_data(entry))

with _create_tuple_for_attribute(ASN1_OBJECT *name, ASN1_STRING *value):
 Py_BuildValue("Ns#", _asn1obj2py(name, 0), ASN1_STRING_to_UTF8(&value)...)

with _get_peer_alt_names:
X509_get_ext_d2i(certificate, NID_subject_alt_name)
Py_None or PyList_New(0)
 + ('DirName', _create_tuple_for_X509_NAME(dirn))
 + ('email', rfc822Name)
 + ('DNS', dNSName)
 + ('URI', uniformResourceIdentifier)
 + ('Registered ID', rid)
 + ('IP Address', ip)
 + ...

There is no way to translate directly a certificate into a python object,
a solution could be to get the DER and to parse it directly in python
using one of the x509 / asn1 / openssl packages.
"""

# import ssl

class KeaReverseProxyRole:
    """
    This class provides role assignment.
    """

    __slots__ = ['use_request_handler']

    def __init__(self):
        """
        Initialize an instance.
        use_request_handler default is False (i.e. use peer certificate).
        """
        self.use_request_handler = False

    @classmethod
    def name(cls):
        """
        Return class name.
        """
        return 'base'

    def assign(self, cert):
        """
        Assign a role according to the given peer certificate or when
        use_request_handler is True the request handler.
        """
        raise NotImplementedError()

class RoleByIssuerCN(KeaReverseProxyRole):
    """
    This class provides role assignment using the issuer common name.
    """

    __slots__ = ['issuers']

    def __init__(self):
        """
        Initialize an instance.
        """
        super().__init__()
        self.issuers = dict()

    def setup(self, issuers):
        """
        Setup an instance.
         - issuers: issuers { <common-name>: <role-name>} dictionary.
        """
        self.issuers = issuers

    def name(self):
        """
        Return class name.
        """
        return 'issuer'

    def assign(self, cert):
        """
        Assign a role according to the given peer certificate using
        the issuer commonName.
        """
        issuer = cert.get('issuer')
        if issuer is None:
            return None
        for rdn in issuer:
            try:
                key, value = rdn[0]
                if key == 'commonName':
                    return self.issuers.get(value)
            except ValueError:
                print('got ValueError')
        return None

class RoleBySubjectCN(KeaReverseProxyRole):
    """
    This class provides role assignment using subject common names.
    """

    __slots__ = ['subjects']

    def __init__(self):
        """
        Initialize an instance.
        """
        super().__init__()
        self.subjects = dict()

    def setup(self, subjects):
        """
        Setup an instance.
         - issuers: issuers { <common-name>: <role-name>} dictionary.
        """
        self.subjects = subjects

    def name(self):
        """
        Return class name.
        """
        return 'subject'

    def assign(self, cert):
        """
        Assign a role according to the given peer certificate using
        the subject commonName.
        """
        subject = cert.get('subject')
        if subject is None:
            return None
        for rdn in subject:
            try:
                key, value = rdn[0]
                if key == 'commonName':
                    return self.subjects.get(value)
            except ValueError:
                pass
        return None

class RoleStatic(KeaReverseProxyRole):
    """
    This class provides static role assignment.
    """

    __slots__ = ['role']

    def __init__(self):
        """
        Initialize an instance.
        """
        super().__init__()
        self.role = None

    def setup(self, role):
        """
        Setup an instance.
        """
        self.role = role

    def name(self):
        """
        Return class name.
        """
        return 'static'

    def assign(self, cert):
        """
        Assign a static role.
        """
        return self.role

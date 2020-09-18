..
   Copyright (C) 2020 Internet Systems Consortium, Inc. ("ISC")

   This Source Code Form is subject to the terms of the End User License
   Agreement. See COPYING file in the premium/ directory.


kea-reverse-proxy - Reverse-proxy with role-based access control for the kea control agent
------------------------------------------------------------------------------------------

Synopsis
~~~~~~~~

:program:`kea-reverse-proxy` [**-v**] [**-4**] [**-6**] [**-c** config-file] [**-t** config-file]

Description
~~~~~~~~~~~

The ``kea-reverse-proxy`` tool provides an HTTPS / HTTP reverse proxy
between clients and the Kea control agent with a role-based access
control.

Arguments
~~~~~~~~~

The arguments are as follows:

``-v``
   Display the version.

``-4``
   Use IPv4 (the default).

``-6``
   Use IPv6 (exclusive of ``-4``)

``-c``
   Configuration file including the configuration for reverse proxy.

``-t``
   Check the configuration file.

Documentation
~~~~~~~~~~~~~

Kea comes with an extensive Kea User's Guide documentation that covers
all aspects of running the Kea software - compilation, installation,
configuration, configuration examples and many more. Kea also features a
Kea Messages Manual, which lists all possible messages Kea can print
with a brief description for each of them. Both documents are typically
available in various formats (txt, html, pdf) with your Kea
distribution. The Kea documentation is available at
https://kb.isc.org/docs/kea-administrator-reference-manual .

Kea source code is documented in the Kea Developer's Guide. Its on-line
version is available at https://jenkins.isc.org/job/Kea_doc/doxygen/.

Kea project website is available at:
https://gitlab.isc.org/isc-projects/kea.

Mailing Lists and Support
~~~~~~~~~~~~~~~~~~~~~~~~~

There are two mailing lists available for Kea project. kea-users
(kea-users at lists.isc.org) is intended for Kea users, while kea-dev
(kea-dev at lists.isc.org) is intended for Kea developers, prospective
contributors and other advanced users. Both lists are available at
http://lists.isc.org. The community provides best effort type of support
on both of those lists.

ISC provides professional support for Kea services. See
https://www.isc.org/kea/ for details.

History
~~~~~~~

The ``kea-reverse-proxy`` was first coded in June 2020 by Francis Dupont.

See Also
~~~~~~~~

:manpage:`kea-ctrl-agent(8)`, :manpage:`kea-secure-shell(8)`,
:manpage:`kea-dhcp4(8)`, :manpage:`kea-dhcp6(8)`, :manpage:`kea-dhcp-ddns(8)`,
:manpage:`kea-admin(8)`, :manpage:`keactrl(8)`, :manpage:`perfdhcp(8)`,
:manpage:`kea-lfc(8)`, Kea Administrator's Guide.

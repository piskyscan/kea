// Copyright (C) 2012-2014 Internet Systems Consortium, Inc. ("ISC")
//
// Permission to use, copy, modify, and/or distribute this software for any
// purpose with or without fee is hereby granted, provided that the above
// copyright notice and this permission notice appear in all copies.
//
// THE SOFTWARE IS PROVIDED "AS IS" AND ISC DISCLAIMS ALL WARRANTIES WITH
// REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY
// AND FITNESS.  IN NO EVENT SHALL ISC BE LIABLE FOR ANY SPECIAL, DIRECT,
// INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
// LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE
// OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
// PERFORMANCE OF THIS SOFTWARE.

#ifndef DHCP6_MINI_LEASE_H
#define DHCP6_MINI_LEASE_H

#include <asiolink/io_address.h>
#include <dhcp/duid.h>

#include <boost/shared_ptr.hpp>
#include <string>
#include <vector>

namespace isc {
namespace dhcpMini {

/// @brief Unique identifier for a subnet (both v4 and v6)
///
/// Let's copy SubnetID definition from subnet.h. We can't include it directly,
/// because subnet.h needs Lease::Type, so it includes lease.h
typedef uint32_t SubnetID;

/// @brief a common structure for IPv6 leases
///
/// This structure holds all information that is common for IPv6 leases.
struct Lease {

	/// @brief Type of lease or pool
	typedef enum {
		TYPE_NA = 0, /// the lease contains non-temporary IPv6 address
		TYPE_TA = 1, /// the lease contains temporary IPv6 address
		TYPE_PD = 2, /// the lease contains IPv6 prefix (for prefix delegation)
	} Type;

    /// @brief returns text representation of a lease type
    /// @param type lease or pool type to be converted
    /// @return text decription
    static std::string typeToText(Type type);

    /// @brief Constructor
    ///
    /// @param addr IP address
    /// @param t1 renewal time
    /// @param t2 rebinding time
    /// @param valid_lft Lifetime of the lease
    /// @param subnet_id Subnet identification
    /// @param cltt Client last transmission time
	Lease(const isc::asiolink::IOAddress& addr, uint32_t t1, uint32_t t2,
			uint32_t valid_lft, SubnetID subnet_id, time_t cltt);

    /// @brief Destructor
    virtual ~Lease() {}

    /// @brief IP address
    ///
    /// IPv6 address
    isc::asiolink::IOAddress addr_;

    /// @brief Renewal timer
    ///
    /// Specifies renewal time. Although technically it is a property of the
    /// IA container and not the address itself, since our data model does not
    /// define a separate IA entity, we are keeping it in the lease. In the
    /// case of multiple addresses/prefixes for the same IA, each must have
    /// consistent T1 and T2 values. This is specified in seconds since cltt.
    uint32_t t1_;

    /// @brief Rebinding timer
    ///
    /// Specifies rebinding time. Although technically it is a property of the
    /// IA container and not the address itself, since our data model does not
    /// define a separate IA entity, we are keeping it in the lease. In the
    /// case of multiple addresses/prefixes for the same IA, each must have
    /// consistent T1 and T2 values. This is specified in seconds since cltt.
    uint32_t t2_;

    /// @brief Valid lifetime
    ///
    /// Expressed as number of seconds since cltt.
    uint32_t valid_lft_;

    /// @brief Client last transmission time
    ///
    /// Specifies a timestamp giving the time when the last transmission from a
    /// client was received.
    time_t cltt_;

    /// @brief Subnet identifier
    ///
    /// Specifies the identification of the subnet to which the lease belongs.
    SubnetID subnet_id_;

    /// @brief Fixed lease?
    ///
    /// Fixed leases are kept after they are released/expired.
    bool fixed_;

    /// @brief Convert Lease to Printable Form
    ///
    /// @return String form of the lease
    virtual std::string toText() const = 0;

    /// @brief returns true if the lease is expired
    /// @return true if the lease is expired
    bool expired() const;

};

/// @brief Structure that holds a lease for IPv6 address and/or prefix
///
/// For performance reasons it is a simple structure, not a class. If we chose
/// make it a class, all fields would have to made private and getters/setters
/// would be required. As this is a critical part of the code that will be used
/// extensively, direct access is warranted.
struct Lease6 : public Lease {

    /// @brief Lease type
    ///
    /// One of normal address, temporary address, or prefix.
    Type type_;

    /// @brief Identity Association Identifier (IAID)
    ///
    /// DHCPv6 stores all addresses and prefixes in IA containers (IA_NA,
    /// IA_TA, IA_PD). All containers may appear more than once in a message.
    /// To differentiate between them, the IAID field is present
    uint32_t iaid_;

    /// @brief Client identifier
    isc::dhcp::DuidPtr duid_;

    /// @brief preferred lifetime
    ///
    /// This parameter specifies the preferred lifetime since the lease was
    /// assigned or renewed (cltt), expressed in seconds.
    uint32_t preferred_lft_;

    /// @brief Constructor
    /// @param type Lease type.
    /// @param addr Assigned address.
    /// @param duid A pointer to an object representing DUID.
    /// @param iaid IAID.
    /// @param preferred Preferred lifetime.
    /// @param valid Valid lifetime.
    /// @param t1 A value of the T1 timer.
    /// @param t2 A value of the T2 timer.
    /// @param subnet_id A Subnet identifier.
	Lease6(Type type, const isc::asiolink::IOAddress& addr,
			isc::dhcp::DuidPtr duid, uint32_t iaid, uint32_t preferred,
			uint32_t valid, uint32_t t1, uint32_t t2, SubnetID subnet_id);

    /// @brief Constructor
    ///
    /// Initialize fields that don't have a default constructor.
	Lease6() :
			Lease(isc::asiolink::IOAddress("::"), 0, 0, 0, 0, 0), type_(TYPE_NA) {
	}

    /// @brief Returns a reference to a vector representing a DUID.
    ///
    /// @warning Since the function returns the reference to a vector (not a
    /// copy), the returned object should be used with caution because it will
    /// remain valid only for the period of time when an object which returned
    /// it exists.
    ///
    /// @return A reference to a vector holding a DUID.
    const std::vector<uint8_t>& getDuidVector() const;

    /// @brief Checks if two lease objects encapsulate the lease for the same
    /// client.
    ///
    /// This function compares address, type, prefix length, IAID and DUID
    /// parameters between two @c Lease6 objects. If these parameters match
    /// it is an indication that both objects describe the lease for the same
    /// client but apparently one is a result of renewal of the other. The
    /// special case of the matching lease is the one that is equal to another.
    ///
    /// @param other A lease to compare to.
    ///
    /// @return true if selected parameters of the two leases match.
    bool matches(const Lease6& other) const;

    /// @brief Compare two leases for equality
    ///
    /// @param other lease6 object with which to compare
    bool operator==(const Lease6& other) const;

    /// @brief Compare two leases for inequality
    ///
    /// @param other lease6 object with which to compare
    bool operator!=(const Lease6& other) const {
        return (!operator==(other));
    }

    /// @brief Convert Lease to Printable Form
    ///
    /// @return String form of the lease
    virtual std::string toText() const;
};

/// @brief Pointer to a Lease6 structure.
typedef boost::shared_ptr<Lease6> Lease6Ptr;

/// @brief Pointer to a const Lease6 structure.
typedef boost::shared_ptr<const Lease6> ConstLease6Ptr;

/// @brief A collection of IPv6 leases.
typedef std::vector<Lease6Ptr> Lease6Collection;

}; // end of isc::dhcpMini namespace
}; // end of isc namespace

#endif // DHCP6_MINI_LEASE_H

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

#ifndef DHCP6_MINI_POOL_H
#define DHCP6_MINI_POOL_H

#include <io_address.h>
#include <lease.h>

#include <boost/shared_ptr.hpp>
#include <vector>

namespace isc {
namespace dhcpMini {

/// @brief base class for Pool
///
/// Stores information about pool of IPv4 or IPv6 addresses.
/// That is a basic component of a configuration.
class Pool {
public:
	/// @note:
	/// PoolType enum was removed. Please use Lease::Type instead

	/// @brief returns Pool-id
	///
	/// @return pool-id value
	/// Pool-id is an unique value that can be used to identify a pool.
	uint32_t getId() const {
		return (id_);
	}

	/// @brief Returns the first address in a pool.
	///
	/// @return first address in a pool
	const isc::dhcpMini::IOAddress& getFirstAddress() const {
		return (first_);
	}

	/// @brief Returns the last address in a pool.
	/// @return last address in a pool
	const isc::dhcpMini::IOAddress& getLastAddress() const {
		return (last_);
	}

	/// @brief Checks if a given address is in the range.
	///
	/// @return true, if the address is in pool
	bool inRange(const isc::dhcpMini::IOAddress& addr) const;

	/// @brief Returns pool type (v4, v6 non-temporary, v6 temp, v6 prefix)
	/// @return returns pool type
	Lease::Type getType() const {
		return (type_);
	}

	/// @brief returns textual representation of the pool
	///
	/// @return textual representation
	virtual std::string toText() const;

	/// @brief virtual destructor
	///
	/// We need Pool to be a polymorphic class, so we could dynamic cast
	/// from PoolPtr to Pool6Ptr if we need to. A class becomes polymorphic,
	/// when there is at least one virtual method.
	virtual ~Pool() {
	}

protected:
	/// @brief protected constructor
	///
	/// This constructor is protected to prevent anyone from instantiating
	/// Pool class directly. Instances of Pool4 and Pool6 should be created
	/// instead.
	///
	/// @param type type of lease that will be served from this pool
	/// @param first first address of a range
	/// @param last last address of a range
	Pool(Lease::Type type, const isc::dhcpMini::IOAddress& first,
			const isc::dhcpMini::IOAddress& last);

	/// @brief returns the next unique Pool-ID
	///
	/// @return the next unique Pool-ID
	static uint32_t getNextID() {
		static uint32_t id = 0;
		return (id++);
	}

	/// @brief pool-id
	///
	/// This ID is used to identify this specific pool.
	uint32_t id_;

	/// @brief The first address in a pool
	isc::dhcpMini::IOAddress first_;

	/// @brief The last address in a pool
	isc::dhcpMini::IOAddress last_;

	/// @brief defines a lease type that will be served from this pool
	Lease::Type type_;
};

/// @brief Pool information for IPv6 addresses and prefixes
///
/// It holds information about pool6, i.e. a range of IPv6 address space that
/// is configured for DHCP allocation.
class Pool6 : public Pool {
public:

    /// @brief the constructor for Pool6 "min-max" style definition
    ///
    /// @throw BadValue if PD is define (PD can be only prefix/len)
    ///
    /// @param type type of the pool (IA or TA)
    /// @param first the first address in a pool
    /// @param last the last address in a pool
	Pool6(Lease::Type type, const isc::dhcpMini::IOAddress& first,
			const isc::dhcpMini::IOAddress& last);

    /// @brief the constructor for Pool6 "prefix/len" style definition
    ///
    /// For addressed, this is just a prefix/len definition.
    ///
    /// Naming convention:
    /// A smaller prefix length yields a shorter prefix which describes a larger
    /// set of addresses. A larger length yields a longer prefix which describes
    /// a smaller set of addresses.
    ///
    /// @param type type of the pool (IA, TA or PD)
    /// @param prefix specifies prefix of the pool
    /// @param prefix_len specifies prefix length of the pool
	Pool6(Lease::Type type, const isc::dhcpMini::IOAddress& prefix,
			uint8_t prefix_len);

    /// @brief returns pool type
    ///
    /// @return pool type
    Lease::Type getType() const {
        return (type_);
    }

    /// @brief returns textual representation of the pool
    ///
    /// @return textual representation
    virtual std::string toText() const;
};

/// @brief a pointer an IPv6 Pool
typedef boost::shared_ptr<Pool6> Pool6Ptr;

/// @brief a pointer to either IPv4 or IPv6 Pool
typedef boost::shared_ptr<Pool> PoolPtr;

/// @brief a container for either IPv4 or IPv6 Pools
typedef std::vector<PoolPtr> PoolCollection;

}; // end of isc::dhcpMini namespace
}; // end of isc namespace

#endif // DHCP6_MINI_POOL_H

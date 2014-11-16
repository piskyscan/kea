// Copyright (C) 2011, 2014  Internet Systems Consortium, Inc. ("ISC")
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

#ifndef DHCP6_MINI_IO_UTILITIES_H
#define DHCP6_MINI_IO_UTILITIES_H

#include <dhcp6-mini_log.h>

#include <cstddef>

namespace isc {
namespace dhcpMini {
/// \brief Read Unsigned 16-Bit Integer from Buffer
///
/// This is essentially a copy of the isc::util::InputBuffer::readUint16.  It
/// should really be moved into a separate library.
///
/// \param buffer Data buffer at least two bytes long of which the first two
///        bytes are assumed to represent a 16-bit integer in network-byte
///        order.
/// \param length Length of the data buffer.
///
/// \return Value of 16-bit integer
inline uint16_t readUint16(const void* buffer, size_t length) {
	if (length < sizeof(uint16_t)) {
		LOG(ERR) <<  "Length (" << length << ") of buffer is insufficient "
				<< "to read a uint16_t" << endl;
	}

	const uint8_t* byte_buffer = static_cast<const uint8_t*>(buffer);

	uint16_t result = (static_cast<uint16_t>(byte_buffer[0])) << 8;
	result |= static_cast<uint16_t>(byte_buffer[1]);

    return (result);
}

/// \brief Write Unisgned 16-Bit Integer to Buffer
///
/// This is essentially a copy of isc::util::OutputBuffer::writeUint16.  It
/// should really be moved into a separate library.
///
/// \param value 16-bit value to convert
/// \param buffer Data buffer at least two bytes long into which the 16-bit
///        value is written in network-byte order.
/// \param length Length of the data buffer.
///
/// \return pointer to the next byte after stored value
inline uint8_t* writeUint16(uint16_t value, void* buffer, size_t length) {
    if (length < sizeof(uint16_t)) {
        LOG(ERR) <<  "Length (" << length << ") of buffer is insufficient "
        		<< "to write a uint16_t" << endl;
    }

    uint8_t* byte_buffer = static_cast<uint8_t*>(buffer);

    byte_buffer[0] = static_cast<uint8_t>((value & 0xff00U) >> 8);
    byte_buffer[1] = static_cast<uint8_t>(value & 0x00ffU);

    return (byte_buffer + sizeof(uint16_t));
}

/// \brief Write Unisgned 32-Bit Integer to Buffer
///
/// \param value 32-bit value to convert
/// \param buffer Data buffer at least four bytes long into which the 32-bit
///        value is written in network-byte order.
/// \param length Length of the data buffer.
///
/// \return pointer to the next byte after stored value
inline uint8_t* writeUint32(uint32_t value, uint8_t* buffer, size_t length) {
    if (length < sizeof(uint32_t)) {
		LOG(ERR) << "Length (" << length << ") of buffer is insufficient "
				<< "to write a uint32_t" << endl;
    }

    uint8_t* byte_buffer = static_cast<uint8_t*>(buffer);

    byte_buffer[0] = static_cast<uint8_t>((value & 0xff000000U) >> 24);
    byte_buffer[1] = static_cast<uint8_t>((value & 0x00ff0000U) >> 16);
    byte_buffer[2] = static_cast<uint8_t>((value & 0x0000ff00U) >>  8);
    byte_buffer[3] = static_cast<uint8_t>((value & 0x000000ffU));

    return (byte_buffer + sizeof(uint32_t));
}

} // namespace dhcpMini
} // namespace isc

#endif // DHCP6_MINI_IO_UTILITIES_H

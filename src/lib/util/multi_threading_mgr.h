// Copyright (C) 2019-2020 Internet Systems Consortium, Inc. ("ISC")
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef MULTI_THREADING_MGR_H
#define MULTI_THREADING_MGR_H

#include <util/thread_pool.h>

#include <boost/noncopyable.hpp>

#include <stdint.h>

namespace isc {
namespace util {

/// @brief Multi Threading Manager.
///
/// This singleton class holds the multi-threading mode.
///
/// The standard way to use it is:
/// @code
/// if (MultiThreadingMgr::instance().getMode()) {
///     multi-threaded code
/// } else {
///     single-threaded code
/// }
/// @endcode
///
/// For instance for a class protected by its mutex:
/// @code
/// namespace locked {
///     void foo() { ... }
/// } // end of locked namespace
///
/// void foo() {
///     if (MultiThreadingMgr::instance().getMode()) {
///         lock_guard<mutex> lock(mutex_);
///         locked::foo();
///     } else {
///         locked::foo();
///     }
/// }
/// @endcode
class MultiThreadingMgr : public boost::noncopyable {
public:

    /// @brief Returns a single instance of Multi Threading Manager.
    ///
    /// MultiThreadingMgr is a singleton and this method is the only way
    /// of accessing it.
    ///
    /// @return the single instance.
    static MultiThreadingMgr& instance();

    /// @brief Get the multi-threading mode.
    ///
    /// @return The current multi-threading mode: true if multi-threading is
    /// enabled, false otherwise.
    bool getMode() const;

    /// @brief Set the multi-threading mode.
    ///
    /// @param enabled The new mode.
    void setMode(bool enabled);

    /// @brief Enter critical section.
    ///
    /// When entering @ref MultiThreadingCriticalSection, increment internal
    /// counter so that any configuration change that might start the packet
    /// thread pool is delayed until leaving the respective section.
    /// If the internal counter is 0, then stop the thread pool.
    void enterCriticalSection();

    /// @brief Exit critical section.
    ///
    /// When leaving @ref MultiThreadingCriticalSection, decrement internal
    /// counter so that the dhcp thread pool can be started according to the
    /// new configuration.
    /// If the internal counter is 0, then start the thread pool.
    void leaveCriticalSection();

    /// @brief Is in critical section flag.
    ///
    /// @return The critical section flag.
    bool isInCriticalSection() const;

    /// @brief Set read only config flag.
    ///
    /// Set read only config flag @ref readonly_config_ to specified value.
    ///
    /// @param value the read only config flag value.
    void setReadOnlyConfig(bool value);

    /// @brief Get read only config flag.
    ///
    /// Get read only config flag @ref readonly_config_ value.
    ///
    /// @return the read only config flag value.
    bool getReadOnlyConfig() const;

    /// @brief Get the dhcp thread pool.
    ///
    /// @return The dhcp thread pool.
    ThreadPool<std::function<void()>>& getThreadPool();

    /// @brief Get the configured dhcp thread pool size.
    ///
    /// @return The dhcp thread pool size.
    uint32_t getThreadPoolSize() const;

    /// @brief Set the configured dhcp thread pool size.
    ///
    /// @param size The dhcp thread pool size.
    void setThreadPoolSize(uint32_t size);

    /// @brief Get the configured dhcp packet queue size.
    ///
    /// @return The dhcp packet queue size.
    uint32_t getPacketQueueSize();

    /// @brief Set the configured dhcp packet queue size.
    ///
    /// @param size The dhcp packet queue size.
    void setPacketQueueSize(uint32_t size);

    /// @brief The system current detected hardware concurrency thread count.
    ///
    /// This function will return 0 if the value can not be determined.
    ///
    /// @return The thread count.
    static uint32_t detectThreadCount();

    /// @brief Apply the multi-threading related settings.
    ///
    /// @param enabled The enabled flag: true if multi-threading is enabled,
    /// false otherwise.
    /// @param thread_count The desired number of threads: non 0 if explicitly
    /// configured, 0 if auto scaling is desired.
    /// @param queue_size The desired thread queue size: non 0 if explicitly
    /// configured, 0 for unlimited size.
    void apply(bool enabled, uint32_t thread_count, uint32_t queue_size);

protected:

    /// @brief Constructor.
    MultiThreadingMgr();

    /// @brief Destructor.
    virtual ~MultiThreadingMgr();

private:

    /// @brief Class method stopping and joining all threads of the pool.
    ///
    /// Stop the dhcp thread pool if running.
    void stopPktProcessing();

    /// @brief Class method (re)starting threads of the pool.
    ///
    /// Start the dhcp thread pool according to current configuration.
    void startPktProcessing();

    /// @brief The current multi-threading mode.
    ///
    /// The multi-threading flag: true if multi-threading is enabled, false
    /// otherwise.
    bool enabled_;

    /// @brief The configured size of the dhcp thread pool.
    uint32_t thread_pool_size_;

    /// @brief Packet processing thread pool.
    ThreadPool<std::function<void()>> thread_pool_;

    /// @brief The read only config flag.
    ///
    /// Performing configuration changes should be handled only on the main
    /// thread while not processing dhcp traffic. This value must be set to
    /// false to indicate that it is safe to perform configuration changes or
    /// to true to indicate the opposite.
    /// By default this value is set to false, but it must be set to true right
    /// after applying configuration.
    bool readonly_config_;
};

/// @note: everything here MUST be used ONLY from the main thread.
///
/// @brief RAII class creating a generic critical section.
///
/// @note: this is useful to properly implement recursive critical sections.
///
/// The constructor calls @ref enter function and then increments the internal
/// counter @ref critical_section_count_. The destructor decrements the internal
/// counter @ref critical_section_count_ and calls @ref leave function.
/// The specialized class can check the value of the counter and perform
/// actions for specific values (e.g. only when it is 0, before it is
/// incremented, or only if it is 0, after it is decremented).
/// This is implemented as a generic class to be able to use the functionality
/// in all specializations, as virtual functions can not work when called from
/// the constructor or destructor of the base class.
/// Each specialization needs to implement @ref enter and @ref leave functions.
///
/// @tparam T is needed to generate a new implementation for each instantiation
/// so that the internal @ref critical_section_count_ is independent for each
/// generated type.
template <typename T>
class CriticalSectionBase : public boost::noncopyable {
public:

    /// @brief Constructor.
    ///
    /// Calls @ref enter and increments @ref critical_section_count_.
    CriticalSectionBase() {
        enter();
        ++critical_section_count_;
    }

    /// @brief Destructor.
    ///
    /// Decrements @ref critical_section_count_ and calls @ref leave.
    virtual ~CriticalSectionBase() {
        validate();
        --critical_section_count_;
        leave();
    }

    /// @brief Get critical section count
    ///
    /// @return The value of @ref critical_section_count_
    static uint32_t getCriticalSectionCount() {
        return (critical_section_count_);
    }

private:

    /// @brief Enter generic function to be called when entering a critical
    /// section.
    void enter();

    /// @brief Leave generic function to be called when leaving a critical
    /// section.
    void leave();

private:

    /// @brief Validate the internal value of @ref critical_section_count_.
    ///
    /// This method is needed to work around the compiler warning by throwing
    /// in the destructor (which of course should never happen).
    void validate() {
        if (critical_section_count_ == 0) {
            isc_throw(InvalidOperation, "invalid negative value for count");
        }
    }

    /// @brief The critical section count.
    ///
    /// This handles multiple recursive critical sections.
    static uint32_t critical_section_count_;
};

template <typename T>
uint32_t CriticalSectionBase<T>::critical_section_count_;

/// @brief Type used to define @ref MultiThreadingCriticalSectionBase as an
/// instantiation of @ref CriticalSectionBase.
struct ThreadSection {
};

/// @brief Type used to define @ref ConfigCriticalSectionBase as an
/// instantiation of @ref CriticalSectionBase.
struct ConfigSection {
};

/// @note: everything here MUST be used ONLY from the main thread.
///
/// @brief RAII class creating a base multi threading critical section which is
/// an instantiation of @ref CriticalSectionBase template class.
///
/// @note: the multi-threading mode MUST NOT be changed in the RAII
/// @c MultiThreadingCriticalSection body.
///
/// @note: this is mainly useful in hook commands which handle configuration
/// changes.
///
/// The constructor is called when entering the critical section.
/// The dhcp thread pool instance will be stopped so that all configuration
/// changes can be safely applied.
/// The destructor is called when leaving the critical section.
/// The dhcp thread pool instance will be started according to the new
/// configuration.
typedef CriticalSectionBase<ThreadSection> MultiThreadingCriticalSectionBase;

/// @note: everything here MUST be used ONLY from the main thread.
///
/// @brief RAII class creating a base configuration critical section which is an
/// instantiation of @ref CriticalSectionBase template class.
///
/// @note: starting and stopping the dhcp thread pool should be handled
/// in the main thread, before creating a configuration critical section.
/// @note: this is implemented by relying on derivation order:
/// @ref MultiThreadingCriticalSection derives:
/// @ref MultiThreadingCriticalSectionBase and then
/// @ref ConfigCriticalSectionBase.
///
/// @note: this is mainly useful to detect calling function which can change the
/// configuration while processing dhcp traffic (from hooks or from processing
/// threads). Changing the configuration while processing packets can lead to
/// inconsistent data in the packet, or ever crashes.
///
/// The constructor is called when entering the critical section. The read only
/// configuration flag will be unset.
/// The destructor is called when leaving the critical section. The read only
/// configuration flag will be set.
typedef CriticalSectionBase<ConfigSection> ConfigCriticalSectionBase;

/// @note: everything here MUST be used ONLY from the main thread.
///
/// @brief RAII class creating a multi threading critical section which
/// implements a base multi threading critical section and a base configuration
/// critical section.
///
/// @note: the order of derivation is important: first the base multi threading
/// critical section and last the base configuration critical section.
///
/// This will guarantee the proper call of all constructors:
/// @ref MultiThreadingCriticalSectionBase and then
/// @ref ConfigCriticalSectionBase (the read only configuration flag is unset
/// when all threads are stopped) and the proper call of all destructors:
/// @ref ~ConfigCriticalSectionBase and then
/// @ref ~MultiThreadingCriticalSectionBase (all threads are started when the
/// read only configuration flag is set).
class MultiThreadingCriticalSection : public MultiThreadingCriticalSectionBase,
                                      public ConfigCriticalSectionBase {
};

/// @note: everything here MUST be used ONLY from the main thread.
///
/// @brief RAII class creating a configuration critical section which implements
/// a base configuration critical section.
///
/// @note: this can be used when modifying the staging configuration and the
/// processing threads are never affected because the configuration is not
/// applied.
/// e.g.: when testing a configuration using commands or in related unittests.
class ConfigCriticalSection : public ConfigCriticalSectionBase {
};

/// @brief class which can be used to check and prevent functions to modify
/// server configuration while processing packets (even in single thread mode).
///
/// @note: this can be used to prevent not thread safe functions to be called on
/// processing threads.
class ReadOnlyConfigProbe {
public:

    /// @brief Constructor.
    ///
    /// Used to check if configuration changes are permitted in current scope.
    ///
    /// @throw @ref InvalidOperation if the configuration is read only.
    ReadOnlyConfigProbe() {
        if (MultiThreadingMgr::instance().getReadOnlyConfig()) {
            isc_throw(isc::InvalidOperation,
                      "configuration modification is not allowed.");
        }
    }
};

}  // namespace util
}  // namespace isc

#endif // MULTI_THREADING_MGR_H

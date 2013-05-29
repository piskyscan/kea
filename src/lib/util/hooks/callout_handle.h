// Copyright (C) 2013  Internet Systems Consortium, Inc. ("ISC")
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

#ifndef CALLOUT_HANDLE_H
#define CALLOUT_HANDLE_H

#include <exceptions/exceptions.h>

#include <boost/any.hpp>
#include <boost/shared_ptr.hpp>

#include <map>
#include <string>
#include <vector>

namespace isc {
namespace util {

/// @brief No such argument
///
/// Thrown if an attempt is made to use an invalid argument name.
class NoSuchArgument : public Exception {
public:
    NoSuchArgument(const char* file, size_t line, const char* what) :
        isc::Exception(file, line, what) {}
};

/// @brief No such callout context item
///
/// Thrown if an attempt is made to get an item of data from this callout's
/// context.
class NoSuchCalloutContext : public Exception {
public:
    NoSuchCalloutContext(const char* file, size_t line, const char* what) :
        isc::Exception(file, line, what) {}
};

//

// Forward declaration of the library handle and related collection classes.
class LibraryHandle;
class LibraryHandleCollection;

/// @brief Per-packet callout handle
///
/// An object of this class is associated with every packet (or request)
/// processed by the server.  It forms the principle means of passing 
/// data between the server and the user-library callouts.
///
/// The class allows access to the following information:
///
/// - Arguments.  When the callouts associated with a hook are called, they
///   are passed information by the server (and can return information to it)
///   through name/value pairs.  Each of these pairs is an argument and the
///   information is accessed through the {get,get}Argument() methods.
///
/// - Per-packet context.  Each packet has a context associated with it, this
///   context being a per-library context.  In other words, As a packet passes
///   through the callouts associated with a given library, the callouts can
///   associate and retrieve information with the packet.  The per-library
///   nature of the context means that the callouts within a given library can
///   pass packet-specific information between one another, they cannot pass
///   information to callous within another library.  Typically such context
///   is created in the "context_create" callout and destroyed in the
///   "context_destroy" callout.  The information is accessed through the
///   {get,set}Context() methods.
///
/// - Per-library context.  Each library has a context associated with it that
///   is independent of the packet.  All callouts in the library share the
///   information in the context, regardless of the packet being processed.
///   The context is typically createed in the library's "load()" method and
///   destroyed in its "unload()" method.  The information is available by
///   obtaining the handle to the library through this class's
///   getLibraryHandle() method, then calling {get,set}Context() on the
///   object returned.

class CalloutHandle {
public:

    /// Typedef to allow abbreviation of iterator specification in methods.
    /// The std::string is the argument name and the "boost::any" is the
    /// corresponding value associated with it.
    typedef std::map<std::string, boost::any> ElementCollection;

    /// Typedef to allow abbreviations in specifications when accessing
    /// context.  The ElementCollection is the name/value collection for
    /// a particular context.  The "int" corresponds to the index of an
    /// associated library handle - there is a 1:1 correspondence between
    /// library handles and a name.value collection.
    ///
    /// The collection of contexts is stored in a map, as not every library
    /// will require creating a context associated with each packet.  In
    /// addition, the structure is more flexible in that the size does not
    /// need to be set when the CalloutHandle is constructed.
    typedef std::map<int, ElementCollection> ContextCollection;


    /// @brief Constructor
    ///
    /// @param manager Pointer to the collection of library handles.
    CalloutHandle(boost::shared_ptr<LibraryHandleCollection>& collection)
        : arguments_(), context_collection_(), library_collection_(collection),
          skip_(false)
    {}

    /// @brief Set argument
    ///
    /// Sets an argument.  If the argument is already present, it is replaced.
    ///
    /// @param name Name of the element in the context to set
    /// @param value Value to set
    template <typename T>
    void setArgument(const std::string& name, T value) {
        arguments_[name] = value;
    }

    /// @brief Get argument
    ///
    /// Gets an argument.  If an argument of the given name does not exist,
    /// a "NoSuchArgument" exception is thrown.
    ///
    /// @param name Name of the element in the argument list to set.
    /// @param value [out] Value to set.  The type of "value" is important:
    ///        it must match the type of the value set.
    ///
    /// @throw NoSuchArgument Thrown if no argument with the name "name" is
    ///        present.
    /// @throw boost::bad_any_cast Thrown if the context element is present,
    ///        but the type of the element is not that expected
    template <typename T>
    void getArgument(const std::string& name, T& value) const {
        ElementCollection::const_iterator element_ptr = arguments_.find(name);
        if (element_ptr == arguments_.end()) {
            isc_throw(NoSuchArgument, "unable to find argument with name " <<
                      name);
        }

        value = boost::any_cast<T>(element_ptr->second);
    }
    
    /// @brief Get argument names
    ///
    /// Returns a vector holding the names of arguments in the argument
    /// vector.
    ///
    /// @return Vector of strings reflecting argument names
    std::vector<std::string> getArgumentNames() const;

    /// @brief Delete argument
    ///
    /// Deletes an argument of the given name.  If an argument of that name
    /// does not exist, the method is a no-op.
    ///
    /// N.B. If the element is a raw pointer, the pointed-to data is NOT deleted
    /// by this method.
    ///
    /// @param name Name of the element in the argument list to set.
    void deleteArgument(const std::string& name) {
        static_cast<void>(arguments_.erase(name));
    }

    /// @brief Delete all arguments
    ///
    /// Deletes all arguments associated with this context.
    ///
    /// N.B. If any elements are raw pointers, the pointed-to data is NOT
    /// deleted by this method.
    void deleteAllArguments() {
        arguments_.clear();
    }

    /// @brief Set skip flag
    ///
    /// Sets the "skip" variable in the callout handle.  This variable is
    /// interrogated by the server to see if the remaining callouts associated
    /// with the current hook should be bypassed.
    ///
    /// @param skip New value of the "skip" flag.
    void setSkip(bool skip) {
        skip_ = skip;
    }

    /// @brief Get skip flag
    ///
    /// Gets the current value of the "skip" flag.
    ///
    /// @return Current value of the skip flag.
    bool getSkip() const {
        return (skip_);
    }

    /// @brief Access current library handle
    ///
    /// Returns a reference to the current library handle.  This function is
    /// only available when called by a callout (which in turn is called
    /// through the "callCallouts" method), as it is only then that the current
    /// library index is valid.  A callout would use this method to get to
    /// the context associated with the library in which it resides.
    ///
    /// @return Reference to the current library handle
    ///
    /// @throw InvalidIndex thrown if this method is called outside of the
    ///        "callCallouts() method. (Exception is so-named because the
    ///        index used to access the library handle in the collection
    ///        is not valid at that point.)
    LibraryHandle& getLibraryHandle() const;

    /// @brief Set context
    ///
    /// Sets an element in the context associated with the current library.  If
    /// an element of the name is already present, it is replaced.
    ///
    /// @param name Name of the element in the context to set
    /// @param value Value to set
    template <typename T>
    void setContext(const std::string& name, T value) {
        getContextForLibrary()[name] = value;
    }

    /// @brief Get context
    ///
    /// Gets an element from the context associated with the current library.
    ///
    /// @param name Name of the element in the context to get.
    /// @param value [out] Value to set.  The type of "value" is important:
    ///        it must match the type of the value set.
    ///
    /// @throw NoSuchCalloutContext Thrown if no context element with the name
    ///        "name" is present.
    /// @throw boost::bad_any_cast Thrown if the context element is present,
    ///        but the type of the element is not that expected
    template <typename T>
    void getContext(const std::string& name, T& value) const {
        const ElementCollection& lib_context = getContextForLibrary();

        ElementCollection::const_iterator element_ptr = lib_context.find(name);
        if (element_ptr == lib_context.end()) {
            isc_throw(NoSuchCalloutContext, "unable to find callout context "
                      "item " << name << " in the context associated with "
                      "current library handle");
        }

        value = boost::any_cast<T>(element_ptr->second);
    }
    
    /// @brief Get context names
    ///
    /// Returns a vector holding the names of items in the context associated
    /// with the current library.
    ///
    /// @return Vector of strings reflecting argument names
    std::vector<std::string> getContextNames() const;

    /// @brief Delete context element
    ///
    /// Deletes an item of the given name from the context associated with the
    /// current library.  If an item  of that name does not exist, the method is
    /// a no-op.
    ///
    /// N.B. If the element is a raw pointer, the pointed-to data is NOT deleted
    /// by this.
    ///
    /// @param name Name of the element in the argument list to set.
    void deleteContext(const std::string& name) {
        static_cast<void>(getContextForLibrary().erase(name));
    }

    /// @brief Delete all context items
    ///
    /// Deletes all items from the context associated with the current library.
    ///
    /// N.B. If any elements are raw pointers, the pointed-to data is NOT
    /// deleted by this.
    void deleteAllContext() {
        getContextForLibrary().clear();
    }


private:
    /// @brief Check index
    ///
    /// Gets the current library index, throwing an exception if it is not set
    /// or is invalid for the current library collection.
    ///
    /// @return Current library index, valid for this library collection.
    int getLibraryIndex() const;

    /// @brief Return reference to context for current library
    ///
    /// Called by all context-accessing functions, this checks if the current
    /// library index is valid (throwing an exception if not).  If it is, it
    /// returns a reference to the appropriate context, creating one if it does
    /// not exist. (The last task allows the list of library handles to grow
    /// dynamically - although only if handles are appended to the end of the
    /// library handle collection.)
    ///
    /// @return Reference to the collection of name/value pairs associated
    ///         with the current library.
    ElementCollection& getContextForLibrary();

    /// @brief Return reference to context for current library (const version)
    ///
    /// Called by all context-accessing functions, this checks if the current
    /// library index is valid (throwing an exception if not).  If it is, it
    /// returns a reference to the appropriate context, creating one if it does
    /// not exist. (The last task allows the list of library handles to grow
    /// dynamically - although only if handles are appended to the end of the
    /// library handle collection.)
    ///
    /// @return Reference to the collection of name/value pairs associated
    ///         with the current library.
    ///
    /// @throw NoSuchCalloutContext Thrown if there is no ElementCollection
    ///        associated with the current library.
    const ElementCollection& getContextForLibrary() const;

    // Member variables

    /// Collection of arguments passed to the callouts
    ElementCollection arguments_;

    /// Context collection - there is one entry per library context.
    ContextCollection context_collection_;

    /// Library handle collection, used to obtain the correct library handle
    /// during a call to a callout.
    boost::shared_ptr<LibraryHandleCollection> library_collection_;

    /// "Skip" flag, indicating if the caller should bypass remaining callouts.
    bool skip_;
};

} // namespace util
} // namespace isc


#endif // CALLOUT_HANDLE_H

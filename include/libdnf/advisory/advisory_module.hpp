// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project


#ifndef LIBDNF_ADVISORY_ADVISORY_MODULE_HPP
#define LIBDNF_ADVISORY_ADVISORY_MODULE_HPP

#include "libdnf/advisory/advisory.hpp"

#include <memory>

namespace libdnf::advisory {

class AdvisoryModule {
public:
    AdvisoryModule(const AdvisoryModule & src);
    AdvisoryModule(AdvisoryModule && src);

    AdvisoryModule & operator=(const AdvisoryModule & src);
    AdvisoryModule & operator=(AdvisoryModule && src) noexcept;
    ~AdvisoryModule();

    /// Get name of this AdvisoryModule.
    ///
    /// @return Name of this AdvisoryModule as std::string.
    std::string get_name() const;

    /// Get stream of this AdvisoryModule.
    ///
    /// @return Stream of this AdvisoryModule as std::string.
    std::string get_stream() const;

    /// Get version of this AdvisoryModule.
    ///
    /// @return Version of this AdvisoryModule as std::string.
    std::string get_version() const;

    /// Get context of this AdvisoryModule.
    ///
    /// @return Context of this AdvisoryModule as std::string.
    std::string get_context() const;

    /// Get arch of this AdvisoryModule.
    ///
    /// @return Arch of this AdvisoryModule as std::string.
    std::string get_arch() const;

    /// Get AdvisoryId of Advisory this AdvisoryModule belongs to.
    ///
    /// @return AdvisoryId of this AdvisoryModule.
    AdvisoryId get_advisory_id() const;

    //TODO(amatej): we should be able to get Advisory and AdvisoryCollection information
    //              from a advisory module, returning new advisory object is one option but it might
    //              be better to set up so sort of inheritance among Advisory, AdvisoryCollection,
    //              AdvisoryPacakge and AdvisoryModule.
    /// Get Advisory this AdvisoryModule belongs to.
    ///
    /// @return newly construted Advisory object of this AdvisoryModule.
    Advisory get_advisory() const;
    /// Get AdvisoryCollection this AdvisoryModule belongs to.
    ///
    /// @return newly construted AdvisoryCollection object of this AdvisoryModule.
    AdvisoryCollection get_advisory_collection() const;


private:
    friend class AdvisoryCollection;

    class Impl;

    AdvisoryModule(Impl * private_module);
    std::unique_ptr<Impl> p_impl;
};

}  // namespace libdnf::advisory

#endif

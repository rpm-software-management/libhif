// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project


#ifndef LIBDNF_CONF_OPTION_PATH_HPP
#define LIBDNF_CONF_OPTION_PATH_HPP

#include "option_string.hpp"

namespace libdnf {

/// Option that stores file/directory path.
/// Support default value, and path verification (absolute, existence).
/// @replaces libdnf:conf/OptionPath.hpp:class:OptionPath
class OptionPath : public OptionString {
public:
    /// Exception that is generated when not allowed input value is detected.
    class NotAllowedValue : public InvalidValue {
    public:
        using InvalidValue::InvalidValue;
        const char * get_domain_name() const noexcept override { return "libdnf::OptionPath"; }
        const char * get_name() const noexcept override { return "NotAllowedValue"; }
        const char * get_description() const noexcept override { return "Not allowed value"; }
    };

    /// Exception that is generated when input path does not exist.
    class PathNotExists : public InvalidValue {
    public:
        using InvalidValue::InvalidValue;
        const char * get_domain_name() const noexcept override { return "libdnf::OptionPath"; }
        const char * get_name() const noexcept override { return "PathNotExists"; }
        const char * get_description() const noexcept override { return "Path does not exist"; }
    };

    /// Constructor sets default value and conditons.
    /// @replaces libdnf:conf/OptionPath.hpp:ctor:OptionPath.OptionPath(const std::string & defaultValue, bool exists = false, bool absPath = false)
    explicit OptionPath(const std::string & default_value, bool exists = false, bool abs_path = false);

    /// Constructor sets default value and conditons.
    /// @replaces libdnf:conf/OptionPath.hpp:ctor:OptionPath.OptionPath(const char * defaultValue, bool exists = false, bool absPath = false)
    explicit OptionPath(const char * default_value, bool exists = false, bool abs_path = false);

    /// Constructor sets default value and conditons.
    /// @replaces libdnf:conf/OptionPath.hpp:ctor:OptionPath.OptionPath(const std::string & defaultValue, const std::string & regex, bool icase, bool exists = false, bool absPath = false)
    OptionPath(
        const std::string & default_value,
        const std::string & regex,
        bool icase,
        bool exists = false,
        bool abs_path = false);

    /// Constructor sets default value and conditons.
    /// @replaces libdnf:conf/OptionPath.hpp:ctor:OptionPath.OptionPath(const char * defaultValue, const std::string & regex, bool icase, bool exists = false, bool absPath = false)
    OptionPath(
        const char * default_value, const std::string & regex, bool icase, bool exists = false, bool abs_path = false);

    /// Makes copy (clone) of this object.
    /// @replaces libdnf:conf/OptionPath.hpp:method:OptionPath.clone()
    OptionPath * clone() const override;

    /// Parses input string and sets new value and priority.
    /// According setting passed in constructor it can verify that the path is absolute, exists and match regex.
    /// The value and priority are stored only if the new priority is equal to or higher than the stored priority.
    /// @replaces libdnf:conf/OptionPath.hpp:method:OptionPath.set(Priority priority, const std::string & value)
    void set(Priority priority, const std::string & value) override;

    /// Tests input value and throws exception if the value is not allowed.
    /// @replaces libdnf:conf/OptionPath.hpp:method:OptionPath.test(const std::string & value)
    void test(const std::string & value) const;

private:
    bool exists;
    bool abs_path;
};

inline OptionPath * OptionPath::clone() const {
    return new OptionPath(*this);
}

}  // namespace libdnf

#endif

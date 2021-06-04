// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project


#ifndef LIBDNF_CONF_OPTION_ENUM_HPP
#define LIBDNF_CONF_OPTION_ENUM_HPP

#include "option.hpp"

#include <functional>
#include <vector>

namespace libdnf {

/// Option that stores value from enumeration. The type of value is template parameter.
/// Support default value and user defined function for conversion from string.
/// @replaces libdnf:conf/OptionEnum.hpp:class:OptionEnum<T>
template <typename T>
class OptionEnum : public Option {
public:
    using ValueType = T;
    using FromStringFunc = std::function<ValueType(const std::string &)>;

    /// Exception that is generated when an invalid input value is detected.
    class InvalidValue : public Option::InvalidValue {
    public:
        using Option::InvalidValue::InvalidValue;
        const char * get_domain_name() const noexcept override { return "libdnf::OptionEnum"; }
    };

    /// Exception that is generated when not allowed input value is detected.
    class NotAllowedValue : public InvalidValue {
    public:
        using InvalidValue::InvalidValue;
        const char * get_name() const noexcept override { return "NotAllowedValue"; }
        const char * get_description() const noexcept override { return "Not allowed value"; }
    };

    OptionEnum(ValueType default_value, const std::vector<ValueType> & enum_vals);
    OptionEnum(ValueType default_value, std::vector<ValueType> && enum_vals);
    OptionEnum(ValueType default_value, const std::vector<ValueType> & enum_vals, FromStringFunc && from_string_func);
    OptionEnum(ValueType default_value, std::vector<ValueType> && enum_vals, FromStringFunc && from_string_func);

    /// Makes copy (clone) of this object.
    /// @replaces libdnf:conf/OptionEnum.hpp:method:OptionEnum<T>.clone()
    OptionEnum * clone() const override;

    /// Sets new value and priority (source).
    /// The value and priority are stored only if the new priority is equal to or higher than the stored priority.
    /// @replaces libdnf:conf/OptionEnum.hpp:method:OptionEnum<T>.set(Priority priority, bool value)
    void set(Priority priority, ValueType value);

    /// Parses input string and sets new value and priority.
    /// The value and priority are stored only if the new priority is equal to or higher than the stored priority.
    /// @replaces libdnf:conf/OptionEnum.hpp:method:OptionEnum<T>.set(Priority priority, const std::string & value)
    void set(Priority priority, const std::string & value) override;

    /// Gets the stored value.
    /// @replaces libdnf:conf/OptionEnum.hpp:method:OptionEnum<T>.getValue()
    T get_value() const;

    /// Gets the default value. Default value is used until it is replaced by set() method.
    /// @replaces libdnf:conf/OptionEnum.hpp:method:OptionEnum<T>.getValueString()
    T get_default_value() const;

    /// Gets a string representation of the stored value.
    /// @replaces libdnf:conf/OptionEnum.hpp:method:OptionEnum<T>.getValueString()
    std::string get_value_string() const override;

    /// Tests input value and throws exception if the value is not allowed.
    /// @replaces libdnf:conf/OptionEnum.hpp:method:OptionEnum<T>.test(ValueType value)
    void test(ValueType value) const;

    /// Parses input string and returns result.
    /// @replaces libdnf:conf/OptionEnum.hpp:method:OptionEnum<T>.fromString(const std::string & value)
    ValueType from_string(const std::string & value) const;

    /// Converts input value to the string.
    /// @replaces libdnf:conf/OptionEnum.hpp:method:OptionEnum<T>.toString(ValueType value)
    std::string to_string(ValueType value) const;

private:
    FromStringFunc from_string_user;
    std::vector<ValueType> enum_vals;
    ValueType default_value;
    ValueType value;
};


/// Option that stores value from enumeration. Specialized template for enumeration of strings.
/// It supports default value.
/// It supports user defined function for conversion from string.
/// @replaces libdnf:conf/OptionEnum.hpp:class:OptionEnum<std::string>
template <>
class OptionEnum<std::string> : public Option {
public:
    using ValueType = std::string;
    using FromStringFunc = std::function<ValueType(const std::string &)>;

    class InvalidValue : public Option::InvalidValue {
    public:
        using Option::InvalidValue::InvalidValue;
        const char * get_domain_name() const noexcept override { return "libdnf::OptionEnum"; }
    };

    class NotAllowedValue : public InvalidValue {
    public:
        using InvalidValue::InvalidValue;
        const char * get_name() const noexcept override { return "NotAllowedValue"; }
    };

    OptionEnum(const std::string & default_value, std::vector<ValueType> enum_vals);
    OptionEnum(const std::string & default_value, std::vector<ValueType> enum_vals, FromStringFunc && from_string_func);

    /// Makes copy (clone) of this object.
    /// @replaces libdnf:conf/OptionEnum.hpp:method:OptionEnum<std::string>.clone()
    OptionEnum * clone() const override;

    /// Parses input string and sets new value and priority.
    /// The value and priority are stored only if the new priority is equal to or higher than the stored priority.
    /// @replaces libdnf:conf/OptionEnum.hpp:method:OptionEnum<std::string>.set(Priority priority, const std::string & value)
    void set(Priority priority, const std::string & value) override;

    /// Gets the stored value.
    /// @replaces libdnf:conf/OptionEnum.hpp:method:OptionEnum<std::string>.getValue()
    const std::string & get_value() const;

    /// Gets the default value. Default value is used until it is replaced by set() method.
    /// @replaces libdnf:conf/OptionEnum.hpp:method:OptionEnum<std::string>.getValueString()
    const std::string & get_default_value() const;

    /// Gets a string representation of the stored value.
    /// @replaces libdnf:conf/OptionEnum.hpp:method:OptionEnum<std::string>.getValueString()
    std::string get_value_string() const override;

    /// Tests input value and throws exception if the value is not allowed.
    /// @replaces libdnf:conf/OptionEnum.hpp:method:OptionEnum<std::string>.test(const std::string & value)
    void test(const std::string & value) const;

    /// Parses input string and returns result.
    /// @replaces libdnf:conf/OptionEnum.hpp:method:OptionEnum<std::string>.fromString(const std::string & value)
    std::string from_string(const std::string & value) const;

private:
    FromStringFunc from_string_user;
    std::vector<ValueType> enum_vals;
    ValueType default_value;
    ValueType value;
};

template <typename T>
inline OptionEnum<T> * OptionEnum<T>::clone() const {
    return new OptionEnum<T>(*this);
}

inline OptionEnum<std::string> * OptionEnum<std::string>::clone() const {
    return new OptionEnum<std::string>(*this);
}

inline const std::string & OptionEnum<std::string>::get_value() const {
    return value;
}

inline const std::string & OptionEnum<std::string>::get_default_value() const {
    return default_value;
}

inline std::string OptionEnum<std::string>::get_value_string() const {
    return value;
}

}  // namespace libdnf

#endif

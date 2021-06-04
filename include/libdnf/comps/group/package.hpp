// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project



#ifndef LIBDNF_COMPS_GROUP_PACKAGE_HPP
#define LIBDNF_COMPS_GROUP_PACKAGE_HPP


#include <string>


namespace libdnf::comps {


enum class PackageType {MANDATORY, DEFAULT, OPTIONAL, CONDITIONAL};

// TODO(dmach): isn't it more a package dependency rather than a package?


/// @replaces dnf:dnf/comps.py:class:Package
/// @replaces dnf:dnf/comps.py:class:CompsTransPkg
class Package {
public:
    explicit Package(std::string name, PackageType type, std::string condition) : name(name), type(type), condition(condition) {}

    /// @replaces dnf:dnf/comps.py:attribute:Package.name
    std::string get_name() const { return name; }
    void set_name(const std::string & value) { name = value; }

    PackageType get_type() const { return type; }
    void set_type(PackageType & value) { type = value; }

    std::string get_type_string() const { 
        switch (type) {
            case PackageType::MANDATORY:
                return "mandatory";
            case PackageType::DEFAULT:
                return "default";
            case PackageType::OPTIONAL:
                return "optional";
            case PackageType::CONDITIONAL:
                return "conditional";
        }
        return "";
    }

    std::string get_condition() const { return condition; }
    void set_condition(const std::string & value) { condition = value; }

private:
    std::string name;
    PackageType type;
    // Used only for CONDITIONAL packages
    std::string condition;
};


}  // namespace libdnf::comps



//dnf:dnf/comps.py:attribute:Package.option_type


#endif

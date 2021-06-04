// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project



#ifndef LIBDNF_COMPS_GROUP_GROUP_HPP
#define LIBDNF_COMPS_GROUP_GROUP_HPP

#include "libdnf/comps/group/package.hpp"
#include "libdnf/common/weak_ptr.hpp"

#include <set>
#include <string>
#include <vector>


namespace libdnf::comps {


struct GroupId {
public:
    GroupId() = default;
    explicit GroupId(int id);

    bool operator==(const GroupId & other) const noexcept { return id == other.id; };
    bool operator!=(const GroupId & other) const noexcept { return id != other.id; };

    // Corresponds to solvable id
    int id{0};
};


inline GroupId::GroupId(int id) : id(id) {}


class GroupQuery;
using GroupQueryWeakPtr = WeakPtr<GroupQuery, false>;


/// @replaces dnf:dnf/comps.py:class:Group
class Group {
public:
    ~Group();

    const std::vector<GroupId> & get_group_ids() const { return group_ids; }
    void add_group_id(GroupId group_id) { group_ids.push_back(group_id); }

    /// Get group id
    std::string get_groupid() const;

    /// Get group name
    std::string get_name() const;

    /// Get group description
    std::string get_description() const;

    /// Get translated name of a group based on current locales.
    /// If a translation is not found, return untranslated name.
    ///
    /// @replaces dnf:dnf/comps.py:attribute:Group.ui_name
    std::string get_translated_name(const char * lang) const;
    std::string get_translated_name() const;

    /// Get translated description of a group based on current locales.
    /// If a translation is not found, return untranslated description.
    ///
    /// @replaces dnf:dnf/comps.py:attribute:Group.ui_description
    std::string get_translated_description(const char * lang) const;
    std::string get_translated_description() const;

    /// Get group order
    std::string get_order() const;

    /// Get group langonly
    std::string get_langonly() const;

    /// Determine if group is visible to the users
    ///
    /// @replaces dnf:dnf/comps.py:attribute:Group.visible
    bool get_uservisible() const;

    /// Determine if group is installed by default
    bool get_default() const;

    /// @replaces dnf:dnf/comps.py:method:Group.packages_iter(self)
    std::vector<Package> get_packages();

    /// @replaces dnf:dnf/comps.py:attribute:Group.conditional_packages
    /// @replaces dnf:dnf/comps.py:attribute:Group.default_packages
    /// @replaces dnf:dnf/comps.py:attribute:Group.mandatory_packages
    /// @replaces dnf:dnf/comps.py:attribute:Group.optional_packages
    std::vector<Package> get_packages_of_type(PackageType type);

    std::set<std::string> get_repos() const;

    /// Determine if group is installed
    /// If it belongs to the @System repo, return true
    bool get_installed() const;

    /// Merge a comps Group with another one
    Group & operator+=(const Group & rhs);

    bool operator<(const Group & rhs) const {
        return get_groupid() < rhs.get_groupid() or (get_installed() and !rhs.get_installed());
    }

    void dump(const std::string & path);

protected:
    explicit Group(GroupQuery * query);

private:
    GroupQueryWeakPtr query;

    // Corresponds to solvable ids for this group (libsolv doesn't merge groups,
    // so there are multiple solvables for one groupid)
    std::vector<GroupId> group_ids;

    std::vector<Package> packages;

    friend class GroupSack;
    friend class GroupQuery;
};


}  // namespace libdnf::comps


#endif  // LIBDNF_COMPS_GROUP_GROUP_HPP

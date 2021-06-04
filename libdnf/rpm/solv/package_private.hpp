// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project


#ifndef LIBDNF_RPM_PACKAGE_PRIVATE_HPP
#define LIBDNF_RPM_PACKAGE_PRIVATE_HPP


#include "id_queue.hpp"
#include "solv_private.hpp"

#include "libdnf/rpm/package.hpp"

extern "C" {
#include <solv/pool.h>
#include <solv/pooltypes.h>
#include <solv/repo.h>
#include <solv/solvable.h>
#include <solv/util.h>
}

#include <cassert>
#include <cstdlib>
#include <string>
#include <vector>

namespace libdnf::rpm::solv {

constexpr const char * BASE_EPOCH = "0";

inline Solvable * get_solvable(Pool * pool, Id package_id) noexcept {
    // Check that libsolv Id is identical with internal structure of PackageId
    static_assert(
        std::is_same<decltype(PackageId::id), ::Id>::value,
        "libdnf PackageId::id type differs from libsolv internal Id type");

    return pool_id2solvable(pool, package_id);
}

inline libdnf::rpm::PackageId get_package_id(Pool * pool, Solvable * solvable) {
    return libdnf::rpm::PackageId(pool_solvable2id(pool, solvable));
}

static char * pool_tmpdup(Pool * pool, const char * s) {
    char * dup = pool_alloctmpspace(pool, static_cast<int>(strlen(s)) + 1);
    return strcpy(dup, s);
}

/// Split evr into its components.
///
/// Believes blindly in 'evr' being well formed. This could be implemented
/// without 'pool' of course but either the caller would have to provide buffers
/// to store the split pieces, or this would call strdup (which is more expensive
/// than the pool temp space).
inline void pool_split_evr(Pool * pool, const char * evr_c, char ** epoch, char ** version, char ** release) {
    char * evr = pool_tmpdup(pool, evr_c);
    char * e;
    char * v;
    char * r;

    for (e = evr + 1; *e != ':' && *e != '-' && *e != '\0'; ++e) {
        ;
    }

    if (*e == '-') {
        *e = '\0';
        v = evr;
        r = e + 1;
        e = nullptr;
    } else if (*e == '\0') {
        v = evr;
        e = nullptr;
        r = nullptr;
    } else {  // *e == ':'
        *e = '\0';
        v = e + 1;
        e = evr;
        for (r = v + 1; *r != '-'; ++r) {
            assert(*r);
        }
        *r = '\0';
        r++;
    }
    *epoch = e;
    *version = v;
    *release = r;
}

static inline unsigned long long lookup_num(Solvable * solvable, Id type) {
    SolvPrivate::internalize_libsolv_repo(solvable->repo);
    return solvable_lookup_num(solvable, type, 0);
}

static inline const char * lookup_cstring(Solvable * solvable, Id type) {
    SolvPrivate::internalize_libsolv_repo(solvable->repo);
    return solvable_lookup_str(solvable, type);
}

static inline void reldeps_for(Solvable * solvable, IdQueue & queue, Id type) {
    Id marker = -1;
    Id solv_type = type;

    if (type == SOLVABLE_REQUIRES) {
        marker = -1;
    }

    if (type == SOLVABLE_PREREQMARKER) {
        solv_type = SOLVABLE_REQUIRES;
        marker = 1;
    }
    solvable_lookup_deparray(solvable, solv_type, &queue.get_queue(), marker);
}

inline const char * get_name(Pool * pool, Id package_id) noexcept {
    return pool_id2str(pool, get_solvable(pool, package_id)->name);
}

inline const char * get_evr(Pool * pool, Id package_id) noexcept {
    return pool_id2str(pool, get_solvable(pool, package_id)->evr);
}

/// @return const char* !! Return temporal value !!
inline const char * get_epoch_cstring(Pool * pool, Id package_id) noexcept {
    char * e;
    char * v;
    char * r;
    pool_split_evr(pool, get_evr(pool, package_id), &e, &v, &r);
    if (e) {
        return e;
    }
    return BASE_EPOCH;
}

inline unsigned long get_epoch(Pool * pool, Id package_id) noexcept {
    char * e;
    char * v;
    char * r;
    char * endptr;
    pool_split_evr(pool, get_evr(pool, package_id), &e, &v, &r);
    if (e) {
        auto converted = std::strtol(e, &endptr, 10);
        assert(converted > 0);
        assert(*endptr == '\0');
        return static_cast<unsigned long>(converted);
    }
    return 0;
}

/// @return const char* !! Return temporal value !!
inline const char * get_version(Pool * pool, Id package_id) noexcept {
    char * e;
    char * v;
    char * r;
    pool_split_evr(pool, get_evr(pool, package_id), &e, &v, &r);
    return v;
}

/// @return const char* !! Return temporal value !!
inline const char * get_release(Pool * pool, Id package_id) noexcept {
    char * e;
    char * v;
    char * r;
    pool_split_evr(pool, get_evr(pool, package_id), &e, &v, &r);
    return r;
}

inline const char * get_arch(Pool * pool, Id package_id) noexcept {
    return pool_id2str(pool, get_solvable(pool, package_id)->arch);
}

/// @return const char* !! Return temporal value !!
inline const char * get_nevra(Pool * pool, Id package_id) noexcept {
    return pool_solvable2str(pool, get_solvable(pool, package_id));
}

/// @return const char* !! Return temporal value !!
const char * get_full_nevra(Pool * pool, Id package_id);

/// @return const char* !! Return temporal value !!
inline const char * get_group(Pool * pool, Id package_id) noexcept {
    return lookup_cstring(get_solvable(pool, package_id), SOLVABLE_GROUP);
}

inline bool is_installed(Pool * pool, Solvable * solvable) {
    return solvable->repo == pool->installed;
}

inline unsigned long long get_package_size(Pool * pool, Id package_id) noexcept {
    return lookup_num(get_solvable(pool, package_id), SOLVABLE_DOWNLOADSIZE);
}

inline unsigned long long get_install_size(Pool * pool, Id package_id) noexcept {
    return lookup_num(get_solvable(pool, package_id), SOLVABLE_INSTALLSIZE);
}

/// @return const char* !! Return temporal value !!
inline const char * get_license(Pool * pool, Id package_id) noexcept {
    return lookup_cstring(get_solvable(pool, package_id), SOLVABLE_LICENSE);
}

/// @return const char* !! Return temporal value !!
inline const char * get_sourcerpm(Pool * pool, Id package_id) {
    Solvable * solvable = get_solvable(pool, package_id);
    SolvPrivate::internalize_libsolv_repo(solvable->repo);
    return solvable_lookup_sourcepkg(solvable);
}

inline unsigned long long get_build_time(Pool * pool, Id package_id) {
    return lookup_num(get_solvable(pool, package_id), SOLVABLE_BUILDTIME);
}

// TODO not supported by libsolv: https://github.com/openSUSE/libsolv/issues/400
/// @return const char* !! Return temporal value !!
//inline const char * get_build_host(Pool * pool, Id package_id) noexcept {
//    return lookup_cstring(get_solvable(pool, package_id), SOLVABLE_BUILDHOST);
//}

/// @return const char* !! Return temporal value !!
inline const char * get_packager(Pool * pool, Id package_id) noexcept {
    return lookup_cstring(get_solvable(pool, package_id), SOLVABLE_PACKAGER);
}

/// @return const char* !! Return temporal value !!
inline const char * get_vendor(Pool * pool, Id package_id) noexcept {
    return lookup_cstring(get_solvable(pool, package_id), SOLVABLE_VENDOR);
}

/// @return const char* !! Return temporal value !!
inline const char * get_url(Pool * pool, Id package_id) noexcept {
    return lookup_cstring(get_solvable(pool, package_id), SOLVABLE_URL);
}

/// @return const char* !! Return temporal value !!
inline const char * get_summary(Pool * pool, Id package_id) noexcept {
    return lookup_cstring(get_solvable(pool, package_id), SOLVABLE_SUMMARY);
}

/// @return const char* !! Return temporal value !!
inline const char * get_description(Pool * pool, Id package_id) noexcept {
    return lookup_cstring(get_solvable(pool, package_id), SOLVABLE_DESCRIPTION);
}

inline std::vector<std::string> get_files(Pool * pool, Id package_id) {
    Solvable * solvable = get_solvable(pool, package_id);
    Dataiterator di;
    std::vector<std::string> ret;

    SolvPrivate::internalize_libsolv_repo(solvable->repo);
    dataiterator_init(
        &di, pool, solvable->repo, package_id, SOLVABLE_FILELIST, nullptr, SEARCH_FILES | SEARCH_COMPLETE_FILELIST);
    while (dataiterator_step(&di) != 0) {
        ret.emplace_back(di.kv.str);
    }
    dataiterator_free(&di);
    return ret;
}

inline void get_provides(Pool * pool, Id package_id, IdQueue & queue) noexcept {
    reldeps_for(get_solvable(pool, package_id), queue, SOLVABLE_PROVIDES);
}

inline void get_requires(Pool * pool, Id package_id, IdQueue & queue) noexcept {
    reldeps_for(get_solvable(pool, package_id), queue, SOLVABLE_REQUIRES);
    IdQueue tmp_queue;
    reldeps_for(get_solvable(pool, package_id), tmp_queue, SOLVABLE_PREREQMARKER);
    queue.append(tmp_queue);
}

inline void get_requires_pre(Pool * pool, Id package_id, IdQueue & queue) noexcept {
    reldeps_for(get_solvable(pool, package_id), queue, SOLVABLE_PREREQMARKER);
}

inline void get_conflicts(Pool * pool, Id package_id, IdQueue & queue) noexcept {
    reldeps_for(get_solvable(pool, package_id), queue, SOLVABLE_CONFLICTS);
}

inline void get_obsoletes(Pool * pool, Id package_id, IdQueue & queue) noexcept {
    reldeps_for(get_solvable(pool, package_id), queue, SOLVABLE_OBSOLETES);
}

inline void get_recommends(Pool * pool, Id package_id, IdQueue & queue) noexcept {
    reldeps_for(get_solvable(pool, package_id), queue, SOLVABLE_RECOMMENDS);
}

inline void get_suggests(Pool * pool, Id package_id, IdQueue & queue) noexcept {
    reldeps_for(get_solvable(pool, package_id), queue, SOLVABLE_SUGGESTS);
}

inline void get_enhances(Pool * pool, Id package_id, IdQueue & queue) noexcept {
    reldeps_for(get_solvable(pool, package_id), queue, SOLVABLE_ENHANCES);
}

inline void get_supplements(Pool * pool, Id package_id, IdQueue & queue) noexcept {
    reldeps_for(get_solvable(pool, package_id), queue, SOLVABLE_SUPPLEMENTS);
}

inline void get_prereq_ignoreinst(Pool * pool, Id package_id, IdQueue & queue) noexcept {
    reldeps_for(get_solvable(pool, package_id), queue, SOLVABLE_PREREQ_IGNOREINST);
}

inline void get_regular_requires(Pool * pool, Id package_id, IdQueue & queue) noexcept {
    reldeps_for(get_solvable(pool, package_id), queue, SOLVABLE_REQUIRES);
}

/// @return const char* !! Return temporal value !!
inline const char * get_baseurl(Pool * pool, Id package_id) noexcept {
    return lookup_cstring(get_solvable(pool, package_id), SOLVABLE_MEDIABASE);
}

/// @return const char* !! Return temporal value !!
inline const char * get_location(Pool * pool, Id package_id) noexcept {
    auto solvable = get_solvable(pool, package_id);
    SolvPrivate::internalize_libsolv_repo(solvable->repo);
    return solvable_lookup_location(solvable, nullptr);
}

std::string get_package_path(Pool * pool, Id package_id);

inline unsigned long long get_hdr_end(Pool * pool, Id package_id) noexcept {
    return lookup_num(get_solvable(pool, package_id), SOLVABLE_HEADEREND);
}

inline unsigned long long get_install_time(Pool * pool, Id package_id) noexcept {
    return lookup_num(get_solvable(pool, package_id), SOLVABLE_INSTALLTIME);
}

inline unsigned long long get_media_number(Pool * pool, Id package_id) noexcept {
    return lookup_num(get_solvable(pool, package_id), SOLVABLE_MEDIANR);
}

inline unsigned long long get_rpmdbid(Pool * pool, Id package_id) noexcept {
    return lookup_num(get_solvable(pool, package_id), RPM_RPMDBID);
}

inline repo::Repo * get_repo(Pool * pool, Id package_id) noexcept {
    auto solvable = get_solvable(pool, package_id);
    return static_cast<repo::Repo *>(solvable->repo->appdata);
}


}  // namespace libdnf::rpm::solv


#endif  // LIBDNF_RPM_PACKAGE_HPP

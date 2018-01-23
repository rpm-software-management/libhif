/*
 * Copyright (C) 2018 Red Hat, Inc.
 *
 * Licensed under the GNU Lesser General Public License Version 2.1
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#include <fnmatch.h>
#include <solv/bitmap.h>
#include <solv/evr.h>
#include "query.hpp"
#include <vector>
#include <assert.h>
#include "../hy-iutil-private.hpp"
#include "../hy-util-private.hpp"
#include "../hy-iutil.h"
#include "../hy-nevra.hpp"
#include "../dnf-sack-private.hpp"
#include "../dnf-advisorypkg.h"
#include "../dnf-advisory-private.hpp"
#include "packageset.hpp"

static int
match_type_num(int keyname) {
    switch (keyname) {
        case HY_PKG_EMPTY:
        case HY_PKG_EPOCH:
        case HY_PKG_LATEST:
        case HY_PKG_LATEST_PER_ARCH:
        case HY_PKG_UPGRADABLE:
        case HY_PKG_UPGRADES:
        case HY_PKG_DOWNGRADABLE:
        case HY_PKG_DOWNGRADES:
            return 1;
    default:
        return 0;
    }
}

static int
match_type_pkg(int keyname) {
    switch (keyname) {
        case HY_PKG:
        case HY_PKG_OBSOLETES:
            return 1;
        default:
            return 0;
    }
}

static int
match_type_reldep(int keyname) {
    switch (keyname) {
        case HY_PKG_CONFLICTS:
        case HY_PKG_ENHANCES:
        case HY_PKG_OBSOLETES:
        case HY_PKG_PROVIDES:
        case HY_PKG_RECOMMENDS:
        case HY_PKG_REQUIRES:
        case HY_PKG_SUGGESTS:
        case HY_PKG_SUPPLEMENTS:
            return 1;
        default:
            return 0;
    }
}

static int
match_type_str(int keyname) {
    switch (keyname) {
        case HY_PKG_ADVISORY:
        case HY_PKG_ADVISORY_BUG:
        case HY_PKG_ADVISORY_CVE:
        case HY_PKG_ADVISORY_SEVERITY:
        case HY_PKG_ADVISORY_TYPE:
        case HY_PKG_ARCH:
        case HY_PKG_DESCRIPTION:
        case HY_PKG_ENHANCES:
        case HY_PKG_EVR:
        case HY_PKG_FILE:
        case HY_PKG_LOCATION:
        case HY_PKG_NAME:
        case HY_PKG_NEVRA:
        case HY_PKG_PROVIDES:
        case HY_PKG_RECOMMENDS:
        case HY_PKG_RELEASE:
        case HY_PKG_REPONAME:
        case HY_PKG_REQUIRES:
        case HY_PKG_SOURCERPM:
        case HY_PKG_SUGGESTS:
        case HY_PKG_SUMMARY:
        case HY_PKG_SUPPLEMENTS:
        case HY_PKG_OBSOLETES:
        case HY_PKG_CONFLICTS:
        case HY_PKG_URL:
        case HY_PKG_VERSION:
            return 1;
        default:
            return 0;
    }
}

static int
valid_filter_str(int keyname, int cmp_type)
{
    if (!match_type_str(keyname))
        return 0;

    cmp_type &= ~HY_NOT; // hy_query_run always handles NOT
    switch (keyname) {
        case HY_PKG_LOCATION:
        case HY_PKG_SOURCERPM:
            return cmp_type == HY_EQ;
        case HY_PKG_ARCH:
            return cmp_type & HY_EQ || cmp_type & HY_GLOB;
        case HY_PKG_NAME:
            return cmp_type & HY_EQ || cmp_type & HY_GLOB || cmp_type & HY_SUBSTR;
        default:
            return 1;
    }
}

static int
valid_filter_num(int keyname, int cmp_type)
{
    if (!match_type_num(keyname))
        return 0;

    cmp_type &= ~HY_NOT; // hy_query_run always handles NOT
    if (cmp_type & (HY_ICASE | HY_SUBSTR | HY_GLOB))
        return 0;
    switch (keyname) {
        case HY_PKG:
            return cmp_type == HY_EQ;
        default:
            return 1;
    }
}

static int
valid_filter_pkg(int keyname, int cmp_type)
{
    if (!match_type_pkg(keyname))
        return 0;
    return cmp_type == HY_EQ || cmp_type == HY_NEQ;
}

static int
valid_filter_reldep(int keyname)
{
    if (!match_type_reldep(keyname))
        return 0;
    return 1;
}

static Id
reldep_keyname2id(int keyname)
{
    switch(keyname) {
        case HY_PKG_CONFLICTS:
            return SOLVABLE_CONFLICTS;
        case HY_PKG_ENHANCES:
            return SOLVABLE_ENHANCES;
        case HY_PKG_OBSOLETES:
            return SOLVABLE_OBSOLETES;
        case HY_PKG_REQUIRES:
            return SOLVABLE_REQUIRES;
        case HY_PKG_RECOMMENDS:
            return SOLVABLE_RECOMMENDS;
        case HY_PKG_SUGGESTS:
            return SOLVABLE_SUGGESTS;
        case HY_PKG_SUPPLEMENTS:
            return SOLVABLE_SUPPLEMENTS;
        default:
            assert(0);
            return 0;
    }
}

static Id
di_keyname2id(int keyname)
{
    switch(keyname) {
        case HY_PKG_DESCRIPTION:
            return SOLVABLE_DESCRIPTION;
        case HY_PKG_NAME:
            return SOLVABLE_NAME;
        case HY_PKG_URL:
            return SOLVABLE_URL;
        case HY_PKG_ARCH:
            return SOLVABLE_ARCH;
        case HY_PKG_EVR:
            return SOLVABLE_EVR;
        case HY_PKG_SUMMARY:
            return SOLVABLE_SUMMARY;
        case HY_PKG_FILE:
            return SOLVABLE_FILELIST;
        default:
            assert(0);
            return 0;
    }
}

static int
type2flags(int type, int keyname)
{
    int ret = 0;
    if (keyname == HY_PKG_FILE)
        ret |= SEARCH_FILES | SEARCH_COMPLETE_FILELIST;
    if (type & HY_ICASE)
        ret |= SEARCH_NOCASE;

    type &= ~HY_COMPARISON_FLAG_MASK;
    switch (type) {
        case HY_EQ:
            return ret | SEARCH_STRING;
        case HY_SUBSTR:
            return ret | SEARCH_SUBSTRING;
        case HY_GLOB:
            return ret | SEARCH_GLOB;
        default:
            assert(0); // not implemented
            return 0;
    }
}

static char *
pool_solvable_epoch_optional_2str(Pool *pool, const Solvable *s, gboolean with_epoch)
{
    const char *e;
    const char *name = pool_id2str(pool, s->name);
    const char *evr = pool_id2str(pool, s->evr);
    const char *arch = pool_id2str(pool, s->arch);
    bool present_epoch = false;

    for (e = evr + 1; *e != '-' && *e != '\0'; ++e) {
        if (*e == ':') {
            present_epoch = true;
            break;
        }
    }
    char *output_string;
    int evr_length, arch_length;
    int extra_epoch_length = 0;
    int name_length = strlen(name);
    evr_length = strlen(evr);
    arch_length = strlen(arch);
    if (!present_epoch && with_epoch) {
        extra_epoch_length = 2;
    } else if (present_epoch && !with_epoch) {
        extra_epoch_length = evr - e - 1;
    }

    output_string = pool_alloctmpspace(
        pool, name_length + evr_length + extra_epoch_length + arch_length + 3);

    strcpy(output_string, name);

    if (evr_length || extra_epoch_length > 0) {
        output_string[name_length++] = '-';

        if (extra_epoch_length > 0) {
            output_string[name_length++] = '0';
            output_string[name_length++] = ':';
            output_string[name_length] = '\0';
        }
    }

    if (evr_length) {
        if (extra_epoch_length >= 0) {
            strcpy(output_string + name_length, evr);
        } else {
            strcpy(output_string + name_length, evr - extra_epoch_length);
            evr_length = evr_length + extra_epoch_length;
        }
    }

    if (arch_length) {
        output_string[name_length + evr_length] = '.';
        strcpy(output_string + name_length + evr_length + 1, arch);
    }
    return output_string;
}

static int
filter_latest_sortcmp(const void *ap, const void *bp, void *dp)
{
    auto pool = static_cast<Pool *>(dp);
    Solvable *sa = pool->solvables + *(Id *)ap;
    Solvable *sb = pool->solvables + *(Id *)bp;
    int r;
    r = sa->name - sb->name;
    if (r)
        return r;
    r = pool_evrcmp(pool, sb->evr, sa->evr, EVRCMP_COMPARE);
    if (r)
        return r;
    return *(Id *)ap - *(Id *)bp;
}

static int
filter_latest_sortcmp_byarch(const void *ap, const void *bp, void *dp)
{
    auto pool = static_cast<Pool *>(dp);
    Solvable *sa = pool->solvables + *(Id *)ap;
    Solvable *sb = pool->solvables + *(Id *)bp;
    int r;
    r = sa->name - sb->name;
    if (r)
        return r;
    r = sa->arch - sb->arch;
    if (r)
        return r;
    r = pool_evrcmp(pool, sb->evr, sa->evr, EVRCMP_COMPARE);
    if (r)
        return r;
    return *(Id *)ap - *(Id *)bp;
}

static void
add_latest_to_map(const Pool *pool, Map *m, Queue *samename,
                  int start_block, int stop_block, int latest)
{
    Solvable *solv_element, *solv_previous_element;
    int version_counter = 0;
    solv_previous_element = pool->solvables + samename->elements[start_block];
    Id id_previous_evr = solv_previous_element->evr;
    for (int pos = start_block; pos < stop_block; ++pos) {
        Id id_element = samename->elements[pos];
        solv_element = pool->solvables + id_element;
        Id id_current_evr = solv_element->evr;
        if (id_previous_evr != id_current_evr) {
            version_counter += 1;
            id_previous_evr = id_current_evr;
        }
        if (latest > 0) {
            if (!(version_counter < latest)) {
                return;
            }
        } else {
            if (version_counter < -latest) {
                continue;
            }
        }
        MAPSET(m, id_element);
    }
}

static void
add_duplicates_to_map(Pool *pool, Map *res, Queue samename, int start_block, int stop_block)
{
    Solvable *s_first, *s_second;
    for (int pos = start_block; pos < stop_block; ++pos) {
        Id id_first = samename.elements[pos];
        s_first = pool->solvables + id_first;
        for (int pos2 = pos + 1; pos2 < stop_block; ++pos2) {
            Id id_second = samename.elements[pos2];
            s_second = pool->solvables + id_second;
            if ((s_first->evr == s_second->evr) && (s_first->arch != s_second->arch)) {
                continue;
            }
            MAPSET(res, id_first);
            MAPSET(res, id_second);
        }
    }
}

class Filter::Impl {
public:
    ~Impl();
private:
    friend struct Filter;
    int cmpType;
    int keyname;
    int matchType;
    std::vector<_Match> matches;
};
Filter::Filter(int keyname, int cmp_type, int match) : pImpl(new Impl)
{
    pImpl->keyname = keyname;
    pImpl->cmpType = cmp_type;
    pImpl->matchType = _HY_NUM;
    _Match match_in;
    match_in.num = match;
    pImpl->matches.push_back(match_in);
}
Filter::Filter(int keyname, int cmp_type, int nmatches, const int *matches) : pImpl(new Impl)
{
    pImpl->keyname = keyname;
    pImpl->cmpType = cmp_type;
    pImpl->matchType = _HY_NUM;
    pImpl->matches.reserve(nmatches);
    for (int i = 0; i < nmatches; ++i) {
        _Match match_in;
        match_in.num = matches[i];
        pImpl->matches.push_back(match_in);
    }
}
Filter::Filter(int keyname, int cmp_type, DnfPackageSet *pset) : pImpl(new Impl)
{
    pImpl->keyname = keyname;
    pImpl->cmpType = cmp_type;
    pImpl->matchType = _HY_PKG;
    _Match match_in;
    match_in.pset = dnf_packageset_clone(pset);
    pImpl->matches.push_back(match_in);
}
Filter::Filter(int keyname, int cmp_type, DnfReldep *reldep) : pImpl(new Impl)
{
    pImpl->keyname = keyname;
    pImpl->cmpType = cmp_type;
    pImpl->matchType = _HY_RELDEP;
    _Match match_in;
    match_in.reldep = static_cast<DnfReldep *>(g_object_ref(reldep));
    pImpl->matches.push_back(match_in);
}
Filter::Filter(int keyname, int cmp_type, DnfReldepList *reldeplist) : pImpl(new Impl)
{
    pImpl->keyname = keyname;
    pImpl->cmpType = cmp_type;
    pImpl->matchType = _HY_RELDEP;
    const int nmatches = dnf_reldep_list_count(reldeplist);
    pImpl->matches.reserve(nmatches);
    for (int i = 0; i < nmatches; ++i) {
        _Match match_in;
        match_in.reldep = dnf_reldep_list_index(reldeplist, i);
        pImpl->matches.push_back(match_in);
    }
}
Filter::Filter(int keyname, int cmp_type, const char *match) : pImpl(new Impl)
{
    pImpl->keyname = keyname;
    pImpl->cmpType = cmp_type;
    pImpl->matchType = _HY_STR;
    _Match match_in;
    match_in.str = g_strdup(match);
    pImpl->matches.push_back(match_in);
}
Filter::Filter(int keyname, int cmp_type, const char **matches) : pImpl(new Impl)
{
    pImpl->keyname = keyname;
    pImpl->cmpType = cmp_type;
    pImpl->matchType = _HY_STR;
    const unsigned nmatches = g_strv_length((gchar**)matches);
    pImpl->matches.reserve(nmatches);
    for (unsigned int i = 0; i < nmatches; ++i) {
        _Match match_in;
        match_in.str = g_strdup(matches[i]);
        pImpl->matches.push_back(match_in);
    }
}

Filter::~Filter() = default;

Filter::Impl::~Impl()
{
    for (auto & match : matches) {
        switch (matchType) {
            case _HY_PKG:
                delete match.pset;
                break;
            case _HY_STR:
                g_free(match.str);
                break;
            case _HY_RELDEP:
                if (match.reldep)
                    g_object_unref (match.reldep);
                break;
            default:
                break;
        }
    }
};

const int Filter::getKeyname() const noexcept { return pImpl->keyname; }
const int Filter::getCmpType() const noexcept { return pImpl->cmpType; }
const int Filter::getMatchType() const noexcept { return pImpl->matchType; }
const std::vector< _Match >& Filter::getMatches() const noexcept { return pImpl->matches; }

class Query::Impl {
private:
    friend struct Query;
    Impl(DnfSack* sack, int flags = 0);
    Impl(const Query::Impl & src_query);
    Impl & operator= (const Impl & src);
    bool applied{0};
    DnfSack *sack;
    int flags;
    std::unique_ptr<PackageSet> result;
    std::vector<Filter> filters;
    void apply();
    void initResult();
    void filterPkg(const Filter & f, Map *m);
    void filterRcoReldep(const Filter & f, Map *m);
    void filterName(const Filter & f, Map *m);
    void filterEpoch(const Filter & f, Map *m);
    void filterEvr(const Filter & f, Map *m);
    void filterNevra(const Filter & f, Map *m);
    void filterVersion(const Filter & f, Map *m);
    void filterRelease(const Filter & f, Map *m);
    void filterArch(const Filter & f, Map *m);
    void filterSourcerpm(const Filter & f, Map *m);
    void filterObsoletes(const Filter & f, Map *m);
    void filterProvidesReldep(const Filter & f, Map *m);
    void filterReponame(const Filter & f, Map *m);
    void filterLocation(const Filter & f, Map *m);
    void filterAdvisory(const Filter & f, Map *m, int keyname);
    void filterLatest(const Filter & f, Map *m);
    void filterUpdown(const Filter & f, Map *m);
    void filterUpdownAble(const Filter  &f, Map *m);
    void filterDataiterator(const Filter & f, Map *m);
};

Query::Impl::Impl(DnfSack* sack, int flags)
: sack(sack), flags(flags) {}

Query::Impl::Impl(const Query::Impl & src)
: applied(src.applied)
, sack(src.sack)
, flags(src.flags)
, filters(src.filters)
{
    if (src.result) {
        result.reset(new PackageSet(*src.result.get()));
    }
}

Query::Impl &
Query::Impl::operator=(const Query::Impl & src)
{
    applied = src.applied;
    sack = src.sack;
    flags = src.flags;
    filters = src.filters;
    if (src.result) {
        result.reset(new PackageSet(*src.result.get()));
    } else {
        result.reset();
    }
    return *this;
}


Query::Query(const Query & query_src) : pImpl(new Impl(*query_src.pImpl)) {}
Query::Query(DnfSack *sack, int flags) : pImpl(new Impl(sack, flags)) {}
Query::~Query() = default;

Query & Query::operator=(const Query & query_src) { *pImpl = *query_src.pImpl; return *this; }

Map *
Query::getResult() noexcept
{
    if (pImpl->result)
        return pImpl->result->getMap();
    else
        return nullptr;
}

const Map * Query::getResult() const noexcept { return pImpl->result->getMap(); }
bool Query::getApplied() { return pImpl->applied; }
DnfSack * Query::getSack() { return pImpl->sack; }

void
Query::clear()
{
    pImpl->applied = false;
    pImpl->result.reset();
    pImpl->filters.clear();
}

size_t
Query::size()
{
    apply();
    return pImpl->result->size();
}


int
Query::addFilter(int keyname, int cmp_type, int match)
{
    if (!valid_filter_num(keyname, cmp_type))
        return DNF_ERROR_BAD_QUERY;
    pImpl->applied = false;
    pImpl->filters.push_back(Filter(keyname, cmp_type, match));
    return 0;
}
int
Query::addFilter(int keyname, int cmp_type, int nmatches, const int *matches)
{
    if (!valid_filter_num(keyname, cmp_type))
        return DNF_ERROR_BAD_QUERY;
    pImpl->applied = false;
    pImpl->filters.push_back(Filter(keyname, cmp_type, nmatches, matches));
    return 0;
}
int
Query::addFilter(int keyname, int cmp_type, DnfPackageSet *pset)
{
    if (!valid_filter_pkg(keyname, cmp_type))
        return DNF_ERROR_BAD_QUERY;
    pImpl->applied = false;
    pImpl->filters.push_back(Filter(keyname, cmp_type, pset));
    return 0;
}
int
Query::addFilter(int keyname, DnfReldep *reldep)
{
    if (!valid_filter_reldep(keyname))
        return DNF_ERROR_BAD_QUERY;
    pImpl->applied = false;
    pImpl->filters.push_back(Filter(keyname, HY_EQ, reldep));
    return 0;
}
int
Query::addFilter(int keyname, DnfReldepList *reldeplist)
{
    if (!valid_filter_reldep(keyname))
        return DNF_ERROR_BAD_QUERY;
    pImpl->applied = false;
    pImpl->filters.push_back(Filter(keyname, HY_EQ, reldeplist));
    return 0;
}
int
Query::addFilter(int keyname, int cmp_type, const char *match)
{
    if ((cmp_type & HY_GLOB) && !hy_is_glob_pattern(match))
        cmp_type = (cmp_type & ~HY_GLOB) | HY_EQ;

    if (!valid_filter_str(keyname, cmp_type))
        return DNF_ERROR_BAD_QUERY;
    pImpl->applied = false;
    switch (keyname) {
        case HY_PKG_CONFLICTS:
        case HY_PKG_ENHANCES:
        case HY_PKG_OBSOLETES:
        case HY_PKG_PROVIDES:
        case HY_PKG_RECOMMENDS:
        case HY_PKG_REQUIRES:
        case HY_PKG_SUGGESTS:
        case HY_PKG_SUPPLEMENTS: {
            DnfSack *sack = pImpl->sack;

            if (cmp_type == HY_GLOB) {
                DnfReldepList *reldeplist = reldeplist_from_str(sack, match);
                if (reldeplist == NULL)
                    return addFilter(HY_PKG_EMPTY, HY_EQ, 1);
                int ret = addFilter(keyname, reldeplist);
                g_object_unref (reldeplist);
                return ret;
            } else {
                DnfReldep *reldep = reldep_from_str(sack, match);
                if (reldep == NULL)
                    return addFilter(HY_PKG_EMPTY, HY_EQ, 1);
                int ret = addFilter(keyname, reldep);
                g_object_unref (reldep);
                return ret;
            }
        }
        default: {
            pImpl->filters.push_back(Filter(keyname, cmp_type, match));
            return 0;
        }
    }
}
int
Query::addFilter(int keyname, int cmp_type, const char **matches)
{
    if (cmp_type == HY_GLOB) {
        bool is_glob = false;
        for (const char **match = matches; *match != NULL; match++) {
            if (hy_is_glob_pattern(*match)) {
                is_glob = true;
                break;
            }
        }
        if (!is_glob) cmp_type = HY_EQ;
    }
    if (!valid_filter_str(keyname, cmp_type))
        return DNF_ERROR_BAD_QUERY;
    pImpl->applied = false;
    pImpl->filters.push_back(Filter(keyname, cmp_type, matches));
    return 0;
}

int
Query::addFilter(HyNevra nevra, bool icase)
{
    if (!nevra->getName().empty() && nevra->getName() != "*") {
        if (icase)
            addFilter(HY_PKG_NAME, HY_GLOB|HY_ICASE, nevra->getName().c_str());
        else
            addFilter(HY_PKG_NAME, HY_GLOB, nevra->getName().c_str());
    }
    if (nevra->getEpoch() != -1)
        addFilter(HY_PKG_EPOCH, HY_EQ, nevra->getEpoch());
    if (!nevra->getVersion().empty() && nevra->getVersion() != "*")
        addFilter(HY_PKG_VERSION, HY_GLOB, nevra->getVersion().c_str());
    if (!nevra->getRelease().empty() && nevra->getRelease() != "*")
        addFilter(HY_PKG_RELEASE, HY_GLOB, nevra->getRelease().c_str());
    if (!nevra->getArch().empty() && nevra->getArch() != "*")
        addFilter(HY_PKG_ARCH, HY_GLOB, nevra->getArch().c_str());
    return 0;
}

void
Query::Impl::initResult()
{
    Pool *pool = dnf_sack_get_pool(sack);
    Id solvid;

    int sack_pool_nsolvables = dnf_sack_get_pool_nsolvables(sack);
    if (sack_pool_nsolvables != 0 && sack_pool_nsolvables == pool->nsolvables)
        result.reset(new PackageSet(sack, dnf_sack_get_pkg_solvables(sack)));
    else {
        result.reset(new PackageSet(sack));
        FOR_PKG_SOLVABLES(solvid)
            result->set(solvid);
        dnf_sack_set_pkg_solvables(sack, result->getMap(), pool->nsolvables);
    }
    if (!(flags & HY_IGNORE_EXCLUDES)) {
        dnf_sack_recompute_considered(sack);
        if (pool->considered)
            map_and(result->getMap(), pool->considered);
    }
}

void
Query::Impl::filterPkg(const Filter & f, Map *m)
{
    assert(f.getMatches().size() == 1);
    assert(f.getMatchType() == _HY_PKG);

    map_free(m);
    map_init_clone(m, dnf_packageset_get_map(f.getMatches()[0].pset));
}

void
Query::Impl::filterRcoReldep(const Filter & f, Map *m)
{
    assert(f.getMatchType() == _HY_RELDEP);

    Pool *pool = dnf_sack_get_pool(sack);
    Id rco_key = reldep_keyname2id(f.getKeyname());
    Queue rco;

    queue_init(&rco);
    for (auto match : f.getMatches()) {
        Id r_id = dnf_reldep_get_id (match.reldep);
        Id s_id = -1;
        while (true) {
            s_id = result->next(s_id);
            if (s_id == -1)
                break;

            Solvable *s = pool_id2solvable(pool, s_id);

            queue_empty(&rco);
            solvable_lookup_idarray(s, rco_key, &rco);
            for (int j = 0; j < rco.count; ++j) {
                Id r_id2 = rco.elements[j];

                if (pool_match_dep(pool, r_id, r_id2)) {
                    MAPSET(m, s_id);
                    break;
                }
            }
        }
    }
    queue_free(&rco);
}

void
Query::Impl::filterName(const Filter & f, Map *m)
{
    Pool *pool = dnf_sack_get_pool(sack);
    Id match_name_id = 0;
    for (auto match_union : f.getMatches()) {
        const char *match = match_union.str;
        if ((f.getCmpType() & HY_EQ) && !(f.getCmpType() & HY_ICASE)) {
            match_name_id = pool_str2id(pool, match, 0);
            if (match_name_id == 0)
                continue;
        }

        Id id = -1;
        while (true) {
            id = result->next(id);
            if (id == -1)
                break;

            Solvable *s = pool_id2solvable(pool, id);

            if (f.getCmpType() & HY_ICASE) {
                const char *name = pool_id2str(pool, s->name);
                if (f.getCmpType() & HY_SUBSTR) {
                    if (strcasestr(name, match) != NULL)
                        MAPSET(m, id);
                    continue;
                }
                if (f.getCmpType() & HY_EQ) {
                    if (strcasecmp(name, match) == 0)
                        MAPSET(m, id);
                    continue;
                }
                if (f.getCmpType() & HY_GLOB) {
                    if (fnmatch(match, name, FNM_CASEFOLD) == 0)
                        MAPSET(m, id);
                    continue;
                }
                continue;
            }

            if (f.getCmpType() & HY_EQ) {
                if (match_name_id == s->name)
                    MAPSET(m, id);
                continue;
            }
            const char *name = pool_id2str(pool, s->name);
            if (f.getCmpType() & HY_GLOB) {
                if (fnmatch(match, name, 0) == 0)
                    MAPSET(m, id);
                continue;
            }
            if (f.getCmpType() & HY_SUBSTR) {
                if (strstr(name, match) != NULL)
                    MAPSET(m, id);
                continue;
            }
        }
    }
}

void
Query::Impl::filterEpoch(const Filter & f, Map *m)
{
    Pool *pool = dnf_sack_get_pool(sack);
    int cmp_type = f.getCmpType();

    for (auto match : f.getMatches()) {
        unsigned long epoch = match.num;

        Id id = -1;
        while (true) {
            id = result->next(id);
            if (id == -1)
                break;

            Solvable *s = pool_id2solvable(pool, id);
            if (s->evr == ID_EMPTY)
                continue;

            const char *evr = pool_id2str(pool, s->evr);
            unsigned long pkg_epoch = pool_get_epoch(pool, evr);

            if ((pkg_epoch > epoch && cmp_type & HY_GT) ||
                (pkg_epoch < epoch && cmp_type & HY_LT) ||
                (pkg_epoch == epoch && cmp_type & HY_EQ))
                MAPSET(m, id);
        }
    }
}

void
Query::Impl::filterEvr(const Filter & f, Map *m)
{
    Pool *pool = dnf_sack_get_pool(sack);
    int cmp_type = f.getCmpType();

    for (auto match : f.getMatches()) {
        Id match_evr = pool_str2id(pool, match.str, 1);

        Id id = -1;
        while (true) {
            id = result->next(id);
            if (id == -1)
                break;
            Solvable *s = pool_id2solvable(pool, id);
            int cmp = pool_evrcmp(pool, s->evr, match_evr, EVRCMP_COMPARE);

            if ((cmp > 0 && cmp_type & HY_GT) || (cmp < 0 && cmp_type & HY_LT) ||
                (cmp == 0 && cmp_type & HY_EQ)) {
                MAPSET(m, id);
            }
        }
    }
}

void
Query::Impl::filterNevra(const Filter & f, Map *m)
{
    Pool *pool = dnf_sack_get_pool(sack);
    int cmp_type = f.getCmpType();
    int fn_flags = (HY_ICASE & f.getCmpType()) ? FNM_CASEFOLD : 0;

    for (auto match : f.getMatches()) {
        const char *nevra_pattern = match.str;
        if (strpbrk(nevra_pattern, "(/=<> "))
            continue;

        gboolean present_epoch = strchr(nevra_pattern, ':') != NULL;

        Id id = -1;
        while (true) {
            id = result->next(id);
            if (id == -1)
                break;
            Solvable* s = pool_id2solvable(pool, id);

            char* nevra = pool_solvable_epoch_optional_2str(pool, s, present_epoch);
            if (!(HY_GLOB & cmp_type)) {
                if (HY_ICASE & cmp_type) {
                    if (strcasecmp(nevra_pattern, nevra) == 0)
                        MAPSET(m, id);
                } else {
                    if (strcmp(nevra_pattern, nevra) == 0)
                        MAPSET(m, id);
                }
            } else if (fnmatch(nevra_pattern, nevra, fn_flags) == 0) {
                MAPSET(m, id);
            }
        }
    }
}

void
Query::Impl::filterVersion(const Filter & f, Map *m)
{
    Pool *pool = dnf_sack_get_pool(sack);
    int cmp_type = f.getCmpType();

    for (auto match_in : f.getMatches()) {
        const char *match = match_in.str;
        char *filter_vr = solv_dupjoin(match, "-0", NULL);

        Id id = -1;
        while (true) {
            id = result->next(id);
            if (id == -1)
                break;
            char *e, *v, *r;
            Solvable *s = pool_id2solvable(pool, id);
            if (s->evr == ID_EMPTY)
                continue;
            const char *evr = pool_id2str(pool, s->evr);

            pool_split_evr(pool, evr, &e, &v, &r);

            if (cmp_type == HY_GLOB) {
                if (fnmatch(match, v, 0))
                    continue;
                MAPSET(m, id);
            }

            char *vr = pool_tmpjoin(pool, v, "-0", NULL);
            int cmp = pool_evrcmp_str(pool, vr, filter_vr, EVRCMP_COMPARE);
            if ((cmp > 0 && cmp_type & HY_GT) ||
                (cmp < 0 && cmp_type & HY_LT) ||
                (cmp == 0 && cmp_type & HY_EQ)) {
                MAPSET(m, id);
            }
        }
        solv_free(filter_vr);
    }
}

void
Query::Impl::filterRelease(const Filter & f, Map *m)
{
    Pool *pool = dnf_sack_get_pool(sack);
    int cmp_type = f.getCmpType();

    for (auto match_in : f.getMatches()) {
        const char *match = match_in.str;
        char *filter_vr = solv_dupjoin("0-", match, NULL);

        Id id = -1;
        while (true) {
            id = result->next(id);
            if (id == -1)
                break;
            char *e, *v, *r;
            Solvable *s = pool_id2solvable(pool, id);
            if (s->evr == ID_EMPTY)
                continue;
            const char *evr = pool_id2str(pool, s->evr);

            pool_split_evr(pool, evr, &e, &v, &r);

            if (cmp_type & HY_GLOB) {
                if (fnmatch(match, r, 0))
                    continue;
                MAPSET(m, id);
            }

            char *vr = pool_tmpjoin(pool, "0-", r, NULL);

            int cmp = pool_evrcmp_str(pool, vr, filter_vr, EVRCMP_COMPARE);

            if ((cmp > 0 && cmp_type & HY_GT) ||
                (cmp < 0 && cmp_type & HY_LT) ||
                (cmp == 0 && cmp_type & HY_EQ)) {
                MAPSET(m, id);
            }
        }
        solv_free(filter_vr);
    }
}

void
Query::Impl::filterArch(const Filter & f, Map *m)
{
    Pool *pool = dnf_sack_get_pool(sack);
    int cmp_type = f.getCmpType();
    Id match_arch_id = 0;
    for (auto match_in : f.getMatches()) {
        const char *match = match_in.str;
        if (cmp_type & HY_EQ) {
            match_arch_id = pool_str2id(pool, match, 0);
            if (match_arch_id == 0)
                continue;
        }

        Id id = -1;
        while (true) {
            id = result->next(id);
            if (id == -1)
                break;
            Solvable *s = pool_id2solvable(pool, id);
            if (cmp_type & HY_EQ) {
                if (match_arch_id == s->arch)
                    MAPSET(m, id);
                continue;
            }
            const char *arch = pool_id2str(pool, s->arch);
            if (cmp_type & HY_GLOB) {
                if (fnmatch(match, arch, 0) == 0)
                    MAPSET(m, id);
                continue;
            }
        }
    }
}

void
Query::Impl::filterSourcerpm(const Filter & f, Map *m)
{
    Pool *pool = dnf_sack_get_pool(sack);

    for (auto match_in : f.getMatches()) {
        const char *match = match_in.str;

        Id id = -1;
        while (true) {
            id = result->next(id);
            if (id == -1)
                break;
            Solvable *s = pool_id2solvable(pool, id);

            const char *name = solvable_lookup_str(s, SOLVABLE_SOURCENAME);
            if (name == NULL)
                name = pool_id2str(pool, s->name);
            if (!g_str_has_prefix(match, name)) // early check
                continue;

            DnfPackage *pkg = dnf_package_new(sack, id);
            const char *srcrpm = dnf_package_get_sourcerpm(pkg);
            if (srcrpm && !strcmp(match, srcrpm))
                MAPSET(m, id);
            g_object_unref(pkg);
        }
    }
}

void
Query::Impl::filterObsoletes(const Filter & f, Map *m)
{
    Pool *pool = dnf_sack_get_pool(sack);
    int obsprovides = pool_get_flag(pool, POOL_FLAG_OBSOLETEUSESPROVIDES);
    Map *target;

    assert(f.getMatchType() == _HY_PKG);
    assert(f.getMatches().size() == 1);
    target = dnf_packageset_get_map(f.getMatches()[0].pset);
    dnf_sack_make_provides_ready(sack);
    Id id = -1;
    while (true) {
        id = result->next(id);
        if (id == -1)
            break;
        Solvable *s = pool_id2solvable(pool, id);
        if (!s->repo)
            continue;
        for (Id *r_id = s->repo->idarraydata + s->obsoletes; *r_id; ++r_id) {
            Id r, rr;

            FOR_PROVIDES(r, rr, *r_id) {
                if (!MAPTST(target, r))
                    continue;
                assert(r != SYSTEMSOLVABLE);
                Solvable *so = pool_id2solvable(pool, r);
                if (!obsprovides && !pool_match_nevr(pool, so, *r_id))
                    continue; /* only matching pkg names */
                MAPSET(m, id);
                break;
            }
        }
    }
}

void
Query::Impl::filterProvidesReldep(const Filter & f, Map *m)
{
    Pool *pool = dnf_sack_get_pool(sack);
    Id p, pp;

    dnf_sack_make_provides_ready(sack);
    for (auto match_in : f.getMatches()) {
        Id r_id = dnf_reldep_get_id (match_in.reldep);
        FOR_PROVIDES(p, pp, r_id)
            MAPSET(m, p);
    }
}

void
Query::Impl::filterReponame(const Filter & f, Map *m)
{
    Pool *pool = dnf_sack_get_pool(sack);
    Solvable *s;
    Repo *r;
    Id id, ourids[pool->nrepos];

    for (id = 0; id < pool->nrepos; ++id)
        ourids[id] = 0;
    FOR_REPOS(id, r) {
        for (auto match_in : f.getMatches()) {
            if (!strcmp(r->name, match_in.str)) {
                ourids[id] = 1;
                break;
            }
        }
    }

    id = -1;
    while (true) {
        id = result->next(id);
        if (id == -1)
            break;
        s = pool_id2solvable(pool, id);
        switch (f.getCmpType() & ~HY_COMPARISON_FLAG_MASK) {
            case HY_EQ:
                if (s->repo && ourids[s->repo->repoid])
                    MAPSET(m, id);
                break;
            default:
                assert(0);
        }
    }
}

void
Query::Impl::filterLocation(const Filter & f, Map *m)
{
    Pool *pool = dnf_sack_get_pool(sack);

    for (auto match_in : f.getMatches()) {
        const char *match = match_in.str;

        Id id = -1;
        while (true) {
            id = result->next(id);
            if (id == -1)
                break;
            Solvable *s = pool_id2solvable(pool, id);

            const char *location = solvable_get_location(s, NULL);
            if (location == NULL)
                continue;
            if (!strcmp(match, location))
                MAPSET(m, id);
        }
    }
}

void
Query::Impl::filterAdvisory(const Filter & f, Map *m, int keyname)
{
    Pool *pool = dnf_sack_get_pool(sack);
    DnfAdvisory *advisory;
    g_autoptr(GPtrArray) pkgs = g_ptr_array_new_with_free_func((GDestroyNotify) g_object_unref);
    Dataiterator di;
    gboolean eq;

    // iterate over advisories
    dataiterator_init(&di, pool, 0, 0, 0, 0, 0);
    dataiterator_prepend_keyname(&di, UPDATE_COLLECTION);
    while (dataiterator_step(&di)) {
        dataiterator_setpos_parent(&di);
        advisory = dnf_advisory_new(pool, di.solvid);

        for (auto match_in : f.getMatches()) {
            const char *match = match_in.str;
            switch(keyname) {
            case HY_PKG_ADVISORY:
                eq = dnf_advisory_match_id(advisory, match);
                break;
            case HY_PKG_ADVISORY_BUG:
                eq = dnf_advisory_match_bug(advisory, match);
                break;
            case HY_PKG_ADVISORY_CVE:
                eq = dnf_advisory_match_cve(advisory, match);
                break;
            case HY_PKG_ADVISORY_TYPE:
                eq = dnf_advisory_match_kind(advisory, match);
                break;
            case HY_PKG_ADVISORY_SEVERITY:
                eq = dnf_advisory_match_severity(advisory, match);
                break;
            default:
                eq = FALSE;
            }
            if (eq) {
                // remember package nevras for matched advisories
                GPtrArray *apkgs = dnf_advisory_get_packages(advisory);
                for (unsigned int p = 0; p < apkgs->len; ++p) {
                    auto apkg = g_ptr_array_index(apkgs, p);
                    g_ptr_array_add(pkgs, g_object_ref(apkg));
                }
                g_ptr_array_unref(apkgs);
            }
        }
        g_object_unref(advisory);
        dataiterator_skip_solvable(&di);
    }

    // convert nevras (from DnfAdvisoryPkg) to pool ids
    Id id = -1;
    while (true) {
        if (pkgs->len == 0)
            break;
        id = result->next(id);
        if (id == -1)
            break;
        Solvable* s = pool_id2solvable(pool, id);
        for (unsigned int p = 0; p < pkgs->len; ++p) {
            auto apkg = static_cast<DnfAdvisoryPkg *>(g_ptr_array_index(pkgs, p));
            if (dnf_advisorypkg_compare_solvable(apkg, pool, s) == 0) {
                MAPSET(m, id);
                // found it, now remove it from the list to speed up rest of query
                g_ptr_array_remove_index(pkgs, p);
                break;
            }
        }
    }
    dataiterator_free(&di);
}

void
Query::Impl::filterLatest(const Filter & f, Map *m)
{
    int keyname = f.getKeyname(); 
    Pool *pool = dnf_sack_get_pool(sack);

    for (auto match_in : f.getMatches()) {
        int latest = match_in.num;
        if (latest == 0)
            continue;
        Queue samename;

        queue_init(&samename);
        Id id = -1;
        while (true) {
            id = result->next(id);
            if (id == -1)
                break;
            queue_push(&samename, id);
        }

        if (keyname == HY_PKG_LATEST_PER_ARCH) {
            solv_sort(samename.elements, samename.count, sizeof(Id),
                      filter_latest_sortcmp_byarch, pool);
        } else {
            solv_sort(samename.elements, samename.count, sizeof(Id),
                      filter_latest_sortcmp, pool);
        }

        Solvable *considered, *highest = 0;
        int start_block = -1;
        int i;
        for (i = 0; i < samename.count; ++i) {
            Id p = samename.elements[i];
            considered = pool->solvables + p;
            if (!highest || highest->name != considered->name ||
                ((keyname == HY_PKG_LATEST_PER_ARCH) && highest->arch != considered->arch)) {
                /* start of a new block */
                if (start_block == -1) {
                    highest = considered;
                    start_block = i;
                    continue;
                }
                add_latest_to_map(pool, m, &samename, start_block, i, latest);
                highest = considered;
                start_block = i;
            }
        }
        if (start_block != -1) {
            add_latest_to_map(pool, m, &samename, start_block, i, latest);
        }
        queue_free(&samename);
    }
}

void
Query::Impl::filterUpdown(const Filter & f, Map *m)
{
    Pool *pool = dnf_sack_get_pool(sack);

    dnf_sack_make_provides_ready(sack);

    if (!pool->installed) {
        return;
    }

    for (auto match_in : f.getMatches()) {
        if (match_in.num == 0)
            continue;

        Id id = -1;
        while (true) {
            id = result->next(id);
            if (id == -1)
                break;
            Solvable *s = pool_id2solvable(pool, id);
            if (s->repo == pool->installed)
                continue;
            if (f.getKeyname() == HY_PKG_DOWNGRADES) {
                if (what_downgrades(pool, id) > 0)
                    MAPSET(m, id);
            } else if (what_upgrades(pool, id) > 0)
                MAPSET(m, id);
        }
    }
}

void
Query::Impl::filterUpdownAble(const Filter  &f, Map *m)
{
    Id p, what;
    Solvable *s;
    Pool *pool = dnf_sack_get_pool(sack);

    dnf_sack_make_provides_ready(sack);

    if (!pool->installed) {
        return;
    }
    for (auto match_in : f.getMatches()) {
        if (match_in.num == 0)
            continue;

        FOR_PKG_SOLVABLES(p) {
            s = pool_id2solvable(pool, p);
            if (s->repo == pool->installed)
                continue;

            what = (f.getKeyname() == HY_PKG_DOWNGRADABLE) ? what_downgrades(pool, p) :
                what_upgrades(pool, p);
            if (what != 0 && map_tst(result->getMap(), what))
                map_set(m, what);
        }
    }
}

void
Query::Impl::filterDataiterator(const Filter & f, Map *m)
{
    Pool *pool = dnf_sack_get_pool(sack);
    Dataiterator di;
    Id keyname = di_keyname2id(f.getKeyname());
    int flags = type2flags(f.getCmpType(), f.getKeyname());

    assert(f.getMatchType() == _HY_STR);

    for (auto match_in : f.getMatches()) {
        const char *match = match_in.str;
        Id id = -1;
        while (true) {
            id = result->next(id);
            if (id == -1)
                break;
            dataiterator_init(&di, pool, 0, id, keyname, match, flags);
            while (dataiterator_step(&di))
                MAPSET(m, di.solvid);
            dataiterator_free(&di);
        }
    }
}

void
Query::apply() { pImpl->apply(); }

void
Query::Impl::apply()
{
    if (applied)
        return;

    Pool *pool = dnf_sack_get_pool(sack);
    Map m;
    if (!result)
        initResult();
    map_init(&m, pool->nsolvables);
    assert(m.size == result->getMap()->size);
    for (auto f : filters) {
        map_empty(&m);
        switch (f.getKeyname()) {
            case HY_PKG:
                filterPkg(f, &m);
                break;
            case HY_PKG_ALL:
            case HY_PKG_EMPTY:
                /* used to set query empty by keeping Map m empty */
                break;
            case HY_PKG_CONFLICTS:
                filterRcoReldep(f, &m);
                break;
            case HY_PKG_NAME:
                filterName(f, &m);
                break;
            case HY_PKG_EPOCH:
                filterEpoch(f, &m);
                break;
            case HY_PKG_EVR:
                filterEvr(f, &m);
                break;
            case HY_PKG_NEVRA:
                filterNevra(f, &m);
                break;
            case HY_PKG_VERSION:
                filterVersion(f, &m);
                break;
            case HY_PKG_RELEASE:
                filterRelease(f, &m);
                break;
            case HY_PKG_ARCH:
                filterArch(f, &m);
                break;
            case HY_PKG_SOURCERPM:
                filterSourcerpm(f, &m);
                break;
            case HY_PKG_OBSOLETES:
                if (f.getMatchType() == _HY_RELDEP)
                    filterRcoReldep(f, &m);
                else {
                    assert(f.getMatchType() == _HY_PKG);
                    filterObsoletes(f, &m);
                }
                break;
            case HY_PKG_PROVIDES:
                assert(f.getMatchType() == _HY_RELDEP);
                filterProvidesReldep(f, &m);
                break;
            case HY_PKG_ENHANCES:
            case HY_PKG_RECOMMENDS:
            case HY_PKG_REQUIRES:
            case HY_PKG_SUGGESTS:
            case HY_PKG_SUPPLEMENTS:
                assert(f.getMatchType() == _HY_RELDEP);
                filterRcoReldep(f, &m);
                break;
            case HY_PKG_REPONAME:
                filterReponame(f, &m);
                break;
            case HY_PKG_LOCATION:
                filterLocation(f, &m);
                break;
            case HY_PKG_ADVISORY:
            case HY_PKG_ADVISORY_BUG:
            case HY_PKG_ADVISORY_CVE:
            case HY_PKG_ADVISORY_SEVERITY:
            case HY_PKG_ADVISORY_TYPE:
                filterAdvisory(f, &m, f.getKeyname());
                break;
            case HY_PKG_LATEST:
            case HY_PKG_LATEST_PER_ARCH:
                filterLatest(f, &m);
                break;
            case HY_PKG_DOWNGRADABLE:
            case HY_PKG_UPGRADABLE:
                filterUpdownAble(f, &m);
                break;
            case HY_PKG_DOWNGRADES:
            case HY_PKG_UPGRADES:
                filterUpdown(f, &m);
                break;
            default:
                filterDataiterator(f, &m);
        }
        if (f.getCmpType() & HY_NOT)
            map_subtract(result->getMap(), &m);
        else
            map_and(result->getMap(), &m);
    }
    map_free(&m);

    applied = true;
    filters.clear();
}

GPtrArray *
Query::run()
{
    GPtrArray *plist = hy_packagelist_create();

    apply();
    Id id = -1;
    while (true) {
        id = pImpl->result->next(id);
        if (id == -1)
            break;
        g_ptr_array_add(plist, dnf_package_new(pImpl->sack, id));
    }
    return plist;
}

DnfPackageSet *
Query::runSet()
{
    apply();
    return new PackageSet(*pImpl->result.get());
}

Id
Query::getIndexItem(int index)
{
    apply();
    return (*pImpl->result.get())[index];
}

void
Query::queryUnion(Query other)
{
    apply();
    other.apply();
    map_or(pImpl->result->getMap(), other.getResult());
}

void
Query::queryIntersection(Query other)
{
    apply();
    other.apply();
    map_and(pImpl->result->getMap(), other.getResult());
}

void
Query::queryDifference(Query other)
{
    apply();
    other.apply();
    map_subtract(pImpl->result->getMap(), other.getResult());
}

bool
Query::empty()
{
    apply();
    const unsigned char *res = pImpl->result->getMap()->map;
    const unsigned char *end = res + pImpl->result->getMap()->size;

    while (res < end) {
        if (*res++)
            return false;
    }
    return true;
}

void
Query::filterExtras()
{
    Pool *pool = dnf_sack_get_pool(pImpl->sack);
    bool matched;
    Solvable *s_installed, *s_available;

    apply();

    Query query_installed(*this);
    query_installed.addFilter(HY_PKG_REPONAME, HY_EQ, HY_SYSTEM_REPO_NAME);
    Query query_available(*this);
    query_available.addFilter(HY_PKG_REPONAME, HY_NEQ, HY_SYSTEM_REPO_NAME);

    query_installed.apply();
    query_available.apply();
    MAPZERO(pImpl->result->getMap());
    Id id_installed = -1;
    while (true) {
        id_installed = query_installed.pImpl->result->next(id_installed);
        if (id_installed == -1)
            break;

        s_installed = pool_id2solvable(pool, id_installed);
        matched = false;
        Id id_available = -1;
        while (true) {
            id_available = query_available.pImpl->result->next(id_available);
            if (id_available == -1)
                break;

            s_available = pool_id2solvable(pool, id_available);
            if ((s_installed->name == s_available->name) && (s_installed->evr == s_available->evr)
                && (s_installed->arch == s_available->arch)) {
                matched = true;
                break;
            }
        }
        if (!matched)
            MAPSET(pImpl->result->getMap(), id_installed);
    }
}

void
Query::filterRecent(const long unsigned int recent_limit)
{
    apply();

    Id id = -1;
    while (true) {
        id = pImpl->result->next(id);
        if (id == -1)
                break;
        DnfPackage *pkg = dnf_package_new(pImpl->sack, id);
        guint64 build_time = dnf_package_get_buildtime(pkg);
        if (build_time <= recent_limit) {
            MAPCLR(pImpl->result->getMap(), id);
        }
    }
}

void
Query::filterDuplicated()
{
    Queue samename;
    Pool *pool = dnf_sack_get_pool(pImpl->sack);

    addFilter(HY_PKG_REPONAME, HY_EQ, HY_SYSTEM_REPO_NAME);
    apply();

    hy_query_to_name_ordered_queue(this, &samename);

    Solvable *considered, *highest = 0;
    int start_block = -1;
    int i;
    MAPZERO(pImpl->result->getMap());
    for (i = 0; i < samename.count; ++i) {
        Id p = samename.elements[i];
        considered = pool->solvables + p;
        if (!highest || highest->name != considered->name) {
            /* start of a new block */
            if (start_block == -1) {
                highest = considered;
                start_block = i;
                continue;
            }
            if (start_block != i - 1) {
                add_duplicates_to_map(pool, pImpl->result->getMap(), samename, start_block,
                                      i);
            }
            highest = considered;
            start_block = i;
        }
    }
    if (start_block != -1) {
        add_duplicates_to_map(pool, pImpl->result->getMap(), samename, start_block, i);
    }
}

void
hy_query_to_name_ordered_queue(HyQuery query, Queue *samename)
{
    hy_query_apply(query);
    Pool *pool = dnf_sack_get_pool(query->getSack());

    queue_init(samename);
    for (int i = 1; i < pool->nsolvables; ++i)
        if (MAPTST(query->getResult(), i))
            queue_push(samename, i);

    solv_sort(samename->elements, samename->count, sizeof(Id), filter_latest_sortcmp, pool);
}

void
hy_query_to_name_arch_ordered_queue(HyQuery query, Queue *samename)
{
    hy_query_apply(query);
    Pool *pool = dnf_sack_get_pool(query->getSack());

    queue_init(samename);
    for (int i = 1; i < pool->nsolvables; ++i)
        if (MAPTST(query->getResult(), i))
            queue_push(samename, i);

    solv_sort(samename->elements, samename->count, sizeof(Id), filter_latest_sortcmp_byarch, pool);
}

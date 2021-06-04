// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project


#include "libdnf/advisory/advisory_module.hpp"

#include "libdnf/advisory/advisory_module_private.hpp"
#include "libdnf/rpm/package_sack_impl.hpp"


namespace libdnf::advisory {

// AdvisoryModule
AdvisoryModule::AdvisoryModule(AdvisoryModule::Impl * private_module) : p_impl(private_module) {}

AdvisoryModule::~AdvisoryModule() = default;

AdvisoryModule::AdvisoryModule(const AdvisoryModule & src) : p_impl(new Impl(*src.p_impl)) {}
AdvisoryModule::AdvisoryModule(AdvisoryModule && other) : p_impl(new Impl(std::move(*other.p_impl))) {}

AdvisoryModule & AdvisoryModule::operator=(const AdvisoryModule & src) {
    *p_impl = *src.p_impl;
    return *this;
}

AdvisoryModule & AdvisoryModule::operator=(AdvisoryModule && src) noexcept {
    p_impl.swap(src.p_impl);
    return *this;
}


std::string AdvisoryModule::get_name() const {
    Pool * pool = p_impl->sack->p_impl->get_pool();
    return pool_id2str(pool, p_impl->name);
}

std::string AdvisoryModule::get_stream() const {
    Pool * pool = p_impl->sack->p_impl->get_pool();
    return pool_id2str(pool, p_impl->stream);
}
std::string AdvisoryModule::get_version() const {
    Pool * pool = p_impl->sack->p_impl->get_pool();
    return pool_id2str(pool, p_impl->version);
}
std::string AdvisoryModule::get_context() const {
    Pool * pool = p_impl->sack->p_impl->get_pool();
    return pool_id2str(pool, p_impl->context);
}
std::string AdvisoryModule::get_arch() const {
    Pool * pool = p_impl->sack->p_impl->get_pool();
    return pool_id2str(pool, p_impl->arch);
}
AdvisoryId AdvisoryModule::get_advisory_id() const {
    return p_impl->advisory;
}
Advisory AdvisoryModule::get_advisory() const {
    return Advisory(p_impl->sack, p_impl->advisory);
}
AdvisoryCollection AdvisoryModule::get_advisory_collection() const {
    return AdvisoryCollection(p_impl->sack, p_impl->advisory, p_impl->owner_collection_index);
}

// AdvisoryModule::Impl
AdvisoryModule::Impl::Impl(
    libdnf::rpm::PackageSack & sack,
    AdvisoryId advisory,
    int owner_collection_index,
    Id name,
    Id stream,
    Id version,
    Id context,
    Id arch)
    : sack(sack.get_weak_ptr())
    , advisory(advisory)
    , owner_collection_index(owner_collection_index)
    , name(name)
    , stream(stream)
    , version(version)
    , context(context)
    , arch(arch) {}

AdvisoryModule::Impl::Impl(const Impl & other)
    : sack(other.sack)
    , advisory(other.advisory)
    , owner_collection_index(other.owner_collection_index)
    , name(other.name)
    , stream(other.stream)
    , version(other.version)
    , context(other.context)
    , arch(other.arch) {}

AdvisoryModule::Impl::Impl(Impl && other)
    : sack(std::move(other.sack))
    , advisory(std::move(other.advisory))
    , owner_collection_index(other.owner_collection_index)
    , name(std::move(other.name))
    , stream(std::move(other.stream))
    , version(std::move(other.version))
    , context(std::move(other.context))
    , arch(std::move(other.arch)) {}

AdvisoryModule::Impl & AdvisoryModule::Impl::operator=(const Impl & other) {
    sack = other.sack;
    advisory = other.advisory;
    owner_collection_index = other.owner_collection_index;
    name = other.name;
    stream = other.stream;
    version = other.version;
    context = other.context;
    arch = other.arch;
    return *this;
}

AdvisoryModule::Impl & AdvisoryModule::Impl::operator=(Impl && other) {
    sack = std::move(other.sack);
    advisory = std::move(other.advisory);
    owner_collection_index = std::move(other.owner_collection_index);
    name = std::move(other.name);
    stream = std::move(other.stream);
    version = std::move(other.version);
    context = std::move(other.context);
    arch = std::move(other.arch);
    return *this;
}

}  // namespace libdnf::advisory

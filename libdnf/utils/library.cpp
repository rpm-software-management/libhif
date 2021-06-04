// SPDX-License-Identifier: LGPL-2.1-or-later
// SPDX-FileCopyrightText: Copyright contributors to the libdnf project


#include "library.hpp"

#include <dlfcn.h>

namespace libdnf::utils {

Library::Library(const std::string & path)
: path(path) {
    handle = dlopen(path.c_str(), RTLD_LAZY);
    if (!handle) {
        const char * err_msg = dlerror();
        throw CantLoad(err_msg);
    }
}

Library::~Library() {
    dlclose(handle);
}

void * Library::get_address(const char * symbol) const {
    dlerror();  // Clear any existing error
    void * address  = dlsym(handle, symbol);
    if (!address)
    {
        const char * err_msg = dlerror();
        if (err_msg) {
            throw NotFound(err_msg);
        }
    }
    return address;
}

}  // namespace libdnf::utils

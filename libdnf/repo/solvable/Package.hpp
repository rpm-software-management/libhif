#ifndef LIBDNF_PACKAGE_HPP
#define LIBDNF_PACKAGE_HPP

#include <vector>

#include "libdnf/hy-types.h"
#include "libdnf/hy-repo-private.hpp"

#include "Dependency.hpp"

namespace libdnf {

struct Package
{
public:
    Package(DnfSack *sack, Id id);
    Package(const Package &package);
    virtual ~Package();

    std::unique_ptr<DependencyContainer> getConflicts() const;
    std::unique_ptr<DependencyContainer> getEnhances() const;
    std::unique_ptr<DependencyContainer> getObsoletes() const;
    std::unique_ptr<DependencyContainer> getProvides() const;
    std::unique_ptr<DependencyContainer> getRecommends() const;
    std::unique_ptr<DependencyContainer> getRequires() const;
    std::unique_ptr<DependencyContainer> getRequiresPre() const;
    std::unique_ptr<DependencyContainer> getSuggests() const;
    std::unique_ptr<DependencyContainer> getSupplements() const;
    Id getId() const;

    const char *getName() const;
    const char *getEvr() const;
    const char *getArch() const;
    unsigned long getEpoch();

    std::string getBaseUrl();
    std::string getDebugName();
    std::string getSourceName();
    std::string getSourceDebugName();
    std::vector<std::string> getFiles();
    std::string getSourceRpm();
    std::string getVersion();
    std::string getRelease();
    const char * getRepoName();
    std::string getDescription();
    std::string getLicense();
    std::string getPackager();
    std::string getSummary();
    std::string getUrl();
    std::string getGroup();
    std::string getLocation();
    unsigned long long getBuildTime();
    unsigned long long downloadSize();
    unsigned long long getHeaderEnd();
    unsigned long long getInstallTime();
    unsigned long long getInstallSize();
    unsigned long long getMediaNumber();
    unsigned long long getRpmdbId();
    unsigned long long getSize();
    const unsigned char * getCheckSum(int *type);
    const unsigned char * getHdrCheckkSum(int *type);

    bool isInstalled() const;

protected:
    Package(DnfSack *sack, HyRepo repo, const char *name, const char *version, const char *arch, bool createSolvable = true);
    Package(DnfSack *sack, HyRepo repo, const std::string &name, const std::string &version, const std::string &arch, bool createSolvable = true);

    void addConflicts(std::shared_ptr<Dependency> dependency);
    void addEnhances(std::shared_ptr<Dependency> dependency);
    void addObsoletes(std::shared_ptr<Dependency> dependency);
    void addProvides(std::shared_ptr<Dependency> dependency);
    void addRecommends(std::shared_ptr<Dependency> dependency);
    void addRequires(std::shared_ptr<Dependency> dependency);
    void addRequiresPre(std::shared_ptr<Dependency> dependency);
    void addSuggests(std::shared_ptr<Dependency> dependency);
    void addSupplements(std::shared_ptr<Dependency> dependency);

    const char *getSolvableVendor() const;
    void setSolvableVendor(const char *vendor);

private:
    unsigned long long lookup_num(Id type);
    void createSolvable(HyRepo repo);
    void fillSolvableData(const char *name, const char *version, const char *arch) const;
    std::unique_ptr<DependencyContainer> getDependencies(Id type, Id marker = -1) const;
    void addDependency(std::shared_ptr<Dependency> dependency, int type, Id marker = -1);

    DnfSack *sack;
    Id id;
};

}

#endif //LIBDNF_PACKAGE_HPP

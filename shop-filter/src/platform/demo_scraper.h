#ifndef DEMO_SCRAPER_H
#define DEMO_SCRAPER_H

#include "platform/iscraper.h"
#include <memory>
#include <random>
#include <vector>

class DemoScraper : public IPlatformScraper {
public:
    explicit DemoScraper(const std::string& name);

    std::string platformName() const override;

    ProductList search(const std::string& keyword, int page = 1) override;

    ProductList search(const std::string& keyword,
                       double minPrice, double maxPrice,
                       int page = 1) override;

    Product getDetail(const std::string& productId) override;

private:
    std::string name_;
    std::mt19937 rng_;

    static const std::vector<std::string> namePrefixes_;
    static const std::vector<std::string> nameSuffixes_;
    static const std::vector<std::string> shops_;

    Product generateProduct(const std::string& keyword, int index);
    std::string generateId(int index);
};

class DemoScraperFactory {
public:
    std::vector<std::unique_ptr<IPlatformScraper>> createAll();
};

#endif

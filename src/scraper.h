#ifndef SCRAPER_H
#define SCRAPER_H

#include "product.h"
#include "platform/iscraper.h"
#include <memory>
#include <vector>
#include <functional>

class ScraperManager {
public:
    ScraperManager();

    void addScraper(std::unique_ptr<IPlatformScraper> scraper);

    const std::vector<std::unique_ptr<IPlatformScraper>>& scrapers() const;

    ProductList searchAll(const std::string& keyword, int maxPages = 3,
                          std::function<void(int, int)> progressCallback = nullptr);

    ProductList searchPlatform(const std::string& platform,
                                const std::string& keyword, int maxPages = 3);

private:
    std::vector<std::unique_ptr<IPlatformScraper>> scrapers_;
};

#endif

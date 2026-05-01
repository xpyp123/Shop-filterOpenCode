#include "scraper.h"
#include <algorithm>
#include <cctype>

ScraperManager::ScraperManager() {}

void ScraperManager::addScraper(std::unique_ptr<IPlatformScraper> scraper) {
    scrapers_.push_back(std::move(scraper));
}

const std::vector<std::unique_ptr<IPlatformScraper>>& ScraperManager::scrapers() const {
    return scrapers_;
}

ProductList ScraperManager::searchAll(const std::string& keyword, int maxPages,
                                       std::function<void(int, int)> progressCallback) {
    ProductList allResults;
    int totalSteps = static_cast<int>(scrapers_.size()) * maxPages;
    int currentStep = 0;

    for (auto& scraper : scrapers_) {
        int pages = std::min(maxPages, scraper->maxPages());
        for (int page = 1; page <= pages; ++page) {
            auto pageResults = scraper->search(keyword, page);
            allResults.insert(allResults.end(),
                              std::make_move_iterator(pageResults.begin()),
                              std::make_move_iterator(pageResults.end()));
            ++currentStep;
            if (progressCallback) {
                progressCallback(currentStep, totalSteps);
            }
        }
    }
    return allResults;
}

ProductList ScraperManager::searchPlatform(const std::string& platform,
                                            const std::string& keyword,
                                            int maxPages) {
    for (auto& scraper : scrapers_) {
        std::string nameLower = scraper->platformName();
        std::transform(nameLower.begin(), nameLower.end(), nameLower.begin(),
                       [](unsigned char c) { return std::tolower(c); });
        std::string pLower = platform;
        std::transform(pLower.begin(), pLower.end(), pLower.begin(),
                       [](unsigned char c) { return std::tolower(c); });

        if (nameLower.find(pLower) != std::string::npos) {
            ProductList results;
            int pages = std::min(maxPages, scraper->maxPages());
            for (int p = 1; p <= pages; ++p) {
                auto pr = scraper->search(keyword, p);
                results.insert(results.end(),
                               std::make_move_iterator(pr.begin()),
                               std::make_move_iterator(pr.end()));
            }
            return results;
        }
    }
    return {};
}

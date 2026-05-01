#ifndef ISCRAPER_H
#define ISCRAPER_H

#include "product.h"
#include <string>

class IPlatformScraper {
public:
    virtual ~IPlatformScraper() = default;

    virtual std::string platformName() const = 0;

    virtual ProductList search(const std::string& keyword, int page = 1) = 0;

    virtual ProductList search(const std::string& keyword,
                                double minPrice, double maxPrice,
                                int page = 1) = 0;

    virtual Product getDetail(const std::string& productId) = 0;

    virtual int maxPages() const { return 5; }
};

#endif

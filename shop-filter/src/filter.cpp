#include "filter.h"
#include <algorithm>
#include <cctype>

void ProductFilter::setPriceRange(double min, double max) {
    minPrice_ = min;
    maxPrice_ = max;
}

void ProductFilter::setMinRating(double r) { minRating_ = r; }
void ProductFilter::setMinReviews(int n) { minReviews_ = n; }
void ProductFilter::setFreeShippingOnly(bool v) { freeShippingOnly_ = v; }
void ProductFilter::setInStockOnly(bool v) { inStockOnly_ = v; }
void ProductFilter::setKeyword(const std::string& kw) { keyword_ = kw; }
void ProductFilter::setPlatform(const std::string& p) { platform_ = p; }
void ProductFilter::setSortMode(SortMode m) { sortMode_ = m; }

void ProductFilter::clearAll() {
    minPrice_ = -1.0;
    maxPrice_ = -1.0;
    minRating_ = 0.0;
    minReviews_ = 0;
    freeShippingOnly_ = false;
    inStockOnly_ = true;
    keyword_.clear();
    platform_.clear();
    sortMode_ = SortMode::Comprehensive;
}

bool ProductFilter::matchesFilters(const Product& p) const {
    if (minPrice_ >= 0 && p.price < minPrice_) return false;
    if (maxPrice_ >= 0 && p.price > maxPrice_) return false;
    if (p.rating < minRating_) return false;
    if (p.reviewCount < minReviews_) return false;
    if (freeShippingOnly_ && !p.freeShipping) return false;
    if (inStockOnly_ && !p.inStock) return false;

    if (!platform_.empty()) {
        std::string pLower = p.platform;
        std::string fLower = platform_;
        std::transform(pLower.begin(), pLower.end(), pLower.begin(),
                       [](unsigned char c) { return std::tolower(c); });
        std::transform(fLower.begin(), fLower.end(), fLower.begin(),
                       [](unsigned char c) { return std::tolower(c); });
        if (pLower.find(fLower) == std::string::npos) return false;
    }

    if (!keyword_.empty()) {
        std::string nameLower = p.name;
        std::string kwLower = keyword_;
        std::transform(nameLower.begin(), nameLower.end(), nameLower.begin(),
                       [](unsigned char c) { return std::tolower(c); });
        std::transform(kwLower.begin(), kwLower.end(), kwLower.begin(),
                       [](unsigned char c) { return std::tolower(c); });
        if (nameLower.find(kwLower) == std::string::npos) return false;
    }

    return true;
}

ProductList ProductFilter::apply(const ProductList& products) const {
    ProductList result;

    for (const auto& p : products) {
        if (matchesFilters(p)) {
            result.push_back(p);
        }
    }

    std::sort(result.begin(), result.end(),
              [this](const Product& a, const Product& b) {
        switch (sortMode_) {
        case SortMode::Comprehensive:
            return a.comprehensiveScore() > b.comprehensiveScore();
        case SortMode::PriceAsc:
            return a.price < b.price;
        case SortMode::PriceDesc:
            return a.price > b.price;
        case SortMode::RatingDesc:
            if (a.rating != b.rating) return a.rating > b.rating;
            return a.reviewCount > b.reviewCount;
        case SortMode::ReviewsDesc:
            return a.reviewCount > b.reviewCount;
        case SortMode::SalesDesc:
            return a.salesCount > b.salesCount;
        case SortMode::DiscountDesc:
            return a.discountPercent() > b.discountPercent();
        default:
            return a.comprehensiveScore() > b.comprehensiveScore();
        }
    });

    return result;
}

#ifndef FILTER_H
#define FILTER_H

#include "product.h"
#include <functional>
#include <string>

enum class SortMode {
    Comprehensive,
    PriceAsc,
    PriceDesc,
    RatingDesc,
    ReviewsDesc,
    SalesDesc,
    DiscountDesc
};

class ProductFilter {
public:
    void setPriceRange(double minPrice, double maxPrice);
    void setMinRating(double minRating);
    void setMinReviews(int minReviews);
    void setFreeShippingOnly(bool only);
    void setInStockOnly(bool only);
    void setKeyword(const std::string& keyword);
    void setPlatform(const std::string& platform);
    void setSortMode(SortMode mode);

    void clearAll();

    ProductList apply(const ProductList& products) const;

private:
    double minPrice_ = -1.0;
    double maxPrice_ = -1.0;
    double minRating_ = 0.0;
    int minReviews_ = 0;
    bool freeShippingOnly_ = false;
    bool inStockOnly_ = true;
    std::string keyword_;
    std::string platform_;
    SortMode sortMode_ = SortMode::Comprehensive;

    bool matchesFilters(const Product& p) const;
};

#endif

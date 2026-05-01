#ifndef PRODUCT_H
#define PRODUCT_H

#include <string>
#include <vector>

struct Product {
    std::string id;
    std::string name;
    std::string platform;
    std::string url;
    std::string imageUrl;
    std::string shopName;

    double price = 0.0;
    double originalPrice = 0.0;
    double rating = 0.0;
    int reviewCount = 0;
    int salesCount = 0;
    bool freeShipping = false;
    bool inStock = true;

    double discountPercent() const {
        if (originalPrice > 0.0) {
            return (1.0 - price / originalPrice) * 100.0;
        }
        return 0.0;
    }

    double comprehensiveScore() const {
        double priceScore = 1.0 / (1.0 + price / 100.0);
        double ratingScore = rating / 5.0;
        double reviewScore = std::min(reviewCount / 10000.0, 1.0);
        double discountScore = std::min(discountPercent() / 50.0, 1.0);
        return priceScore * 0.35 + ratingScore * 0.30 +
               reviewScore * 0.20 + discountScore * 0.15;
    }
};

using ProductList = std::vector<Product>;

#endif

#include "platform/demo_scraper.h"
#include <chrono>
#include <memory>
#include <sstream>

const std::vector<std::string> DemoScraper::namePrefixes_ = {
    "2024新款", "正品", "官方", "旗舰", "高端", "经典", "爆款", "热销",
    "限时特惠", "超值", "品质", "轻奢", "舒适", "智能", "便携", "时尚",
    "韩版", "简约", "复古", "ins风", "日系", "国潮", "大容量", "多功能"
};

const std::vector<std::string> DemoScraper::nameSuffixes_ = {
    "限时包邮", "买一送一", "7天无理由", "赠运费险", "48小时发货",
    "正品保障", "30天价保", "顺丰包邮", "质保3年", "全国联保"
};

const std::vector<std::string> DemoScraper::shops_ = {
    "官方旗舰店", "品牌直营店", "优选好货", "全球购直营",
    "某某数码专营店", "某某服饰旗舰店", "某某家居馆", "某某美妆店"
};

DemoScraper::DemoScraper(const std::string& name) : name_(name) {
    auto seed = std::chrono::steady_clock::now().time_since_epoch().count();
    rng_.seed(static_cast<unsigned int>(seed));
}

std::string DemoScraper::platformName() const { return name_; }

std::string DemoScraper::generateId(int index) {
    std::ostringstream oss;
    oss << name_ << "_" << index;
    return oss.str();
}

Product DemoScraper::generateProduct(const std::string& keyword, int index) {
    Product p;
    p.id = generateId(index);
    p.platform = name_;

    auto randInt = [this](int lo, int hi) {
        std::uniform_int_distribution<int> dist(lo, hi);
        return dist(rng_);
    };

    auto randDouble = [this](double lo, double hi) {
        std::uniform_real_distribution<double> dist(lo, hi);
        return dist(rng_);
    };

    std::string prefix = namePrefixes_[randInt(0, namePrefixes_.size() - 1)];
    std::string suffix = nameSuffixes_[randInt(0, nameSuffixes_.size() - 1)];
    p.name = prefix + " " + keyword + " " + suffix;

    double basePrice = randDouble(29.9, 2999.0);
    p.originalPrice = basePrice * randDouble(1.0, 2.5);

    bool onSale = randInt(0, 100) < 65;
    p.price = onSale ? basePrice * randDouble(0.35, 0.88) : basePrice;

    p.price = std::round(p.price * 100.0) / 100.0;
    p.originalPrice = std::round(p.originalPrice * 100.0) / 100.0;
    if (!onSale) p.originalPrice = p.price;

    p.rating = std::round(randDouble(2.5, 5.0) * 10.0) / 10.0;

    p.reviewCount = randInt(10, 50000);

    p.salesCount = randInt(p.reviewCount, p.reviewCount * 3 + 100);

    p.freeShipping = randInt(0, 100) < 55;

    p.inStock = randInt(0, 100) < 92;

    p.shopName = shops_[randInt(0, shops_.size() - 1)];

    p.url = "https://www." + name_ + ".com/item/" + p.id + ".html";
    p.imageUrl = "https://img." + name_ + ".com/product/" + p.id + ".jpg";

    return p;
}

ProductList DemoScraper::search(const std::string& keyword, int page) {
    ProductList results;
    int count = (page == 1) ? 20 : 15;
    int startIndex = (page - 1) * 20;

    for (int i = 0; i < count; ++i) {
        results.push_back(generateProduct(keyword, startIndex + i + 1));
    }
    return results;
}

ProductList DemoScraper::search(const std::string& keyword,
                                 double minPrice, double maxPrice,
                                 int page) {
    ProductList all = search(keyword, page);
    ProductList filtered;
    for (auto& p : all) {
        if (p.price >= minPrice && p.price <= maxPrice) {
            filtered.push_back(std::move(p));
        }
    }
    return filtered;
}

Product DemoScraper::getDetail(const std::string& productId) {
    Product p;
    p.id = productId;
    p.platform = name_;
    p.name = "Product " + productId;
    p.price = 99.99;
    p.originalPrice = 149.99;
    p.rating = 4.5;
    p.reviewCount = 1000;
    p.salesCount = 5000;
    p.freeShipping = true;
    p.inStock = true;
    p.shopName = shops_[0];
    return p;
}

std::vector<std::unique_ptr<IPlatformScraper>> DemoScraperFactory::createAll() {
    std::vector<std::unique_ptr<IPlatformScraper>> scrapers;
    scrapers.push_back(std::make_unique<DemoScraper>("淘宝 (Demo)"));
    scrapers.push_back(std::make_unique<DemoScraper>("京东 (Demo)"));
    scrapers.push_back(std::make_unique<DemoScraper>("拼多多 (Demo)"));
    scrapers.push_back(std::make_unique<DemoScraper>("亚马逊 (Demo)"));
    return scrapers;
}

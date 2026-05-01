#include "mainwindow.h"
#include "platform/demo_scraper.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QGridLayout>
#include <QFormLayout>
#include <QHeaderView>
#include <QMessageBox>
#include <QApplication>

#include <algorithm>
#include <sstream>
#include <iomanip>

SearchWorker::SearchWorker(ScraperManager* mgr, const std::string& keyword,
                           int maxPages, QObject* parent)
    : QObject(parent), mgr_(mgr), keyword_(keyword), maxPages_(maxPages) {}

void SearchWorker::run() {
    try {
        auto results = mgr_->searchAll(keyword_, maxPages_,
            [this](int cur, int total) {
                emit progress(cur, total);
            });
        emit finished(std::move(results));
    } catch (const std::exception& e) {
        emit error(QString::fromStdString(e.what()));
    }
}

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), workerThread_(nullptr), worker_(nullptr) {
    setWindowTitle("ShopFilter - 智能商品筛选器");
    resize(1200, 750);

    DemoScraperFactory factory;
    for (auto& s : factory.createAll()) {
        scraperManager_.addScraper(std::move(s));
    }

    setupUI();
    setupConnections();
}

MainWindow::~MainWindow() {
    if (workerThread_ && workerThread_->isRunning()) {
        workerThread_->quit();
        workerThread_->wait();
    }
}

void MainWindow::setupUI() {
    auto* central = new QWidget(this);
    setCentralWidget(central);
    auto* mainLayout = new QVBoxLayout(central);
    mainLayout->setSpacing(10);

    auto* searchGroup = new QGroupBox("搜索设置");
    auto* searchLayout = new QHBoxLayout(searchGroup);
    searchLayout->setSpacing(10);

    auto* keywordLabel = new QLabel("关键词:");
    keywordLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    searchLayout->addWidget(keywordLabel);
    keywordEdit_ = new QLineEdit();
    keywordEdit_->setPlaceholderText("输入商品关键词...");
    keywordEdit_->setMinimumWidth(200);
    searchLayout->addWidget(keywordEdit_);

    auto* platformLabel = new QLabel("平台:");
    platformLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    searchLayout->addWidget(platformLabel);
    platformCombo_ = new QComboBox();
    platformCombo_->addItem("全部平台");
    for (auto& s : scraperManager_.scrapers()) {
        platformCombo_->addItem(QString::fromStdString(s->platformName()));
    }
    searchLayout->addWidget(platformCombo_);

    searchBtn_ = new QPushButton("🔍 搜索");
    searchBtn_->setMinimumWidth(100);
    searchBtn_->setStyleSheet(
        "QPushButton { background-color: #3b82f6; color: white; font-weight: bold; "
        "padding: 8px 20px; border-radius: 6px; }"
        "QPushButton:hover { background-color: #2563eb; }"
        "QPushButton:pressed { background-color: #1d4ed8; }"
        "QPushButton:disabled { background-color: #93c5fd; color: #e5e7eb; }");
    searchLayout->addWidget(searchBtn_);
    searchLayout->addStretch();

    mainLayout->addWidget(searchGroup);

    auto* filterGroup = new QGroupBox("筛选条件");
    auto* filterLayout = new QGridLayout(filterGroup);
    filterLayout->setSpacing(10);

    auto* sortLabel = new QLabel("排序方式:");
    sortLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    filterLayout->addWidget(sortLabel, 0, 0);
    sortCombo_ = new QComboBox();
    sortCombo_->addItem("综合推荐", static_cast<int>(SortMode::Comprehensive));
    sortCombo_->addItem("价格从低到高", static_cast<int>(SortMode::PriceAsc));
    sortCombo_->addItem("价格从高到低", static_cast<int>(SortMode::PriceDesc));
    sortCombo_->addItem("评分最高", static_cast<int>(SortMode::RatingDesc));
    sortCombo_->addItem("评价最多", static_cast<int>(SortMode::ReviewsDesc));
    sortCombo_->addItem("销量最高", static_cast<int>(SortMode::SalesDesc));
    sortCombo_->addItem("折扣最大", static_cast<int>(SortMode::DiscountDesc));
    filterLayout->addWidget(sortCombo_, 0, 1);

    auto* priceLabel = new QLabel("价格区间:");
    priceLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    filterLayout->addWidget(priceLabel, 0, 2);
    auto* priceLayout = new QHBoxLayout();
    minPriceSpin_ = new QDoubleSpinBox();
    minPriceSpin_->setRange(0, 999999);
    minPriceSpin_->setPrefix("¥");
    minPriceSpin_->setDecimals(2);
    minPriceSpin_->setFixedWidth(110);
    maxPriceSpin_ = new QDoubleSpinBox();
    maxPriceSpin_->setRange(0, 999999);
    maxPriceSpin_->setValue(9999);
    maxPriceSpin_->setPrefix("¥");
    maxPriceSpin_->setDecimals(2);
    maxPriceSpin_->setFixedWidth(110);
    priceLayout->addWidget(minPriceSpin_);
    priceLayout->addWidget(new QLabel(" ~ "));
    priceLayout->addWidget(maxPriceSpin_);
    filterLayout->addLayout(priceLayout, 0, 3);

    auto* ratingLabel = new QLabel("最低评分:");
    ratingLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    filterLayout->addWidget(ratingLabel, 0, 4);
    minRatingSpin_ = new QDoubleSpinBox();
    minRatingSpin_->setRange(0, 5);
    minRatingSpin_->setSingleStep(0.1);
    minRatingSpin_->setDecimals(1);
    minRatingSpin_->setFixedWidth(90);
    filterLayout->addWidget(minRatingSpin_, 0, 5);

    auto* reviewsLabel = new QLabel("最低评价数:");
    reviewsLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    filterLayout->addWidget(reviewsLabel, 1, 0);
    minReviewsSpin_ = new QSpinBox();
    minReviewsSpin_->setRange(0, 999999);
    minReviewsSpin_->setSingleStep(100);
    minReviewsSpin_->setFixedWidth(90);
    filterLayout->addWidget(minReviewsSpin_, 1, 1);

    freeShippingCheck_ = new QCheckBox("仅包邮");
    filterLayout->addWidget(freeShippingCheck_, 1, 2);
    inStockCheck_ = new QCheckBox("仅看有货");
    inStockCheck_->setChecked(true);
    filterLayout->addWidget(inStockCheck_, 1, 3);

    filterBtn_ = new QPushButton("应用筛选");
    filterBtn_->setMinimumWidth(90);
    filterLayout->addWidget(filterBtn_, 1, 4);

    resetBtn_ = new QPushButton("↺ 重置");
    resetBtn_->setMinimumWidth(80);
    resetBtn_->setStyleSheet(
        "QPushButton { background-color: #e5e7eb; color: #4b5563; font-weight: 600; "
        "padding: 8px 16px; border-radius: 6px; }"
        "QPushButton:hover { background-color: #d1d5db; }"
        "QPushButton:pressed { background-color: #9ca3af; }");
    filterLayout->addWidget(resetBtn_, 1, 5);

    mainLayout->addWidget(filterGroup);

    table_ = new QTableWidget(this);
    table_->setColumnCount(10);
    table_->setHorizontalHeaderLabels({
        "序号", "商品名称", "价格(¥)", "原价(¥)", "折扣", "评分",
        "评价数", "销量", "包邮", "平台"
    });
    table_->setSelectionBehavior(QAbstractItemView::SelectRows);
    table_->setSelectionMode(QAbstractItemView::SingleSelection);
    table_->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table_->setAlternatingRowColors(true);
    table_->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    table_->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    table_->setColumnWidth(2, 90);
    table_->setColumnWidth(3, 90);
    table_->setColumnWidth(4, 70);
    table_->setColumnWidth(5, 60);
    table_->setColumnWidth(6, 80);
    table_->setColumnWidth(7, 80);
    table_->setColumnWidth(8, 50);
    table_->setColumnWidth(9, 110);
    table_->verticalHeader()->setVisible(false);

    mainLayout->addWidget(table_, 1);

    auto* statusLayout = new QHBoxLayout();
    statusLabel_ = new QLabel("就绪 — 输入关键词开始搜索");
    statusLayout->addWidget(statusLabel_);
    statusLayout->addStretch();
    progressBar_ = new QProgressBar();
    progressBar_->setMaximumWidth(300);
    progressBar_->setVisible(false);
    statusLayout->addWidget(progressBar_);
    mainLayout->addLayout(statusLayout);
}

void MainWindow::setupConnections() {
    connect(searchBtn_, &QPushButton::clicked, this, &MainWindow::onSearch);
    connect(filterBtn_, &QPushButton::clicked, this, &MainWindow::onFilter);
    connect(resetBtn_, &QPushButton::clicked, this, [this]() {
        filter_.clearAll();
        minPriceSpin_->setValue(0);
        maxPriceSpin_->setValue(9999);
        minRatingSpin_->setValue(0);
        minReviewsSpin_->setValue(0);
        freeShippingCheck_->setChecked(false);
        inStockCheck_->setChecked(true);
        sortCombo_->setCurrentIndex(0);
        displayProducts(currentProducts_);
    });
    connect(keywordEdit_, &QLineEdit::returnPressed, this, &MainWindow::onSearch);
}

void MainWindow::onSearch() {
    QString kw = keywordEdit_->text().trimmed();
    if (kw.isEmpty()) {
        QMessageBox::warning(this, "提示", "请输入搜索关键词");
        return;
    }

    searchBtn_->setEnabled(false);
    progressBar_->setVisible(true);
    progressBar_->setValue(0);
    statusLabel_->setText("正在搜索，请稍候...");

    workerThread_ = new QThread(this);
    worker_ = new SearchWorker(&scraperManager_, kw.toStdString(), 3);
    worker_->moveToThread(workerThread_);

    connect(workerThread_, &QThread::started, worker_, &SearchWorker::run);
    connect(worker_, &SearchWorker::progress,
            this, &MainWindow::onSearchProgress);
    connect(worker_, &SearchWorker::finished,
            this, &MainWindow::onSearchFinished);
    connect(worker_, &SearchWorker::error,
            this, &MainWindow::onSearchError);
    connect(worker_, &SearchWorker::finished, workerThread_, &QThread::quit);
    connect(worker_, &SearchWorker::error, workerThread_, &QThread::quit);
    connect(workerThread_, &QThread::finished, worker_, &QObject::deleteLater);

    workerThread_->start();
}

void MainWindow::onSearchProgress(int current, int total) {
    progressBar_->setMaximum(total);
    progressBar_->setValue(current);
    statusLabel_->setText(
        QString("正在搜索... %1/%2").arg(current).arg(total));
}

void MainWindow::onSearchFinished(ProductList results) {
    currentProducts_ = std::move(results);

    displayProducts(currentProducts_);

    searchBtn_->setEnabled(true);
    progressBar_->setVisible(false);
    updateStatusLabel();
}

void MainWindow::onSearchError(const QString& msg) {
    searchBtn_->setEnabled(true);
    progressBar_->setVisible(false);
    statusLabel_->setText("搜索失败，请重试");
    QMessageBox::critical(this, "错误", msg);
}

void MainWindow::onFilter() {
    filter_.setSortMode(static_cast<SortMode>(
        sortCombo_->currentData().toInt()));
    filter_.setPriceRange(minPriceSpin_->value(), maxPriceSpin_->value());
    filter_.setMinRating(minRatingSpin_->value());
    filter_.setMinReviews(minReviewsSpin_->value());
    filter_.setFreeShippingOnly(freeShippingCheck_->isChecked());
    filter_.setInStockOnly(inStockCheck_->isChecked());

    if (platformCombo_->currentIndex() > 0) {
        filter_.setPlatform(platformCombo_->currentText().toStdString());
    } else {
        filter_.setPlatform("");
    }

    filteredProducts_ = filter_.apply(currentProducts_);
    displayProducts(filteredProducts_);
    updateStatusLabel();
}

void MainWindow::displayProducts(const ProductList& products) {
    table_->setRowCount(static_cast<int>(products.size()));

    for (size_t i = 0; i < products.size(); ++i) {
        const auto& p = products[i];
        int row = static_cast<int>(i);

        auto* idxItem = new QTableWidgetItem(QString::number(i + 1));
        idxItem->setTextAlignment(Qt::AlignCenter);
        table_->setItem(row, 0, idxItem);

        table_->setItem(row, 1, new QTableWidgetItem(
            QString::fromStdString(p.name)));

        auto* priceItem = new QTableWidgetItem(
            QString::number(p.price, 'f', 2));
        priceItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        priceItem->setForeground(QColor("#e74c3c"));
        priceItem->setFont(QFont(priceItem->font().family(), -1, QFont::Bold));
        table_->setItem(row, 2, priceItem);

        auto* origPriceItem = new QTableWidgetItem(
            p.discountPercent() > 0.5
                ? QString::number(p.originalPrice, 'f', 2)
                : "");
        origPriceItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        if (p.discountPercent() > 0.5) {
            origPriceItem->setForeground(QColor("#95a5a6"));
            origPriceItem->setFont(QFont(origPriceItem->font().family(), -1, -1, true));
        }
        table_->setItem(row, 3, origPriceItem);

        auto* discountItem = new QTableWidgetItem(
            p.discountPercent() > 0.5
                ? QString("-%1%").arg(static_cast<int>(p.discountPercent()))
                : "");
        discountItem->setTextAlignment(Qt::AlignCenter);
        if (p.discountPercent() > 30) {
            discountItem->setForeground(QColor("#e74c3c"));
        } else if (p.discountPercent() > 0.5) {
            discountItem->setForeground(QColor("#e67e22"));
        }
        table_->setItem(row, 4, discountItem);

        auto* ratingItem = new QTableWidgetItem(
            QString::number(p.rating, 'f', 1));
        ratingItem->setTextAlignment(Qt::AlignCenter);
        if (p.rating >= 4.5) {
            ratingItem->setForeground(QColor("#27ae60"));
            ratingItem->setFont(QFont(ratingItem->font().family(), -1, QFont::Bold));
        } else if (p.rating >= 4.0) {
            ratingItem->setForeground(QColor("#2ecc71"));
        }
        table_->setItem(row, 5, ratingItem);

        auto* reviewsItem = new QTableWidgetItem(
            p.reviewCount >= 10000
                ? QString::number(p.reviewCount / 10000) + "万+"
                : QString::number(p.reviewCount));
        reviewsItem->setTextAlignment(Qt::AlignCenter);
        table_->setItem(row, 6, reviewsItem);

        auto* salesItem = new QTableWidgetItem(
            p.salesCount >= 10000
                ? QString::number(p.salesCount / 10000) + "万+"
                : QString::number(p.salesCount));
        salesItem->setTextAlignment(Qt::AlignCenter);
        table_->setItem(row, 7, salesItem);

        auto* shippingItem = new QTableWidgetItem(
            p.freeShipping ? "✓" : "");
        shippingItem->setTextAlignment(Qt::AlignCenter);
        if (p.freeShipping) {
            shippingItem->setForeground(QColor("#27ae60"));
        }
        table_->setItem(row, 8, shippingItem);

        table_->setItem(row, 9, new QTableWidgetItem(
            QString::fromStdString(p.platform)));
    }
}

void MainWindow::updateStatusLabel() {
    statusLabel_->setText(
        QString("共找到 %1 条商品，当前显示 %2 条")
            .arg(currentProducts_.size())
            .arg(table_->rowCount()));
}

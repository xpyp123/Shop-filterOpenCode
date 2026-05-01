#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QDoubleSpinBox>
#include <QSpinBox>
#include <QCheckBox>
#include <QTableWidget>
#include <QProgressBar>
#include <QLabel>
#include <QStatusBar>
#include <QThread>
#include <QTimer>

#include "scraper.h"
#include "filter.h"

class SearchWorker : public QObject {
    Q_OBJECT
public:
    explicit SearchWorker(ScraperManager* mgr, const std::string& keyword,
                          int maxPages, QObject* parent = nullptr);

public slots:
    void run();

signals:
    void progress(int current, int total);
    void finished(ProductList results);
    void error(const QString& message);

private:
    ScraperManager* mgr_;
    std::string keyword_;
    int maxPages_;
};

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void onSearch();
    void onFilter();
    void onSearchProgress(int current, int total);
    void onSearchFinished(ProductList results);
    void onSearchError(const QString& msg);

private:
    void setupUI();
    void setupConnections();
    void displayProducts(const ProductList& products);
    void updateStatusLabel();

    ScraperManager scraperManager_;
    ProductFilter filter_;
    ProductList currentProducts_;
    ProductList filteredProducts_;

    QLineEdit* keywordEdit_;
    QComboBox* platformCombo_;
    QComboBox* sortCombo_;
    QDoubleSpinBox* minPriceSpin_;
    QDoubleSpinBox* maxPriceSpin_;
    QDoubleSpinBox* minRatingSpin_;
    QSpinBox* minReviewsSpin_;
    QCheckBox* freeShippingCheck_;
    QCheckBox* inStockCheck_;
    QPushButton* searchBtn_;
    QPushButton* filterBtn_;
    QPushButton* resetBtn_;
    QTableWidget* table_;
    QProgressBar* progressBar_;
    QLabel* statusLabel_;

    QThread* workerThread_;
    SearchWorker* worker_;
};

#endif

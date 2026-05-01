#include <QApplication>
#include "mainwindow.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    app.setApplicationName("ShopFilter");
    app.setApplicationVersion("1.0.0");

    app.setStyleSheet(R"(
        * {
            font-family: "Microsoft YaHei", "Segoe UI", "PingFang SC",
                         "Noto Sans CJK SC", sans-serif;
        }

        QMainWindow {
            background-color: #f3f4f6;
        }
        QWidget {
            font-size: 13px;
            color: #4b5563;
        }

        QGroupBox {
            background: #ffffff;
            border: 1px solid #d1d5db;
            border-radius: 6px;
            margin-top: 14px;
            padding: 20px 16px 12px 16px;
            font-weight: bold;
            font-size: 13px;
            color: #4b5563;
        }
        QGroupBox::title {
            subcontrol-origin: margin;
            left: 12px;
            padding: 0 6px;
            color: #6b7280;
            font-size: 13px;
        }

        QLineEdit {
            padding: 7px 12px;
            border: 1px solid #d1d5db;
            border-radius: 6px;
            background: #f9fafb;
            color: #4b5563;
            min-height: 16px;
            font-size: 13px;
        }
        QLineEdit:focus {
            border-color: #3b82f6;
            background: #ffffff;
        }
        QLineEdit::placeholder {
            color: #9ca3af;
        }

        QComboBox {
            padding: 7px 12px;
            border: 1px solid #d1d5db;
            border-radius: 6px;
            background: #f9fafb;
            color: #4b5563;
            min-height: 16px;
            min-width: 100px;
        }
        QComboBox:focus {
            border-color: #3b82f6;
        }
        QComboBox:hover {
            border-color: #9ca3af;
        }
        QComboBox::drop-down {
            border: none;
            width: 24px;
            subcontrol-position: right center;
            subcontrol-origin: padding;
        }
        QComboBox QAbstractItemView {
            border: 1px solid #d1d5db;
            border-radius: 6px;
            background: #ffffff;
            selection-background-color: #eff6ff;
            selection-color: #4b5563;
            outline: none;
            padding: 4px;
        }

        QSpinBox, QDoubleSpinBox {
            padding: 7px 12px;
            border: 1px solid #d1d5db;
            border-radius: 6px;
            background: #f9fafb;
            color: #4b5563;
            min-height: 16px;
        }
        QSpinBox:focus, QDoubleSpinBox:focus {
            border-color: #3b82f6;
            background: #ffffff;
        }
        QSpinBox:hover, QDoubleSpinBox:hover {
            border-color: #9ca3af;
        }

        QPushButton {
            padding: 8px 20px;
            border: none;
            border-radius: 6px;
            font-weight: 600;
            font-size: 13px;
            background: #e5e7eb;
            color: #4b5563;
        }
        QPushButton:hover {
            background: #d1d5db;
        }
        QPushButton:pressed {
            background: #9ca3af;
        }
        QPushButton:disabled {
            background: #e5e7eb;
            color: #9ca3af;
        }

        QCheckBox {
            spacing: 8px;
            color: #4b5563;
        }
        QCheckBox::indicator {
            width: 18px;
            height: 18px;
            border: 1px solid #d1d5db;
            border-radius: 4px;
            background: #ffffff;
        }
        QCheckBox::indicator:checked {
            background: #3b82f6;
            border-color: #3b82f6;
        }
        QCheckBox::indicator:hover {
            border-color: #9ca3af;
        }

        QTableWidget {
            background: #ffffff;
            border: 1px solid #d1d5db;
            border-radius: 6px;
            gridline-color: #e5e7eb;
            outline: none;
            alternate-background-color: #f9fafb;
        }
        QTableWidget::item {
            padding: 8px 12px;
        }
        QTableWidget::item:selected {
            background-color: #eff6ff;
            color: #4b5563;
        }
        QHeaderView::section {
            background: #f3f4f6;
            border: none;
            border-bottom: 1px solid #d1d5db;
            border-right: 1px solid #e5e7eb;
            padding: 10px 8px;
            font-weight: 700;
            font-size: 12px;
            color: #6b7280;
        }

        QProgressBar {
            border: none;
            border-radius: 4px;
            background: #e5e7eb;
            text-align: center;
            height: 8px;
            font-size: 11px;
        }
        QProgressBar::chunk {
            background: #3b82f6;
            border-radius: 4px;
        }

        QScrollBar:vertical {
            border: none;
            background: transparent;
            width: 8px;
            margin: 4px 0;
        }
        QScrollBar::handle:vertical {
            background: #d1d5db;
            border-radius: 4px;
            min-height: 32px;
        }
        QScrollBar::handle:vertical:hover {
            background: #9ca3af;
        }
        QScrollBar::add-line:vertical,
        QScrollBar::sub-line:vertical {
            height: 0;
        }

        QStatusBar {
            background: transparent;
            border: none;
            color: #9ca3af;
            font-size: 12px;
        }

        QLabel {
            color: #4b5563;
        }
    )");

    MainWindow window;
    window.show();

    return app.exec();
}

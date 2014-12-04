/********************************************************************************
** Form generated from reading UI file 'vschddoneedit.ui'
**
** Created by: Qt User Interface Compiler version 5.4.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VSCHDDONEEDIT_H
#define UI_VSCHDDONEEDIT_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_VSCHddOneEdit
{
public:
    QHBoxLayout *horizontalLayout;
    QPushButton *pushButton;
    QVBoxLayout *verticalLayout;
    QSlider *horizontalSlider;
    QTableWidget *tableWidget;
    QCheckBox *checkBox;

    void setupUi(QWidget *VSCHddOneEdit)
    {
        if (VSCHddOneEdit->objectName().isEmpty())
            VSCHddOneEdit->setObjectName(QStringLiteral("VSCHddOneEdit"));
        VSCHddOneEdit->resize(700, 97);
        VSCHddOneEdit->setMinimumSize(QSize(700, 97));
        VSCHddOneEdit->setMaximumSize(QSize(700, 97));
        QIcon icon;
        icon.addFile(QStringLiteral(":/logo/resources/vscsmall.jpg"), QSize(), QIcon::Normal, QIcon::Off);
        VSCHddOneEdit->setWindowIcon(icon);
        horizontalLayout = new QHBoxLayout(VSCHddOneEdit);
        horizontalLayout->setSpacing(15);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(10, 0, 10, 0);
        pushButton = new QPushButton(VSCHddOneEdit);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setMinimumSize(QSize(64, 64));
        pushButton->setMaximumSize(QSize(64, 64));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/device/resources/harddisk.png"), QSize(), QIcon::Normal, QIcon::Off);
        pushButton->setIcon(icon1);
        pushButton->setIconSize(QSize(65, 65));

        horizontalLayout->addWidget(pushButton);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalSlider = new QSlider(VSCHddOneEdit);
        horizontalSlider->setObjectName(QStringLiteral("horizontalSlider"));
        horizontalSlider->setMinimumSize(QSize(550, 0));
        horizontalSlider->setMaximumSize(QSize(550, 16777215));
        horizontalSlider->setStyleSheet(QLatin1String("QSlider::groove:horizontal {\n"
"border: 1px solid #999999;\n"
"height: 8px; /* the groove expands to the size of the slider by default. by giving it a height, it has a fixed size */\n"
"background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #B1B1B1, stop:1 #c4c4c4);\n"
"}\n"
"QSlider::handle:horizontal {\n"
"background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #b4b4b4, stop:1 #8f8f8f);\n"
"border: 1px solid #5c5c5c;\n"
"width: 18px;\n"
"margin: -2px 0; /* handle is placed by default on the contents rect of the groove. Expand outside the groove */\n"
"border-radius: 3px;\n"
"}"));
        horizontalSlider->setOrientation(Qt::Horizontal);

        verticalLayout->addWidget(horizontalSlider);

        tableWidget = new QTableWidget(VSCHddOneEdit);
        if (tableWidget->columnCount() < 5)
            tableWidget->setColumnCount(5);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(4, __qtablewidgetitem4);
        if (tableWidget->rowCount() < 1)
            tableWidget->setRowCount(1);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        tableWidget->setVerticalHeaderItem(0, __qtablewidgetitem5);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        tableWidget->setItem(0, 0, __qtablewidgetitem6);
        QTableWidgetItem *__qtablewidgetitem7 = new QTableWidgetItem();
        tableWidget->setItem(0, 1, __qtablewidgetitem7);
        QTableWidgetItem *__qtablewidgetitem8 = new QTableWidgetItem();
        tableWidget->setItem(0, 2, __qtablewidgetitem8);
        QTableWidgetItem *__qtablewidgetitem9 = new QTableWidgetItem();
        tableWidget->setItem(0, 3, __qtablewidgetitem9);
        QTableWidgetItem *__qtablewidgetitem10 = new QTableWidgetItem();
        tableWidget->setItem(0, 4, __qtablewidgetitem10);
        tableWidget->setObjectName(QStringLiteral("tableWidget"));
        tableWidget->setEnabled(true);
        tableWidget->setMinimumSize(QSize(550, 70));
        tableWidget->setMaximumSize(QSize(550, 70));
        tableWidget->setStyleSheet(QStringLiteral("font: 11pt \"Arial\";"));
        tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
        tableWidget->setDragEnabled(false);
        tableWidget->setAlternatingRowColors(false);
        tableWidget->setShowGrid(true);
        tableWidget->setGridStyle(Qt::SolidLine);
        tableWidget->setWordWrap(true);
        tableWidget->setCornerButtonEnabled(false);
        tableWidget->verticalHeader()->setVisible(true);
        tableWidget->verticalHeader()->setCascadingSectionResizes(false);
        tableWidget->verticalHeader()->setProperty("showSortIndicator", QVariant(false));
        tableWidget->verticalHeader()->setStretchLastSection(true);

        verticalLayout->addWidget(tableWidget);


        horizontalLayout->addLayout(verticalLayout);

        checkBox = new QCheckBox(VSCHddOneEdit);
        checkBox->setObjectName(QStringLiteral("checkBox"));

        horizontalLayout->addWidget(checkBox);


        retranslateUi(VSCHddOneEdit);

        QMetaObject::connectSlotsByName(VSCHddOneEdit);
    } // setupUi

    void retranslateUi(QWidget *VSCHddOneEdit)
    {
        VSCHddOneEdit->setWindowTitle(QApplication::translate("VSCHddOneEdit", "Form", 0));
        pushButton->setText(QString());
        QTableWidgetItem *___qtablewidgetitem = tableWidget->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("VSCHddOneEdit", "Name", 0));
        QTableWidgetItem *___qtablewidgetitem1 = tableWidget->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("VSCHddOneEdit", "Total", 0));
        QTableWidgetItem *___qtablewidgetitem2 = tableWidget->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QApplication::translate("VSCHddOneEdit", "Free", 0));
        QTableWidgetItem *___qtablewidgetitem3 = tableWidget->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QApplication::translate("VSCHddOneEdit", "Limit", 0));
        QTableWidgetItem *___qtablewidgetitem4 = tableWidget->horizontalHeaderItem(4);
        ___qtablewidgetitem4->setText(QApplication::translate("VSCHddOneEdit", "Recording", 0));

        const bool __sortingEnabled = tableWidget->isSortingEnabled();
        tableWidget->setSortingEnabled(false);
        QTableWidgetItem *___qtablewidgetitem5 = tableWidget->item(0, 0);
        ___qtablewidgetitem5->setText(QApplication::translate("VSCHddOneEdit", "C:\\", 0));
        QTableWidgetItem *___qtablewidgetitem6 = tableWidget->item(0, 1);
        ___qtablewidgetitem6->setText(QApplication::translate("VSCHddOneEdit", "1T", 0));
        QTableWidgetItem *___qtablewidgetitem7 = tableWidget->item(0, 2);
        ___qtablewidgetitem7->setText(QApplication::translate("VSCHddOneEdit", "80G", 0));
        QTableWidgetItem *___qtablewidgetitem8 = tableWidget->item(0, 3);
        ___qtablewidgetitem8->setText(QApplication::translate("VSCHddOneEdit", "1000G", 0));
        QTableWidgetItem *___qtablewidgetitem9 = tableWidget->item(0, 4);
        ___qtablewidgetitem9->setText(QApplication::translate("VSCHddOneEdit", "30G", 0));
        tableWidget->setSortingEnabled(__sortingEnabled);

        checkBox->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class VSCHddOneEdit: public Ui_VSCHddOneEdit {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VSCHDDONEEDIT_H

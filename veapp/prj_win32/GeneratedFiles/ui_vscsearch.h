/********************************************************************************
** Form generated from reading UI file 'vscsearch.ui'
**
** Created by: Qt User Interface Compiler version 5.4.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VSCSEARCH_H
#define UI_VSCSEARCH_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_VSCSearch
{
public:
    QGridLayout *gridLayout;
    QTableWidget *tableWidget;
    QVBoxLayout *verticalLayout;
    QProgressBar *progressBar;
    QHBoxLayout *horizontalLayout;
    QPushButton *pushButtonStart;
    QPushButton *pushButtonStop;
    QPushButton *pushButtonSelect;
    QPushButton *pushButtonAdd;
    QComboBox *comboBox;

    void setupUi(QWidget *VSCSearch)
    {
        if (VSCSearch->objectName().isEmpty())
            VSCSearch->setObjectName(QStringLiteral("VSCSearch"));
        VSCSearch->resize(829, 482);
        gridLayout = new QGridLayout(VSCSearch);
        gridLayout->setSpacing(0);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        tableWidget = new QTableWidget(VSCSearch);
        if (tableWidget->columnCount() < 7)
            tableWidget->setColumnCount(7);
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
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(5, __qtablewidgetitem5);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        __qtablewidgetitem6->setTextAlignment(Qt::AlignJustify|Qt::AlignVCenter);
        tableWidget->setHorizontalHeaderItem(6, __qtablewidgetitem6);
        tableWidget->setObjectName(QStringLiteral("tableWidget"));
        tableWidget->setFrameShape(QFrame::StyledPanel);
        tableWidget->setFrameShadow(QFrame::Sunken);
        tableWidget->setLineWidth(1);
        tableWidget->setAutoScroll(true);
        tableWidget->setTextElideMode(Qt::ElideLeft);
        tableWidget->setVerticalScrollMode(QAbstractItemView::ScrollPerItem);
        tableWidget->setWordWrap(true);
        tableWidget->horizontalHeader()->setDefaultSectionSize(150);
        tableWidget->horizontalHeader()->setMinimumSectionSize(23);
        tableWidget->verticalHeader()->setVisible(true);
        tableWidget->verticalHeader()->setCascadingSectionResizes(true);
        tableWidget->verticalHeader()->setDefaultSectionSize(23);
        tableWidget->verticalHeader()->setMinimumSectionSize(23);
        tableWidget->verticalHeader()->setProperty("showSortIndicator", QVariant(false));
        tableWidget->verticalHeader()->setStretchLastSection(false);

        gridLayout->addWidget(tableWidget, 0, 0, 1, 1);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        progressBar = new QProgressBar(VSCSearch);
        progressBar->setObjectName(QStringLiteral("progressBar"));
        progressBar->setStyleSheet(QLatin1String("QProgressBar::chunk {\n"
"     background-color: rgb(41, 42, 43)\n"
"}"));
        progressBar->setValue(0);
        progressBar->setTextVisible(false);
        progressBar->setFormat(QStringLiteral(""));

        verticalLayout->addWidget(progressBar);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        pushButtonStart = new QPushButton(VSCSearch);
        pushButtonStart->setObjectName(QStringLiteral("pushButtonStart"));
        pushButtonStart->setMinimumSize(QSize(60, 30));
        pushButtonStart->setMaximumSize(QSize(60, 30));
        pushButtonStart->setStyleSheet(QLatin1String("QPushButton { \n"
"border:none;\n"
"color:white;\n"
"background-image: url(:/action/resources/button.png);}\n"
"QPushButton:hover {\n"
"	background-image: url(:/action/resources/buttonpress.png);}\n"
"QPushButton:pressed {\n"
"	background-image: url(:/action/resources/buttonpress.png);}\n"
""));

        horizontalLayout->addWidget(pushButtonStart);

        pushButtonStop = new QPushButton(VSCSearch);
        pushButtonStop->setObjectName(QStringLiteral("pushButtonStop"));
        pushButtonStop->setMinimumSize(QSize(60, 30));
        pushButtonStop->setMaximumSize(QSize(60, 30));
        pushButtonStop->setAutoFillBackground(false);
        pushButtonStop->setStyleSheet(QLatin1String("QPushButton { \n"
"border:none;\n"
"color:white;\n"
"background-image: url(:/action/resources/button.png);}\n"
"QPushButton:hover {\n"
"	background-image: url(:/action/resources/buttonpress.png);}\n"
"QPushButton:pressed {\n"
"	background-image: url(:/action/resources/buttonpress.png);}\n"
""));

        horizontalLayout->addWidget(pushButtonStop);

        pushButtonSelect = new QPushButton(VSCSearch);
        pushButtonSelect->setObjectName(QStringLiteral("pushButtonSelect"));
        pushButtonSelect->setMinimumSize(QSize(60, 30));
        pushButtonSelect->setMaximumSize(QSize(60, 30));
        pushButtonSelect->setStyleSheet(QLatin1String("QPushButton { \n"
"border:none;\n"
"color:white;\n"
"background-image: url(:/action/resources/button.png);}\n"
"QPushButton:hover {\n"
"	background-image: url(:/action/resources/buttonpress.png);}\n"
"QPushButton:pressed {\n"
"	background-image: url(:/action/resources/buttonpress.png);}\n"
""));

        horizontalLayout->addWidget(pushButtonSelect);

        pushButtonAdd = new QPushButton(VSCSearch);
        pushButtonAdd->setObjectName(QStringLiteral("pushButtonAdd"));
        pushButtonAdd->setMinimumSize(QSize(60, 30));
        pushButtonAdd->setMaximumSize(QSize(60, 30));
        pushButtonAdd->setStyleSheet(QLatin1String("QPushButton { \n"
"border:none;\n"
"color:white;\n"
"background-image: url(:/action/resources/button.png);}\n"
"QPushButton:hover {\n"
"	background-image: url(:/action/resources/buttonpress.png);}\n"
"QPushButton:pressed {\n"
"	background-image: url(:/action/resources/buttonpress.png);}\n"
""));

        horizontalLayout->addWidget(pushButtonAdd);

        comboBox = new QComboBox(VSCSearch);
        comboBox->setObjectName(QStringLiteral("comboBox"));
        comboBox->setMaximumSize(QSize(260, 30));

        horizontalLayout->addWidget(comboBox);


        verticalLayout->addLayout(horizontalLayout);


        gridLayout->addLayout(verticalLayout, 1, 0, 1, 1);


        retranslateUi(VSCSearch);

        QMetaObject::connectSlotsByName(VSCSearch);
    } // setupUi

    void retranslateUi(QWidget *VSCSearch)
    {
        VSCSearch->setWindowTitle(QApplication::translate("VSCSearch", "Form", 0));
        QTableWidgetItem *___qtablewidgetitem = tableWidget->horizontalHeaderItem(1);
        ___qtablewidgetitem->setText(QApplication::translate("VSCSearch", "IP Address", 0));
        QTableWidgetItem *___qtablewidgetitem1 = tableWidget->horizontalHeaderItem(2);
        ___qtablewidgetitem1->setText(QApplication::translate("VSCSearch", "Port", 0));
        QTableWidgetItem *___qtablewidgetitem2 = tableWidget->horizontalHeaderItem(3);
        ___qtablewidgetitem2->setText(QApplication::translate("VSCSearch", "Manufacturer", 0));
        QTableWidgetItem *___qtablewidgetitem3 = tableWidget->horizontalHeaderItem(4);
        ___qtablewidgetitem3->setText(QApplication::translate("VSCSearch", "Model", 0));
        QTableWidgetItem *___qtablewidgetitem4 = tableWidget->horizontalHeaderItem(5);
        ___qtablewidgetitem4->setText(QApplication::translate("VSCSearch", "Version", 0));
        QTableWidgetItem *___qtablewidgetitem5 = tableWidget->horizontalHeaderItem(6);
        ___qtablewidgetitem5->setText(QApplication::translate("VSCSearch", "ONVIF Address", 0));
        pushButtonStart->setText(QApplication::translate("VSCSearch", "Start", 0));
        pushButtonStop->setText(QApplication::translate("VSCSearch", "Stop", 0));
        pushButtonSelect->setText(QApplication::translate("VSCSearch", "Select", 0));
        pushButtonAdd->setText(QApplication::translate("VSCSearch", "Add", 0));
    } // retranslateUi

};

namespace Ui {
    class VSCSearch: public Ui_VSCSearch {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VSCSEARCH_H

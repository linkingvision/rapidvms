/********************************************************************************
** Form generated from reading UI file 'vscrecorderone.ui'
**
** Created by: Qt User Interface Compiler version 5.4.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VSCRECORDERONE_H
#define UI_VSCRECORDERONE_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "vscrecorderpanel.h"

QT_BEGIN_NAMESPACE

class Ui_VSCRecorderOne
{
public:
    QVBoxLayout *verticalLayout;
    QGridLayout *gridLayout;
    QLabel *label_2;
    QLabel *label;
    QLineEdit *lineEditName;
    QLineEdit *lineEditIP;
    VSCRecorderPanel *pushButton;

    void setupUi(QWidget *VSCRecorderOne)
    {
        if (VSCRecorderOne->objectName().isEmpty())
            VSCRecorderOne->setObjectName(QStringLiteral("VSCRecorderOne"));
        VSCRecorderOne->resize(300, 360);
        VSCRecorderOne->setMinimumSize(QSize(300, 360));
        VSCRecorderOne->setMaximumSize(QSize(300, 360));
        QIcon icon;
        icon.addFile(QStringLiteral(":/logo/resources/vscsmall.jpg"), QSize(), QIcon::Normal, QIcon::Off);
        VSCRecorderOne->setWindowIcon(icon);
        verticalLayout = new QVBoxLayout(VSCRecorderOne);
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        gridLayout = new QGridLayout();
        gridLayout->setSpacing(0);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        label_2 = new QLabel(VSCRecorderOne);
        label_2->setObjectName(QStringLiteral("label_2"));

        gridLayout->addWidget(label_2, 2, 0, 1, 1);

        label = new QLabel(VSCRecorderOne);
        label->setObjectName(QStringLiteral("label"));

        gridLayout->addWidget(label, 1, 0, 1, 1);

        lineEditName = new QLineEdit(VSCRecorderOne);
        lineEditName->setObjectName(QStringLiteral("lineEditName"));

        gridLayout->addWidget(lineEditName, 1, 1, 1, 1);

        lineEditIP = new QLineEdit(VSCRecorderOne);
        lineEditIP->setObjectName(QStringLiteral("lineEditIP"));

        gridLayout->addWidget(lineEditIP, 2, 1, 1, 1);


        verticalLayout->addLayout(gridLayout);

        pushButton = new VSCRecorderPanel(VSCRecorderOne);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setMinimumSize(QSize(300, 300));
        pushButton->setMaximumSize(QSize(300, 300));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/action/resources/computer.png"), QSize(), QIcon::Normal, QIcon::Off);
        pushButton->setIcon(icon1);
        pushButton->setIconSize(QSize(256, 256));

        verticalLayout->addWidget(pushButton);


        retranslateUi(VSCRecorderOne);

        QMetaObject::connectSlotsByName(VSCRecorderOne);
    } // setupUi

    void retranslateUi(QWidget *VSCRecorderOne)
    {
        VSCRecorderOne->setWindowTitle(QApplication::translate("VSCRecorderOne", "Master Hard Disk Selecting", 0));
        label_2->setText(QApplication::translate("VSCRecorderOne", "IP", 0));
        label->setText(QApplication::translate("VSCRecorderOne", "Name", 0));
        pushButton->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class VSCRecorderOne: public Ui_VSCRecorderOne {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VSCRECORDERONE_H

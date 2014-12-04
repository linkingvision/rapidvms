/********************************************************************************
** Form generated from reading UI file 'vschdddevice.ui'
**
** Created by: Qt User Interface Compiler version 5.4.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VSCHDDDEVICE_H
#define UI_VSCHDDDEVICE_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_VSCHddDevice
{
public:
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *hboxLayout;
    QSpacerItem *spacerItem;
    QPushButton *okButton;
    QPushButton *cancelButton;

    void setupUi(QDialog *VSCHddDevice)
    {
        if (VSCHddDevice->objectName().isEmpty())
            VSCHddDevice->setObjectName(QStringLiteral("VSCHddDevice"));
        VSCHddDevice->resize(591, 52);
        QIcon icon;
        icon.addFile(QStringLiteral(":/logo/resources/vscsmall.png"), QSize(), QIcon::Normal, QIcon::Off);
        VSCHddDevice->setWindowIcon(icon);
        verticalLayout_2 = new QVBoxLayout(VSCHddDevice);
        verticalLayout_2->setSpacing(9);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));

        verticalLayout_2->addLayout(verticalLayout);

        hboxLayout = new QHBoxLayout();
        hboxLayout->setSpacing(6);
        hboxLayout->setObjectName(QStringLiteral("hboxLayout"));
        hboxLayout->setContentsMargins(0, 0, 0, 0);
        spacerItem = new QSpacerItem(131, 31, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout->addItem(spacerItem);

        okButton = new QPushButton(VSCHddDevice);
        okButton->setObjectName(QStringLiteral("okButton"));

        hboxLayout->addWidget(okButton);

        cancelButton = new QPushButton(VSCHddDevice);
        cancelButton->setObjectName(QStringLiteral("cancelButton"));

        hboxLayout->addWidget(cancelButton);


        verticalLayout_2->addLayout(hboxLayout);


        retranslateUi(VSCHddDevice);
        QObject::connect(okButton, SIGNAL(clicked()), VSCHddDevice, SLOT(accept()));
        QObject::connect(cancelButton, SIGNAL(clicked()), VSCHddDevice, SLOT(reject()));

        QMetaObject::connectSlotsByName(VSCHddDevice);
    } // setupUi

    void retranslateUi(QDialog *VSCHddDevice)
    {
        VSCHddDevice->setWindowTitle(QApplication::translate("VSCHddDevice", "Master Hard Disk Selecting", 0));
        okButton->setText(QApplication::translate("VSCHddDevice", "OK", 0));
        cancelButton->setText(QApplication::translate("VSCHddDevice", "Cancel", 0));
    } // retranslateUi

};

namespace Ui {
    class VSCHddDevice: public Ui_VSCHddDevice {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VSCHDDDEVICE_H

/********************************************************************************
** Form generated from reading UI file 'vscversion.ui'
**
** Created by: Qt User Interface Compiler version 5.4.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VSCVERSION_H
#define UI_VSCVERSION_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_VSCVersion
{
public:
    QVBoxLayout *verticalLayout;
    QGridLayout *gridLayout;
    QLineEdit *ver;
    QLabel *label;
    QLabel *label_2;
    QLineEdit *info;
    QSpacerItem *verticalSpacer;

    void setupUi(QWidget *VSCVersion)
    {
        if (VSCVersion->objectName().isEmpty())
            VSCVersion->setObjectName(QStringLiteral("VSCVersion"));
        VSCVersion->resize(591, 369);
        QIcon icon;
        icon.addFile(QStringLiteral(":/logo/resources/vscsmall.jpg"), QSize(), QIcon::Normal, QIcon::Off);
        VSCVersion->setWindowIcon(icon);
        verticalLayout = new QVBoxLayout(VSCVersion);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        ver = new QLineEdit(VSCVersion);
        ver->setObjectName(QStringLiteral("ver"));
        ver->setReadOnly(true);

        gridLayout->addWidget(ver, 0, 1, 1, 1);

        label = new QLabel(VSCVersion);
        label->setObjectName(QStringLiteral("label"));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        label_2 = new QLabel(VSCVersion);
        label_2->setObjectName(QStringLiteral("label_2"));

        gridLayout->addWidget(label_2, 1, 0, 1, 1);

        info = new QLineEdit(VSCVersion);
        info->setObjectName(QStringLiteral("info"));
        info->setMaximumSize(QSize(16777215, 16777215));
        info->setReadOnly(true);

        gridLayout->addWidget(info, 1, 1, 1, 1);


        verticalLayout->addLayout(gridLayout);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);


        retranslateUi(VSCVersion);

        QMetaObject::connectSlotsByName(VSCVersion);
    } // setupUi

    void retranslateUi(QWidget *VSCVersion)
    {
        VSCVersion->setWindowTitle(QApplication::translate("VSCVersion", "Master Hard Disk Selecting", 0));
        label->setText(QApplication::translate("VSCVersion", "Version", 0));
        label_2->setText(QApplication::translate("VSCVersion", "Info", 0));
    } // retranslateUi

};

namespace Ui {
    class VSCVersion: public Ui_VSCVersion {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VSCVERSION_H

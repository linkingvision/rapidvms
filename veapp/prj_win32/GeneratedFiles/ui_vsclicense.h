/********************************************************************************
** Form generated from reading UI file 'vsclicense.ui'
**
** Created by: Qt User Interface Compiler version 5.4.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VSCLICENSE_H
#define UI_VSCLICENSE_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_VSCLicense
{
public:
    QVBoxLayout *verticalLayout;
    QGridLayout *gridLayout;
    QLineEdit *hostId;
    QLabel *label;
    QLabel *label_2;
    QLineEdit *channel;
    QTextEdit *lic;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButtonApply;
    QPushButton *pushButtonCancel;
    QSpacerItem *horizontalSpacer_2;

    void setupUi(QWidget *VSCLicense)
    {
        if (VSCLicense->objectName().isEmpty())
            VSCLicense->setObjectName(QStringLiteral("VSCLicense"));
        VSCLicense->resize(591, 369);
        QIcon icon;
        icon.addFile(QStringLiteral(":/logo/resources/vscsmall.jpg"), QSize(), QIcon::Normal, QIcon::Off);
        VSCLicense->setWindowIcon(icon);
        verticalLayout = new QVBoxLayout(VSCLicense);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        hostId = new QLineEdit(VSCLicense);
        hostId->setObjectName(QStringLiteral("hostId"));
        hostId->setReadOnly(true);

        gridLayout->addWidget(hostId, 0, 1, 1, 1);

        label = new QLabel(VSCLicense);
        label->setObjectName(QStringLiteral("label"));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        label_2 = new QLabel(VSCLicense);
        label_2->setObjectName(QStringLiteral("label_2"));

        gridLayout->addWidget(label_2, 1, 0, 1, 1);

        channel = new QLineEdit(VSCLicense);
        channel->setObjectName(QStringLiteral("channel"));
        channel->setEnabled(true);
        channel->setMaximumSize(QSize(50, 16777215));
        channel->setReadOnly(true);

        gridLayout->addWidget(channel, 1, 1, 1, 1);


        verticalLayout->addLayout(gridLayout);

        lic = new QTextEdit(VSCLicense);
        lic->setObjectName(QStringLiteral("lic"));
        lic->setStyleSheet(QStringLiteral("font: 10pt \"Arial\";"));

        verticalLayout->addWidget(lic);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        pushButtonApply = new QPushButton(VSCLicense);
        pushButtonApply->setObjectName(QStringLiteral("pushButtonApply"));
        pushButtonApply->setMinimumSize(QSize(60, 30));
        pushButtonApply->setMaximumSize(QSize(60, 30));
        pushButtonApply->setIconSize(QSize(60, 40));

        horizontalLayout->addWidget(pushButtonApply);

        pushButtonCancel = new QPushButton(VSCLicense);
        pushButtonCancel->setObjectName(QStringLiteral("pushButtonCancel"));
        pushButtonCancel->setMinimumSize(QSize(60, 30));
        pushButtonCancel->setMaximumSize(QSize(60, 30));
        pushButtonCancel->setIconSize(QSize(60, 40));

        horizontalLayout->addWidget(pushButtonCancel);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(VSCLicense);

        QMetaObject::connectSlotsByName(VSCLicense);
    } // setupUi

    void retranslateUi(QWidget *VSCLicense)
    {
        VSCLicense->setWindowTitle(QApplication::translate("VSCLicense", "Master Hard Disk Selecting", 0));
        label->setText(QApplication::translate("VSCLicense", "Host ID", 0));
        label_2->setText(QApplication::translate("VSCLicense", "Channel", 0));
        pushButtonApply->setText(QApplication::translate("VSCLicense", "Apply", 0));
        pushButtonCancel->setText(QApplication::translate("VSCLicense", "Cancel", 0));
    } // retranslateUi

};

namespace Ui {
    class VSCLicense: public Ui_VSCLicense {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VSCLICENSE_H

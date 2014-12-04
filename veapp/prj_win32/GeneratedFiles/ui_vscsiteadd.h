/********************************************************************************
** Form generated from reading UI file 'vscsiteadd.ui'
**
** Created by: Qt User Interface Compiler version 5.4.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VSCSITEADD_H
#define UI_VSCSITEADD_H

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
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_VSCSiteAdd
{
public:
    QWidget *horizontalLayoutWidget_2;
    QHBoxLayout *horizontalLayout_2;
    QGridLayout *gridLayout;
    QLabel *labelIP;
    QLabel *labelPort;
    QLineEdit *lineEditPassword;
    QLabel *labelName;
    QLineEdit *lineEditUser;
    QLineEdit *lineEditName;
    QLineEdit *lineEditIP;
    QLabel *labelUser;
    QLabel *labelPassword;
    QLineEdit *lineEditPort;
    QWidget *widget;
    QWidget *gridLayoutWidget;
    QGridLayout *gridLayout_2;
    QPushButton *pushButtonCancel;
    QPushButton *pushButtonApply;

    void setupUi(QWidget *VSCSiteAdd)
    {
        if (VSCSiteAdd->objectName().isEmpty())
            VSCSiteAdd->setObjectName(QStringLiteral("VSCSiteAdd"));
        VSCSiteAdd->resize(852, 432);
        VSCSiteAdd->setStyleSheet(QStringLiteral(""));
        horizontalLayoutWidget_2 = new QWidget(VSCSiteAdd);
        horizontalLayoutWidget_2->setObjectName(QStringLiteral("horizontalLayoutWidget_2"));
        horizontalLayoutWidget_2->setGeometry(QRect(10, 20, 965, 331));
        horizontalLayout_2 = new QHBoxLayout(horizontalLayoutWidget_2);
        horizontalLayout_2->setSpacing(5);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setHorizontalSpacing(2);
        gridLayout->setVerticalSpacing(0);
        labelIP = new QLabel(horizontalLayoutWidget_2);
        labelIP->setObjectName(QStringLiteral("labelIP"));

        gridLayout->addWidget(labelIP, 5, 0, 1, 1);

        labelPort = new QLabel(horizontalLayoutWidget_2);
        labelPort->setObjectName(QStringLiteral("labelPort"));

        gridLayout->addWidget(labelPort, 6, 0, 1, 1);

        lineEditPassword = new QLineEdit(horizontalLayoutWidget_2);
        lineEditPassword->setObjectName(QStringLiteral("lineEditPassword"));
        lineEditPassword->setMaximumSize(QSize(300, 16777215));

        gridLayout->addWidget(lineEditPassword, 8, 1, 1, 1);

        labelName = new QLabel(horizontalLayoutWidget_2);
        labelName->setObjectName(QStringLiteral("labelName"));

        gridLayout->addWidget(labelName, 3, 0, 1, 1);

        lineEditUser = new QLineEdit(horizontalLayoutWidget_2);
        lineEditUser->setObjectName(QStringLiteral("lineEditUser"));
        lineEditUser->setMaximumSize(QSize(300, 16777215));

        gridLayout->addWidget(lineEditUser, 7, 1, 1, 1);

        lineEditName = new QLineEdit(horizontalLayoutWidget_2);
        lineEditName->setObjectName(QStringLiteral("lineEditName"));
        lineEditName->setMaximumSize(QSize(300, 16777215));

        gridLayout->addWidget(lineEditName, 3, 1, 1, 1);

        lineEditIP = new QLineEdit(horizontalLayoutWidget_2);
        lineEditIP->setObjectName(QStringLiteral("lineEditIP"));
        lineEditIP->setMaximumSize(QSize(300, 16777215));

        gridLayout->addWidget(lineEditIP, 5, 1, 1, 1);

        labelUser = new QLabel(horizontalLayoutWidget_2);
        labelUser->setObjectName(QStringLiteral("labelUser"));

        gridLayout->addWidget(labelUser, 7, 0, 1, 1);

        labelPassword = new QLabel(horizontalLayoutWidget_2);
        labelPassword->setObjectName(QStringLiteral("labelPassword"));

        gridLayout->addWidget(labelPassword, 8, 0, 1, 1);

        lineEditPort = new QLineEdit(horizontalLayoutWidget_2);
        lineEditPort->setObjectName(QStringLiteral("lineEditPort"));
        lineEditPort->setMaximumSize(QSize(300, 16777215));

        gridLayout->addWidget(lineEditPort, 6, 1, 1, 1);


        horizontalLayout_2->addLayout(gridLayout);

        widget = new QWidget(horizontalLayoutWidget_2);
        widget->setObjectName(QStringLiteral("widget"));
        widget->setMinimumSize(QSize(550, 300));
        widget->setMaximumSize(QSize(600, 400));
        widget->setStyleSheet(QStringLiteral(""));

        horizontalLayout_2->addWidget(widget);

        gridLayoutWidget = new QWidget(VSCSiteAdd);
        gridLayoutWidget->setObjectName(QStringLiteral("gridLayoutWidget"));
        gridLayoutWidget->setGeometry(QRect(240, 360, 191, 61));
        gridLayout_2 = new QGridLayout(gridLayoutWidget);
        gridLayout_2->setSpacing(0);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        gridLayout_2->setContentsMargins(0, 0, 0, 0);
        pushButtonCancel = new QPushButton(gridLayoutWidget);
        pushButtonCancel->setObjectName(QStringLiteral("pushButtonCancel"));
        pushButtonCancel->setMinimumSize(QSize(60, 30));
        pushButtonCancel->setMaximumSize(QSize(60, 30));
        pushButtonCancel->setIconSize(QSize(60, 40));

        gridLayout_2->addWidget(pushButtonCancel, 0, 1, 1, 1);

        pushButtonApply = new QPushButton(gridLayoutWidget);
        pushButtonApply->setObjectName(QStringLiteral("pushButtonApply"));
        pushButtonApply->setMinimumSize(QSize(60, 30));
        pushButtonApply->setMaximumSize(QSize(60, 30));
        pushButtonApply->setIconSize(QSize(60, 40));

        gridLayout_2->addWidget(pushButtonApply, 0, 0, 1, 1);


        retranslateUi(VSCSiteAdd);

        QMetaObject::connectSlotsByName(VSCSiteAdd);
    } // setupUi

    void retranslateUi(QWidget *VSCSiteAdd)
    {
        VSCSiteAdd->setWindowTitle(QApplication::translate("VSCSiteAdd", "Form", 0));
        labelIP->setText(QApplication::translate("VSCSiteAdd", "IP Address", 0));
        labelPort->setText(QApplication::translate("VSCSiteAdd", "Port", 0));
        labelName->setText(QApplication::translate("VSCSiteAdd", "Device Name", 0));
        labelUser->setText(QApplication::translate("VSCSiteAdd", "User", 0));
        labelPassword->setText(QApplication::translate("VSCSiteAdd", "Password", 0));
        pushButtonCancel->setText(QApplication::translate("VSCSiteAdd", "Cancel", 0));
        pushButtonApply->setText(QApplication::translate("VSCSiteAdd", "Apply", 0));
    } // retranslateUi

};

namespace Ui {
    class VSCSiteAdd: public Ui_VSCSiteAdd {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VSCSITEADD_H

/********************************************************************************
** Form generated from reading UI file 'vscuser.ui'
**
** Created by: Qt User Interface Compiler version 5.4.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VSCUSER_H
#define UI_VSCUSER_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_VSCUser
{
public:
    QVBoxLayout *verticalLayout;
    QGridLayout *gridLayout;
    QLineEdit *passwd;
    QLabel *label_2;
    QLabel *label;
    QLineEdit *user;
    QLabel *label_3;
    QCheckBox *checkBox;
    QSpacerItem *verticalSpacer;

    void setupUi(QWidget *VSCUser)
    {
        if (VSCUser->objectName().isEmpty())
            VSCUser->setObjectName(QStringLiteral("VSCUser"));
        VSCUser->resize(591, 369);
        QIcon icon;
        icon.addFile(QStringLiteral(":/logo/resources/vscsmall.jpg"), QSize(), QIcon::Normal, QIcon::Off);
        VSCUser->setWindowIcon(icon);
        verticalLayout = new QVBoxLayout(VSCUser);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setHorizontalSpacing(40);
        passwd = new QLineEdit(VSCUser);
        passwd->setObjectName(QStringLiteral("passwd"));
        passwd->setMaximumSize(QSize(16777215, 16777215));
        passwd->setReadOnly(true);

        gridLayout->addWidget(passwd, 1, 2, 1, 1);

        label_2 = new QLabel(VSCUser);
        label_2->setObjectName(QStringLiteral("label_2"));

        gridLayout->addWidget(label_2, 1, 0, 1, 1);

        label = new QLabel(VSCUser);
        label->setObjectName(QStringLiteral("label"));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        user = new QLineEdit(VSCUser);
        user->setObjectName(QStringLiteral("user"));
        user->setReadOnly(true);

        gridLayout->addWidget(user, 0, 2, 1, 1);

        label_3 = new QLabel(VSCUser);
        label_3->setObjectName(QStringLiteral("label_3"));

        gridLayout->addWidget(label_3, 2, 0, 1, 1);

        checkBox = new QCheckBox(VSCUser);
        checkBox->setObjectName(QStringLiteral("checkBox"));

        gridLayout->addWidget(checkBox, 2, 2, 1, 1);


        verticalLayout->addLayout(gridLayout);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);


        retranslateUi(VSCUser);

        QMetaObject::connectSlotsByName(VSCUser);
    } // setupUi

    void retranslateUi(QWidget *VSCUser)
    {
        VSCUser->setWindowTitle(QApplication::translate("VSCUser", "Master Hard Disk Selecting", 0));
        label_2->setText(QApplication::translate("VSCUser", "Password", 0));
        label->setText(QApplication::translate("VSCUser", "User", 0));
        user->setText(QApplication::translate("VSCUser", "admin", 0));
        label_3->setText(QApplication::translate("VSCUser", "Auto login", 0));
        checkBox->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class VSCUser: public Ui_VSCUser {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VSCUSER_H

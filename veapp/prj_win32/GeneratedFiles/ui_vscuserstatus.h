/********************************************************************************
** Form generated from reading UI file 'vscuserstatus.ui'
**
** Created by: Qt User Interface Compiler version 5.4.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VSCUSERSTATUS_H
#define UI_VSCUSERSTATUS_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_VSCUserStatus
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QLineEdit *lineEditName;
    QHBoxLayout *_2;
    QSpacerItem *spacerItem;
    QPushButton *pbSave;
    QPushButton *pbCancel;

    void setupUi(QDialog *VSCUserStatus)
    {
        if (VSCUserStatus->objectName().isEmpty())
            VSCUserStatus->setObjectName(QStringLiteral("VSCUserStatus"));
        VSCUserStatus->resize(468, 138);
        QIcon icon;
        icon.addFile(QStringLiteral(":/logo/resources/vscsmall.png"), QSize(), QIcon::Normal, QIcon::Off);
        VSCUserStatus->setWindowIcon(icon);
        verticalLayout = new QVBoxLayout(VSCUserStatus);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(30);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        label = new QLabel(VSCUserStatus);
        label->setObjectName(QStringLiteral("label"));
        label->setStyleSheet(QStringLiteral("font: 12pt \"Arial\";"));

        horizontalLayout->addWidget(label);

        lineEditName = new QLineEdit(VSCUserStatus);
        lineEditName->setObjectName(QStringLiteral("lineEditName"));
        lineEditName->setStyleSheet(QStringLiteral("font: 12pt \"Arial\";"));
        lineEditName->setReadOnly(true);

        horizontalLayout->addWidget(lineEditName);


        verticalLayout->addLayout(horizontalLayout);

        _2 = new QHBoxLayout();
        _2->setSpacing(6);
        _2->setObjectName(QStringLiteral("_2"));
        _2->setContentsMargins(0, 0, 0, 0);
        spacerItem = new QSpacerItem(131, 31, QSizePolicy::Expanding, QSizePolicy::Minimum);

        _2->addItem(spacerItem);

        pbSave = new QPushButton(VSCUserStatus);
        pbSave->setObjectName(QStringLiteral("pbSave"));

        _2->addWidget(pbSave);

        pbCancel = new QPushButton(VSCUserStatus);
        pbCancel->setObjectName(QStringLiteral("pbCancel"));

        _2->addWidget(pbCancel);


        verticalLayout->addLayout(_2);


        retranslateUi(VSCUserStatus);
        QObject::connect(pbSave, SIGNAL(clicked()), VSCUserStatus, SLOT(accept()));
        QObject::connect(pbCancel, SIGNAL(clicked()), VSCUserStatus, SLOT(reject()));

        QMetaObject::connectSlotsByName(VSCUserStatus);
    } // setupUi

    void retranslateUi(QDialog *VSCUserStatus)
    {
        VSCUserStatus->setWindowTitle(QApplication::translate("VSCUserStatus", "User Status", 0));
        label->setText(QApplication::translate("VSCUserStatus", "User", 0));
        lineEditName->setText(QApplication::translate("VSCUserStatus", "admin", 0));
        pbSave->setText(QApplication::translate("VSCUserStatus", "Logout", 0));
        pbCancel->setText(QApplication::translate("VSCUserStatus", "Cancel", 0));
    } // retranslateUi

};

namespace Ui {
    class VSCUserStatus: public Ui_VSCUserStatus {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VSCUSERSTATUS_H

/********************************************************************************
** Form generated from reading UI file 'vscipcgroupconf.ui'
**
** Created by: Qt User Interface Compiler version 5.4.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VSCIPCGROUPCONF_H
#define UI_VSCIPCGROUPCONF_H

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

class Ui_VSCIPCGroupConf
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

    void setupUi(QDialog *VSCIPCGroupConf)
    {
        if (VSCIPCGroupConf->objectName().isEmpty())
            VSCIPCGroupConf->setObjectName(QStringLiteral("VSCIPCGroupConf"));
        VSCIPCGroupConf->resize(677, 152);
        QIcon icon;
        icon.addFile(QStringLiteral(":/logo/resources/vscsmall.png"), QSize(), QIcon::Normal, QIcon::Off);
        VSCIPCGroupConf->setWindowIcon(icon);
        VSCIPCGroupConf->setModal(true);
        verticalLayout = new QVBoxLayout(VSCIPCGroupConf);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(30);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        label = new QLabel(VSCIPCGroupConf);
        label->setObjectName(QStringLiteral("label"));
        label->setStyleSheet(QStringLiteral("font: 12pt \"Arial\";"));

        horizontalLayout->addWidget(label);

        lineEditName = new QLineEdit(VSCIPCGroupConf);
        lineEditName->setObjectName(QStringLiteral("lineEditName"));
        lineEditName->setStyleSheet(QStringLiteral("font: 12pt \"Arial\";"));

        horizontalLayout->addWidget(lineEditName);


        verticalLayout->addLayout(horizontalLayout);

        _2 = new QHBoxLayout();
        _2->setSpacing(6);
        _2->setObjectName(QStringLiteral("_2"));
        _2->setContentsMargins(0, 0, 0, 0);
        spacerItem = new QSpacerItem(131, 31, QSizePolicy::Expanding, QSizePolicy::Minimum);

        _2->addItem(spacerItem);

        pbSave = new QPushButton(VSCIPCGroupConf);
        pbSave->setObjectName(QStringLiteral("pbSave"));
        pbSave->setMinimumSize(QSize(60, 30));
        pbSave->setMaximumSize(QSize(60, 30));
        pbSave->setStyleSheet(QLatin1String("QPushButton { \n"
"border:none;\n"
"color:white;\n"
"background-image: url(:/action/resources/button.png);}\n"
"QPushButton:hover {\n"
"	background-image: url(:/action/resources/buttonpress.png);}\n"
"QPushButton:pressed {\n"
"	background-image: url(:/action/resources/buttonpress.png);}\n"
""));

        _2->addWidget(pbSave);

        pbCancel = new QPushButton(VSCIPCGroupConf);
        pbCancel->setObjectName(QStringLiteral("pbCancel"));
        pbCancel->setMinimumSize(QSize(60, 30));
        pbCancel->setMaximumSize(QSize(60, 30));
        pbCancel->setStyleSheet(QLatin1String("QPushButton { \n"
"border:none;\n"
"color:white;\n"
"background-image: url(:/action/resources/button.png);}\n"
"QPushButton:hover {\n"
"	background-image: url(:/action/resources/buttonpress.png);}\n"
"QPushButton:pressed {\n"
"	background-image: url(:/action/resources/buttonpress.png);}\n"
""));

        _2->addWidget(pbCancel);


        verticalLayout->addLayout(_2);


        retranslateUi(VSCIPCGroupConf);
        QObject::connect(pbSave, SIGNAL(clicked()), VSCIPCGroupConf, SLOT(accept()));
        QObject::connect(pbCancel, SIGNAL(clicked()), VSCIPCGroupConf, SLOT(reject()));

        QMetaObject::connectSlotsByName(VSCIPCGroupConf);
    } // setupUi

    void retranslateUi(QDialog *VSCIPCGroupConf)
    {
        VSCIPCGroupConf->setWindowTitle(QApplication::translate("VSCIPCGroupConf", "Camera Group Configuration", 0));
        label->setText(QApplication::translate("VSCIPCGroupConf", "Group Name", 0));
        pbSave->setText(QApplication::translate("VSCIPCGroupConf", "Save", 0));
        pbCancel->setText(QApplication::translate("VSCIPCGroupConf", "Cancel", 0));
    } // retranslateUi

};

namespace Ui {
    class VSCIPCGroupConf: public Ui_VSCIPCGroupConf {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VSCIPCGROUPCONF_H

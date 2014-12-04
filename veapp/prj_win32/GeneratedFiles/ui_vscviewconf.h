/********************************************************************************
** Form generated from reading UI file 'vscviewconf.ui'
**
** Created by: Qt User Interface Compiler version 5.4.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VSCVIEWCONF_H
#define UI_VSCVIEWCONF_H

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

class Ui_VSCViewConf
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QLineEdit *lineEditName;
    QHBoxLayout *_2;
    QSpacerItem *spacerItem;
    QPushButton *pbNew;
    QPushButton *pbModify;

    void setupUi(QDialog *VSCViewConf)
    {
        if (VSCViewConf->objectName().isEmpty())
            VSCViewConf->setObjectName(QStringLiteral("VSCViewConf"));
        VSCViewConf->resize(677, 152);
        QIcon icon;
        icon.addFile(QStringLiteral(":/logo/resources/vscsmall.png"), QSize(), QIcon::Normal, QIcon::Off);
        VSCViewConf->setWindowIcon(icon);
        verticalLayout = new QVBoxLayout(VSCViewConf);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(30);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        label = new QLabel(VSCViewConf);
        label->setObjectName(QStringLiteral("label"));
        label->setStyleSheet(QStringLiteral("font: 12pt \"Arial\";"));

        horizontalLayout->addWidget(label);

        lineEditName = new QLineEdit(VSCViewConf);
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

        pbNew = new QPushButton(VSCViewConf);
        pbNew->setObjectName(QStringLiteral("pbNew"));
        pbNew->setMinimumSize(QSize(60, 30));
        pbNew->setMaximumSize(QSize(60, 30));
        pbNew->setStyleSheet(QLatin1String("QPushButton { \n"
"border:none;\n"
"color:white;\n"
"background-image: url(:/action/resources/button.png);}\n"
"QPushButton:hover {\n"
"	background-image: url(:/action/resources/buttonpress.png);}\n"
"QPushButton:pressed {\n"
"	background-image: url(:/action/resources/buttonpress.png);}\n"
""));

        _2->addWidget(pbNew);

        pbModify = new QPushButton(VSCViewConf);
        pbModify->setObjectName(QStringLiteral("pbModify"));
        pbModify->setMinimumSize(QSize(60, 30));
        pbModify->setMaximumSize(QSize(60, 30));
        pbModify->setStyleSheet(QLatin1String("QPushButton { \n"
"border:none;\n"
"color:white;\n"
"background-image: url(:/action/resources/button.png);}\n"
"QPushButton:hover {\n"
"	background-image: url(:/action/resources/buttonpress.png);}\n"
"QPushButton:pressed {\n"
"	background-image: url(:/action/resources/buttonpress.png);}\n"
""));

        _2->addWidget(pbModify);


        verticalLayout->addLayout(_2);


        retranslateUi(VSCViewConf);
        QObject::connect(pbModify, SIGNAL(clicked()), VSCViewConf, SLOT(accept()));
        QObject::connect(pbNew, SIGNAL(clicked()), VSCViewConf, SLOT(reject()));

        QMetaObject::connectSlotsByName(VSCViewConf);
    } // setupUi

    void retranslateUi(QDialog *VSCViewConf)
    {
        VSCViewConf->setWindowTitle(QApplication::translate("VSCViewConf", "View Configuration", 0));
        label->setText(QApplication::translate("VSCViewConf", "View Name", 0));
        pbNew->setText(QApplication::translate("VSCViewConf", "New", 0));
        pbModify->setText(QApplication::translate("VSCViewConf", "Save", 0));
    } // retranslateUi

};

namespace Ui {
    class VSCViewConf: public Ui_VSCViewConf {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VSCVIEWCONF_H

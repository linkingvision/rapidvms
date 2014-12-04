/********************************************************************************
** Form generated from reading UI file 'vscdevicelist.ui'
**
** Created by: Qt User Interface Compiler version 5.4.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VSCDEVICELIST_H
#define UI_VSCDEVICELIST_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "vscdevicetrash.h"
#include "vscdevicetree.h"

QT_BEGIN_NAMESPACE

class Ui_VSCDeviceList
{
public:
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout;
    QPushButton *pbRecorder;
    QLabel *lbRecorder;
    QPushButton *pbCamera;
    QLabel *label_3;
    QPushButton *pbSurveillance;
    QLabel *label_4;
    QPushButton *pbSearch;
    QLabel *label_5;
    QPushButton *pbPlayback;
    QLabel *label_6;
    QPushButton *pbEmap;
    QLabel *label;
    QPushButton *pbVIPC;
    QLabel *label_8;
    VSCDeviceTrash *pbTrash;
    QLabel *label_7;
    QSpacerItem *verticalSpacer;
    VSCDeviceTree *treeWidget;

    void setupUi(QWidget *VSCDeviceList)
    {
        if (VSCDeviceList->objectName().isEmpty())
            VSCDeviceList->setObjectName(QStringLiteral("VSCDeviceList"));
        VSCDeviceList->resize(280, 584);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(VSCDeviceList->sizePolicy().hasHeightForWidth());
        VSCDeviceList->setSizePolicy(sizePolicy);
        VSCDeviceList->setMinimumSize(QSize(280, 0));
        VSCDeviceList->setMaximumSize(QSize(400, 16777215));
        VSCDeviceList->setStyleSheet(QStringLiteral(""));
        gridLayout = new QGridLayout(VSCDeviceList);
        gridLayout->setSpacing(0);
        gridLayout->setContentsMargins(0, 0, 0, 0);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setSizeConstraint(QLayout::SetNoConstraint);
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        pbRecorder = new QPushButton(VSCDeviceList);
        pbRecorder->setObjectName(QStringLiteral("pbRecorder"));
        pbRecorder->setMinimumSize(QSize(65, 40));
        pbRecorder->setMaximumSize(QSize(65, 40));
        pbRecorder->setStyleSheet(QStringLiteral("selection-background-color: rgb(0, 255, 0);"));
        QIcon icon;
        icon.addFile(QStringLiteral(":/device/resources/computer.png"), QSize(), QIcon::Normal, QIcon::Off);
        pbRecorder->setIcon(icon);
        pbRecorder->setIconSize(QSize(30, 30));

        verticalLayout->addWidget(pbRecorder);

        lbRecorder = new QLabel(VSCDeviceList);
        lbRecorder->setObjectName(QStringLiteral("lbRecorder"));
        lbRecorder->setStyleSheet(QStringLiteral("font: 8pt \"Arial Unicode MS\";"));
        lbRecorder->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(lbRecorder);

        pbCamera = new QPushButton(VSCDeviceList);
        pbCamera->setObjectName(QStringLiteral("pbCamera"));
        pbCamera->setMinimumSize(QSize(65, 40));
        pbCamera->setMaximumSize(QSize(65, 40));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/device/resources/dome.png"), QSize(), QIcon::Normal, QIcon::Off);
        pbCamera->setIcon(icon1);
        pbCamera->setIconSize(QSize(30, 30));

        verticalLayout->addWidget(pbCamera);

        label_3 = new QLabel(VSCDeviceList);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setStyleSheet(QStringLiteral("font: 8pt \"Arial Unicode MS\";"));
        label_3->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(label_3);

        pbSurveillance = new QPushButton(VSCDeviceList);
        pbSurveillance->setObjectName(QStringLiteral("pbSurveillance"));
        pbSurveillance->setMinimumSize(QSize(65, 40));
        pbSurveillance->setMaximumSize(QSize(65, 40));
        QIcon icon2;
        icon2.addFile(QStringLiteral(":/action/resources/surveillance.png"), QSize(), QIcon::Normal, QIcon::Off);
        pbSurveillance->setIcon(icon2);
        pbSurveillance->setIconSize(QSize(30, 30));

        verticalLayout->addWidget(pbSurveillance);

        label_4 = new QLabel(VSCDeviceList);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setStyleSheet(QStringLiteral("font: 8pt \"Arial Unicode MS\";"));
        label_4->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(label_4);

        pbSearch = new QPushButton(VSCDeviceList);
        pbSearch->setObjectName(QStringLiteral("pbSearch"));
        pbSearch->setMinimumSize(QSize(65, 40));
        pbSearch->setMaximumSize(QSize(65, 40));
        QIcon icon3;
        icon3.addFile(QStringLiteral(":/action/resources/search.png"), QSize(), QIcon::Normal, QIcon::Off);
        pbSearch->setIcon(icon3);
        pbSearch->setIconSize(QSize(30, 30));

        verticalLayout->addWidget(pbSearch);

        label_5 = new QLabel(VSCDeviceList);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setStyleSheet(QStringLiteral("font: 8pt \"Arial Unicode MS\";"));
        label_5->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(label_5);

        pbPlayback = new QPushButton(VSCDeviceList);
        pbPlayback->setObjectName(QStringLiteral("pbPlayback"));
        pbPlayback->setMinimumSize(QSize(65, 40));
        pbPlayback->setMaximumSize(QSize(65, 40));
        QIcon icon4;
        icon4.addFile(QStringLiteral(":/action/resources/mining.png"), QSize(), QIcon::Normal, QIcon::Off);
        pbPlayback->setIcon(icon4);
        pbPlayback->setIconSize(QSize(40, 40));

        verticalLayout->addWidget(pbPlayback);

        label_6 = new QLabel(VSCDeviceList);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setStyleSheet(QStringLiteral("font: 8pt \"Arial Unicode MS\";"));
        label_6->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(label_6);

        pbEmap = new QPushButton(VSCDeviceList);
        pbEmap->setObjectName(QStringLiteral("pbEmap"));
        pbEmap->setMinimumSize(QSize(65, 40));
        pbEmap->setMaximumSize(QSize(65, 40));
        pbEmap->setLayoutDirection(Qt::LeftToRight);
        pbEmap->setStyleSheet(QStringLiteral(""));
        QIcon icon5;
        icon5.addFile(QStringLiteral(":/action/resources/map.png"), QSize(), QIcon::Normal, QIcon::Off);
        pbEmap->setIcon(icon5);
        pbEmap->setIconSize(QSize(35, 35));

        verticalLayout->addWidget(pbEmap);

        label = new QLabel(VSCDeviceList);
        label->setObjectName(QStringLiteral("label"));
        QFont font;
        font.setFamily(QStringLiteral("Arial Unicode MS"));
        font.setPointSize(8);
        font.setBold(false);
        font.setItalic(false);
        font.setWeight(50);
        label->setFont(font);
        label->setStyleSheet(QStringLiteral("font: 8pt \"Arial Unicode MS\";"));
        label->setTextFormat(Qt::RichText);
        label->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(label);

        pbVIPC = new QPushButton(VSCDeviceList);
        pbVIPC->setObjectName(QStringLiteral("pbVIPC"));
        pbVIPC->setMinimumSize(QSize(65, 40));
        pbVIPC->setMaximumSize(QSize(65, 40));
        QIcon icon6;
        icon6.addFile(QStringLiteral(":/device/resources/virtualipc.png"), QSize(), QIcon::Normal, QIcon::Off);
        pbVIPC->setIcon(icon6);
        pbVIPC->setIconSize(QSize(35, 35));

        verticalLayout->addWidget(pbVIPC);

        label_8 = new QLabel(VSCDeviceList);
        label_8->setObjectName(QStringLiteral("label_8"));
        label_8->setStyleSheet(QStringLiteral("font: 8pt \"Arial Unicode MS\";"));
        label_8->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(label_8);

        pbTrash = new VSCDeviceTrash(VSCDeviceList);
        pbTrash->setObjectName(QStringLiteral("pbTrash"));
        pbTrash->setMinimumSize(QSize(65, 40));
        pbTrash->setMaximumSize(QSize(65, 40));
        QIcon icon7;
        icon7.addFile(QStringLiteral(":/action/resources/user-trash-2.png"), QSize(), QIcon::Normal, QIcon::Off);
        pbTrash->setIcon(icon7);
        pbTrash->setIconSize(QSize(30, 30));

        verticalLayout->addWidget(pbTrash);

        label_7 = new QLabel(VSCDeviceList);
        label_7->setObjectName(QStringLiteral("label_7"));
        label_7->setStyleSheet(QStringLiteral("font: 8pt \"Arial Unicode MS\";"));
        label_7->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(label_7);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);


        horizontalLayout->addLayout(verticalLayout);

        treeWidget = new VSCDeviceTree(VSCDeviceList);
        QIcon icon8;
        icon8.addFile(QStringLiteral(":/device/resources/view.png"), QSize(), QIcon::Normal, QIcon::Off);
        QIcon icon9;
        icon9.addFile(QStringLiteral(":/device/resources/harddisk.png"), QSize(), QIcon::Normal, QIcon::Off);
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem(treeWidget);
        __qtreewidgetitem->setIcon(0, icon1);
        QTreeWidgetItem *__qtreewidgetitem1 = new QTreeWidgetItem(treeWidget);
        __qtreewidgetitem1->setIcon(0, icon8);
        QTreeWidgetItem *__qtreewidgetitem2 = new QTreeWidgetItem(treeWidget);
        __qtreewidgetitem2->setIcon(0, icon9);
        QTreeWidgetItem *__qtreewidgetitem3 = new QTreeWidgetItem(treeWidget);
        __qtreewidgetitem3->setIcon(0, icon6);
        treeWidget->setObjectName(QStringLiteral("treeWidget"));
        treeWidget->setIconSize(QSize(20, 20));
        treeWidget->setHorizontalScrollMode(QAbstractItemView::ScrollPerItem);
        treeWidget->setIndentation(30);
        treeWidget->setRootIsDecorated(true);
        treeWidget->setUniformRowHeights(false);
        treeWidget->setItemsExpandable(true);
        treeWidget->setAnimated(true);
        treeWidget->setWordWrap(false);
        treeWidget->setHeaderHidden(true);
        treeWidget->setExpandsOnDoubleClick(true);
        treeWidget->setColumnCount(1);
        treeWidget->header()->setCascadingSectionResizes(true);
        treeWidget->header()->setProperty("showSortIndicator", QVariant(true));

        horizontalLayout->addWidget(treeWidget);


        gridLayout->addLayout(horizontalLayout, 0, 0, 1, 1);


        retranslateUi(VSCDeviceList);

        QMetaObject::connectSlotsByName(VSCDeviceList);
    } // setupUi

    void retranslateUi(QWidget *VSCDeviceList)
    {
        VSCDeviceList->setWindowTitle(QString());
        pbRecorder->setText(QString());
        lbRecorder->setText(QApplication::translate("VSCDeviceList", "Recorder", 0));
#ifndef QT_NO_TOOLTIP
        pbCamera->setToolTip(QApplication::translate("VSCDeviceList", "Add IP Camera", 0));
#endif // QT_NO_TOOLTIP
        pbCamera->setText(QString());
        label_3->setText(QApplication::translate("VSCDeviceList", "Camera", 0));
#ifndef QT_NO_TOOLTIP
        pbSurveillance->setToolTip(QApplication::translate("VSCDeviceList", "Add View", 0));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        pbSurveillance->setStatusTip(QApplication::translate("VSCDeviceList", "2", 0));
#endif // QT_NO_STATUSTIP
#ifndef QT_NO_WHATSTHIS
        pbSurveillance->setWhatsThis(QApplication::translate("VSCDeviceList", "3", 0));
#endif // QT_NO_WHATSTHIS
        pbSurveillance->setText(QString());
        label_4->setText(QApplication::translate("VSCDeviceList", "View", 0));
#ifndef QT_NO_TOOLTIP
        pbSearch->setToolTip(QApplication::translate("VSCDeviceList", "Search IP Camera", 0));
#endif // QT_NO_TOOLTIP
        pbSearch->setText(QString());
        label_5->setText(QApplication::translate("VSCDeviceList", "Search", 0));
#ifndef QT_NO_TOOLTIP
        pbPlayback->setToolTip(QApplication::translate("VSCDeviceList", "Playback ", 0));
#endif // QT_NO_TOOLTIP
        pbPlayback->setText(QString());
        label_6->setText(QApplication::translate("VSCDeviceList", "Mining", 0));
        pbEmap->setText(QString());
        label->setText(QApplication::translate("VSCDeviceList", "eMap", 0));
        pbVIPC->setText(QString());
        label_8->setText(QApplication::translate("VSCDeviceList", "Virutal IPC", 0));
#ifndef QT_NO_TOOLTIP
        pbTrash->setToolTip(QApplication::translate("VSCDeviceList", "Drop IP Camera to Here", 0));
#endif // QT_NO_TOOLTIP
        pbTrash->setText(QString());
        label_7->setText(QApplication::translate("VSCDeviceList", "Trush", 0));
        QTreeWidgetItem *___qtreewidgetitem = treeWidget->headerItem();
        ___qtreewidgetitem->setText(0, QApplication::translate("VSCDeviceList", "VS Cloud", 0));

        const bool __sortingEnabled = treeWidget->isSortingEnabled();
        treeWidget->setSortingEnabled(false);
        QTreeWidgetItem *___qtreewidgetitem1 = treeWidget->topLevelItem(0);
        ___qtreewidgetitem1->setText(0, QApplication::translate("VSCDeviceList", "Cameras", 0));
        QTreeWidgetItem *___qtreewidgetitem2 = treeWidget->topLevelItem(1);
        ___qtreewidgetitem2->setText(0, QApplication::translate("VSCDeviceList", "Views", 0));
        QTreeWidgetItem *___qtreewidgetitem3 = treeWidget->topLevelItem(2);
        ___qtreewidgetitem3->setText(0, QApplication::translate("VSCDeviceList", "Disks", 0));
        QTreeWidgetItem *___qtreewidgetitem4 = treeWidget->topLevelItem(3);
        ___qtreewidgetitem4->setText(0, QApplication::translate("VSCDeviceList", "Virtual IPCs", 0));
        treeWidget->setSortingEnabled(__sortingEnabled);

    } // retranslateUi

};

namespace Ui {
    class VSCDeviceList: public Ui_VSCDeviceList {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VSCDEVICELIST_H

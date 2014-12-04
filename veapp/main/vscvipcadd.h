#ifndef __VSC_VIPC_ADD_H
#define __VSC_VIPC_ADD_H

#include <QWidget>
#include "ui_vscvipcadd.h"
#include "utility.hpp"
#include "factory.hpp"
#include "vscvwidget.h"

class VSCVIPCAdd : public QWidget
{
    Q_OBJECT

public:
    VSCVIPCAdd(VIPCDeviceParam &Param, QWidget *parent);
    ~VSCVIPCAdd();
public:
    void mouseDoubleClickEvent(QMouseEvent *e);
    void createContentMenu();
    void setupDefaultValue();
    void updateParamValue(QLineEdit *ld, s8 * pParam);
	
private slots:

    void radioButtonClicked();
    void applyConfig();
signals:

public:
    void SetupConnections();
	
public:
    Ui::VSCVIPCAdd ui;
private:
    QWidget *m_pParent;
    QAction *m_pFloating;
    QAction *m_pUnFloating;
    BOOL m_bFloated;
    VIPCDeviceParam m_Param;
    u32 m_nId;
};

#endif // __VSC_VIPC_ADD_H

#ifndef VSC_CAMERA_ADD_H
#define VSC_CAMERA_ADD_H

#include <QWidget>
#include "ui_vsccameraadd.h"
#include "utility.hpp"
#include "factory.hpp"
#include "vscvwidget.h"

class VSCCameraAdd : public QWidget
{
    Q_OBJECT

public:
    VSCCameraAdd(DeviceParam &Param, QWidget *parent);
    ~VSCCameraAdd();
public:
    void mouseDoubleClickEvent(QMouseEvent *e);
    void createContentMenu();
    void setupDefaultValue();
    void updateParamValue(QLineEdit *ld, s8 * pParam);
	
private slots:
    void floatingAction();
    void unFloatingAction();

    void radioButtonClicked();
    void applyConfig();
signals:
    //void CameraTreeUpdated();

public:
    void SetupConnections();
    void PreView();
	
public:
    Ui::VSCCameraAdd ui;
private:
    QWidget *m_pParent;
    QAction *m_pFloating;
    QAction *m_pUnFloating;
    BOOL m_bFloated;
    DeviceParam m_Param;
    u32 m_nId;
    VSCVWidget *m_pVideo;
    
};

#endif // VSCVIEW_H


#ifndef VSCMAINWINDOWS_H
#define VSCMAINWINDOWS_H

#include <QtWidgets/QMainWindow>
#include "ui_vscmainwindows.h"
#include "vevent.hpp"
#include "vscdockwidget.h"
#include "vscview.h"

class VSCDeviceList;
class VSCToolBar;
class VSCMainWindows : public QMainWindow
{
    Q_OBJECT

public:
    void SetupToolBar();
    void SetupMenuBar();
    void CreateActions();
    void CreateDockWindows();
    void SetupConnections();


public slots:
    void AddSurveillance();
    void AddCamera();
    void AddEmap();
    void AddRecorder();
    
    void Search();
    void Setting();
    void EditCamera(int nId);
    void DeleteCamera(int nId);
    void MainCloseTab(int index);
    void about();
    void UserStatus();
    void SetFullScreen();
    void AddEvent();
    void EditDisk();

    /* Site */
    void AddSite();


    /* VIPC */
    void AddVIPC();
    void EditVIPC(int nId);
    void DeleteVIPC(int nId);

    /* View */
    void DeleteView(int nId);

    /* VIPC */
    void AddVGroup();
    void EditVGroup(int nId);
    void DeleteVGroup(int nId);
    void MapVGroup();

signals:
    void CameraDeleted();
protected:
       void closeEvent(QCloseEvent* e)
       {
           QMainWindow::closeEvent(e);
           QApplication::quit();
       }
public:
    VSCMainWindows(QWidget *parent = 0);
    ~VSCMainWindows();
private:
    QTabWidget * m_pMainArea;
    VSCView *m_pView;

private:
    QAction *pActSurveillance;
    QAction *pActSearch;



    QAction *pActDeviceList;
    QAction *pActDeviceAdd;
    QAction *pActDeviceDel;
    QAction *pActDeviceConf;
    QAction *aboutAct;
    QAction *aboutQtAct;

private:
    VSCDeviceList * m_pDeviceList;
    VSCToolBar * m_pToolBar;
    VEventThread * m_pEventThread;

private:
    Ui::VSCMainWindowsClass ui;
};

#endif // VSCMAINWINDOWS_H

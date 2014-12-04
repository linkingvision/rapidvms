#ifndef VSC_SEARCH_H
#define VSC_SEARCH_H

#include <QWidget>
#include "ui_vscsearch.h"
#include "utility.hpp"
#include "factory.hpp"
#include "QEvent"
#include "devicesearcher.h"

using namespace ONVIF;

class VSCSearch : public QWidget
{
    Q_OBJECT
public:
    VSCSearch(QWidget *parent = 0);
    ~VSCSearch();

public:
    void SetupConnections();
	
public:
    static void SearchCallback(void *pParam, char *pIPAddr, char *pOnvifAddr, int type);
    void SearchCallback1(char *pIPAddr, char *pOnvifAddr, int type);
    void OnvifCallProfileS();
	void updateParamValue(QTableWidgetItem *item, s8 * pParam);
	void intTest(int a);
	
	bool event(QEvent *event);
	bool CheckIP(s8 * ipAddr);
	bool CheckPort(s8 * Port);
	
public slots:
    void StartSearch();
    void StopSearch();
    void AddAll();
	void SelectAll();
	void AddItem(astring IP, astring Port, astring Manufacturer, astring Model, 
	astring FirmwareVersion, astring ONVIFAddress);
	
signals:
    //void CameraTreeUpdated();
	void NewSearchedItem(astring IP, astring Port, astring Manufacturer, astring Model, astring FirmwareVersion, astring ONVIFAddress);
public slots:
    void SearchReceiveData(const QHash<QString, QString> &data);

public:
    Ui::VSCSearch ui;    
    int m_nSearchCnt;
    static BOOL m_bStarted;
    DeviceSearcher *m_DeviceSearcher;
    int m_nSearchProcess;

private:
	BOOL m_bSelectedAll;
};

#endif // VSCVIEW_H

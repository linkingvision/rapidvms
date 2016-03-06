#ifndef __V_VID_PB_CONTROL_H__
#define __V_VID_PB_CONTROL_H__

#include <QWidget>

#define NOMINMAX
#define NOMINMAX 
//#include <inttypes.h>


#include "tinythread.h"
#include "fast_mutex.h"
#include "utility.hpp"
#include "videotype.hpp"
#include "vtaskmgr.hpp"

using  namespace tthread;

namespace Ui {
class VVidPBControl;
}
class VSCDateSet;
class VSCExport;

class VE_LIBRARY_API VVidPBDelegate
{
public:
	VVidPBDelegate(){}
	~VVidPBDelegate(){}
public:
	virtual bool SearchRec(u32 nStart, u32 nEnd, RecordItemMap &pMap){return false;}
	virtual bool SearchHasRec(HasRecordItemMap &pMap){ return false; }
	virtual VTaskItem * CreateExportTask(u32 nStart, u32 nEnd){return NULL;}
};

class VE_LIBRARY_API VVidPBControl : public QWidget
{
    Q_OBJECT

public:
	VVidPBControl(QWidget *parent = 0, Qt::WindowFlags flags = 0);
	~VVidPBControl();
public:
	bool SetDelegate(VVidPBDelegate *pDelegate);
	bool SetPlayParam(astring strStor, astring strId, astring strCamName);
	bool SetPlaying(bool bPlaying);
	
public slots:
	void SlotDateSetting();
	void SlotExport();
	void SlotSeekTime(unsigned int nTime);
	void SlotSeekTimeFromDate(unsigned int nTime);
	void SlotPlay();
	void SlotPause();
	void SlotUpdateTime(unsigned int nTime);
	void SlotClearPlay();

signals:
	void SignalTimeUpdate(unsigned int nTime);
	void SignalSeekTime(unsigned int nTime);
	void SignalPlay();
	void SignalPause();

public:
	Ui::VVidPBControl *p_ui;
	Ui::VVidPBControl &ui;
private:
	VSCDateSet * m_pDateSetting;
	VSCExport * m_pExport;
	bool m_bPause;
	astring m_strStor;
	astring m_strId;
	astring m_strCamName;
};

#endif // __V_VID_PB_CONTROL_H__

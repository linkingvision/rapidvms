#ifndef __VSC_VID_LIST_H__
#define __VSC_VID_LIST_H__

#include <QWidget>
#include <QVBoxLayout>
#include "ui_vscvidlist.h"
#include "server/factory.hpp"
#include "common/vscvidinf.h"
#include "QTimer"

class VSCVidList : public QWidget
{
	Q_OBJECT
public:
	VSCVidList(QWidget *parent);
	~VSCVidList();
public:
	/* set the live conf or Pb Vid interface */
	void SetCurrVidInf(VSCVidInf *pVidInf);
	
public slots:
	void SlotShowCameraTree();
	void SlotShowGroupTree();
	void SlotShowViewTree();
	void SlotShowEmapTree();

private:
	Ui::VSCVidList ui;
private:
	VSCVidInf * m_pVidInf;
	QVBoxLayout* m_treeLayout;
	VSCVidTreeInf *m_pCameraTree;
	VSCVidTreeInf *m_pGroupTree;
	VSCVidTreeInf *m_pViewTree;
	VSCVidTreeInf *m_pEmapTree;
    
};

#endif // __VSC_VID_LIST_H__

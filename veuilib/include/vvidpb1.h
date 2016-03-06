#ifndef __V_VID_PB_1_H__
#define __V_VID_PB_1_H__

#include <QWidget>

#define NOMINMAX
#define NOMINMAX 
//#include <inttypes.h>


#include "tinythread.h"
#include "fast_mutex.h"
#include "utility.hpp"
#include "vvidpbwidget.h"
#include "vscvwidget.h"
#include "server/factory.hpp"
#include "client/clientfactory.hpp"
#include "client/storstream.hpp"
using  namespace tthread;

namespace Ui {
class VVidPB1;
}

class VE_LIBRARY_API VVidPB1 : public QWidget
{
    Q_OBJECT

public:
	VVidPB1(ClientFactory &pFactory, QWidget *parent = 0, Qt::WindowFlags flags = 0);
	~VVidPB1();
private slots:
	void ShowFocusClicked(int nId);
public:
	bool StartPlay(astring strStorId, astring strCamId, 
			astring strCamName,  u32 nPlaytime);

public:
	Ui::VVidPB1 *p_ui;
	Ui::VVidPB1 &ui;
private:
	VVidPBWidget *m_pVideo;
	//VSCVWidget *m_pVideo;
	ClientFactory &m_pFactory;
	astring m_strStor;
	astring m_strCam;
	astring m_strCamName;
};


#endif // __V_VID_PB_1_H__

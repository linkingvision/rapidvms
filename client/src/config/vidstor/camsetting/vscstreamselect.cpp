#include <QMessageBox>
#include <QVariant>
#include "config/vidstor/camsetting/vscstreamselect.h"
#include "debug.hpp"
#include "vscloading.hpp"
#include "client/storsyncinf.hpp"

VSCStreamSelect::VSCStreamSelect(ClientFactory &pFactory, VidStor &stor, astring strCam, QWidget *parent)
	: QWidget(parent), m_pFactory(pFactory), m_pStor(stor), m_strCam(strCam)
{
	ui.setupUi(this);
	VSCLoading * pLoading = VSCLoading::Create();
	StorSyncInf syncInf(m_pStor);
	
	syncInf.Connect();


	VidCamera pCam;
	
	syncInf.GetVidCamera(m_strCam, pCam);
	
	/* Get the stream list from server */
	VidStreamList streamList;
	syncInf.GetStreamList(m_strCam, streamList);
	for (s32 i = 0; i < streamList.cvidstream_size(); i ++)
	{
		VidStream stream = streamList.cvidstream(i);
		ui.stream1->insertItem(i, stream.strname().c_str(), stream.strtoken().c_str());
		ui.stream2->insertItem(i, stream.strname().c_str(), stream.strtoken().c_str());
	}
	ui.stream1->setEditable(false);
	ui.stream2->setEditable(false);

	ui.stream1->setCurrentIndex(0);
	ui.stream2->setCurrentIndex(1);

	/* Set the current select items */
	for (s32 i = 0; i < streamList.cvidstream_size(); i ++)
	{
		VidStream stream = streamList.cvidstream(i);
		if (pCam.strprofiletoken1() == stream.strtoken())
		{
			ui.stream1->setCurrentIndex(i);
		}
		if (pCam.strprofiletoken2() == stream.strtoken())
		{
			ui.stream2->setCurrentIndex(i);
		}
	}

	if (pCam.bprofiletoken() == false)
	{
		/* If auto,  just disable it */
		ui.stream1->setEnabled(false);
		ui.stream2->setEnabled(false);
		ui.autoSelect->setToggle(false);
	}else
	{
		ui.autoSelect->setToggle(true);
		ui.stream1->setEnabled(true);
		ui.stream2->setEnabled(true);
	}
	if (pCam.ntype() != VID_ONVIF_S)
	{
		ui.autoSelect->setToggle(false);
	}
	
	connect( this->ui.pushButtonApply, SIGNAL( clicked() ), this, SLOT(applyConfig()));
	connect( this->ui.autoSelect, SIGNAL( toggled(bool) ), this, SLOT(autoSelectClicked(bool)));

	delete pLoading;
}

void VSCStreamSelect::applyConfig()
{
	VSCLoading loading(NULL);
	StorSyncInf syncInf(m_pStor);
	
	syncInf.Connect();


	VidCamera pCam;
	
	syncInf.GetVidCamera(m_strCam, pCam);

	pCam.set_bprofiletoken(ui.autoSelect->isToggled());
	if (ui.autoSelect->isToggled() == true)
	{
		QVariant currentData1 = ui.stream1->currentData();
		astring strToken1 = currentData1.toString().toStdString();
		pCam.set_strprofiletoken1(currentData1.toString().toStdString());
		QVariant currentData2 = ui.stream2->currentData();
		astring strToken2 = currentData2.toString().toStdString();
		pCam.set_strprofiletoken2(currentData1.toString().toStdString());
	}

	
	syncInf.AddCam(pCam);
	return;

}

void VSCStreamSelect::autoSelectClicked(bool checked)
{
	if (ui.autoSelect->isToggled() == true)
	{
		ui.stream1->setEnabled(true);
		ui.stream2->setEnabled(true);		
	}else
	{
		ui.stream1->setEnabled(false);
		ui.stream2->setEnabled(false);		
	}
}



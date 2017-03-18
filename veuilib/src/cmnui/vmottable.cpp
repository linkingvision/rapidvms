/** <!--
 *
 *  Copyright (C) 2017 veyesys support@veyesys.com
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Affero General Public License as
 *  published by the Free Software Foundation, either version 3 of the
 *  License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Affero General Public License for more details.
 *
 *  You should have received a copy of the GNU Affero General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  If you would like this software to be made available to you under an 
 *  alternate commercial license please email support@veyesys.com 
 *  for more information.
 *
 * -->
 */
#include "vmottable.hpp"
#include "recordwrapper.hpp"
#include <QRubberBand>
#include <QMouseEvent>
#include <QLabel>
#include <QPainter>

class VMotTablePri
{
public:
	VMotTablePri();
	~VMotTablePri();
public:
	VVidMotReg conf;
	QRubberBand* pRubberBand;
	int nStartX;
	int nStartY;
	QNetworkAccessManager *pNetwManager;
	QPixmap motBackground;
};

VMotTablePri::VMotTablePri()
	:nStartX(0), nStartY(0), pNetwManager(NULL)
{

}
VMotTablePri::~VMotTablePri()
{
	if (pNetwManager)
	{
		delete pNetwManager;
		pNetwManager = NULL;
	}
}

VMotTable::VMotTable(QWidget *parent, Qt::WindowFlags flags)
	: QWidget(parent, flags), 
	m_pPri(NULL), m_bPressed(false)
{
	/* Width min 24 hour * 30,  heigh min 7 day * 30 */
	m_pPri = new VMotTablePri();
	m_pPri->pRubberBand = new QRubberBand(QRubberBand::Rectangle,this);
 	m_pPri->pRubberBand->setStyleSheet("border:1px solid red; background-color:rgb(200, 200, 200);");
	m_pPri->pNetwManager = new QNetworkAccessManager(this);
	connect(m_pPri->pNetwManager, SIGNAL(finished(QNetworkReply*)), 
					this, SLOT(SlotNetowkManagerFinished(QNetworkReply*)));
}

bool VMotTable::UpdateImage(astring strUrl)
{
	QUrl url(strUrl.c_str());
	QNetworkRequest request(url);
	m_pPri->pNetwManager->get(request);
	return true;
}

VMotTable::~VMotTable()
{
	if (m_pPri)
	{
		delete m_pPri;
		m_pPri = NULL;
	}
	
}

void VMotTable::SlotNetowkManagerFinished(QNetworkReply *reply)
{
	if (reply->error() != QNetworkReply::NoError) {
	    //qDebug() << "Error in" << reply->url() << ":" << reply->errorString();
	    return;
	}
	QVariant attribute = reply->attribute(QNetworkRequest::RedirectionTargetAttribute);
	if (attribute.isValid()) {
	    QUrl url = attribute.toUrl();
	    //qDebug() << "must go to:" << url;
	    return;
	}
	//qDebug() << "ContentType:" << reply->header(QNetworkRequest::ContentTypeHeader).toString();
	QByteArray jpegData = reply->readAll();
	m_pPri->motBackground.loadFromData(jpegData);
	update();
}


bool VMotTable::SetCurrentMot(VVidMotReg &pMot)
{
	m_pPri->conf = pMot;

	return true;
}

bool VMotTable::GetCurrentMot(VVidMotReg &pMot)
{	
	pMot = m_pPri->conf;
	return true;
}

void VMotTable::paintEvent(QPaintEvent *e)
{
	/* Move the label */
	double widthPerW = (double)width()/(double)(VVID_MOT_REGION_W);
	double heightPerH = (double)height()/(double)(VVID_MOT_REGION_H);

	QPainter painter(this);
	painter.setPen(Qt::NoPen);

	painter.drawPixmap(0, 0, width(), height(), m_pPri->motBackground);
	for (int i = 0; i < VVID_MOT_REGION_H; i ++)
	{
		for (int j = 0; j < VVID_MOT_REGION_W; j ++)
		{
			if (m_pPri->conf.line[i].enable[j] == 0)
			{
				painter.setBrush(QBrush(QColor(80,80,80, 80),Qt::SolidPattern));
			}
			else if (m_pPri->conf.line[i].enable[j] == 1) /* Red */
			{
				painter.setBrush(QBrush(QColor(255,0,0, 80),Qt::SolidPattern));
			}else 
			{
				painter.setBrush(QBrush(QColor(255,0,0, 80),Qt::SolidPattern));
			}
            		painter.drawRect(widthPerW * (double)j, heightPerH * (double)i,
						widthPerW - 1, heightPerH - 1);

		}
	}

	return;

}
void VMotTable::mousePressEvent(QMouseEvent *e)
{
	QPoint pos = e->pos();
	
	m_pPri->nStartX = pos.x();
	m_pPri->nStartY = pos.y();
	m_pPri->pRubberBand->setGeometry(pos.x(),pos.y(),0,0);
	m_pPri->pRubberBand->show();	
	m_bPressed = true;
}
void VMotTable::mouseMoveEvent(QMouseEvent *e)
{
	QPoint pos = e->pos();
	if(pos.x() >= this->rect().right())
	{
	    pos.setX(this->rect().right());
	}

	if(pos.x() <= this->rect().left())
	{
	    pos.setX(this->rect().left());
	}

	if(pos.y() <= this->rect().top())
	{
	    pos.setY(this->rect().top());
	}

	if(pos.y() >= this->rect().bottom())
	{
	    pos.setY(this->rect().bottom());
	}

	m_pPri->pRubberBand->setGeometry(m_pPri->nStartX,
				m_pPri->nStartY, pos.x()-m_pPri->nStartX, pos.y()-m_pPri->nStartY);
	m_pPri->pRubberBand->show();
	
}
void VMotTable::mouseReleaseEvent(QMouseEvent *e)
{
}

bool VMotTable::UpdateMotType(bool type)
{
	if (m_bPressed == false)
	{
		return true;
	}else
	{
		m_bPressed = false;
	}
	double widthPerW = (double)width()/(double)(VVID_MOT_REGION_W);
	double heightPerH = (double)height()/(double)(VVID_MOT_REGION_H);

	int nStartX = m_pPri->nStartX/widthPerW;
	int nStartY = m_pPri->nStartY/heightPerH;

	int debugX = nStartX + m_pPri->pRubberBand->width() / widthPerW;
	int debugY = nStartY + m_pPri->pRubberBand->height() / heightPerH;

	for (int x = nStartX; x <= nStartX + m_pPri->pRubberBand->width()/widthPerW; 
				x ++)
	{
		for (int y = nStartY; y <= nStartY + m_pPri->pRubberBand->height()/heightPerH; 
		y ++)
		{
			m_pPri->conf.line[y].enable[x] = (u8)type;
			VDC_DEBUG("%s  (%d, %d)\n", __FUNCTION__, x, y);
		}
	}
	
	update();
	m_pPri->pRubberBand->hide();

	return true;
}
void VMotTable::SlotMotOff()
{
	UpdateMotType(false);
}
void VMotTable::SlotMotOn()
{
	UpdateMotType(true);
}

bool VMotTable::SetMotAll()
{
	m_pPri->conf = VVidMotReg::CreateAll();
	update();
	return true;
}
bool VMotTable::SetMotClear()
{
	m_pPri->conf = VVidMotReg::CreateOff();
	update();
	return true;
}


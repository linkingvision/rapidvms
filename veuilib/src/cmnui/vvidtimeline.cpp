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
#include "vvidtimeline.h"
#include "vvidpbcontrol.h"

const QString TimeLineGrid::mTimeFormat = "hh:mm:ss";
const QString TimeLineGrid::mDayFormat = "yyyy/MM/dd";
const QString TimeLineGrid::mDayTimeFormat = "yyyy/MM/dd   hh:mm:ss";
const double TimeLineGrid::mOverlayOpacity = 0.3;

TimeLineGrid::TimeLineGrid(QGraphicsItem *parent) : QGraphicsItem(parent)
{

}

void TimeLineGrid::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{    
    //painter->fillRect(boundingRect(), QBrush(QColor(230, 230, 230, 255)));

#ifdef DEBUG
    painter->drawText(mSettings.borderIndentX + 10,
                      mSize.height() - 30 - mSettings.borderIndentX,
                      "BeginTime : " + mTimeCenterMark.addMSecs(-1 * (quint64)mTimeDelta).toString());

    painter->drawText(mSettings.borderIndentX + 10,
                      mSize.height() - 15 - mSettings.borderIndentX,
                      "EndTime : " + mTimeCenterMark.addMSecs((quint64)mTimeDelta).toString());

    painter->drawText(mSettings.borderIndentX + 10,
                      mSize.height() - 45 - mSettings.borderIndentX,
                      "MousePos : " + QString::number(mMousePos.x()));
#endif

    painter->setPen(QPen(mStyle.borderColor));

    // Paint item area border
    //painter->drawRect(graphicsRect());

    // Paint central mark and mouse mark and corresponding texts
    if (mTimeCenterMark.isValid()){
        drawMarks(painter);
    }
}

void TimeLineGrid::drawMarks(QPainter *painter)
{
    quint64 startTime = mTimeCenterMark.toMSecsSinceEpoch() - mTimeDelta;
    quint64 endTime = mTimeCenterMark.toMSecsSinceEpoch() + mTimeDelta;
    //quint64 currTime = QDateTime::currentDateTime().toMSecsSinceEpoch();
    quint64 currTime = mTimeCenterMark.toMSecsSinceEpoch();
    double msecPerPixel = (2 * mTimeDelta) / mSize.width();    

    QFont font = painter->font();
    
    QString textFormat = 2 * mTimeDelta < day ? mTimeFormat : mDayFormat;
    font.setPointSize(mSettings.borderIndentY * 0.5);
    QFontMetrics fm(font);
    font.setBold(true);
    painter->setFont(font);

    // Current time mark and it's text
    QPair<int, int> currTimeMarkBorders(-1, -1);
    drawCurrTimeMark(msecPerPixel, currTime, startTime, endTime,
                     currTimeMarkBorders, mDayTimeFormat, fm, painter);

    // Mouse time mark and it's text
    drawMouseTimeMark(painter);


    // Grid marks
    drawGridMarks(fm, msecPerPixel, startTime, textFormat, currTimeMarkBorders, painter);
}

void TimeLineGrid::drawGridMarks(const QFontMetrics& fm, const double& msecPerPixel,
                                 quint64& startTime, const QString textFormat,
                                 QPair<int, int> &currTimeMarkBorders, QPainter *painter)
{
    painter->setPen(QPen(mStyle.timeMarksTextColor));

    // calculate step between grid items in msec
    double pixelsPerMsec = 1 / msecPerPixel;
    double part = mMousePos.x() / mSize.width();
    quint64 mSecSinseStart = 2 * mTimeDelta*part + mTimeCenterMark.addMSecs(-1 * (quint64)mTimeDelta).toMSecsSinceEpoch();
    QString mouseTimeString = QDateTime().fromMSecsSinceEpoch(mSecSinseStart).toString("yyyy/MM/dd hh:mm:ss");
    quint16 textWidth = fm.width(mouseTimeString);
    quint16 maxNumberOfTextMarks = mSize.width() / (textWidth*1.5);
    int triangleRectWidth = (mSettings.borderIndentY - fm.height()) / 2 + 1;

    if (maxNumberOfTextMarks == 0){
        return;
    }

    quint64 stepMsec = calculateStep(maxNumberOfTextMarks);

    // find first item
    quint64 timeMark = startTime;
    if (startTime % stepMsec){
        timeMark = (startTime / stepMsec - 1)*stepMsec;
    }

    while (true)
    {
        // to make text appear smoothly
        int pos = timeMark > startTime ?
            pixelsPerMsec * (timeMark - startTime) : -pixelsPerMsec * (startTime - timeMark);

        QString timeText = QDateTime().fromMSecsSinceEpoch(timeMark).toString(textFormat);
        if (pos - fm.width(timeText) / 2 < mSize.width() - mSettings.borderIndentX)
        {
            // make an item semitransparent when it overlays the current mark text
            double opacity = 1;
            if (currTimeMarkBorders.first != -1 && currTimeMarkBorders.second != -1)
            {
                QPair<int, int> intersection(std::max(currTimeMarkBorders.first, pos - fm.width(timeText) / 2),
                                             std::min(currTimeMarkBorders.second, pos + fm.width(timeText) / 2));

                if (intersection.first < intersection.second){
                    opacity = mOverlayOpacity;
                }
            }

            painter->setOpacity(opacity);
            painter->drawLine(pos, mSettings.borderIndentY, pos, mSettings.borderIndentY - triangleRectWidth);
            paintText(true, pos, timeText, painter, mStyle.timeMarksTextColor);

            timeMark += stepMsec;
        }
        else{
            break;
        }
    }
}

void TimeLineGrid::drawCurrTimeMark(const double &msecPerPixel, const quint64 &currTime, const quint64& startTime,
                                    const quint64 &endTime, QPair<int, int>& currTimeMarkBorders,
                                    const QString textFormat, const QFontMetrics &fm, QPainter *painter)
{
    // Current time mark and it's text
    QString currMarkTimeString = mTimeCenterMark.toString(textFormat);
    quint16 currTimeMarkWidth = fm.width(currMarkTimeString);
    quint64 currTimeMarkWidthMsec = currTimeMarkWidth * msecPerPixel;

    QPair<quint64, quint64> intersection(std::max(currTime - currTimeMarkWidthMsec, startTime),
                                         std::min(currTime + currTimeMarkWidthMsec, endTime));

    if (intersection.first < intersection.second)
    {
    	 QPen linePen(mStyle.timeMarksTextColor);
        linePen.setWidth(2);
        painter->setPen(linePen);

        double pixelsPerMsec = 1/msecPerPixel;
        int currTimePos = (currTime > startTime) ?
            pixelsPerMsec * (currTime - startTime) : -pixelsPerMsec * (startTime - currTime);

        currTimeMarkBorders.first = currTimePos - currTimeMarkWidth/2;
        currTimeMarkBorders.second = currTimePos + currTimeMarkWidth/2;

        // line
        if (currTimePos < mSize.width() - mSettings.borderIndentX &&
            currTimePos > mSettings.borderIndentX)
        {
            painter->drawLine(currTimePos, mSettings.borderIndentY, currTimePos, mSize.height() - mSettings.borderIndentY);
        }
	 painter->setPen(QPen(mStyle.currMarkColor));
        // curr time mark text, the size is calculated depending on the indent from the border
        paintText(true, currTimePos, currMarkTimeString, painter, mStyle.currMarkColor);
    }
}

void TimeLineGrid::drawMouseTimeMark(QPainter *painter)
{
    // The mouse mark and it's text


    // line, border check
    int linePosX = mMousePos.x();
    QRect area = graphicsRect();

    if (linePosX < area.left()){
        linePosX = area.left();
    }
    else if (linePosX > area.right()){
        linePosX = area.right();
    }

    QPen linePen(mStyle.mouseMarkColor);
    linePen.setWidth(2);
    painter->setPen(linePen);
    painter->drawLine(linePosX, mSettings.borderIndentY, linePosX, mSize.height() - mSettings.borderIndentY);

    painter->setPen(QPen(mStyle.mouseMarkColor));
    // mouse time mark text, the size is calculated depending on the indent from the border
    double part = mMousePos.x() / mSize.width();
    quint64 mSecSinseStart = 2 * mTimeDelta*part + mTimeCenterMark.addMSecs(-1 * (quint64)mTimeDelta).toMSecsSinceEpoch();
    QString mouseTimeString = QDateTime().fromMSecsSinceEpoch(mSecSinseStart).toString("yyyy/MM/dd hh:mm:ss");
    paintText(false, linePosX, mouseTimeString, painter, mStyle.mouseMarkColor);
}

quint64 TimeLineGrid::calculateStep(const int& maxNumberOfTextMarks)
{
    quint64 stepMsec = 2 * mTimeDelta / maxNumberOfTextMarks;
    const quint64 month = (quint64)day * 30;
    const quint64 year = (quint64)day * 365;

    if (stepMsec < second){
        return quint64(-1); //error value
    }

    quint64 timeUnit = second;
    quint64 tempStepMsec = stepMsec;

    while (true)
    {
        if (tempStepMsec >= minute &&  tempStepMsec < hour){
            timeUnit = minute;
        }
        else if (tempStepMsec >= hour &&  tempStepMsec < day){
            timeUnit = hour;
        }
        else if (tempStepMsec >= day &&  tempStepMsec < month){
            timeUnit = day;
        }
        else if (tempStepMsec >= month && tempStepMsec < year){
            timeUnit = month;
        }
        else if (tempStepMsec >= year){
            timeUnit = year;
        }

        int numberOfUnits = tempStepMsec / timeUnit;
        if (numberOfUnits > 2){
            numberOfUnits = 5 * std::ceil((double)numberOfUnits / 5);
        }

        tempStepMsec = numberOfUnits * timeUnit;
        int currNumberOfTimeMarks = 2 * mTimeDelta / tempStepMsec;

        if (currNumberOfTimeMarks > maxNumberOfTextMarks) //minutes or seconds
        {
            switch (timeUnit)
            {
            case second: tempStepMsec += second; break;
            case minute: tempStepMsec += minute; break;
            case hour: tempStepMsec   += hour; break;
            case day: tempStepMsec    += day; break;
            case month: tempStepMsec  = year; break;
            case year: tempStepMsec   += year; break;
            default: break;
            }

            continue;
        }

        if ((timeUnit == second || timeUnit == minute) && numberOfUnits > mMaxNumberOfUnits)
        {
            tempStepMsec = 60 * timeUnit; // promote to the next time unit
            continue;
        }

        if (2 * mTimeDelta > day && timeUnit < day){
            tempStepMsec = day;
        }

        stepMsec = tempStepMsec;
        break;
    }

    return stepMsec;
}

void TimeLineGrid::paintText(bool topBottom, int xPos, QString text, QPainter* painter, QColor color)
{
    QPen pen = painter->pen();
    QFont font = painter->font();
    QFontMetrics fm(font);

    pen.setColor(color);
    painter->setPen(pen);

    int yPos = topBottom ? (mSettings.borderIndentY - fm.height()) / 2 :
                            mSize.height() - fm.height() - (mSettings.borderIndentY - fm.height()) / 2;

    QRect textRect(xPos - fm.width(text) / 2, yPos, fm.width(text), fm.height());
    painter->drawText(textRect, Qt::AlignCenter, text);
}

bool TimeLineGrid::setTimeRange(const QDateTime& centralTime, const quint64& timeDelta)
{
    // If the new scale is valid, set it
    if (timeDelta >= mSettings.maximumScale && timeDelta <= mSettings.minimumScale)
    {
        mTimeCenterMark = centralTime;
        mTimeDelta = timeDelta;
        update();

        emit rangeChanged(mTimeCenterMark.addMSecs((-1)*timeDelta), mTimeCenterMark.addMSecs(timeDelta));

        return true;
    }

    return false;
}

void TimeLineGrid::setStyle(const TimeLineGridStyle& style)
{
    mStyle = style;
}

void TimeLineGrid::setSettings(const TimeLineGridSettings& settings)
{
    mSettings = settings;
}

void TimeLineGrid::setMousePos(const QPoint &pos, bool isDragging)
{
    if (isDragging)
    {
        int mouseDelta = mMousePos.x() - pos.x();
        if (mouseDelta != 0)
        {
            int sign = mouseDelta / std::abs(mouseDelta);
            quint64 deltaMSec = 2 * mTimeDelta*std::abs(mouseDelta / mSize.width());
            mTimeCenterMark = mTimeCenterMark.addMSecs(sign*deltaMSec);
        }
    }

    mMousePos = pos;
    update();
}

void TimeLineGrid::setSize(const QSizeF &size, const QPointF& pos)
{
    mSize = size;
    setPos(pos);
}

QDateTime TimeLineGrid::getTimeMark() const
{
    return mTimeCenterMark;
}

quint64 TimeLineGrid::getTimeDelta() const
{
    return mTimeDelta;
}

QPoint TimeLineGrid::getMousePos() const
{
    return mMousePos;
}

TimeLineGrid::TimeLineGridSettings TimeLineGrid::getSettings() const
{
    return mSettings;
}

TimeLineGrid::TimeLineGridStyle TimeLineGrid::getStyle() const
{
    return mStyle;
}

QRectF TimeLineGrid::boundingRect() const
{
    return QRectF(0, 0, mSize.width(), mSize.height());
}

QRect TimeLineGrid::graphicsRect() const
{
    // Timeline item painting area, with indents taken into account
    return QRect(mSettings.borderIndentX,
                 mSettings.borderIndentY,
                 mSize.width() - 2 * mSettings.borderIndentX,
                 mSize.height() - 2 * mSettings.borderIndentY);
}


TimeLineThread::TimeLineThread()
:m_pDelegate(NULL), m_SearchStart(0), m_SearchEnd(0), 
m_bCleared(true)
{
}

bool TimeLineThread::SetDelegate(VVidPBDelegate *pDelegate)
{
	XGuard guard(m_cMutex);
	m_pDelegate = pDelegate;
	setTime(mCentralTime, mTimeDelta);
	return true;
}

bool TimeLineThread::ClearPlay()
{
	VVidTimeLineCmd cmd;
	cmd.type = VVID_TIMELINE_CMD_CLEAR;

	m_Queue.Push(cmd);
	return true;
}

void TimeLineThread::Quit()
{
	VVidTimeLineCmd cmd;
	cmd.type = VVID_TIMELINE_CMD_QUIT;

	m_Queue.Push(cmd);
	wait();
}

void TimeLineThread::setTime(const QDateTime& centralTime, const quint64& timeDelta)
{
	VVidTimeLineCmd cmd;
	cmd.type = VVID_TIMELINE_CMD_TIME;
	cmd.timeCenter = centralTime.toMSecsSinceEpoch();
	cmd.timeDelta = timeDelta;
	m_Queue.Push(cmd);
}

bool TimeLineThread::GetRecItems(RecordItemMap &pRecItems)
{
	XGuard guard(m_cMutex);
	pRecItems = m_RecItems;
	return true;
}

void TimeLineThread::run()
{
	while(1)
	{
		m_Queue.BlockingPeek();

		VVidTimeLineCmd cmd = m_Queue.Pop();
		switch (cmd.type)
		{
			case VVID_TIMELINE_CMD_QUIT:
			{
				return;	
				break;
			}
			case VVID_TIMELINE_CMD_TIME:
			{
				mCentralTime.setMSecsSinceEpoch(cmd.timeCenter);
				mTimeDelta = cmd.timeDelta;
    				quint64 startTime = cmd.timeCenter - mTimeDelta;
				if (startTime < 0)
				{
					startTime = 0;
				}
    				quint64 endTime = cmd.timeCenter + mTimeDelta;

				if (m_SearchEnd == 0 
					|| startTime < m_SearchStart 
					|| endTime > m_SearchEnd || m_bCleared == true)
				{
					UpdateSearch();
					m_bCleared = false;
				}else 
				{
					/* Do not need update time */
				}
				break;
			}
			case VVID_TIMELINE_CMD_CLEAR:
			{
				XGuard guard(m_cMutex);

				m_RecItems.clear();
				m_bCleared = true;
				emit RecItemUpdated();
				break;
			}
			default:
			{
				break;
			}
		}
	}
}

void TimeLineThread::UpdateSearch()
{
	if (m_pDelegate == NULL)
	{
		return;
	}	
	quint64 nCurrent = mCentralTime.toMSecsSinceEpoch();
	quint64 nTimeSearch = 3600 * 24 * 1000;
	/* Total 48 hours */
	m_SearchStart = nCurrent - nTimeSearch;
	if (m_SearchStart < 0)
	{
		m_SearchStart = 0;
	}
	m_SearchEnd = nCurrent + nTimeSearch;
	
	

	
	if (m_pDelegate)
	{
		/* Update the record list by the delegate */
		RecordItemMap cRecItems;
		m_pDelegate->SearchRec(m_SearchStart/1000, m_SearchEnd/1000, cRecItems);
		XGuard guard(m_cMutex);
		m_RecItems.clear();
		m_RecItems = cRecItems;
		emit RecItemUpdated();
	}
}


TimeLineItems::TimeLineItems(QGraphicsItem *parent) :
QGraphicsItem(parent)
{
	connect(&m_pThread, SIGNAL(RecItemUpdated()),
					this, SLOT(SlotUpdateSearch()), Qt::QueuedConnection);
	m_pThread.start();
	
}

TimeLineItems::~TimeLineItems()
{
	m_pThread.Quit();
}

void TimeLineItems::SlotUpdateSearch()
{
	m_pThread.GetRecItems(m_RecItems);
	update();
}

bool TimeLineItems::SetDelegate(VVidPBDelegate *pDelegate)
{
	XGuard guard(m_cMutex);
	m_pThread.SetDelegate(pDelegate);
	return true;
}

bool TimeLineItems::ClearPlay()
{
	XGuard guard(m_cMutex);
	m_pThread.ClearPlay();
	return true;
}

void TimeLineItems::setSize(const QSizeF &size, const QPointF &pos)
{
	XGuard guard(m_cMutex);
	mSize = size;
	setPos(pos);
	update();
}

void TimeLineItems::setTime(const QDateTime& centralTime, const quint64& timeDelta)
{
	XGuard guard(m_cMutex);
	mCentralTime = centralTime;
	mTimeDelta = timeDelta;
	m_pThread.setTime(centralTime,timeDelta);
}


void TimeLineItems::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	XGuard guard(m_cMutex);
	quint64 startTime64 = mCentralTime.toMSecsSinceEpoch() - mTimeDelta;
	if (startTime64 < 0)
	{
		startTime64 = 0;
	}
	quint64 endTime64 = mCentralTime.toMSecsSinceEpoch() + mTimeDelta;

	u32 startTime = startTime64/1000;
	u32 endTime = endTime64/1000;

	double rate = mSize.width()/((mTimeDelta * 2)/1000);

	/* draw the record list to the ui */
	RecordItemMap::iterator it = m_RecItems.begin(); 
	for(; it!=m_RecItems.end(); ++it)
	{
		VdbRecordItem item = (*it).second;
		if (item.end < startTime  && item.start > endTime)
		{
			continue;
		}
		QColor color(255 ,48, 48, 255);
		//priority last
		if ((item.type & R_SCHED)== R_SCHED)
		{
			color = QColor(34, 139, 34, 255);
		}
		//priority middle
		if ((item.type & R_MANUAL)== R_MANUAL)
		{
			color = QColor(0 ,206, 209, 255);
		}

		//priority frist
		if ((item.type & R_ALARM) == R_ALARM)
		{
			color = QColor(255 ,48, 48, 255);
		}

		/* Draw the item */
		if (item.start < startTime 
			&& item.end > startTime)
		{
			//34, 139, 34
			painter->fillRect(0, 0, (double)(item.end - startTime) * rate + 1,
				mSize.height(), QBrush(color));
		}
		if (item.start >= startTime)
			//&& item.end <= endTime)
		{
			int startPos = (double)(item.start - startTime) * rate - 1;
			if (startPos < 0)
			{
				startPos = 0;
			}
			int myWidth = mSize.width();
			painter->fillRect(startPos, 0,
				(double)(item.end - item.start) * rate + 1,
				mSize.height(), QBrush(color));
		}
	}
	
}

QRectF TimeLineItems::boundingRect() const
{
    return QRectF(QPointF(0, 0), QPointF(mSize.width(), mSize.height()));
}

//////////////////////////////////////////////////////////////////////////////
///////////////         SphereTimeLineScaler            //////////////////////
//////////////////////////////////////////////////////////////////////////////

SphereTimeLineScaler::SphereTimeLineScaler(QObject* parent /*= 0*/) :
                                           mZoomStepTime(350),
                                           mElementalZoomTime(50),
                                           mZoomStepRelaxationCoeff(0.000625),
                                           mScheduledScaling(0),
                                           QObject(parent)
{
    mZoomingTimeLine = new QTimeLine(mZoomStepTime, this);
    mZoomingTimeLine->setUpdateInterval(mElementalZoomTime);

    connect(mZoomingTimeLine, SIGNAL(valueChanged(qreal)), this, SLOT(onUpdateScale(qreal)));
}

void SphereTimeLineScaler::startScaling(const int& delta)
{
    int stepNum = delta / 120; // One wheel step = 120 units (or 15 degrees)
    mScheduledScaling += stepNum;

    if (mScheduledScaling * stepNum < 0){ // // If zooming direction has changed, drop all planned zooming iterations
        mScheduledScaling = stepNum;
    }

    if (mZoomingTimeLine->state() == QTimeLine::NotRunning){
        mZoomingTimeLine->start();
    }
}

void SphereTimeLineScaler::stopScaling()
{
    mZoomingTimeLine->stop();
}

void SphereTimeLineScaler::onUpdateScale(qreal)
{
    // The more zomoming iterations are scheduled, the faster the zooming is
    qreal factor = 1.0 + qreal(mScheduledScaling)*mZoomStepRelaxationCoeff;
    emit scale(factor);
}

void SphereTimeLineScaler::scalingFinished()
{
    mScheduledScaling = mScheduledScaling > 0?
                        mScheduledScaling - 1 : mScheduledScaling + 1;
}

void SphereTimeLineScaler::setZoomStepTime(const quint64& zoomStepTime)
{
    if (zoomStepTime > 0)
    {
        mZoomStepTime = zoomStepTime;
        mZoomingTimeLine->setDuration(mZoomStepTime);
    }
}

void SphereTimeLineScaler::setElementalZoomTime(const quint64& elementalZoomTime)
{
    mElementalZoomTime = elementalZoomTime;
    mZoomingTimeLine->setUpdateInterval(50);
}

quint64 SphereTimeLineScaler::getZoomStepTime() const
{
    return mZoomingTimeLine->duration();
}

quint64 SphereTimeLineScaler::getElementalZoomTime() const
{
    return mZoomingTimeLine->updateInterval();
}

quint64 SphereTimeLineScaler::getDefaultScale() const
{
    return mDefaultScale;
}

//////////////////////////////////////////////////////////////////////////////
///////////////          SphereTimeLineScroller         //////////////////////
//////////////////////////////////////////////////////////////////////////////

SphereTimeLineScroller::SphereTimeLineScroller(QObject* parent /*= 0*/) :
                        mDragIsOngoing(false),
                        mMouseDragDistance(0),
                        mInitialVelocity(0),
                        mMsecPerPixel(0),
                        mFrictionCoeff(0.66),
                        QObject(parent)
{
    mScrollingTimeLine = new QTimeLine(mTotalElementalScrollDuration, this);
    mScrollingTimeLine->setUpdateInterval(mElementalScrollTime);

    connect(mScrollingTimeLine, SIGNAL(valueChanged(qreal)), this, SLOT(onUpdateScroll(qreal)));
}

void SphereTimeLineScroller::startScrolling(const QDateTime startTime, const double msecPerPixel)
{
    // If there was a move at all
    if (mMouseDragDistance)
    {
        // Start speed in px/msec, where 1 px = 1 millimeter
        mInitialVelocity = (double)mMouseDragDistance / (QDateTime::currentDateTime().toMSecsSinceEpoch() -
                                                         mLastMouseTrack.toMSecsSinceEpoch());

        // The time until the scrolling stops
        double scrollTime = std::abs(mInitialVelocity / (mFrictionCoeff*mFreeFallAcceleration)) * 1000;

        if (scrollTime > 0)
        {
            mMsecPerPixel = msecPerPixel;
            mScrollStartTime = startTime;

            mScrollingTimeLine->setDuration(scrollTime);
            mScrollingTimeLine->start();
        }
    }
}

void SphereTimeLineScroller::stopScrolling()
{
    mInitialVelocity = 0;
    mMouseDragDistance = 0;

    if (mScrollingTimeLine->state() == QTimeLine::Running){
        mScrollingTimeLine->stop();
    }
}

void SphereTimeLineScroller::onScrollFinished()
{
    mInitialVelocity = 0;
    mMouseDragDistance = 0;
}

void SphereTimeLineScroller::onUpdateScroll(qreal)
{
    if (mScrollingTimeLine->state() == QTimeLine::NotRunning){
        return;
    }

    int elapsedTime = mScrollingTimeLine->currentTime();
    double acceleration = mFrictionCoeff*mFreeFallAcceleration / 1000; //  in m/(sec^2)

    if (mInitialVelocity > 0){ //direction
        acceleration = -acceleration;
    }

    double newPos = mInitialVelocity*elapsedTime +
                    acceleration*elapsedTime*elapsedTime / 2; // Distance from the start: v0*t - (a*t^2)/2

    QDateTime newCentralTime = mScrollStartTime.addMSecs(newPos*mMsecPerPixel); // New central time

    emit scroll(newCentralTime);
}

void SphereTimeLineScroller::addScrollingDelta(const int& delta)
{
    mMouseDragDistance += delta;

    if (mMouseDragDistance * delta < 0)
    {
        mMouseDragDistance = delta;
        mLastMouseTrack = QDateTime::currentDateTime();
    }
}

void SphereTimeLineScroller::setLastMouseTrackTime(const QDateTime& time)
{
    mLastMouseTrack = time;
}

void SphereTimeLineScroller::setFrictionCoefficient(const double& coeff)
{
    mFrictionCoeff = coeff;
}

void SphereTimeLineScroller::setDragIsOngoing(const bool& isOngoing)
{
    mDragIsOngoing = isOngoing;
}


bool SphereTimeLineScroller::scalingIsOngoing() const
{
    return mScrollingTimeLine->state() == QTimeLine::Running;
}

double SphereTimeLineScroller::getFrictionCoefficient() const
{
    return mFrictionCoeff;
}

bool SphereTimeLineScroller::dragIsOngoing() const
{
    return mDragIsOngoing;
}


//////////////////////////////////////////////////////////////////////////////
///////////////             VVIDTimeLineWidget              //////////////////////
//////////////////////////////////////////////////////////////////////////////

VVIDTimeLineWidget::VVIDTimeLineWidget(QWidget* parent) : QGraphicsView(parent)
{
    setScene(new QGraphicsScene(this));
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);

    // Scaling and scrolling
    mScaler = new SphereTimeLineScaler(this);
    mScroller = new SphereTimeLineScroller(this);

    // Interface
    mGrid = new TimeLineGrid();
    mGrid->setTimeRange(QDateTime::currentDateTime().addSecs(-60 *10), mScaler->getDefaultScale());
    mGrid->setZValue(1);

    // Items
    mItems = new TimeLineItems();
    mItems->setZValue(0);
    

    scene()->addItem(mGrid);
    scene()->addItem(mItems);

    // Connections

    connect(mScroller, SIGNAL(scroll(QDateTime)), this, SLOT(setCentralTime(QDateTime)));
    connect(mScaler, SIGNAL(scale(qreal)), this, SLOT(setScale(qreal)));
    connect(mGrid, SIGNAL(rangeChanged(QDateTime, QDateTime)), this, SIGNAL(rangeChanged(QDateTime, QDateTime)));

    viewport()->setCursor(Qt::OpenHandCursor);
}

VVIDTimeLineWidget::~VVIDTimeLineWidget()
{
}

bool VVIDTimeLineWidget::SetDelegate(VVidPBDelegate *pDelegate)
{	
	mItems->SetDelegate(pDelegate);
	return true;
}

bool VVIDTimeLineWidget::ClearPlay()
{	
	mItems->ClearPlay();
	return true;
}

u32 VVIDTimeLineWidget::GetCurrentTime()
{
	return mGrid->getTimeMark().toMSecsSinceEpoch()/1000;
}


void VVIDTimeLineWidget::mousePressEvent(QMouseEvent *event)
{
    // Used to select an element
    QPoint pos = event->pos();

    // If there is only one element, and it's events
    if (rect().contains(event->pos()) &&
             event->button() == Qt::LeftButton) // otherwise scroll the timeline
    {
		mScroller->stopScrolling();
		mScroller->setDragIsOngoing(true);
		mScroller->setLastMouseTrackTime(QDateTime::currentDateTime());
		viewport()->setCursor(Qt::ClosedHandCursor);
    }

    QGraphicsView::mousePressEvent(event);
}

void VVIDTimeLineWidget::mouseMoveEvent(QMouseEvent *event)
{
    // If the cursor is out of widget's borders
    if (!mGrid->graphicsRect().contains(event->pos()))
    {
        mScroller->setDragIsOngoing(false);
    }

    QPoint pos = event->pos();

    int prevMousePos = mGrid->getMousePos().x();
    mGrid->setMousePos(event->pos(), mScroller->dragIsOngoing());

    if (mScroller->dragIsOngoing())
    {
		int delta = prevMousePos - event->pos().x();
		mScroller->addScrollingDelta(delta); // Add new range to the scrolling path
		mItems->setTime(mGrid->getTimeMark(), mGrid->getTimeDelta());
		emit SignalSeekTime(mGrid->getTimeMark().toMSecsSinceEpoch()/1000);
    }
    else // paint pop up info about the task
    {
        QPoint pos = event->pos();
	 viewport()->setCursor(Qt::OpenHandCursor);
    }

    QGraphicsView::mouseMoveEvent(event);
}

void VVIDTimeLineWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        QPoint pos = event->pos();
        {
            viewport()->setCursor(Qt::OpenHandCursor);
        }

        if (mScroller->dragIsOngoing()) // start scrolling
        {
            mScroller->setDragIsOngoing(false);
            double msecPerPx = (mGrid->getTimeDelta() * 2) / mGrid->graphicsRect().width();
            //mScroller->startScrolling(mGrid->getTimeMark(), msecPerPx);
        }
    }

    QGraphicsView::mouseReleaseEvent(event);
}


void VVIDTimeLineWidget::SlotZoomIn()
{
	int delta = 240;
	mScaler->startScaling(delta * 3);
}
void VVIDTimeLineWidget::SlotZoomOut()
{
	int delta = -240;
	mScaler->startScaling(delta * 3);
}

void VVIDTimeLineWidget::wheelEvent(QWheelEvent *event)
{
    mScaler->startScaling(event->delta() *3);

    QGraphicsView::wheelEvent(event);
}

void VVIDTimeLineWidget::setScale(qreal factor)
{
    int newDelta = mGrid->getTimeDelta() / factor;

    if (mGrid->setTimeRange(mGrid->getTimeMark(), newDelta)){
        mItems->setTime(mGrid->getTimeMark(), mGrid->getTimeDelta());
    }
    else{
        mScaler->stopScaling();
    }
}

void VVIDTimeLineWidget::SlotUpdateTime(unsigned int nTime)
{
	QDateTime time;
	quint64 time64 =  (quint64)nTime * 1000;
	time.setMSecsSinceEpoch(time64);
	setCentralTime(time);
}

void VVIDTimeLineWidget::setCentralTime(QDateTime time)
{
    if (mGrid->setTimeRange(time, mGrid->getTimeDelta())){
        mItems->setTime(mGrid->getTimeMark(), mGrid->getTimeDelta());
    }
}

void VVIDTimeLineWidget::resizeEvent(QResizeEvent *event)
{
    rearrangeWidgets(event->size());
    QGraphicsView::resizeEvent(event);
}

void VVIDTimeLineWidget::rearrangeWidgets(QSize size)
{
    mGrid->setSize(size, QPointF(0, 0));

    QRect graphicsRect = mGrid->graphicsRect();
    mItems->setSize(graphicsRect.size(), graphicsRect.topLeft());
    mItems->setTime(mGrid->getTimeMark(), mGrid->getTimeDelta());

    scene()->setSceneRect(mGrid->boundingRect());
}



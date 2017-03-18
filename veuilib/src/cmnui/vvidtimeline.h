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
#ifndef __VVID_TIMELINE_H__
#define __VVID_TIMELINE_H__

#include <QMap>
#include <QHash>
#include <QRect>
#include <QPair>
#include <QPoint>
#include <QMutex>
#include <QDebug>
#include <QTimer>
#include <QLabel>
#include <QWidget>
#include <QAction>
#include <QPointF>
#include <QString>
#include <QPainter>
#include <QTimeLine>
#include <QDateTime>
#include <QTabWidget>
#include <QWheelEvent>
#include <QPushButton>
#include <QPainterPath>
#include <QGraphicsItem>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsProxyWidget>
#include <QThread>

#define NOMINMAX
#define NOMINMAX 

#include <memory>
#include "XSDK/XMutex.h"
#include "XSDK/XGuard.h"
#include "utility.hpp"
#include "utility/videotype.hpp"
#include "XSDK/TimeUtils.h"
#include "XSDK/XBlockingQueue.h"

using namespace XSDK;
using namespace UtilityLib;

typedef enum __VVidTimeLineCmdType
{
	VVID_TIMELINE_CMD_TIME = 1,
	VVID_TIMELINE_CMD_CLEAR,
	VVID_TIMELINE_CMD_QUIT,
	VVID_TIMELINE_CMD_LAST
}VVidTimeLineCmdType;

typedef struct __VVidTimeLineCmd
{
	VVidTimeLineCmdType type;
	quint64 timeCenter;
	quint64 timeDelta;
}VVidTimeLineCmd;


inline uint qHash(const QRect& rect, uint seed = 0)
{
    return qHash(rect.x()) ^ qHash(rect.width()) ^ qHash(seed);
}

inline uint qHash(const QPair<QDateTime, QDateTime>& data, uint seed = 0)
{
    return qHash(data.first.toMSecsSinceEpoch()) ^ qHash(data.second.toMSecsSinceEpoch()) ^ qHash(seed);
}

class VVidPBDelegate;

/**
* Class responsible for painting timeline's interface
*/

class TimeLineGrid : public QObject, public QGraphicsItem
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)

public:
    struct TimeLineGridStyle
    {
        QColor currMarkColor;                       // Color of the current time mark and it's text. Default - Qt::red
        QColor mouseMarkColor;                      // Color of the mouse mark aand it's text. Default - Qt::blue
        QColor timeMarksTextColor;                  // Text marks color. Default - 115,115,115 (light grey)
        QColor borderColor;                         // Frame color. Default - Qt::black

	TimeLineGridStyle(const QColor& currentTimeMarkColor = QColor(255, 0, 0),
                          const QColor& mouseTimeMarkColor = QColor(0, 0, 127),
                          const QColor& timeMarksColor = QColor(80, 80, 80),
                          const QColor& gridBorderColor = QColor(200, 200, 200, 150)) :
                          currMarkColor(currentTimeMarkColor), mouseMarkColor(mouseTimeMarkColor),
                          timeMarksTextColor(timeMarksColor), borderColor(gridBorderColor){}
    };

    struct TimeLineGridSettings
    {
        quint32 borderIndentY;                      // Item painting region's vertical indent (from the borders of the widget, px)
        quint32 borderIndentX;			            // Item painting region's horizontal indent (from the borders of the widget, px)
        quint64 maximumScale;                       // Max zoom time interval - MINUTE
        quint64 minimumScale;                       // Min zoom time interval - WEEK

        TimeLineGridSettings(const quint32 borderIndentHorisontal = 0,
                             const quint32 borderIndentVertical = 26,
                             const quint64 maximumTimeScale = minute/15,
                             const quint64 minimumTimeScale = day) :
                             borderIndentX(borderIndentHorisontal),
                             borderIndentY(borderIndentVertical),
                             maximumScale(maximumTimeScale),
                             minimumScale(minimumTimeScale) {}
    };

private:
   void drawMarks(QPainter* painter);
   void drawCurrTimeMark(const double& msecPerPixel, const quint64& currTime, const quint64& startTime,
                         const quint64& endTime, QPair<int, int>& currTimeMarkBorders,
                         const QString textFormat, const QFontMetrics& fm, QPainter* painter);

   void drawMouseTimeMark(QPainter* painter);
   void drawGridMarks(const QFontMetrics& fm, const double& msecPerPixel,
                      quint64& startTime, const QString textFormat,
                      QPair<int, int> &currTimeMarkBorders, QPainter *painter);

public:
    TimeLineGrid(QGraphicsItem* parent = 0);

    //setters
    void setStyle(const TimeLineGridStyle& style);
    void setSettings(const TimeLineGridSettings& settings);

    void setSize(const QSizeF& size, const QPointF& pos);
    bool setTimeRange(const QDateTime& centralTime, const quint64& timeDelta);
    void setMousePos(const QPoint& pos, bool isDragging = false);

    //getters
    QDateTime getTimeMark() const;
    quint64 getTimeDelta() const;
    QPoint getMousePos() const;
    TimeLineGridSettings getSettings() const;
    TimeLineGridStyle getStyle() const;

    quint64 calculateStep(const int& maxNumberOfTextMarks);
    void paintText(bool topBottom, int xPos, QString text, QPainter* painter, QColor color);
    QRectF boundingRect() const;
    QRect graphicsRect() const;                        /**< Timeline item painting region rect */

protected:
    void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0);

signals:
    void rangeChanged(QDateTime startTime, QDateTime endTime);
	
private:
    static const int second = 1000;
    static const int minute = second * 60;
    static const int hour = minute * 60;
    static const int day = hour * 24;
    static const int week = day * 7;

    static const int mMaxNumberOfUnits = 30;

    static const QString mTimeFormat;
    static const QString mDayFormat;
    static const QString mDayTimeFormat;
    static const double mOverlayOpacity;

private:
    QDateTime mTimeCenterMark;
    quint64 mTimeDelta;                               // Current scale - msec from the central mark to both borders
    QPoint mMousePos;
    QSizeF mSize;                                     // Current grid scale

    TimeLineGridStyle mStyle;
    TimeLineGridSettings mSettings;
};

/**
* Paints timeline items
*/

class VVidTimeLineWidget;

class TimeLineThread : public QThread
{
	Q_OBJECT
public:
	TimeLineThread();
public:
	bool SetDelegate(VVidPBDelegate *pDelegate);
	bool ClearPlay();

	void setTime(const QDateTime& centralTime, const quint64& timeDelta);
	bool GetRecItems(RecordItemMap &pRecItems);

public:
	void run();
	void Quit();
	void UpdateSearch();
	
signals:
	void RecItemUpdated();

private:
	QDateTime mCentralTime;
	quint64 mTimeDelta;                                       // Current scale - number of msec form the center to any border*/

	VVidPBDelegate *m_pDelegate;
	XMutex m_cMutex;
	RecordItemMap m_RecItems;
	XBlockingQueue<VVidTimeLineCmd> m_Queue;
	bool m_Quit;
	quint64 m_SearchStart;
	quint64 m_SearchEnd;
	bool m_bCleared;
};

class TimeLineItems : public QObject, public QGraphicsItem	
{   
	Q_OBJECT
public:
    TimeLineItems(QGraphicsItem * parent = 0);
     ~TimeLineItems();
     bool SetDelegate(VVidPBDelegate *pDelegate);
     bool ClearPlay();

    void setTime(const QDateTime& centralTime, const quint64& timeDelta);
    void setSize(const QSizeF& size, const QPointF& pos);

    //graphic  
    void paint(QPainter* painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0);
    QRectF boundingRect() const;
public slots:
	void SlotUpdateSearch();

private:
	QDateTime mCentralTime;
	quint64 mTimeDelta;                                       // Current scale - number of msec form the center to any border*/
	QSizeF mSize;                                             // Current area size */
	RecordItemMap m_RecItems;
	XMutex m_cMutex;
	TimeLineThread m_pThread;
};

class SphereTimeLineScaler : public QObject
{
    Q_OBJECT
public:
    SphereTimeLineScaler(QObject* parent = 0);

    void setZoomStepTime(const quint64& zoomStepTime);
    void setElementalZoomTime(const quint64& elementalZoomTime);

    quint64 getDefaultScale() const;
    quint64 getZoomStepTime() const;
    quint64 getElementalZoomTime() const;

private slots:
    void onUpdateScale(qreal);                          // Elementary scale update when zooming
    void scalingFinished();

public slots:
    void startScaling(const int& delta);                // delta = number of wheel steps
    void stopScaling();

signals:
    void scale(qreal factor);
private:
    QTimeLine* mZoomingTimeLine;                       // Updates the scale when zooming
    int mScheduledScaling;                             // Planned elementary zooming actions
    static const int mDefaultScale = 60000 * 60 * 1;           // Default scale - 1 MINUTES * 60 * 1 1 hour

    double mZoomStepRelaxationCoeff;                   // Elementary scaling coefficient
    quint16 mZoomStepTime;                             // Scaling time
    quint16 mElementalZoomTime;                        // Elementary scaling time

};

/**
* Calculates time line scrolling
*/

class SphereTimeLineScroller : public QObject
{
    Q_OBJECT
public:
    SphereTimeLineScroller(QObject* parent = 0);

    //setters
    void setFrictionCoefficient(const double& coeff);
    void addScrollingDelta(const int& delta);
    void setLastMouseTrackTime(const QDateTime& time);
    void setDragIsOngoing(const bool& isOngoing);

    //getters
    double getFrictionCoefficient() const;
    bool dragIsOngoing() const;
    bool scalingIsOngoing() const;

    private slots:
    void onUpdateScroll(qreal);
    void onScrollFinished();

    public slots:
    void startScrolling(const QDateTime startTime, const double msecPerPixel);
    void stopScrolling();

signals:
    void scroll(QDateTime newCentralTime);
private:
    QTimeLine* mScrollingTimeLine;                        // Updates the current position when scrolling
    bool mDragIsOngoing;

    int mMouseDragDistance;                               // Mouse move distance with left button being pressed
    QDateTime mLastMouseTrack;                            // Moving start time
    double mInitialVelocity;                              // Scrolling start speed
    double mFrictionCoeff;
    double mMsecPerPixel;                                 // Scale - msec/px
    QDateTime mScrollStartTime;                           // Scrolling start pos

    static const int mFreeFallAcceleration = 10;
    static const int mElementalScrollTime = 25;           // Elementary scrolling period - 25 msec
    //static const int mTotalElementalScrollDuration = 350; // msec
    static const int mTotalElementalScrollDuration = 350; // msec
};

/**
* The timeline class itself
*/

class VVIDTimeLineWidget : public QGraphicsView
{
    Q_OBJECT
public:
    explicit VVIDTimeLineWidget(QWidget* parent = 0);
	~VVIDTimeLineWidget();
public:
	bool SetDelegate(VVidPBDelegate *pDelegate);
	bool ClearPlay();
	u32 GetCurrentTime();

public slots:
	void setScale(qreal factor);                          // Change timeline scale. Calculated as follows: current time delta / factor
	void setCentralTime(QDateTime time);                  // Central time mark change
	void SlotZoomIn();
	void SlotZoomOut();
	void SlotUpdateTime(unsigned int nTime);
	
signals:
	void SignalSeekTime(unsigned int nTime);

protected:
    void mouseMoveEvent(QMouseEvent* event);
    void mousePressEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    void wheelEvent(QWheelEvent * event);
    void resizeEvent(QResizeEvent * event);

signals:
    void eventClicked(quint64 taskId, QDateTime startTime);  // Item selection signal
    void rangeChanged(QDateTime startTime, QDateTime endTime);
private:
    //graphics
    TimeLineGrid* mGrid;
    TimeLineItems* mItems;
    SphereTimeLineScaler* mScaler;
    SphereTimeLineScroller* mScroller;
    
private:
    void rearrangeWidgets(QSize size);
};

#endif // TIMELINE_H

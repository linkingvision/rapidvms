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
#include "utility.hpp"
#include "debug.hpp"
#include "videotype.hpp"
#include <QWidget>
#include "server/factory.hpp"
#include <QThread>
#include <QProxyStyle>
#include "vstyle.hpp"

using namespace UtilityLib;

VStyle::VStyle(QStyle *style)
:QProxyStyle(style)
{
	
}

void VStyle::drawPrimitive(QStyle::PrimitiveElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const
{
	if (element == PE_IndicatorTabClose)
	{
		int size = proxy()->pixelMetric(QStyle::PM_SmallIconSize);
		QIcon::Mode mode = option->state & State_Enabled ?
			(option->state & State_Raised ? QIcon::Active : QIcon::Normal)
			: QIcon::Disabled;
		if (!(option->state & State_Raised)
			&& !(option->state & State_Sunken)
			&& !(option->state & QStyle::State_Selected))
			mode = QIcon::Disabled;

		QIcon::State state = option->state & State_Sunken ? QIcon::On : QIcon::Off;
		QPixmap pixmap = QIcon(":/action/resources/libuiclosesmall.png").pixmap(size, mode, state);
		proxy()->drawItemPixmap(painter, option->rect, Qt::AlignCenter, pixmap);
	}else if (element == PE_IndicatorArrowLeft)
	{
		int size = proxy()->pixelMetric(QStyle::PM_SmallIconSize);
		QIcon::Mode mode = option->state & State_Enabled ?
			(option->state & State_Raised ? QIcon::Active : QIcon::Normal)
			: QIcon::Disabled;
		if (!(option->state & State_Raised)
			&& !(option->state & State_Sunken)
			&& !(option->state & QStyle::State_Selected))
			mode = QIcon::Disabled;

		QIcon::State state = option->state & State_Sunken ? QIcon::On : QIcon::Off;
		QPixmap pixmap = QIcon(":/action/resources/libuiclosesmall.png").pixmap(size, mode, state);
		proxy()->drawItemPixmap(painter, option->rect, Qt::AlignCenter, pixmap);
	}
	else
	{
		QProxyStyle::drawPrimitive(element, option, painter, widget);
	}
}

int VStyle::pixelMetric(PixelMetric metric, const QStyleOption* option,
const QWidget* widget) const
{
#if 0
	if (metric == PM_DockWidgetHandleExtent || metric == PM_DockWidgetSeparatorExtent)
	{
		//qDebug()<<"PM_DockWidgetHandleExtent";
		return 110;
	}
	else
#endif
		return QProxyStyle::pixelMetric(metric, option, widget);
}
/* 	http://doc.qt.io/qt-5/qstyle.html
	QStyle::SP_DockWidgetCloseButton
	QStyle::SP_ArrowLeft
	QStyle::SP_ArrowRight
*/

QIcon VStyle::standardIcon(StandardPixmap standardIcon, const QStyleOption* option,
const QWidget* widget) const
{
	if (standardIcon == SP_ArrowLeft)
	{
		return QIcon(QPixmap(":/action/resources/libuileft.png"));
	} else if (standardIcon == SP_ArrowRight)
	{
		return QIcon(QPixmap(":/action/resources/libuiright.png"));
	}
	else
	{
		return QProxyStyle::standardIcon(standardIcon, option, widget);
	}
}


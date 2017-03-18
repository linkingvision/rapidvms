//------------------------------------------------------------------------------
// File: vstyle.hpp
//
// Desc: 
//
// Copyright (c) 2014-2018 INTINT. All rights reserved.
//------------------------------------------------------------------------------

#ifndef __V_STYLE_HPP__
#define __V_STYLE_HPP__
#include "utility.hpp"
#include "debug.hpp"
#include "videotype.hpp"
#include <QWidget>
#include "server/factory.hpp"
#include <QThread>
#include <QProxyStyle>
#include <QStyleOption>

using namespace UtilityLib;

class VE_LIBRARY_API VStyle : public QProxyStyle
{
public:
	VStyle(QStyle *style = 0);

	int pixelMetric(PixelMetric metric, const QStyleOption* option = 0,
	const QWidget* widget = 0) const;
	void drawPrimitive(PrimitiveElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget = 0) const;


public slots:
	QIcon standardIcon(StandardPixmap standardIcon, const QStyleOption* option = 0,
		const QWidget* widget = 0) const;
};

#endif /* __V_V_IPC_MGR_HPP__ */

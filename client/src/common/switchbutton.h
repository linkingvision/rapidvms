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
#ifndef SWITCHBUTTON_H
#define SWITCHBUTTON_H

#include <QPushButton>

/**
 * This is a button which can be switched on and off.
 * It looks similar as the on/off switches of Android.
 * <p>
 * The method isChecked() return the on/off state.
 */

class SwitchButton : public QPushButton {
    Q_OBJECT

public:
    /** Constructor */
    explicit SwitchButton(QWidget* parent = 0);

    /** Set the color for on and off status. */
    void setColors(const QColor on=Qt::darkGreen, const QColor off=Qt::gray);

    /** Set the labels for on and off status. */
    void setLabels(const QString on=QString(tr("ON")), const QString off=QString(tr("OFF")));

protected:

    void paintEvent(QPaintEvent* paint);

    /** Color for on state */
    QColor onColor;

    /** Color for off state */
    QColor offColor;

    /** Label for on state */
    QString onLabel;

    /** Label for off state */
    QString offLabel;

    /** Color of the label in on state */
    QColor onLabelColor;

    /** Color of the label in off state */
    QColor offLabelColor;

};

#endif // SWITCHBUTTON_H


























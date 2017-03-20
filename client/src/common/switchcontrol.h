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
#ifndef SWITCH_CONTROL
#define SWITCH_CONTROL

#include <QWidget>
#include <QTimer>

class SwitchControl : public QWidget
{
    Q_OBJECT

public:
    explicit SwitchControl(QWidget *parent = 0);

    // 返回开关状态 - 打开：true 关闭：false
    bool isToggled() const;

    // 设置开关状态
    void setToggle(bool checked);

    // 设置背景颜色
    void setBackgroundColor(QColor color);

    // 设置选中颜色
    void setCheckedColor(QColor color);

    // 设置不可用颜色
    void setDisbaledColor(QColor color);

protected:
    // 绘制开关
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

    // 鼠标按下事件
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

    // 鼠标释放事件 - 切换开关状态、发射toggled()信号
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

    // 大小改变事件
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;

    // 缺省大小
    QSize sizeHint() const Q_DECL_OVERRIDE;
    QSize minimumSizeHint() const Q_DECL_OVERRIDE;

signals:
    // 状态改变时，发射信号
    void toggled(bool checked);

private slots:
    // 状态切换时，用于产生滑动效果
    void onTimeout();

private:
    bool m_bChecked;         // 是否选中
    QColor m_background;     // 背景颜色
    QColor m_checkedColor;   // 选中颜色
    QColor m_disabledColor;  // 不可用颜色
    QColor m_thumbColor;     // 拇指颜色
    qreal m_radius;          // 圆角
    qreal m_nX;              // x点坐标
    qreal m_nY;              // y点坐标
    qint16 m_nHeight;        // 高度
    qint16 m_nMargin;        // 外边距
    QTimer m_timer;          // 定时器
};

#endif // SWITCH_CONTROL
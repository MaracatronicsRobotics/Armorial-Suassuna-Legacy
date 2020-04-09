/***
 * Maracatronics Robotics
 * Federal University of Pernambuco (UFPE) at Recife
 * http://www.maracatronics.com/
 *
 * This file is part of Armorial project.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ***/

#include "qsfmlwidget.h"

QSFMLCanvas::QSFMLCanvas(QWidget *parent, unsigned int frameTime) :
QWidget(parent), mInitialized(false)
{
    setAttribute(Qt::WA_PaintOnScreen);
    setAttribute(Qt::WA_OpaquePaintEvent);
    setAttribute(Qt::WA_NoSystemBackground);

    setFocusPolicy(Qt::StrongFocus);

    mTimer.setInterval(frameTime);
}

#ifdef Q_WS_X11
#include <qt qx11info_x11.h="">
#include <x11 xlib.h="">
#endif

void QSFMLCanvas::showEvent(QShowEvent* e)
{
    if(!mInitialized)
    {
       #ifdef Q_WS_X11
       XFlush(QX11Info::display());
       #endif

       sf::RenderWindow::create(winId());
       onInit();

       connect(&mTimer, SIGNAL(timeout()), this, SLOT(repaint()));
       mTimer.start();
       mInitialized = true;
    }
}

QPaintEngine* QSFMLCanvas::paintEngine() const
{
    return NULL;
}

void QSFMLCanvas::paintEvent(QPaintEvent* e)
{
    sf::RenderWindow::clear();
    onUpdate();
    sf::RenderWindow::display();
}

QSFMLCanvas::~QSFMLCanvas()
{

}

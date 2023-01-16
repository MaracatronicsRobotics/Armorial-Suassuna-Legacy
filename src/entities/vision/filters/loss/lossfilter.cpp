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


#include "lossfilter.h"

float LossFilter::_filterTime = 300;
QReadWriteLock LossFilter::_filterMutex = QReadWriteLock();

LossFilter::LossFilter() {
    _isInitialized = false;
    _firstIt = false;
}

void LossFilter::startLoss() {
    _isInitialized = true;
    _timer.start();
}

bool LossFilter::isInitialized() {
    return _isInitialized;
}

bool LossFilter::checkLoss() {
    if(_firstIt) {
        _firstIt = false;
        return true;
    }

    if(_timer.getMilliseconds() >= getLossTime()) {
        return true;
    }
    else {
        return false;
    }
}

float LossFilter::getLossTime() {
    _filterMutex.lockForRead();
    float filterTime = _filterTime;
    _filterMutex.unlock();

    return filterTime;
}

void LossFilter::setLossTime(float lossTime) {
    _filterMutex.lockForWrite();
    _filterTime = lossTime;
    _filterMutex.unlock();
}

void LossFilter::setFirstIt() {
    _firstIt = true;
}

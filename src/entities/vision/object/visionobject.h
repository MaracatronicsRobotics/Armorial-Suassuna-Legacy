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


#ifndef VISIONOBJECT_H
#define VISIONOBJECT_H

#include <Armorial/Common/Enums/Color/Color.h>
#include <Armorial/Common/Types/Object/Object.h>

#include <include/proto/messages.pb.h>

#include <src/entities/vision/filters/filters.h>

class VisionObject : public Common::Types::Object
{
public:
    /*!
     * \brief VisionObject class constructor.
     */
    VisionObject();

    /*!
     * \return This object vision confidence.
     */
    float getConfidence();

    /*!
     * \return A boolean that tells if this object is valid or not.
     */
    bool isObjectValid();

    /*!
     * \brief Update this vision object with given parameters.
     * \param confidence, pos, orientation The given parameters.
     */
    void updateObject(const float& confidence, const Geometry::Vector2D& pos, const Geometry::Angle& orientation);

    /*!
     * \brief Update this vision object as a loss object in the given frame.
     */
    void updateObject();

    /*!
     * \brief Convert this VisionObject instance into an Armorial::Robot instance.
     */
    Armorial::Robot convertToRobotDto(const Common::Enums::Color& teamColor, const quint8& robotId);

    /*!
     * \brief Convert this VisionObject instance into an Armorial::Ball instance.
     */
    Armorial::Ball convertToBallDto();

protected:
    /*!
     * \return If the object is safe (passed through the noise filter).
     */
    bool isObjectSafe();

    /*!
     * \return If the object is lost (timeout in the loss filter).
     */
    bool isObjectLoss();

    /*!
     * \brief Mark this vision object as invalid.
     */
    void setInvalid();

    /*!
     * \brief Mark this vision object as valid.
     */
    void setValid();

private:
    // Object params
    float _confidence;
    bool _valid;

    // Timer for angularSpeed
    Utils::Timer _aSpeedTimer;

    // Object filters
    LossFilter _lossFilter;
    NoiseFilter _noiseFilter;
    KalmanFilter _kalmanFilter;
};

#endif // VISIONOBJECT_H

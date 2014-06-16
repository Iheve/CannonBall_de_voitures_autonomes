#pragma once
#include <vector>
#include "aruco.h"
#include"marker.h"

/**
 * Abstract which represent an IA
 * @author Thibaut Coutelou, Benjamin Mugnier, Guillaume Perrin
 */
class AI
{
    public:
        AI();
        ~AI();
        /**
         * Action realized by the AI
         * @param TheMarkers markers found
         * @param steering the new value of the steering
         * @param throttle the new value of the throttle
         * @param width capture width in pixel
         */
        virtual void getCommand(vector<aruco::Marker>* TheMarkers, int* steering, int* throttle, int width);
};


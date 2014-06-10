package fr.ensimag.cannonball;

import java.util.ArrayList;

public interface IA {
    /**
     * Action realized by the IA
     * @param TheMarkers markers found
     * @param steering the new value of the steering
     * @param throttle the new value of the throttle
     * @param width capture width in pixel
     */
    void getCommand(ArrayList<Marker> TheMarkers, int steering, int throttle, int width);
}

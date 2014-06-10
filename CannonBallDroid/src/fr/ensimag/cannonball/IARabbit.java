package fr.ensimag.cannonball;

import java.util.ArrayList;

public class IARabbit implements IA {

	int frame;
	ArrayList<Element> elements;
	
	@Override
	public void getCommand(ArrayList<Marker> TheMarkers, int steering,
			int throttle, int width) {
		
		//Update our map
		frame++;
		for (Marker it : TheMarkers) {
			elements.get(it.id).marker = it;
			elements.get(it.id).lastTimeSeen = frame;
		}

		Element rabbit = elements.get(18244);
		float x = rabbit.marker.getCenter().x;
		float d = rabbit.marker.Tvec.ptr<float>(0)[2];

		if (rabbit.lastTimeSeen == 0) {
			steering = 90;
			throttle = 91;
			System.out.println("Idle");
			return;
		}

		if (rabbit.lastTimeSeen == frame) {
			float xrel = (x - (width / 2)) / (width / 2);
			float ang = ((atan(xrel) * 180) / 3.1415) * factor + 90;
			float ang = (atan(xrel) * 180) / 3.1415;

			if (d > 2.0) {
				throttle = 86;
				steering = ang * 0.25 + 90;
			}
			else if (d > 1.0) {
				throttle = 88;
				steering = ang * 0.75 + 90;
			}
			else if (d > 0.5) {
				throttle = 89;
				steering = ang * 0.75 + 90;
			} else {
				throttle = 91;
			}
			
			System.out.println("Update");
			System.out.println("a:" + ang + " d:" + d);
			return;
		}

		if ((frame - rabbit.lastTimeSeen) < 3 && d > 1.5) {
			if (steering < 90) {
				(steering) += 1;
			}
			if (steering > 90) {
				(steering) -= 1;
			}
			throttle = 88;
			System.out.println("Keep going");
			return;
		}

		//*steering = 90;
		throttle = 91;
		System.out.println("Idle");
		
	}


}

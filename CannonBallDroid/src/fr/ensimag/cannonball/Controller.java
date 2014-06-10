package fr.ensimag.cannonball;

import java.util.ArrayList;

import org.opencv.core.Mat;
import org.opencv.highgui.Highgui;
import org.opencv.highgui.VideoCapture;

import android.content.Context;
import android.os.AsyncTask;



public class Controller extends AsyncTask<String, Void, Void> {
	//Arduino
	Serial arduino;
	private String serial_port = "\\\\.\\COM5";

	//MQTT
	//mqtt_sender *sender;
	private String mqtt_host;

	//Current state
	private int steering = 90;
	private int throttle = 91;

	//aruco
	private VideoCapture mVideoCapturer;
	private Mat mInputImage;

	private Mat mInputImageCopy;
	private ArrayList<Marker> mMarkers;
	private CameraParameters mCameraParameters;
	private MarkerDetector mMDetector;
	private Dictionary D;
	private String mDict;
	private String mCamParam;
	private float mMarkerSize;

	private RunMode run_mode;
	
	private Context mContext;
	
	public Controller(Context context) {
	    mContext = context;
	}


	/**
	 * Initialization of the arduino
	 */
	
	private void initArduino() {
	    Serial.createInstance(mContext);
		arduino = Serial.getInstance();
	}
	

	/*
	private void initMQTT() {
		std::cout << "Connecting to " << mqtt_host << std::endl;
		sender = new mqtt_sender("sender", mqtt_host, 1883);
		sender->publish_to_mqtt("presence", "Hello mqtt");
	}
	*/

	private void initAruco() {
		//Open webcam
		mVideoCapturer.open(0);
		mVideoCapturer.set(Highgui.CV_CAP_PROP_FRAME_WIDTH, 1920 / 3);
		mVideoCapturer.set(Highgui.CV_CAP_PROP_FRAME_HEIGHT, 1080 / 3);
		if (!mVideoCapturer.isOpened()) {
			System.err.println("Could not open video");
			System.exit(-1);
		}

		//Load dictionary
		if (D.fromFile(mDict) == false) {
			System.err.println("Could not open dictionary");
			System.exit(-1);
		}
		if (D.size() == 0) {
			System.err.println("Invalid dictionary");
			System.exit(-1);
		}
		//HighlyReliableMarkers.loadDictionary(D);

		//read first image to get the dimensions
		//TheVideoCapturer >> TheInputImage; //Something like TheInputImage.get(TheVideoCapture)

			//read camera parameters if passed
			//TheCameraParameters.readFromXMLFile(TheCamParam);
			//TheCameraParameters.resize(TheInputImage.size());

			//Configure other parameters
			//if (ThePyrDownLevel > 0)
			// MDetector.pyrDown(ThePyrDownLevel);

			//MDetector.setMakerDetectorFunction(aruco::HighlyReliableMarkers::detect);
			//MDetector.setThresholdParams(21, 7);
			//MDetector.setCornerRefinementMethod(aruco::MarkerDetector::LINES);
			//MDetector.setWarpSize((D[0].n() + 2) * 8);
			//MDetector.setMinMaxSize(0.005f, 0.5f);

			//namedWindow("in", 1);
	}


	/**
	 * Send steering and throttle angles to the arduino
	 */
	/*
	private void sendCommand(Serial** arduin, int steering, int throttle) {
		if ((*arduin)->IsConnected()) {
			char buff[2];
			buff[0] = (char)(steering);
			buff[1] = (char)(throttle);
			if (!(*arduin)->WriteData(buff, 2)) {
				std::cout << "Serial write fail !" << std::endl;
			}
		}
	}
	*/

	/**
	 * Send metrics to the MQTT broker
	 */
	/*
	private void sendMetrics(int steering, int throttle, double laps, double avg) {
		sender->publish_to_mqtt(TOPIC_STEER, (char*)std::to_string(steering).c_str());
		sender->publish_to_mqtt(TOPIC_THROT, (char*)std::to_string(throttle).c_str());
		sender->publish_to_mqtt(TOPIC_LAPS, (char*)std::to_string(laps).c_str());
		sender->publish_to_mqtt(TOPIC_AVG, (char*)std::to_string(avg).c_str());
		//cout << "SENT!" << endl;
	}
	*/

	private void updateView() {
		//print marker info and draw the markers in image
		mInputImage.copyTo(mInputImageCopy);
		for (int i = 0; i < mMarkers.size(); i++) {
			//TheMarkers.get(i).draw(TheInputImageCopy, Scalar(0, 0, 255), 1);
		}

		//cv::imshow("in", TheInputImageCopy);
		//cv::waitKey(10);
		
	}

	private void usage() {
		System.out.println("Usage : dictionary.yml intrinsics.yml marker_size MQTT_host serial_port run_mode");
		System.out.println("dictionary.yml : Dictinoary of markers used");
		System.out.println("intrinsics.yml : Camera parameters (calibration)");
		System.out.println("marker_size : size of the markers in meters");
		System.out.println("MQTT_host : address of the MQTT broker");
		System.out.println("serial_port : COM port of the arduino (ex : 'COM5')");
		System.out.println("run mode : either 'rabbit' (follow the marker) or 'cannon' (race)");
	}

	private void readParams(String argv[]) {
		if (argv.length < 7) {
			usage();
			//TODO pause
			System.exit(-1);
		}

		mDict = argv[1];
		mCamParam = argv[2];
		mMarkerSize = Float.parseFloat(argv[3]);
		mqtt_host = argv[4];
		serial_port = argv[5];
		String argv6 = argv[6];
		if (argv6 == "rabbit") {
			run_mode = RunMode.RABBIT;
		}
		else if (argv6 == "cannon") {
			run_mode = RunMode.CANNON;
		}
		else if (argv6 == "map") {
			run_mode = RunMode.MAP;
		}
		else {
			usage();
			//TODO pause
			System.exit(-1);
		}
	}

	private void choose_run_mode(IA ia) {
		if (run_mode == RunMode.RABBIT) {
			ia = new IARabbit();
			//sender->publish_to_mqtt(TOPIC_MODE, "Rabbit");
		}
		else if (run_mode == RunMode.CANNON) {
			//ia = new IAcannonball();
			//sender->publish_to_mqtt(TOPIC_MODE, "CannonBall");
		}
		else {
			//ia = new IAmap();
			//sender->publish_to_mqtt(TOPIC_MODE, "Map");
		}
	}

    @Override
    protected Void doInBackground(String... params) {
        readParams(params);

        //Arduino
        initArduino();

        //MQTT
        //mosqpp::lib_init();
        //initMQTT();

        IA ia = null;
        choose_run_mode(ia);

        //Aruco
        initAruco();

        int index = 0;
        //double tick = (double)getTickCount();
        double laps = 0;
        float total = 0;
        do {
            //Calculate processing time
            index++;
            /* I don't care for now */
            /*
            laps = (((double)getTickCount() - tick) / getTickFrequency() * 1000);
            total += (float)laps;
            //std::cout << "time enlapsed : " << laps << " ms" << " (avg : " << total / index << ")" << std::endl;
            tick = (double)getTickCount();
            */

            //Get a new frame
            mVideoCapturer.retrieve(mInputImage);
            mVideoCapturer.grab();

            //Detection of markers in the image passed
            //MDetector.detect(TheInputImage, TheMarkers, TheCameraParameters, TheMarkerSize);

            //Get steering and throttle from IA
            
            /*
                BE CAREFULL, NOT CHANGING THRTO AND STEER ATM!
             */
            //ia.getCommand(TheMarkers, steering, throttle, TheInputImage.size().width);

            //Send command on the serial bus
            //sendCommand(&arduin, steering, throttle);

            //Send metrics to the MQTT brocker
            //sendMetrics(steering, throttle, laps, total / index);

            //show input with augmented information
            updateView();
        } while (true);
    }
}
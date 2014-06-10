package fr.ensimag.cannonball;

import java.util.ArrayList;

public class Dictionary extends ArrayList<MarkerCode>{
	/**
	 * This class represent a marker dictionary as a vector of MarkerCodes
	 * 
	 * 
	 */
	  boolean fromFile(String filename) {
		    FileStoraqe fs(filename, cv::FileStorage::READ);
		    int nmarkers, markersize;
		    
		    // read number of markers
		    fs["nmarkers"] >> nmarkers; // cardinal of D
		    fs["markersize"] >> markersize; // n
		       
		    // read each marker info
		    for (int i=0; i<nmarkers; i++) {  
				String s;
			    fs["marker_" + toStr(i)] >> s;
				MarkerCode m(markersize);
				m.fromString(s);
			 	push_back(m);
		    }
		    fs.release();   
		    return true;
		  };
}

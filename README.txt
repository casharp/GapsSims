READ ME
6/3/18
----------------------------
Amani Garvin
amg2331@columbia.edu
amani3g@github.com
-----------------------------

Structure:

This code is all based on the C script in gaps_geom.C (which follows closely the examples in KDetSim.org). However the rest of the code is in C++ and is structured to create and track data from multiple detectors created using it.

Main:
-------
main.cc - Runs all functions, include header files here
	- There is a problem with running MIP's and plotting fields using the modules written below so there is an example of running and plotting an MIP in main right now.

Variations on "Main":
------------------------
loop.cc - This is the loop used to create the lookup table. The code will loop over a range of x (width) and y (depth) positions, to create single x-ray events. The total charge collected as a result of this event is gathered by the innermost loop over all 8 electrodes, and displayed as a histogram.

miptracks.cc - This models a MIP particle passing through the detector (entrance and exit points are not the same). Code can be used to show charge collection as a function of the angle of incidence.


Header Files:
-------------------------
gaps_detector.h - this defines the gaps class, and its member functions
	-  Gaps Class Constructor takes 4 parameters
		   - number of electrodes
		   - bias voltage
		   - space charge density [cm^-3]
		   - read out electrode
	- Member Functions:   
	  	 - set_Square_Grooves() - this creates square grooves between strip electrodes
		 - switch_readoutEl(new_readout) - this will switch the readout electrode to a new readout electrode and recalculates the fields, this is useful for reading out a MIP from multiple electrodes.
		 
	- You can change any of the public members of the detector also public members of KDetector are also public members of the GAPS detector
	- If you don't change those members they are defaulted to the dimensions of the Shimadzu detector.
pad_detector.h - this defines a simple pad detector made of silicon (basically a capacitor)
	       - Pad Detector Constructor takes 4 parameters
	       	     - dimensions x & y
		     - bias voltage
		     - space charge density [cm^-3]

run_mips.h - this contains functions related to running MIPS, MIPS can simulate X-ray events in the detector by setting the entry and exit point equal to each other
	   - run_MIP - runs a MIP in your detector, and plots the track the MIP took as well as the induced charge on your read out electrode over time
	   - get_time_readout() - takes MIP root file as a parameter and calculates the readout time for the total charge induced on the readout electrode
	   - get_total charge() - takes MIP root file as a parameter and calculates the total charge induced on the read out electrode

plot_fields.h - this contains functions related to plotting static fields in the detector
	      - plot_all_fields() - plots the electric field, potential, weighting potential, and geometry of the detector


-------------------------------------
Notes on Using KDetSim:
--------------------------------------
- Using KDetSim1.1: kdetsim.org for more information
- all length units are in nanometers (dimX, dimY, etc)
- all time units are in seconds

- Materials & Permittivity (values set in EG histogram)
  - 0 = Silicon, 11.7
  - 1 = PolySilicon, 11.7
  - 2 = Silicon Oxide, 3.9
  - 10 = Diamond, 5.7
  - 20 = Air, 1
  - 100 = Aluminum, 1
  
- Electrode (values set in DM histogram to calculate the weighting potential)
  - 2 = bias on electrode
  - 16385 = read out electrode
  - 1 = not a read out electrode
  - 0 = not an electrode

-Space Charge (values sets in NeffH histogram) are in unites of 10^12 [cm^-3]
  
---------------------------------------
Things I'm Working On Right Now
_______________________________________
1. Fixing Plotting in run_mip.h and plot_fields.h
2. Making Functions in run_mip.h
3. Uploading all of this to GitHub! (Done!)

#ifndef __GAPS_DETECTOR_H_INCLUDED__
#define __GAPS_DETECTOR_H_INCLUDED__

#include <iostream>


class Gaps_Detector: public KDetector
{
  
  public:

  
  int numEl;  //number of electrodes
  Float_t v_B;    //voltage applied to bottom place
  Float_t neff;   //space charge density in units 10^12 [cm^-3]
  
  int readout; //read out electrode

  Float_t dimX, dimY;
  
  Float_t depthS, widthS; //shallow groove depth and width
  Float_t depthD, widthD; //deep groove depth and width

  Float_t widthGR, widthE; //width of guard ring and edge of detector

  Float_t widthGroove, depthGroove; //width and depth of grooves
  Float_t widthEl, depthEl; //width and depth of electrodes

  int nx, ny, nz, SStep; // grid size
  string detector_name;
  
  Gaps_Detector(int nEl, Float_t vb, Float_t ncharge, int readout)
  {

    //KDetector Gaps_Detector;
    
    numEl = nEl;
    v_B = vb;
    neff = ncharge;

    SStep= 1;
    nx = 1000;
    ny = 50;
    nz = 1;
    
    //detector
    dimX = 50.8*pow(10,3); //2inch diameter
    dimY = 2.5*pow (10,3); //2.5 mm thick

    //deep and shallow groove
    depthD = 500; //depth of deep groove 500um
    widthD = 1 * pow(10,3); //width of deep groove 1mm

    depthS = 350; //depth of shallow groove 350um
    widthS = 1*pow(10,3); //width of shallow groove 1mm
    
    
    //guard ring and edge of detector
    widthGR = 1*pow(10,3); //guard ring width 1mm
    widthE = 1*pow(10,3); //edge width 1mm

  
    //electrodes
    widthGroove = 1*pow(10,3); //1 mm wide groove between electrodes
    depthGroove = 350; //350mm deep groove between electrodes
    widthEl = (dimX - (2*(widthD + widthS) + 2*(widthGR + widthE) + ((numEl-1)*widthGroove)))/numEl; //width of electrodes
    depthEl = .4; //150 nm thick electrode layer

    //bias voltage applied to detector
    Voltage=v_B;
  
    //init geometry
    EG=new TH3I("EG","EG",nx,0,dimX,ny,0,dimY,nz,0,1);
    EG->GetXaxis()->SetTitle("x [#mum]");
    EG->GetYaxis()->SetTitle("y [#mum]");
    EG->GetZaxis()->SetTitle("z [#mum]");

    //init material
    DM=new TH3I("DM","DM",nx,0,dimX,ny,0,dimY,nz,0,1);
    DM->GetXaxis()->SetTitle("x [#mum]");
    DM->GetYaxis()->SetTitle("y [#mum]");
    DM->GetZaxis()->SetTitle("z [#mum]");

    //init space charge histo
    NeffH=new TH3F("Neff","Neff",nx,0,dimX,ny,0,dimY,nz,0,1);
    NeffH->GetXaxis()->SetTitle("x [#mum]");
    NeffH->GetYaxis()->SetTitle("y [#mum]");
    NeffH->GetZaxis()->SetTitle("z [#mum]");  

    //  SetUpMaterial
    for(int j=0;j<=dimY;j++)
      for(int i=0;i<=dimX;i++) 
	{
	  DM->SetBinContent(i,j,1,0); 
	  NeffH->SetBinContent(i,j,1,neff);   
	}

    // Collection electrode - set the collection electrode to 1 at the base of the detector for Ramos Theorem
    int Mat = 1;
    int Wpval=2; //variable for weighting potential, 2 sets bias on plate
    Float_t BackPos[3]={dimX/2, depthEl/2, 0.5};
    Float_t BackSiz[3]={dimX/2, depthEl/2, 0.5};
    ElRectangle(BackPos,BackSiz,Wpval,Mat); 

    //print statement
    cout << "Gaps Detector Initialized"<< endl;
    cout << "number of electrodes: "<< nEl << ", Voltage: "<< vb << "V, Space Charge: " << ncharge<<"E12"<< endl;

    //setting electrodes
    set_Electrodes(readout);
  }

  void set_Electrodes(int whichReadEl)
  {

    int MatE = 1; //electrode material
    int WpvalE = 1; //ground

    //set guard ring (grounded electrode)
    Float_t GuardRingSiz[3] = {widthGR/2, depthEl/2, 0.5};
    Float_t GuardRing1Pos[3] = {(dimX - (widthE + widthD + widthGR/2)), (dimY-depthEl/2), 0.5};
    Float_t GuardRing2Pos[3] ={(widthE + widthD + widthGR/2),(dimY-depthEl/2), 0.5};
    ElRectangle(GuardRing1Pos, GuardRingSiz, WpvalE, MatE);
    ElRectangle(GuardRing2Pos, GuardRingSiz, WpvalE, MatE);
    
    //Set strip electrodes
    Float_t StripPos[3] = {(widthE+widthGR+widthD+widthS + (widthEl/2)), (dimY - depthEl/2), 0.5};
    Float_t StripSiz[3] = {(widthEl/2), depthEl/2, 0.5};

    for( int El =0; El<numEl; El++) //loop through strip electrodes
      {
	if(El==whichReadEl)
	  {
	    WpvalE=16385; //16385 designates the read out node
	    StripPos[0] = (widthE + widthGR + widthD + widthS) + (El*widthGroove) + (El*widthEl) + (widthEl/2); 
	    ElRectangle(StripPos,StripSiz,WpvalE,MatE);     
	  }
	else
	  {
	    WpvalE=1; //1 grounds all other electrodes
	    StripPos[0] = (widthE + widthGR + widthD + widthS) + (El*widthGroove) + (El*widthEl) + (widthEl/2); 
	    ElRectangle(StripPos,StripSiz,WpvalE,MatE);   
	  }
      }
    cout <<"electrodes set with read out electrode: " << whichReadEl << endl;
  };

  void set_Square_Grooves()
  {
    //set deep groove
  
    int WpvalG = 0; //not an electrode
    int MatG = 20; //air

    Float_t DGrooveSiz[3] = {widthD/2, depthD/2, 0.5};
    Float_t DGroove1Pos[3] = {(dimX - (widthE + widthD/2)), (dimY - depthD/2), 0.5};
    Float_t DGroove2Pos[3] = {(widthE + widthD/2), (dimY - depthD/2), 0.5};
    ElRectangle(DGroove2Pos, DGrooveSiz, WpvalG, MatG);
    ElRectangle(DGroove1Pos, DGrooveSiz, WpvalG, MatG);

  
    //set shallow groove
    Float_t SGrooveSiz[3] = {widthS/2, depthS/2, 0.5};
    Float_t SGroove1Pos[3] = {(dimX - (widthE + widthD + widthGR + widthS/2)), (dimY - depthS/2), 0.5};
    Float_t SGroove2Pos[3] = {(widthE + widthD + widthGR + widthS/2), (dimY - depthS/2), 0.5};
    ElRectangle(SGroove1Pos, SGrooveSiz, WpvalG, MatG);
    ElRectangle(SGroove2Pos, SGrooveSiz, WpvalG, MatG);
      
    //set grooves between strips
    Float_t GrooveSiz[3] = {widthGroove/2, depthGroove/2, 0.5};
    Float_t GroovePos[3] = {(widthE+widthD+widthS+widthGR+(widthGroove/2)), (dimY - depthGroove/2), 0.5};
  
    int numGroove = numEl-1;
    for (int i = 0; i<numGroove; i++)
      {	
	GroovePos[0] = (widthE + widthD + widthS + widthGR +(i+1)*widthEl + i*widthGroove + widthGroove/2);
	ElRectangle(GroovePos, GrooveSiz, WpvalG, MatG);
      }

    cout << "square grooves set"<< endl;

  };


  void switch_readoutEl(int new_readout)
  {
    
    Float_t StripPos[3] = {(widthE+widthGR+widthD+widthS + (widthEl/2)), (dimY - depthEl/2), 0.5};
    Float_t StripSiz[3] = {(widthEl/2), depthEl/2, 0.5};
    
    for( int El =0; El<numEl; El++)
      {
	if(El == new_readout)
	  {
	    int Wpval = 16385;
	    StripPos[0] = (widthE + widthGR + widthD + widthS)
	      + (El*widthGroove)
	      + (El*widthEl)
	      + (widthEl/2);
	    
	    ElRectangle(StripPos, StripSiz, Wpval, Mat);
	  }
	else
	  {
	    int Wpval = 1;
	    StripPos[0] = (widthE+ widthGR + widthD + widthS)
	      + (El*widthGroove)
	      + (El*widthEl)
	      + (widthEl/2);
	    
	    ElRectangle(StripPos, StripSiz, Wpval, Mat);
	  }

	//resetting boundary conditions and field
	SetBoundaryConditions();
	CalField(0);
	CalField(1);
      }
  };
};

#endif

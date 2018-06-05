#ifndef __PAD_DETECTOR_H_INCLUDED__
#define __PAD_DETECTOR_H_INCLUDED__

#include <iostream>

class Pad_Detector: public KDetector
{
  
 public:
 
  Float_t v_B;
  Float_t neff;
  Float_t dimX, dimY;
  int nx, ny, nz, SStep;
  Float_t depthEl;
  
  Pad_Detector(lx, ly, v, q)
    {
      v_B = v;
      neff = q;
      dimX = lx;
      dimY = ly;
      
      depthEl = .5; //50nm thick electrode layer
      Voltage = v;


      EG = new TH3I("EG", "EG", nx, 0, dimX, ny, 0, dimY, nz, 0, 1);
      EG->GetXaxis()->SetTitle("x [#mum]");
      EG->GetYaxis()->SetTitle("y [#mum]");
      EG->GetZaxis()->SetTitle("z [#mum]");
      
      DM = new TH3I("EG", "EG", nx, 0, dimX, ny, 0, dimY, nz, 0, 1);
      DM->GetXaxis()->SetTitle("x [#mum]");
      DM->GetYaxis()->SetTitle("y [#mum]");
      DM->GetZaxis()->SetTitle("z [#mum]");
      
      NeffH = new TH3F("Neff", "Neff", nx, 0, dimX, ny, 0, dimY, nz, 0, 1);
      NeffH->GetXaxis()->SetTitle("x [#mum]");
      NeffH->GetYaxis()->SetTitle("y [#mum]");
      NeffH->GetZaxis()->SetTitle("z [#mum]");
      
      //setting up material
      for(int j=0; j<=dimY, j++)
	for(int i=0; i<=dimX, i++)
	  {
	    DM->SetBinContent(i, j, 1, 1);
	    NeffH->SetBinContent(i, j, 1, neff);
	  }
      
      //Bottom Electrode
      int Mat = 1;
      int Wpval = 2;
      Float_t BackPos[3] = {dimX/2, depthEl/2, 0.5};
      Float_t BackSiz[3] = {dimX/2, depthEl/2, 0.5};
      ElRectangle(BackPos, BackSiz, Wpval, Mat);
      
      //Top Electrode
      Mat =1;
      Wpval = 16385;
      Float_t topPos[3] = {dimX/2, dimY-(depthEl/2), 0.5};
      Float_t topSiz[3] = {dimX/2, depthEl/2, 0.5);
      ElRectantle(topPos, topSiz, Wpval, Mat);
      
      //Set Boundary Conditions and Calculate Fields
      SetBoundaryConditions();
      CalField(0);
      CalField(1);
      
      //Set Histogram Ranges
      SetDrifHisto(1000e-9);
      SetPrecision(10e-10);
      det.diff =1;
      
      
      

    };
  
};

#endif

#include <math.h>
{
  KDetector det;
  det.SStep = 1;
  det.nx = 10;
  det.ny = 100;
  

  Float_t dimX = 2 * pow(10,3); //2mm
  Float_t dimY = 2 * pow(10,3);
  Float_t depthEl = .5; // 50nm thick electrode layer

  det.Voltage = -200.; //-200V applied to bottom of parrallel plate capacitor

  //init geometry
  det.EG = new TH3I("EG","EG",det.nx,0,dimX,det.ny,0,dimY,det.nz,0,1);
  det.EG->GetXaxis()->SetTitle("x [#mum]");
  det.EG->GetYaxis()->SetTitle("y [#mum]");
  det.EG->GetZaxis()->SetTitle("z [#mum]");

  //init material
  det.DM = new TH3I("EG","EG",det.nx,0,dimX,det.ny,0,dimY,det.nz,0,1);
  det.DM->GetXaxis()->SetTitle("x [#mum]");
  det.DM->GetYaxis()->SetTitle("y [#mum]");
  det.DM->GetZaxis()->SetTitle("z [#mum]");

  //init space charge

  det.NeffH = new TH3F("Neff","Neff",det.nx,0,dimX,det.ny,0,dimY,det.nz,0,1);
  det.NeffH->GetXaxis()->SetTitle("x [#mum]");
  det.NeffH->GetYaxis()->SetTitle("y [#mum]");
  det.NeffH->GetZaxis()->SetTitle("z [#mum]");

  //Set Up Material
  float neff = 0.1; //space charge in 10^12 [cm^-3]
  for(int j=0; j<=det.ny; j++)
    for(int i=0; i<det.nx; i++)
      {
	det.DM->SetBinContent(i,j,1,2);
	det.NeffH->SetBinContent(i,j,1,neff);
      }

  //collection electrode
  int Mat = 1;
  int Wpval = 2;
  Float_t BackPos[3]={dimX/2, 1, 0.5};
  Float_t BackSiz[3]={dimX/2, 0.5, 0.5};
  det.ElRectangle(BackPos, BackSiz, Wpval, Mat);

  //top electrode
  Mat =1;
  Wpval = 16385;
  Float_t topPos[3]={dimX/2, dimY-1, 0.5};
  Float_t topSiz[3] = {dimX/2, 1, 0.5};
  det.ElRectangle(topPos, topSiz, Wpval, Mat);

  //set dirilecth boundary conditions
  det.SetBoundaryConditions();

  //calculate fields, weighting and electric
  det.CalField(0);
  det.CalField(1);

  det.SetDriftHisto(1000e-9);
  det.SetPrecision(10e-10);
  det.diff=1;

  TCanvas Show;
  Show.Divide(2,2);
  Show.cd(1);

  //Draw electric potential [V]
  char EPname[]="EP";
  EP = det.Draw(EPname);
  EP->Draw("COLZ");
  EP->SetTitle("Electric Potential [V]");

  Show.cd(2);

  //Draw weighting potential [no units]
  char Wname[] = "WP";
  WP = det.Draw(Wname);
  WP->Draw("COLZ");
  WP->SetTitle("Weighting Potential");

  Show.cd(3);

  // Draw absolute value of electric field [V/um]
  char EFname[] = "EF";
  EF = det.Draw(EFname);
  EF->Draw("COLZ");
  EF->SetTitle("Absolute Value of Electric Field [V/um]");

  Show.cd(4);

  // Draw material (red = air, white=silicon, violet=electrode Al, Blue=oxide);                   
  char Mname[] = "M";
  M = det.Draw(Mname);
  M->Draw("COLZ");
  M->SetTitle("Geometry");


  // SIMULATION OF INDUCED CURRENT AT SIGNAL STRIP                                                                                              
  TCanvas Drift;
  Drift.Divide(2,1);
  Drift.cd(1);

  float particle_x = dimX/2; //x-coordinate of halfway point of electrode                                              
  float particle_y = dimY/2; //halfway through detector
  det.diff=1;// diffusion is o                  
  det.SetEntryPoint(particle_x, particle_y, 0.5);
  det.SetExitPoint(particle_x, particle_y, 0.5);
  int n_buckets = 50; //50 e-h pairs along the track generated
  det.ShowMipIR(n_buckets);
  det.MipIR(n_buckets); //simulation of drift for 50 buckets along the particle track


 // Draw mip track                                                                                                                  
  Drift.cd(2);
  
  det.neg->SetLineColor(4);  //elecrons are blue
  det.pos->SetLineColor(2);  // holes are red
  
  det.sum->GetCumulative()->DrawCopy();
  det.neg->GetCumulative()->DrawCopy("SAME");
  det.pos->GetCumulative()->DrawCopy("SAME");

  
}

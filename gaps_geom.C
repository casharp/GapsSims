#include <math.h>
{
  KDetector det; 
  det.SStep=1; 
  det.nx=1000; 
  det.ny=50;
  det.nz=1;

  //detector
  Float_t dimX = 50.8 * pow(10,3); //2inch diameter
  Float_t dimY = 2.5 * pow (10,3); //2.5 mm thick

  //deep and shallow groove
  Float_t depthD = 500; //depth of deep groove 500um
  Float_t depthS = 350; //depth of shallow groove 350um
  Float_t widthD = 1* pow(10,3); //width of deep groove 1mm
  Float_t widthS = 1* pow(10,3); //width of shallow groove 1mm

  //guard ring and edge of detector
  Float_t widthGR = 1* pow(10,3); //guard ring width 1mm
  Float_t widthE = 1* pow(10,3); //edge width 1mm

  
  //electrodes
  Float_t numEl = 8; //number of electrodes
  Float_t widthGroove = 1 * pow(10,3); //1 mm wide groove between electrodes
  Float_t depthGroove = 350; //350mm deep groove between electrodes
  Float_t widthEl = (dimX - (2*(widthD + widthS) + 2*(widthGR + widthE) + ((numEl-1)*widthGroove)))/numEl; //width of electrodes
  Float_t depthEl = .4; //150 nm thick electrode layer

  //bias voltage applied to detector
  det.Voltage=-250.;
  //operating temperature
  det.Temperature=233; //233K operating

  
  //init geometry
  det.EG=new TH3I("EG","EG",det.nx,0,dimX,det.ny,0,dimY,det.nz,0,1);
  det.EG->GetXaxis()->SetTitle("x [#mum]");
  det.EG->GetYaxis()->SetTitle("y [#mum]");
  det.EG->GetZaxis()->SetTitle("z [#mum]");

  //init material
  det.DM=new TH3I("DM","DM",det.nx,0,dimX,det.ny,0,dimY,det.nz,0,1);
  det.DM->GetXaxis()->SetTitle("x [#mum]");
  det.DM->GetYaxis()->SetTitle("y [#mum]");
  det.DM->GetZaxis()->SetTitle("z [#mum]");

  //init space charge histo
  det.NeffH=new TH3F("Neff","Neff",det.nx,0,dimX,det.ny,0,dimY,det.nz,0,1);
  det.NeffH->GetXaxis()->SetTitle("x [#mum]");
  det.NeffH->GetYaxis()->SetTitle("y [#mum]");
  det.NeffH->GetZaxis()->SetTitle("z [#mum]");

  //  SetUpMaterial
  Float_t neff = .02; //space charge in 10^12 [cm^-3]
  
  for(int j=0; j<=det.dimY; j++)
    for(int i=0; i<=det.dimX; i++) 
      {
	det.DM->SetBinContent(i,j,1,0); 
	det.NeffH->SetBinContent(i,j,1,neff);  	
      }

  // Collection electrode - set the collection electrode to 1 at the base of the detector for Ramos Theorem
  int Mat = 1;
  int Wpval=2; //variable for weighting potential, 2 sets bias on plate
  Float_t BackPos[3]={dimX/2, 1, 0.5};
  Float_t BackSiz[3]={dimX/2, 0.5, 0.5};
  det.ElRectangle(BackPos,BackSiz,Wpval,Mat); 

  
  //Setting up strip electrodes
  Mat = 1;
  Wpval = 1;
  Float_t StripPos[3] = {(widthE+widthGR+widthD+widthS + (widthEl/2)), (dimY - depthEl/2), 0.5};
  Float_t StripSiz[3] = {(widthEl/2), depthEl/2, 0.5};

  //choose a read out electrode
  int whichReadEl = 0; //1st electrode is the read out
  
  for( int El =0; El<numEl; El++) //loop through electrodes
    {
      if(El==whichReadEl)
	{
	  Wpval=16385; //16385 designates the read out node
	  StripPos[0] = (widthE + widthGR + widthD + widthS) + (El*widthGroove) + (El*widthEl) + (widthEl/2); 
	  det.ElRectangle(StripPos,StripSiz,Wpval,Mat);	    
	}
      else
	{
	  Wpval=1; //1 grounds all other electrodes
	  StripPos[0] = (widthE + widthGR + widthD + widthS) + (El*widthGroove) + (El*widthEl) + (widthEl/2); 
	  det.ElRectangle(StripPos,StripSiz,Wpval,Mat);	  
	}
    }

  
  //set deep groove
  
  Wpval = 0; //not an electrode
  Mat = 20; //air

  Float_t DGrooveSiz[3] = {widthD/2, depthD/2, 0.5};
  Float_t DGroove1Pos[3] = {(dimX - (widthE + widthD/2)), (dimY - depthD/2), 0.5};
  Float_t DGroove2Pos[3] = {(widthE + widthD/2), (dimY - depthD/2), 0.5};
  det.ElRectangle(DGroove2Pos, DGrooveSiz, Wpval, Mat);
  det.ElRectangle(DGroove1Pos, DGrooveSiz, Wpval, Mat);

  
  //set shallow groove

  Float_t SGrooveSiz[3] = {widthS/2, depthS/2, 0.5};
  Float_t SGroove1Pos[3] = {(dimX - (widthE + widthD + widthGR + widthS/2)), (dimY - depthS/2), 0.5};
  Float_t SGroove2Pos[3] = {(widthE + widthD + widthGR + widthS/2), (dimY - depthS/2), 0.5};
  det.ElRectangle(SGroove1Pos, SGrooveSiz, Wpval, Mat);
  det.ElRectangle(SGroove2Pos, SGrooveSiz, Wpval, Mat);
    
  //set guard ring (grounded electrode)
  Wpval = 1;
  Mat = 1; 
  Float_t GuardRingSiz[3] = {widthGR/2, depthEl/2, 0.5};
  Float_t GuardRing1Pos[3] = {(dimX - (widthE + widthD + widthGR/2)), (dimY-depthEl/2), 0.5};
  Float_t GuardRing2Pos[3] ={(widthE + widthD + widthGR/2),(dimY-depthEl/2), 0.5};
  det.ElRectangle(GuardRing1Pos, GuardRingSiz, Wpval, Mat);
  det.ElRectangle(GuardRing2Pos, GuardRingSiz, Wpval, Mat);
  
  //set grooves between strips


  Float_t GrooveSiz[3] = {widthGroove/2, depthGroove/2, 0.5};
  Float_t GroovePos[3] = {(widthE+widthD+widthS+widthGR+(widthGroove/2)), (dimY - depthGroove/2), 0.5};
  
  int numGroove = numEl-1;

  for (int i = 0; i<numGroove; i++)
    {
      Mat = 20;
      Wpval = 0; 
      
      GroovePos[0] = (widthE + widthD + widthS + widthGR +(i+1)*widthEl + i*widthGroove + widthGroove/2);

      det.ElRectangle(GroovePos, GrooveSiz, Wpval, Mat);
    }
  

  //Set dirilecht boundary conditions
  det.SetBoundaryConditions();

  //Calculate fields, weighting and electric
  det.CalField(0);
  det.CalField(1);
  
  det.SetDriftHisto(1000e-9); // this sets the range of current histogram 
  det.SetPrecision(1e-10); // this is the precision of tracking in terms of time
  det.diff=1; // diffusion on = 1, diffusion off=0


  //Plot Fields
  
  TCanvas Show;
  Show.Divide(2,2);
  Show.cd(1);
  
  // Draw electric potential [V]
  char EPname[] = "EP";
  EP = det.Draw(EPname);
  EP->Draw("COLZ");
  EP->SetTitle("Electric Potential [V]");
  
  Show.cd(2);
  
  // Draw weighting potential (no units)
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
  
  // Draw material
  char Mname[] = "M";
  M = det.Draw(Mname);
  M->Draw("COLZ");
  M->SetTitle("Geometry");


  
  
  /*/ SIMULATION OF INDUCED CURRENT AT SIGNAL STRIP

  float particle_x = (widthE + widthGR + widthD + widthS)
    + (widthGroove*whichReadEl)
    + (widthEl*whichReadEl)
    + (widthEl/2); //x-coordinate of halfway point of electrode
  float particle_y = dimY/2; //halfway through detector 

  det.SetEntryPoint(particle_x, particle_y, 0.5);
  det.SetExitPoint(particle_x, particle_y, 0.5);

  int n_buckets = 100; //50 e-h pairs along the track generated

  det.diff=1;// diffusion is on 
  

 // Draw mip track                                                                                  
  TCanvas Drift;
  Drift.Divide(2,1);
  Drift.cd(1);
  det.ShowMipIR(n_buckets);   
  Drift.cd(2);
  det.MipIR(n_buckets); //simulation of drift for 50 buckets along the particle track   
                                         
  det.neg->SetLineColor(4);  //elecrons are blue                                                    
  det.pos->SetLineColor(2);  // holes are red
    
  det.sum->GetCumulative()->DrawCopy();
  det.neg->GetCumulative()->DrawCopy("SAME");
  det.pos->GetCumulative()->DrawCopy("SAME");


  
  
  // SIMULATION OF INDUCED CURRENT ON SIGNAL STRIP FOR HIT NEIGHBOUR                                 

  
  particle_x = (widthE + widthGR + widthD + widthS)
    + (widthGroove*whichReadEl)
    + (widthEl*(whichReadEl+1))
    + (widthGroove/2); 
  particle_y = dimY/2; 
  det.SetEntryPoint(particle_x, particle_y, 0.5);
  det.SetExitPoint(particle_x, particle_y, 0.5);

  TCanvas Drift2;
  Drift2.Divide(2,2);

  Drift2.cd(1);
  det.ShowMipIR(n_buckets); //simulation of drift for 50 buckets along the particle track    

  Drift2.cd(2);
  det.MipIR(n_buckets);  // B=0; 30 e-h pair buckets along the track generated                  

  det.sum->GetCumulative()->DrawCopy();  // total induced current
  det.neg->SetLineColor(4);  //elecrons are blue                                                    
  det.pos->SetLineColor(2);  // holes are red                                                       
  det.neg->GetCumulative()->DrawCopy("SAME");
  det.pos->GetCumulative()->DrawCopy("SAME");

  
  
  //SWITCH READ OUT ELECTRODE TO THE NEXT ELECTRODE


  int nextReadEl = whichReadEl+=1;

  for( int El =0; El<numEl; El++) //loop through electrodes
    {
      if(El==nextReadEl)
	{
	  Wpval=16385; //16385 designates the read out node
	  StripPos[0] = (widthE + widthGR + widthD + widthS) + (El*widthGroove) + (El*widthEl) + (widthEl/2); 
	  det.ElRectangle(StripPos,StripSiz,Wpval,Mat);	    
	}
      else
	{
	  Wpval=1; //1 grounds all other electrodes
	  StripPos[0] = (widthE + widthGR + widthD + widthS) + (El*widthGroove) + (El*widthEl) + (widthEl/2); 
	  det.ElRectangle(StripPos,StripSiz,Wpval,Mat);	  
	}
    }


  //Set dirilecht boundary conditions
  det.SetBoundaryConditions();

  //Calculate fields, weighting and electric
  det.CalField(0);
  det.CalField(1);

  //plotting new MIP with new read out electrode

  Drift2.cd(3);
  det.ShowMipIR(n_buckets); //simulation of drift for 50 buckets along the particle track    

  Drift2.cd(4);
  det.MipIR(n_buckets);  // B=0; 30 e-h pair buckets along the track generated                  

  det.sum->GetCumulative()->DrawCopy();  // total induced current
  det.neg->SetLineColor(4);  //elecrons are blue                                                    
  det.pos->SetLineColor(2);  // holes are red                                                       
  det.neg->GetCumulative()->DrawCopy("SAME");
  det.pos->GetCumulative()->DrawCopy("SAME");
  
  
  f.Close();*/

  
}

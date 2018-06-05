#include "gaps_detector.h"

int main()
{
  Gaps_Detector det1(8, -250, .02, 7);
  det1.set_Square_Grooves();

  
  Float_t particle_x = (det1.widthE + det1.widthGR + det1.widthD + det1.widthS)
    + (det1.widthGroove*det1.readout)
    + (det1.widthEl*det1.readout)
    + (det1.widthEl/2);      
  Float_t particle_y = det1.dimY/2; //halfway through detector 
  
  Float_t entrypoint[3] = {particle_x, particle_y, 0.5};
  Float_t exitpoint[3] = {particle_x, particle_y, 0.5};
  int n_buckets = 100;
  string directory = "~/Documents/KDetSimDist.1.1/gaps_detector/8strip_mip_output_understrip_readout_7";
  
  /*
  det1.SetBoundaryConditions();
  det1.CalField(0);
  det1.CalField(1);
  
  det1.diff = 1;
  det1.SetDriftHisto(1000e-9);
  det1.SetPrecision(1e-10);

  det1.SetEntryPoint(entrypoint[0], entrypoint[1], 0.5);
  det1.SetExitPoint(exitpoint[0], exitpoint[1], 0.5);

  TCanvas Mipplot;
  gStyle->SetOptStat(0);
  Mipplot.Divide(2,2);

  Mipplot.cd(1);
  det1.ShowMipIR(n_buckets);

  Mipplot.cd(2);
  det1.MipIR(n_buckets);

  det1.sum->GetCumulative()->DrawCopy();
  det1.neg->SetLineColor(4);
  det1.pos->SetLineColor(2);
  det1.neg->GetCumulative()->DrawCopy("SAME");
  det1.pos->GetCumulative()->DrawCopy("SAME");

  string psname = directory + ".ps";
  Mipplot.SaveAs(psname.c_str());
  */
  
};

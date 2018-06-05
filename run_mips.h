#ifndef __RUN_MIPS_H_INCLUDED__
#define __RUN_MIPS_H_INCLUDED__

#include <iostream>

void run_MIP(Float_t in[3], Float_t out[3], int n_buckets, string directory)::KDetector
{

  //saves the plots of the MIP
  //first plot shows the MIP through the detector
  //second plot shows the induced charge on the read out electrode as a function of time
  
  diff = 1;
  SetDriftHisto(1000e-9);
  SetPrecision(1e-10);
  
  cout<< "Diffusion is turned on." << endl;
  cout << "Range of histogram is 1 microsecond, time-precision of run is 1e-10s." << endl;

  SetEntryPoint(in[0], in[1], 0.5);
  SetExitPoint(out[0], out[1], 0.5);
  
  TCanvas Mipplot;
  gStyle->SetOptStat(0);
  Mipplot.Divide(1,2);
  
  Mipplot.cd(1);
  ShowMipIR(n_buckets);
  
  Mipplot.cd(2);
  MipIR(n_buckets);
  
  sum->GetCumulative()->DrawCopy();
  neg->SetLineColor(4);
  pos->SetLineColor(2);
  neg->GetCumulative()->DrawCopy("SAME");
  pos->GetCumulative()->DrawCopy("SAME");
  
  string psname = directory + ".ps";
  Mipplot.SaveAs(psname.c_str());

};

Float_t get_time_readout()
{
  //gets read out time from MIP run

};

#endif

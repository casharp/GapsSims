#ifndef __PLOT_FIELDS_H__
#define __PLOT_FIELDS_H__

void plot_all_fields(string directory)::KDetector
{
  //saves the plot of 4 fields of the detector

  TCanvas Fields;
  Fields.Divide(2,2);
  
  Fields.cd(1);
  char EPname[] = "EP";
  EP = Draw(EPname);
  EP->Draw("COLZ");
  EP->SetTitle("Electric Potential [V]");

  Fields.cd(2);
  char Wname[] = "WP";
  WP = det.Draw(Wname);
  WP->Draw("COLZ");
  WP->SetTitle("Weighting Potential");

  Fields.cd(3);
  char EFname[] = "EF";
  EF = det.Draw(EFname);
  EF->Draw("Colz");
  EF->SetTitle("Absolute Value of Electric Field [V/um]");

  Fields.cd(4);
  char Mname[] = "M";
  M = det.Draw(Mname);
  M->Draw("COLZ");
  M->SetTitle("Geometry");

  string psname = directory + ".ps";
  Fields.SaveAs(psname.c_str());

};

#endif

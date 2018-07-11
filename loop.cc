#include "gaps_detector.h"

int main()
{
    string directory = "~/Documents/GapsSims/LoopOutput/";
    
//array of charge collected on each strip
    int size = 8;
    double values[size];
    ofstream myfile;
    myfile.open ("charge.txt", std::fstream::in | std::fstream::out | std::fstream::app);
    
        //loop over y-values
    double eventY[9];
    for (int k = 250; k < 2500; k+=500)
    {
        eventY[k] = k;
        
        //loop over x-values
        double eventX[17];
    for (int j = 3500; j < 25500; j+=1375)
    {
        eventX[j] = j;
    
        //loop over electrode number
    for(int i = 0; i < 8; i++)
  
    {
    Gaps_Detector det1(8, -250, .02, i);
    det1.set_Square_Grooves();
    
    int n_buckets = 100;
    
  // e-field calculation
    det1.SetBoundaryConditions();
    det1.CalField(0);
    det1.CalField(1);
    
    det1.diff = 1;
    det1.SetDriftHisto(1000e-9);
    det1.SetPrecision(1e-10);
    
    det1.SetEntryPoint(j, k, 0.5);
    det1.SetExitPoint(j, k, 0.5);

    //Plot the e-h pair creation within the detector (Note that last strip will show as active)
        TCanvas Mipplot;
        gStyle->SetOptStat(0);
    
        det1.ShowMipIR(n_buckets);
        
        
        stringstream a;
        a << j;
        
        stringstream b;
        b << k;

        string dsname = directory + "event_x" + a.str();
        dsname += "_y" + b.str();
        dsname += ".ps";
        Mipplot.SaveAs(dsname.c_str());

        
    //Fill the array with values from MipIR
    det1.MipIR(n_buckets);
    det1.sum->Print();
    values[i]=det1.sum->GetSum();
    cout << j << k << i << values[i] << eventX[j] << eventY[k];
 
        myfile << std::setw(5) << eventX[j] << " " << std::setw(4) << eventY[k] << "   " << std::setw(1) << (i + 1) << "  " << values[i] << '\n';
   
    
    } //close electrode loop
   
    //plotting histogram with the array
    TCanvas Mipplot;
    gStyle->SetOptStat(0);
        Mipplot.SetGrid();
  
    TH2 *h2 = new TH2D("h2","Charge Per Strip",10,0,9,101,0,100);
    
    double x[8];
    double y[8];
    int n = 8;
    
                   
    for (int i = 0; i < 8; i++)
    {
        x[i] = (i + 1);
        y[i] = (0-values[i]);
    }
    
    TGraph* gr = new TGraph(n,x,y);
    gr->SetMinimum(0);
    gr->SetMaximum(100);
    gr->SetMarkerColor(1);
    gr->SetMarkerSize(1.5);
    gr->SetMarkerStyle(29);
    gr->SetLineColor(46);
    gr->SetLineWidth(3);
    gr->SetFillColor(46);
    gr->SetTitle("Charge per Strip");
    gr->GetXaxis()->SetTitle("Strip Number");
    gr->GetYaxis()->SetTitle("Buckets Collected (#/100)");
    gr->Draw("AB1");
    //gr->Fit("gaus");

        
    Mipplot.Update();
    Mipplot.GetFrame()->SetFillColor(40);
    Mipplot.GetFrame()->SetBorderSize(12);
    Mipplot.Modified();
    
    //saving histogram

    stringstream a;
    a << j;

    stringstream b;
    b << k;
    
        string psname = directory + "collection_x" + a.str();
        psname += "_y" + b.str();
        psname += ".ps";
        Mipplot.SaveAs(psname.c_str());
    
    
    
    } //close x loop
    } //close y loop

myfile.close();

};

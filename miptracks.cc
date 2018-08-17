#include "gaps_detector.h"
#include <math.h>
#include <vector>


int miptracks()
{
    string directory = "~/Documents/GapsSims/MIPtracks/";
    
    //array of charge collected on each strip
    int size = 8;
    double values[size];
    double sumval[size];
    ofstream myfile;
    myfile.open ("mip_charge.txt", std::fstream::in | std::fstream::out | std::fstream::app);
    
    //loop over electrode number
    for(int i = 0; i < 8; i++)
    {
        
        Gaps_Detector det1(8, -250, .02, i);
        det1.set_Round_Grooves();
        
        int n_buckets = 100;
        
        det1.SetBoundaryConditions();
        det1.CalField(0);
        det1.CalField(1);
        
        det1.diff = 1;
        det1.SetDriftHisto(1000e-9);
        det1.SetPrecision(1e-10);
        
        
        std::vector<int> Exits; //create integer vector of exit values to be filled in following for loop
        float b;
        
        std::vector<int> Angles; //create integer vecotr of angle values to be filled in following for loop
        
        for(int a = 0; a <= 25000; a += 5000)
        {
            for(int theta = 0; theta <+ 90; theta += 5)
            {
                b = -2500 * (tan(theta*180/M_PI)) + a;
                Exits.push_back(b);
                Angles.push_back(theta);
            }
            
            int VectorSize = static_cast<int>(Exits.size());
            
            for(int c = 0; c < VectorSize; c++)
            {
                    det1.SetEntryPoint(a,2500, 0.5);
                    det1.SetExitPoint(Exits[c],0, 0.5);
                
                
                
                    TCanvas Mipplot;
                    gStyle->SetOptStat(0);
        
                    det1.ShowMipIR(n_buckets);
        
                    stringstream m;
                    m << a;
                    stringstream n;
                    n << Angles[c];
                        
                    string dsname = directory + "event2_x" + m.str();
                    dsname += "_theta" + n.str();
                    dsname += ".ps";
                    Mipplot.SaveAs(dsname.c_str());
        
        
        
                    //Fill the array with values from MipIR
                    det1.MipIR(n_buckets);
                    det1.sum->Print();
                    values[i]=det1.sum->GetSum();
                    cout << i << values[i];
        
                    myfile << std::setw(5) << a << "  " << std::setw(2) << Angles[c] << "  " << std::setw(1) << (i + 1) << "  " << values[i] << "  ";
        
                    sumval[0]=(0-(values[0]+values[1]+values[2]+values[3]+values[4]+values[5]+values[6]+values[7]));
            }   
        }
        
    } //close electrode loop
    
    myfile << "   " << sumval[0] << '\n';
    
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
    
    
    string psname = directory + "collection2.ps";
    Mipplot.SaveAs(psname.c_str());
    
    myfile.close();
    return 0;
};

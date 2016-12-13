#include "../include/fonctions.h"

using namespace std;

int main(int argc ,char *argv[]){
	stringstream converter;
	converter << argv[0];
	string program;
	converter >> program;
	converter.clear();

	if(argc != 1){
		cout << "[Integrals] only expects to have 1 parameter\n";
		cout << "[Offline] USAGE is : " << program << "\n";
		return -1;
	} else if(argc == 1){
		int Beta[9] = {4,6,8,10,12,14,16,18,20};

		TFile ROOTFile("Results.root","RECREATE");

		for(int b=0; b<9; b++){
			string canvas = "results-" + intTostring(Beta[b]);
			string canvastitle = "Deviation simulation to data";
			TCanvas* Canvas = new TCanvas(canvas.c_str(),canvastitle.c_str());
			Canvas->Divide(2,1);
			Canvas->cd(2)->Divide(2,2);

			string peakratio = "peakratio-" + intTostring(Beta[b]);
			string peakratiotitle = "Deviation of the peak ratio - inclanation " + intTostring(Beta[b]);
			TH2D* PeakRatio = new TH2D(peakratio.c_str(),peakratiotitle.c_str(),10,-2.5,47.5,5,1.875,3.125);
			PeakRatio->SetXTitle("Scintillator threshold (mm)");
			PeakRatio->SetYTitle("RPC threshold (mm)");
			PeakRatio->SetZTitle("%");

			string peakdistance = "peakdistance-" + intTostring(Beta[b]);
			string peakdistancetitle = "Deviation of the peak distance - inclanation " + intTostring(Beta[b]);
			TH2D* PeakDistance = new TH2D(peakdistance.c_str(),peakdistancetitle.c_str(),10,-2.5,47.5,5,1.875,3.125);
			PeakDistance->SetXTitle("Scintillator threshold (mm)");
			PeakDistance->SetYTitle("RPC threshold (mm)");
			PeakDistance->SetZTitle("%");

			string backratio = "backratio-" + intTostring(Beta[b]);
			string backratiotitle = "Deviation of the backward muon ratio - inclanation " + intTostring(Beta[b]);
			TH2D* BackRatio = new TH2D(backratio.c_str(),backratiotitle.c_str(),10,-2.5,47.5,5,1.875,3.125);
			BackRatio->SetXTitle("Scintillator threshold (mm)");
			BackRatio->SetYTitle("RPC threshold (mm)");
			BackRatio->SetZTitle("%");

			string missratio = "missratio-" + intTostring(Beta[b]);
			string missratiotitle = "Deviation of the missed muon ratio - inclanation " + intTostring(Beta[b]);
			TH2D* MissRatio = new TH2D(missratio.c_str(),missratiotitle.c_str(),10,-2.5,47.5,5,1.875,3.125);
			MissRatio->SetXTitle("Scintillator threshold (mm)");
			MissRatio->SetYTitle("RPC threshold (mm)");
			MissRatio->SetZTitle("%");

			string combined = "combined-" + intTostring(Beta[b]);
			string combinedtitle = "Combined deviation - inclanation " + intTostring(Beta[b]);
			TH2D* Combined = new TH2D(combined.c_str(),combinedtitle.c_str(),10,-2.5,47.5,5,1.875,3.125);
			Combined->SetXTitle("Scintillator threshold (mm)");
			Combined->SetYTitle("RPC threshold (mm)");
			Combined->SetZTitle("%");

			for(int ts=0; ts<5; ts++){
				double RPCThr = 2. + ts*0.25;

				string inputfileName = "/home/alex/Desktop/RPCs/Lyon_Electronics/Alexis_DATA/Data/Muon-Acceptance/Results/High-Stat-RPC-Thr/"
						+ intTostring(Beta[b]) + "/Results-" + intTostring(Beta[b]) + "-" + floatTostring(RPCThr) + ".csv";

				ifstream inputFile(inputfileName.c_str(), ios::in);

				if(inputFile){
					while(inputFile.good()){
						double Data[18] = {-9999};

						for(int i=0; i<18; i++){
							inputFile >> Data[i];
							if(Data[i] == -9999) break;
						}

						//Filling of the histograms
						double ScintThr = Data[0];

						double pratio = 100*abs((Data[9]/Data[16] - __PRatio_Data)/__PRatio_Data);
						double pdistance = 100*abs((Data[17]-Data[10] - __PDistance_Data)/__PDistance_Data);
						double bratio = 100*abs((Data[12]/(Data[2]+Data[12]) - __BRatio_Data)/__BRatio_Data);
						double mratio = 100*abs((Data[5]/(Data[2]+Data[12]) - __MRatio_Data)/__MRatio_Data);

						double stdev = sqrt(pratio*pratio + pdistance*pdistance + bratio*bratio + mratio*mratio)/4.;

						PeakRatio->Fill(ScintThr,RPCThr,pratio);
						PeakDistance->Fill(ScintThr,RPCThr,pdistance);
						BackRatio->Fill(ScintThr,RPCThr,bratio);
						MissRatio->Fill(ScintThr,RPCThr,mratio);
						Combined->Fill(ScintThr,RPCThr,stdev);
					}
				} else {
					cout << "Couldn't open : " << inputfileName << endl;
				}
			}

			Canvas->cd(1);
			Combined->GetZaxis()->SetRangeUser(0,100);
			gStyle->SetOptStat("0");
			gStyle->SetPalette(87);
			Combined->Draw("COLZ");
			Canvas->Update();

			Canvas->cd(2)->cd(1);
			PeakRatio->GetZaxis()->SetRangeUser(0,100);
			gStyle->SetOptStat("0");
			gStyle->SetPalette(87);
			PeakRatio->Draw("COLZ");
			Canvas->Update();

			Canvas->cd(2)->cd(2);
			PeakDistance->GetZaxis()->SetRangeUser(0,100);
			gStyle->SetOptStat("0");
			gStyle->SetPalette(87);
			PeakDistance->Draw("COLZ");
			Canvas->Update();

			Canvas->cd(2)->cd(3);
			BackRatio->GetZaxis()->SetRangeUser(0,100);
			gStyle->SetOptStat("0");
			gStyle->SetPalette(87);
			BackRatio->Draw("COLZ");
			Canvas->Update();

			Canvas->cd(2)->cd(4);
			MissRatio->GetZaxis()->SetRangeUser(0,100);
			gStyle->SetOptStat("0");
			gStyle->SetPalette(87);
			MissRatio->Draw("COLZ");

			string PDF = "Results-" + intTostring(Beta[b]) + ".pdf";
			string PNG = "Results-" + intTostring(Beta[b]) + ".png";
			Canvas->SaveAs(PDF.c_str());
			Canvas->SaveAs(PNG.c_str());
			Canvas->Write();
		}

		ROOTFile.Close();

		return 0;
	}
}

{
  char *name    = "R3B_BoxCsI_APD_SourceGamma";
  char *listing = "R3B_BoxCsI_APD_SourceGamma";
  char *upcom   = "Simulation of CsI(Tl) crystal for R3B, with Gamma Source";
  char *downcom = "Gamma Source. APD as detector";
  gROOT->ProcessLine(".x InitLitrani.C(4,name,listing,upcom,downcom,kFALSE,kFALSE,kTRUE)");

  TPhotoElecCompton *source;
  Int_t nRuns = 10;         // number of the Runs
  Double_t gammaE = 0.662;  //energy of the incomming gamma (MeV)
  Double_t nPhotMeV = 60000; // 60 000 cambiar                           
  Int_t nBins = 8192;//(int)(gammaE*nPhotMeV/2.0);
 // gLitPhys->SetLate(5000);



//**************************************************************************** CRYSTAL DEFINITION:  CsI(Tl), emission spectrum



  TOpticMaterial *CsITl;
 //  CsITl = new TOpticMaterial("CsITl","CsITl",kFALSE,1.0,"AbsorptionLength_CsITl");
CsITl = new TOpticMaterial("CsITl","CsITl",kFALSE,1.0,465.0,-1,-1,260, 108, 4.51);
  CsITl->IsIsotropic("RefrIndex_CsITl");
  CsITl->SetPhotMev(nPhotMeV);         //Set the intrinsic light yield 
  CsITl->SetXSectnPE("PhotoEl_CsITl"); //Set the name of the TSplineFit for the photoelectric effect
 // CsITl->SetXSectnPE(" PhotoEl_CaWO4"); //Set the name of the TSplineFit for the photoelectric effect
  TLitSpectrum *sp;
  cout << "   (1) - Measurements of Remi Chipaux" << endl;
  sp = new TLitSpectrum("Spectrum_CsITl","Emission Spectrum | CsI doped with Tallium measured by R Chipaux");
  sp->AddOneComponent(410,40.0,0.333333,600.0,-1.0,0.666666,3500.0,-1.0,0.333333);
  sp->AddOneComponent(470,30.0,0.075,600.0,-1.0,0.666666,3500.0,-1.0,0.333333);
  sp->AddOneComponent(520,40.0,0.666666,600.0,-1.0,0.666666,3500.0,-1.0,0.333333);
  sp->Normalize();

  CsITl->SetLitSpectrum(sp);
  CsITl->dEdx("DEdx_GammaInCsITl",1000.0,kFALSE);//DEdx_GammaInCsITl by Hector, using Geant 4, gives the value
 
//*********************************************************************************************** OTHER MATERIALS DEFINITIONS
 TOpticMaterial *air;
  air = new TOpticMaterial("Air","Air",kFALSE,1.0,10000.0);
  air->IsIsotropic(1.0);
  TOpticMaterial *glue;
   glue = new TOpticMaterial("Glue","Glue",kFALSE,1.0,"AbsorptionLength_DC93500");
// glue = new TOpticMaterial("Glue","Glue",kFALSE,1.0,"AbsorptionLength_DC93500");
  glue->IsIsotropic(1.5124);       ////(1.4124);
  TOpticMaterial *epoxy;
  epoxy = new TOpticMaterial("Epoxy","Epoxy window for APD",kFALSE,1.0,0.22);
  epoxy->IsIsotropic(1.54);     ///(1.52);
  TOpticMaterial *Silicium;
  Silicium = new TOpticMaterial("Silicium","Silicium of APD",kTRUE,1.0,"AbsorptionLength_Silicium");
  Silicium->IsIsotropic("RefrIndex_Silicium");
  const Double_t PropDiffus = 0.05;
  TRevetment *vm2000;
  vm2000 = new TRevetment("vm2000","vm2000","Air",PropDiffus,"RIndexRev_vm2000", "IIndexRev_vm2000",1.0);
  TRevetment *vm2000;  vm2000 = new TRevetment("badvm2000","badvm2000","Air",PropDiffus,"RIndexRev_vm2000", "IIndexRev_vm2000",1.0,0.05);
  TRevetment *teflon;
  teflon = new TRevetment("teflon","teflon","Air",PropDiffus,1.3, 1.3,1.0);

  TRevetment *revcaps;
  revcaps  = new TRevetment("Revcaps","Revcaps","Glue",1.0,"RIndexRev_CMScapsula","IIndexRev_CMScapsula",1.0,0.1);

  TRevetment *totabsorbing;
  totabsorbing = new TRevetment("TotAbsorbing","Totally absorbing revetment");
// ****************************************************************************************************************** GEOMETRY 
  const Double_t zero = 0.0;
  const Double_t crystal_dx = 0.5;  //half width of crystal in Ox direction (1cm)
  const Double_t crystal_dy = 0.5;  //half width of crystal in Oy direction (1cm)
  const Double_t crystal_dz = 5.0;  //half length of crystal axis, put here along Oz (for 10cm)
// We define the Glue and Epoxy position
  const Double_t glue_dz    = 0.0025; //half width of glue between crystal and APD [Montecchi] in Oz (glue = 50 um)
  const Double_t epoxy_dz   = 0.015;   //half width of epoxy window (epoxy = 300 um) in Oz.
  Double_t glue_z  = crystal_dz + glue_dz; //position z center of glue for APD. 
  Double_t epoxy_z = crystal_dz + 2*glue_dz + epoxy_dz;  //position z center of epoxy window for APD. 
// Definition of the position for APD
  const Double_t apd_dx  = 0.5;       //half-width of APDs (1cm) in direction Ox (to BR) 
  const Double_t apd_dy  = 0.5;       //half-width of APds (1cm) in direction Oy (to CR) 
  const Double_t apd_dz  = 0.0225;    //half-width of silicium of APD - (apd = 450 um)  (ESTABA 45 um)
  cout<< ""<< endl;
  cout<< "APD - 450 microns"<< endl;
  cout<< ""<< endl;
  Double_t apd_x  = zero;             //position center in x of APD.
  Double_t apd_y  = zero;             //position center in y of APD.
  Double_t apd_z;                     //position center in z of APD.
  apd_z =  crystal_dz + 2*glue_dz + 2*epoxy_dz + apd_dz;
// ****************************************************************************************************************** SHAPES
  TSBRIK *crystal;

  crystal = new TSBRIK("crystal","crystal","CsITl","vm2000",crystal_dx,crystal_dy,crystal_dz);   //Definition of the crystal shape 
  crystal->SetLineColor(5);

  TSBRIK *glue_apd;
  glue_apd = new TSBRIK("glue_apd","glue_pad","Glue","TotAbsorbing",apd_dx,apd_dy,glue_dz);     // Definition of the glue shape for APD
  glue_apd->SetLineColor(2);
  glue_apd->SetLineWidth(2);

  TSBRIK *epoxy_apd;
  epoxy_apd = new TSBRIK("epoxy_apd","epoxy_apd","Epoxy","TotAbsorbing",apd_dx,apd_dy,epoxy_dz);  // Definition of the epoxy window shape for APD
  epoxy_apd->SetLineColor(4);
  epoxy_apd->SetLineWidth(2);

//  APD
  TSBRIK *apd;
  apd = new TSBRIK("apd","apd","Silicium","TotAbsorbing",apd_dx,apd_dy,apd_dz);
  apd->fSuppl->SetAPD("hamamatsu1","hamamatsu1",4,5,"GainProfile_USCAPD");                         //USC APD
  apd->SetLineColor(104);
  apd->SetLineWidth(2);

// ****************************************************************************************************************** NODOS Y CONTACTOS

  TSNode *node1 = new TSNode("node1","node1",crystal);
  node1->SetLineColor(1);
  node1->SetLineWidth(2);
  node1->cd();

  TSNode *nodeglue = new TSNode("nodeglue","node for glue",  glue_apd,apd_x,apd_y,glue_z);             //glue
  TSNode *nodewin  = new TSNode("nodewin","node for window",epoxy_apd,apd_x,apd_y,epoxy_z);           //window
  TSNode *nodeAPD  = new TSNode("nodeAPD","node for APD",apd,apd_x,apd_y,apd_z);                       //APD

  TContact *contact_cg; 
  contact_cg  =new TContact("CsITl_Glue","CsITl_Glue","crystal","glue_apd",containing);                //  Definition of the contact crystal-glue
  TContact *contact_gw;
  contact_gw  =new TContact("Glue_EpoxyWindow","Glue_EpoxyWindow","glue_apd","epoxy_apd",identical);   //  Definition of the contact glue-window(epoxy) 
  TContact *contact_wapd;
  contact_wapd=new TContact("EpoyxWindow_APD","EpoxyWindow_APD","epoxy_apd","apd",identical);          //  Definition of the contact window-APD

// Generate events ******************************************************************************************** GENERACION DE EVENTOS
// Definition of the Gamma Source
  const Double_t src_dz = 0.1;      //half-width of gamma source, 2mm     
  const Double_t src_dist = 0.0;    //source distance from the crystal(cm)
//
  T3Vector StartCasc(0.0, 0.0, -crystal_dz - src_dz - src_dist); //source position
  const Double_t Theta0 = 0.01;
  Double_t sTheta0,cTheta0;
  sTheta0 = TMath::Sin(Theta0);
  cTheta0 = TMath::Cos(Theta0);
  const Double_t Phi0 = 0.0;
  Double_t sPhi0,cPhi0;
  sPhi0 = TMath::Sin(Phi0);
  cPhi0 = TMath::Cos(Phi0);
  T3Vector AxisCasc(sTheta0*cPhi0,sTheta0*sPhi0,cTheta0); //gamma direction
  source = new TPhotoElecCompton("Source","Source","crystal",gammaE,StartCasc,AxisCasc);
//
// Results
//
  TH1F* spectrum = new TH1F("spectrum", "spectrum", nBins, 0, nBins*1.0);
//
//*************************************************************************************************************************** Runs
//
  for (Int_t i=0; i<nRuns; ++i){
  cout << "Run number: " << i+1 << "/" << nRuns  << endl;
  source->Gen(101+i,4);
   cout<< "Fotones vistos en este run: " << gCs->fNpSeen << endl; 
  if (gCs->fNpSeen) spectrum->Fill(8*(gCs->fNpSeen));

}
// Drawing

  gLit->BookCanvas();
  gTwoPad->SelectPad2();
  node1->Draw();
  gTwoPad->ChangePad();
  gGp->SetTitle("z coordinate of beam");
//Summary() creates all summary histograms
  gGp->Summary();
//Draws one of the available histogram.
  gStyle->SetOptFit(1111);
  gGs->DoStat();
  gLit->CloseFiles();
}

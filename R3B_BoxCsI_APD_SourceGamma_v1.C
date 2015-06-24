{
  char *name    = "R3B_BoxCsI_APD_SourceGamma";
  char *listing = "R3B_BoxCsI_APD_SourceGamma";
  char *upcom   = "Simulation of CsI(Tl) crystal for R3B, with Gamma Source";
  char *downcom = "Gamma Source. APD as detector";
  gROOT->ProcessLine(".x InitLitrani.C(4,name,listing,upcom,downcom,kFALSE,kFALSE,kTRUE)");
//
// arguments of InitLitrani:
//
// size     = 0 | 1 | 2 | 3 according to the size you want for the canvas
// name     name of the produced ROOT file (without ".root")
// listing  name of the listing file (without ".lis")
// upcom    name of the comment appearing on top    left of canvas
// downcom  name of the comment appearing on bottom left of canvas
// otherseq true if one wants an other sequence of random numbers at each new launching
// indebug  true if one wants to run Litrani2 in debug mode
// WithDate if true, appends date and time to name of root file and name of listing
//
//   R3B_BoxCsI_APD based on GLASTWITHPINS and CMS1_setup and Gamma.C (gamma source)
//
//  R3B_BoxCsI_APD.C is an example of a setup with a crystal of CsI(Tl) having the shape of a
//rectangular parallelipiped. All the necessary fits or interpolations are searched on
//the ROOT database file SplineFitDB.rdb. The material CsI(Tl) is optically isotropic.
//  On the upper small face of the CsI(Tl) crystal is the APD, it has the shape of a TSBRIK. 
//The APD is GLUED onto the crystal. The slice of glue has also the shape of a TSBRIK. The APD
//has a window made of epoxy. This window has also the shape of TSBRIKs. So there are 4
// shapes in this setup:
//  -  1 TSBRIK  (crystal of CsITl)
//  -  1 TSBRIK  (glue)
//  -  1 TSBRIK  (windows of epoxy)
//  -  1 TSBRIK  (APD made of silicium)
//  CsI(Tl) is the only material which is fluorescent, i.e. able to emit light when
//crossed by particles. The other materials are the glue, the epoxy, the silicium and air.
//  The crystal of CsITl is wrapped with vm2000, it is the reason why we call the
//revetment "vm2000". The vm2000 revetment is well described by the fits
//"RIndexRev_vm2000" and "IIndexRev_vm2000". Values for the real part of the index have
//been carefully adjusted so that we obtain the values of the albedo of the revetment
//vm2000 that have been measured in the GLAST experiment. The revetment "vm2000" is mainly
//diffusive, but has a small proportion of 2.0% of photons reflected. This also has been
//measured and is described properly by Litrani. Between the crystal and the vm2000 foil
//is a slice of air. This is why we had to introduce material air.
//  The TSBRIKs made of glue, the TSBRIKS made of epoxy and the TSBRIKs made of silicium
//(PIN diode) are considered to be wrapped with a totally absorbing revetment which is
//called "totabsorbing".
//  We have yet to find the optical characteristics of the epoxy. For the time being, we
//put arbitrary values.
//
// Source option
// default = TSpontan
// 1 = TPhotoElecCompton
  TPhotoElecCompton *source;
  Int_t nRuns = 50;         // number of the Runs
//Histos and stuff
  Double_t gammaE = 0.662;  //energy of the incomming gamma (MeV)
  Double_t nPhotMeV = 60000; // 60 000 cambiar
  Int_t nBins = (int)(gammaE*nPhotMeV/2.0);
//
// Materials
//
//  CsITl is slow, so it is a good idea to change the default value of TooLate. According
//to Arache Djannati of the GLAST experiment, the time of the gate of the GLAST experiment
//is 2500 ns, which is quite short for CsI(Tl) ! There will be many photons killed because
//too late !
  gLitPhys->SetLate(2500);
//  Definition of the material CsITl. The magnetic permeability is declared to be 1, not
//depending upon wavelength. The absorpion length is given by the fit "AbsorptionLength_CsITl". 
//CsITl is declared as optically isotropic, the fits giving the
//variation of the index of refraction as a function of wavelength is "RefrIndex_CsITl",
//
// The fluorescent characteristics of CsI are taken from :
//     Measurements done by Remi Chipaux from Saclay
//     Measurements done by John E. Eldridge
// Please choose the one you want.
//
  TOpticMaterial *CsITl;
  CsITl = new TOpticMaterial("CsITl","CsITl",kFALSE,1.0,465.0,-1,-1,260, 108, 4.51);
// verified the  absorption length parameter La????, see TOpticalMaterial and the 
//the 
// if you ou intend to call IsIsotropic(), give only the La 
// and Lb,Lc=-1
//                                                      
// TOpticMaterial::TOpticMaterial(const char *name,const char *title,Bool_t sensible,
//    Double_t mu,Double_t absLa,Double_t absLb,Double_t absLc,
//    Float_t A,Float_t Z,Float_t density)
//where:
// Double_t mu: is the magnetic permeability 
// Double_t absLb,absLb,absLc : are absorption length parameters La, Lb, Lc
// Float_t A, Z, density: are the mass, atomic number and density(g/cm3) 
//
//CsITl = new TOpticMaterial("CsITl","CsITl",kFALSE,1.0,"AbsorptionLength_CsITl");
  CsITl->IsIsotropic("RefrIndex_CsITl");
//for the Gamma Source
  CsITl->SetPhotMev(nPhotMeV);         //Set the intrinsic light yield 
  CsITl->SetXSectnPE("PhotoEl_CsITl"); //Set the name of the TSplineFit for the photoelectric effect
//
  TLitSpectrum *sp;
  Int_t itype;
  cout << endl;
  cout << "You have the choice between 2 fluorescent spectra for CsITl :" << endl;
  cout << "   (1) - Measurements of Remi Chipaux" << endl;
  cout << "   (2) - Measurements of John E. Eldridge" << endl;
  cout << endl;
  cout << "Please enter your choice : ";
  cin  >> itype;
  if (itype != 1) itype = 2;
  switch (itype) {
  case 1:
   //    CsITl->FluoComponent(410.0,40.0,0.222222,600.0);
   //    CsITl->FluoComponent(410.0,40.0,0.111111,3500.0);
   //    CsITl->FluoComponent(470.0,30.0,0.05,600.0);
   //    CsITl->FluoComponent(470.0,30.0,0.025,3500.0);
   //    CsITl->FluoComponent(520.0,40.0,0.444444,600.0);
   //    CsITl->FluoComponent(520.0,40.0,0.222222,3500.0);
   sp = new TLitSpectrum("Spectrum_CsITl","Emission Spectrum | CsI doped with Tallium measured by R Chipaux");
   //the gaussians parameters
   sp->AddOneComponent(410,40.0,0.333333,600.0,-1.0,0.666666,3500.0,-1.0,0.333333);
   sp->AddOneComponent(470,30.0,0.075,600.0,-1.0,0.666666,3500.0,-1.0,0.333333);
   sp->AddOneComponent(520,40.0,0.666666,600.0,-1.0,0.666666,3500.0,-1.0,0.333333);
   sp->Normalize();
   break;
  default:
   //    CsITl->FluoComponent(551.25,51.73,0.4870625,600.0);
   //    CsITl->FluoComponent(551.25,51.73,0.2922375,3500.0);
   //    CsITl->FluoComponent(428.40,49.78,0.0615,600.0);
   //    CsITl->FluoComponent(428.40,49.78,0.0369,3500.0);
//    CsITl->FluoComponent(653.64,39.48,0.0764375,600.0);
//    CsITl->FluoComponent(653.64,39.48,0.0458625,3500.0);
   sp = new TLitSpectrum("Spectrum_CsITlJE","Emission Spectrum | CsI doped with Tallium measured by J.E Elridge");
   sp->AddOneComponent(551.25,51.73,0.7793,600.0,-1.0,0.625,3500.0,-1.0,0.375);
   sp->AddOneComponent(428.40,49.78,0.0984,600.0,-1.0,0.625,3500.0,-1.0,0.375);
   sp->AddOneComponent(653.64,39.48,0.1223,600.0,-1.0,0.625,3500.0,-1.0,0.375);
   sp->Normalize();
   break;
}
  CsITl->SetLitSpectrum(sp);
//Here 1000 should be 50000 !
  CsITl->dEdx("DEdx_GammaInCsITl",1000.0,kFALSE);
//DEdx_GammaInCsITl by Hector, using Geant 4, gives the value
//of the deposited dE/dx per cm in the CsI(Tl)crystal as a function of the energy of the gammas 
//
//  Definition of air, with magnetic permeability of 1 and absorption length of 10000 cm,
//independent of wavelength. Air is also isotropic, with index of refraction of 1.0 inde-
//pendent of wavelength.
  TOpticMaterial *air;
  air = new TOpticMaterial("Air","Air",kFALSE,1.0,10000.0);
  air->IsIsotropic(1.0);
//
//  Definition of glue, with magnetic permeability of 1 and absorption length depending
//upon wavelength according to the fit "AbsorptionLength_DC93500". We do not know yet what kind of glue
//will be chosen by GLAST, so we put a fixed index of 1.45 for the time being.
  TOpticMaterial *glue;
  glue = new TOpticMaterial("Glue","Glue",kFALSE,1.0,"AbsorptionLength_DC93500");
// Refractive index of 1.4124 found in DOW CORNING 93-500 documentation, no wave length
//dependency provided.
  glue->IsIsotropic(1.4124);
//
//  Definition of window made of epoxy, with magnetic permeability of 1, 
//absorption length of 0.22 cm,refraction index of 1.52, both not depending upon wavelength.
// These values extrapolated from indications given by Hammamatsu.
  TOpticMaterial *epoxy;
  epoxy = new TOpticMaterial("Epoxy","Epoxy window for APD",kFALSE,1.0,0.22);
  epoxy->IsIsotropic(1.52);
//
//  Definition of silicium, defined as sensible ( i.e. if a photon dies inside, it may be
//detected), with magnetic permeability of 1 and absorption length depending upon wave-
//length according to the fit "AbsorptionLength_Silicium" in file f_abslsilicium.. Silicium is isotro-
//pic with index of refraction depending  upon wavelength according to the fit
//"RefrIndex_Silicium" in file f_refrisilicium.
  TOpticMaterial *Silicium;
  Silicium = new TOpticMaterial("Silicium","Silicium of APD",kTRUE,1.0,"AbsorptionLength_Silicium");
  Silicium->IsIsotropic("RefrIndex_Silicium");
//
//  Definition of the revetment vm2000, with a slice of Air in-between ( a slice, not a
//thin slice ) with real part and imaginary part of index of refraction varying in
//function of wavelength according to the fits "RIndexRev_vm2000" and "IIndexRev_vm2000".
//The magnetic permeability is 1, independent of wavelength. There is no supplementary
//absorption.  The revetment is 98.0% diffusing.
  const Double_t PropDiffus = 0.05;
  TRevetment *vm2000;
  vm2000 = new TRevetment("vm2000","vm2000","Air",PropDiffus,
			"RIndexRev_vm2000", "IIndexRev_vm2000",1.0);
//  Bad vm2000 is for the revetment covering the chamfers! We give to this
//part of the revetment a supplementary absorption of 5%!
  TRevetment *vm2000;
  vm2000 = new TRevetment("badvm2000","badvm2000","Air",PropDiffus,
			"RIndexRev_vm2000", "IIndexRev_vm2000",1.0,0.05);
//
//Definition of the revetment Teflon, with a slice of Air in-between ( a slice, not a
//thin slice ) with real part and imaginary part of index of refraction assumed the fixe value 
//1.3 according to the RIndexRev_teflon.C file. It is necessary to find the real part and 
//imaginary part of index of refraction varying in function of wavelength
//The magnetic permeability is 1, independent of wavelength. There is no supplementary
//absorption.
  TRevetment *teflon;
  teflon = new TRevetment("teflon","teflon","Air",PropDiffus,1.3, 1.3,1.0);
//
//  Definition of the wrapping "Revcaps", describing the surface of the capsule containing
//the APD, which is in contact with face 5 of the crystal.
  TRevetment *revcaps;
  revcaps  = new TRevetment("Revcaps","Revcaps","Glue",1.0,"RIndexRev_CMScapsula",
			  "IIndexRev_CMScapsula",1.0,0.1);
//
//  Definition of the revetment "TotAbsorbing", giving only the 1st 2 arguments. When the
//other arguments 3 to 8 are not given, following default values are assumed :
//
// (3) - nameMat  = "none" : no slice material between face and revetment.
// (4) - diffus   = 0.0    : Reflecting, not diffusing. Irrelevant because of (8).
// (5) - Rindex   = 2.0    : Real part of index. Irrelevant because of (8).
// (6) - Iindex   = 6.0    : Imaginary part of Index. Irrelevant because of (8).
// (7) - Mu       = 1.0    : Magnetic permeability. Irrelevant because of (8).
// (8) - supplAbs = 1.0    : supplementary absorption of 100%.
//
// When a photon reaches such a revetment, it is absorbed in any case, it has no chance
//neither of making a total reflection ( there is no slice of material between shape and
//revetment ) nor of being reflected by the revetment (because the revetment has a
//supplementary absorption of 100%).
  TRevetment *totabsorbing;
  totabsorbing = new TRevetment("TotAbsorbing","Totally absorbing revetment");
//
// GEOMETRY 
//
//Now comes the definition of the geometry. The crystal is assumed to have the shape of
//a rectangular parallelipiped ( a TSBRIK ) with semi-axis crystal_dx/dy/dz. The long axis
//of the crystal is along Oz. The shape "crystal" is taken as the reference shape ( because
//of the code node1->cd(); ). So that the local coordinate system of the shape "crystal" is
//at the same time the world coordinate system. The shape "crystal" is made of CsITl and is
//covered with "vm2000", except for the parts of the shape which are in contact with other
//shapes. 
  const Double_t zero = 0.0;
  const Double_t crystal_dx = 0.5;  //half width of crystal in Ox direction (1cm)
  const Double_t crystal_dy = 0.5;  //half width of crystal in Oy direction (1cm)
  const Double_t crystal_dz = 5.0;  //half length of crystal axis, put here along Oz (for 10cm)
//
//teste com cilindro
//  Double_t cryst_r = 1.0/2.0;  // Crystal radius
//  TSCYL* crystal = new TSCYL("crystal","crystal","CsITl","none",cryst_r,crystal_dz);
//
// We define the Glue and Epoxy position
  const Double_t glue_dz    = 0.0025; //half width of glue between crystal and APD [Montecchi] in Oz
  const Double_t epoxy_dz   = 0.05;   //half width of epoxy window (1mm) in Oz.
  Double_t glue_z  = crystal_dz + glue_dz; //position z center of glue for APD. 
  Double_t epoxy_z = crystal_dz + 2*glue_dz + epoxy_dz;  //position z center of epoxy window for APD. 
//
// Definition of the position for APD
  const Double_t apd_dx  = 0.5;       //half-width of APDs (1cm) in direction Ox (to BR) 
  const Double_t apd_dy  = 0.5;       //half-width of APds (1cm) in direction Oy (to CR) 
//const Double_t apd_dz  = 0.00025;    //half-width of silicium of APD - 50 microns 
  const Double_t apd_dz  = 0.00225;    //half-width of silicium of APD - 450 microns 
  cout<< ""<< endl;
  cout<< "APD - 450 microns"<< endl;
  cout<< ""<< endl;
  Double_t apd_x  = zero;             //position center in x of APD.
  Double_t apd_y  = zero;             //position center in y of APD.
  Double_t apd_z;                     //position center in z of APD.
  apd_z =  crystal_dz + 2*glue_dz + 2*epoxy_dz + apd_dz;
//
//
//Definition of the crystal shape 
  TSBRIK *crystal;
  crystal = new TSBRIK("crystal","crystal","CsITl","vm2000",
		     crystal_dx,crystal_dy,crystal_dz);
  crystal->SetLineColor(106);
// 
// Definition of the glue shape for APD
  TSBRIK *glue_apd;
  glue_apd = new TSBRIK("glue_apd","glue_pad","Glue","TotAbsorbing",apd_dx,apd_dy,glue_dz);
  glue_apd->SetLineColor(2);
  glue_apd->SetLineWidth(2);
//
// Definition of the epoxy window shape for APD
  TSBRIK *epoxy_apd;
  epoxy_apd = new TSBRIK("epoxy_apd","epoxy_apd","Epoxy","TotAbsorbing",apd_dx,apd_dy,epoxy_dz);
  epoxy_apd->SetLineColor(4);
  epoxy_apd->SetLineWidth(2);
//
//  APD
  TSBRIK *apd;
  apd = new TSBRIK("apd","apd","Silicium","TotAbsorbing",apd_dx,apd_dy,apd_dz);
//apd->fSuppl->SetAPD("hamamatsu1","hamamatsu1",4,5,"GainProfile_CMSAPD"); //CMS APD
  apd->fSuppl->SetAPD("hamamatsu1","hamamatsu1",4,5,"GainProfile_USCAPD"); //USC APD
  apd->SetLineColor(104);
  apd->SetLineWidth(2);
//
//
//TSNode gives the position of a shape with respect to the node currently selected, by
//mean of a translation vector and a rotation matrix. No node being actually selected,
//node1 will be the main node and will define the WCS (world coordinate system). As no
//translation vector is given, the crystal will not be translated (translation vector 0
//assumed). As no rotation matrix is given, the crystal will not be rotated and will
//remain with its axis parallel to Oz. node1, the crystal, is selected and will remain the
//reference for all other nodes. For all other nodes, the proper translation vector is
//given. No shape is rotated, no rotation matrix is given.
  TSNode *node1 = new TSNode("node1","node1",crystal);
  node1->SetLineColor(1);
  node1->SetLineWidth(2);
  node1->cd();
//glue
  TSNode *nodeglue = new TSNode("nodeglue","node for glue",  glue_apd,apd_x,apd_y,glue_z);
//window
  TSNode *nodewin  = new TSNode("nodewin","node for window",epoxy_apd,apd_x,apd_y,epoxy_z);
//APD
  TSNode *nodeAPD  = new TSNode("nodeAPD","node for APD",         apd,apd_x,apd_y,apd_z);
//
//  
//  Definition of the contact crystal-glue
  TContact *contact_cg; 
  contact_cg  =new TContact("CsITl_Glue","CsITl_Glue","crystal","glue_apd",containing);
//  Definition of the contact glue-window(epoxy) 
  TContact *contact_gw;
  contact_gw  =new TContact("Glue_EpoxyWindow","Glue_EpoxyWindow","glue_apd","epoxy_apd",identical);
//  Definition of the contact window-APD
  TContact *contact_wapd;
  contact_wapd=new TContact("EpoyxWindow_APD","EpoxyWindow_APD","epoxy_apd","apd",identical);
//
// Generate events
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
//
//
//TPhotoElecCompton(Text_t* name, Text_t* title, Text_t* namesh, Double_t E0, T3Vector sourceposition)
 
//  A non-collimated (isotropic) source

//  PARAMETERS OF THE CONSTRUCTOR :

// -  Text_t*  name   : Name of the TPhotoElecCompton.
// -  Text_t*  title  : Title of the TPhotoElecCompton.
// -  Text_t*  namesh : name of the shape used as reference for giving the
//                      coordinnates of the incoming Gamma.
// -  Double_t E0     : energy in MeV of the incoming Gamma.
// -  T3Vector sp     : Source Position of the Gamma. IN THE LOCAL COORDINATES
//                      of the shape "namesh". [cm]
//
//
  source = new TPhotoElecCompton("Source","Source","crystal",gammaE,StartCasc,AxisCasc);
//
// Results
//
  TH1F* spectrum = new TH1F("spectrum", "spectrum", nBins, 0, nBins*1.0);
//
// Runs
//
  for (Int_t i=0; i<nRuns; ++i){
  cout << "Run number: " << i << " " << endl;
  source->Gen(101+i,4);
  // PMT spectrum
  if (gCs->fNpSeen) spectrum->Fill(gCs->fNpSeen);
  // Generation of photons
}
//
//
// Drawing
//
  gLit->BookCanvas();
  gTwoPad->SelectPad2();
  node1->Draw();
  gTwoPad->ChangePad();
//  Here the generation of photons is finished. It remains to analyze the results.
//For that, you have at hand the 2 classes TResults and TPublication, and all the
//power of ROOT! Look at the web description of the classes Tresults and
//TPublication where you will find a full description of all types of histograms
//available at the end of a production of Litrani. You will learn there that
//for instance gGp is a pointer towards the summary histograms of Litrani. Giving
//to the title of the class TPublication pointed to by gGp "z coordinate of beam"
//will have for result that all the summary histograms will contain this in their
//title. For instance, the histogram on efficiency that we plot here will have as
//title : "Efficiency versus z coordinate of beam".
  gGp->SetTitle("z coordinate of beam");
//Summary() creates all summary histograms
  gGp->Summary();
//Draws one of the available histogram.
  gStyle->SetOptFit(1111);
  gGs->DoStat();
  gLit->CloseFiles();
}

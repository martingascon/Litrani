{
  char *name    = "LOvsSize";										// name     name of the produced ROOT file (without ".root")
  char *listing = "LOvsSize ";										// listing  name of the listing file (without ".lis")
  char *upcom   = "L.O. study inside a variable thickness CsI(Tl) crystal";				// upcom    name of the comment appearing on top    left of canvas
  char *downcom = "Beam random inside the crystal, Detector is a phototube";				// downcom  name of the comment appearing on bottom left of canvas
  gROOT->ProcessLine(".x InitLitrani.C(8,name,listing,upcom,downcom,kFALSE,kFALSE,kTRUE)");		
//(Canvas_size=0-8,..,kFalse(true=random No at each new launching, kFalse (true if one wants to run Litrani2 in debug mode),kTrue (appends date & time to name of root file and lis)

  gLit->SetPrintFreq(50000);


// gamma source
  const Double_t pideg    = 180.0;
  const Int_t    nRuns    = 11;
  const Double_t gammaE   = 1.0;
  const Double_t nPhotMeV = 10000;

  TResults::fgEnergyOfGamma = gammaE;
  Int_t i;
  TPhotoElecCompton *source;
  Int_t nBins = (Int_t)(gammaE*nPhotMeV/2.0);


// Materials
  TOpticMaterial *air;
  air = new TOpticMaterial("Air","Air",kFALSE,1.0,10000.0);			//  Material "Air" is declared, with magnetic permeability fixed at 1.0, absorption length
  air->IsIsotropic(1.0);							//fixed at 10000 cm, index of refraction fixed at 1.0. Nothing depends upon wavelength.
 
  TOpticMaterial *CsITl;
  CsITl = new TOpticMaterial("CsITl","CsITl",kFALSE,1.0,"AbsorptionLength_CsITl");
  CsITl->IsIsotropic("RefrIndex_CsITl");
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
  CsITl->dEdx("DEdx_MusInCsITlNoL",1000.0,kFALSE);

//  Definition of the material sodocal, the window of the Phototube. It has an absorption
//length of 1000 cm (we have not been able to find the true values for the absorption
//length of sodocal), a magnetic permeability of 1.0. It is optically isotropic, with an
//index of refraction varying as a function of wavelength by the fit "RefrIndex_Sodocal"
  TOpticMaterial *sodocal;
  sodocal = new TOpticMaterial("Sodocal","Sodocal",kFALSE,1.0,1000.0);
  sodocal->IsIsotropic("RefrIndex_Sodocal");

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
  
  const Double_t PropDiffus = 0.98;
  TRevetment *tyvek;
  tyvek = new TRevetment("tyvek","tyvek","Air",PropDiffus,"RIndexRev_Tyvek", "IIndexRev_Tyvek",1.0);
  
  TRevetment *aluminium = new TRevetment("Aluminium", "Aluminium", "none", 0.0,"RIndexRev_Aluminium","IIndexRev_Aluminium",1.0);
//
//  Geometry
//

//  Now comes the definition of the geometry. The crystal is assumed to have the shape of
//a rectangular parallelipiped ( a TSBRIK ) with semi-axis crystal_dx/dy/dz. The long axis
//of the crystal is along Oz. The shape "crystal" is taken as the reference shape ( because
//of the code node1->cd(); ). So that the local coordinate system of the shape "crystal" is
//at the same time the world coordinate system. The shape "crystal" is made of CsITl and is
//covered with "tyvek", except for the parts of the shape which are in contact with other
//shapes.
  const Double_t zero = 0.0;
  const Double_t crystal_dx = 0.5;  //half width of crystal in Ox direction
  const Double_t crystal_dy = 0.5;  //half width of crystal in Oy direction
  const Double_t crystal_dz = 0.5;  //half length of crystal axis, put here along Oz
 
  const Double_t tube_r     = 2.2;  //radius for both phototubes
  const Double_t tube_dz    = 0.2;  //half width of both phototube windows in Oz direction.
  Double_t tube1_z   = crystal_dz + tube_dz;


//  TSCYL describing the window of the phototube.
  const Double_t tube_r   = 2.2; //radius of the phototube
  const Double_t tube_dz  = 0.2; //half height of the cylinder building the window of PM
//

  Double_t tube_z;              		 //translation in z of phototube
  tube_z  =   crystal_dz + tube_dz;

// the shape "crystal" is brick and is covered by air
  TSBRIK *crystal;
  crystal = new TSBRIK("crystal","CsI(Tl) crystal","CsITl","tyvek",crystal_dx,crystal_dy,crystal_dz);

//Definition of the window of the 2 PMs
  TSCYL *pm;
  pm = new TSCYL("Phototube","Phototube","Sodocal","TotAbsorbing",tube_r,tube_dz);
//Declares that the shape pm is a phototube, that the upper face (1) is the photocathode,
  pm->fSuppl->SetPhotoTube("XP2020","Phototube Phillips XP2020",1,"QuantumEff_XP2020");
  pm->SetLineColor(28);
  pm->SetLineWidth(2);

//  TSNode gives the position of a shape with respect to the node currently
//selected, by mean of a translation vector and a rotation matrix. No node
//being actually selected, node1 will be the main node and will define the
//WCS (world coordinate system). As no translation vector is given, the
//crystal will not be translated (translation vector 0 assumed). As no
//rotation matrix is given, the crystal will not be rotated and will remain
//with its axis parallel to Oz. Node1, the crystal, is selected and will remain the
//reference for all other nodes.
  TSNode *node1;
  node1 = new TSNode("node1","node1",crystal);
  node1->SetLineColor(1);
  node1->SetLineWidth(2);  
  node1->cd();

//  Node4 will give the position of the pm shape with respect to the node
//currently selected, the crystal node. A translation vector (0,0,tube_z) will
//put the phototube shape exactly above the air gap. No rotation matrix being given, the
//shape will stay with its axis parallel to the Oz axis.
  TSNode *node2;
  node2 = new TSNode("node2","node2",pm,zero,zero,tube_z);


//  There is a contact between crystal shape and the pm shape. 
  TContact *contact12;
  contact12 = new TContact("crys-pm","crys-pm","crystal","Phototube",containing);


//
  TH1F* spectrum = new TH1F("spectrum", "spectrum", nBins, 0, nBins*1.0);
//
//Generate events
//
  const T3Vector StartGamma(0.0, 0.0, - crystal_dz - 0.5);
  T3Vector AxisGamma;
  Double_t ThetaDeg,Theta0,sTheta0,cTheta0;
  Double_t Phi0,sPhi0,cPhi0;
  Phi0 = 0.0;
  sPhi0 = TMath::Sin(Phi0);
  cPhi0 = TMath::Cos(Phi0);
  ThetaDeg = -50.0;
  Theta0 = (ThetaDeg*TMath::Pi())/pideg;
  sTheta0 = TMath::Sin(Theta0);
  cTheta0 = TMath::Cos(Theta0);
  AxisGamma.Set(sTheta0*cPhi0,sTheta0*sPhi0,cTheta0);
  source = new TPhotoElecCompton("Source","Source","crystal",gammaE,StartGamma,AxisGamma);
  //
  // Runs
  //
//  for (i=1; i<=nRuns; ++i) {
//    source->Gen(i,4,ThetaDeg);
 //   ThetaDeg += 10.0;
 //   Theta0 = (ThetaDeg*TMath::Pi())/pideg;
 //   sTheta0 = TMath::Sin(Theta0);
 //   cTheta0 = TMath::Cos(Theta0);
 //   AxisGamma.Set(sTheta0*cPhi0,sTheta0*sPhi0,cTheta0);
 //   source->SetGammaDirection(AxisGamma);
 //   if (gCs->fNpSeen) spectrum->Fill(gCs->fNpSeen);
  }

  //
  // Drawing
  //
  gLit->BookCanvas();
  gTwoPad->SelectPad2();
  cnode->Draw();
  gTwoPad->ChangePad();
  spectrum->Draw();
  gGp->SetTitle("Incident angle of gamma in degree");
  gGp->Summary();
  gGs->DoStat();
  gLit->CloseFiles();


}

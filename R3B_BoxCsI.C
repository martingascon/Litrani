{
  char *name    = "R3B_BoxCsI";
  char *listing = "R3B_BoxCsI";
  char *upcom   = "Transmission study inside a Box (TSBRIK)";
  char *downcom = "Source of light: laser. Generic detector";
  gROOT->ProcessLine(".x InitLitrani.C(5,name,listing,upcom,downcom,kFALSE,kFALSE,kTRUE)");
//
//  
//  R3B_BoxCsI based on Transm1.C
//
// arguments of InitLitrani:
//
// size     size of main canvas of TwoPadDisplay [ 1 <= size <= 8 ]
// name     name of the produced ROOT file (without ".root")
// listing  name of the listing file (without ".lis")
// upcom    name of the comment appearing on top    left of canvas
// downcom  name of the comment appearing on bottom left of canvas
// otherseq true if one wants an other sequence of random numbers at each new launching
// indebug  true if one wants to run Litrani in debug mode
// WithDate if true, appends date and time to name of root file and name of listing
//
// The over-simplification concerns the following points :
//
//  (1) - The beam is issued from a point, not from a surface
//  (2) - The aperture of the Beam is 0
//  (3) - The detector is perfect, without quantum efficiency or limitation in acceptance.
//  (4) - CsI(Tl) having the shape of a rectangular parallelipiped. In this example, 
//        the material CsI(Tl) isoptically isotropic.Material CsI is treated as isotropic 
//        and not negative birefringent.
//
//  For a more realistic example, look at R3B_BoxCsI_APD.C or R3B_BoxCsI_APD_SourceGamma.C.
//  The setup is composed of a crystal of CsI, of shape TSBRIK, on top of an other TSBRIK
//made of air and containing a source of light, and below an other TSBRIK also made of air
//and whose upper face is a perfect detector.
//
//  Following fit is used in this macro:
//    - "AbsorptionLength_CsITl"
//  This fit has been declared nowhere! So it will be searched and found in the "database"
//    SplineFitDB.rdb
//
//  To know how to define your own fits, please read the documentation about SplineFit at:
//
//  ---> http://gentit.home.cern.ch/gentit/splinefit/
//
//  BEGIN of code
//
  const Double_t un   = 1.0;
  const Double_t deux = 2.0;
//
//  Material "Air" is declared, with magnetic permeability fixed at 1.0, absorption length
//fixed at 10000 cm, index of refraction fixed at 1.0. Nothing depends upon wavelength.
  TOpticMaterial *air;
  air = new TOpticMaterial("Air","Air",kFALSE,1.0,10000.0);
  air->IsIsotropic(1.0);
//
//
//  Definition of silicium, defined as sensible ( i.e. if a photon dies inside, it
//is considered as detected), with magnetic permeability of 1 and absorption length
//depending upon wavelength according to the fit "AbsLSilicium". Silicium is isotropic
//with index of refraction depending  upon wavelength according to the fit
//"Refr_I_Silicium".
  TOpticMaterial *Silicium;
  Silicium = new TOpticMaterial("Silicium","Silicium of APD",kTRUE,un,"AbsorptionLength_Silicium");
  Silicium->IsIsotropic("RefrIndex_Silicium");
//
//
//  Definition of the thin slice of nitrure being the window of the APD. It has a
//thickness of 125 nm and has an index of refraction varying with wavelength accor-
//ding to the fit "RefrNitrure". Look on the web at the description of class
//ThinSlice for more details.
  ThinSlice *tsAPD;// this is not used
  tsAPD = new ThinSlice("WindowAPD","WindowAPD",125.0,"RefrIndex_Nitrure");
//
//
// Definition of the material CsITl
//  CsITl is slow, so it is a good idea to change the default value of TooLate. According
//to Arache Djannati of the GLAST experiment, the time of the gate of the GLAST experiment
//is 2500 ns, which is quite short for CsI(Tl) ! There will be many photons killed because
//too late !
  //gLitPhys->SetLate(2500);
//  Definition of the material CsITl. The magnetic permeability is declared to be 1, not
//depending upon wavelength. The absorpion length is given by the fit "AbsorptionLength_CsITl" in the
//file f_abslcsitl.C. CsITl is declared as optically isotropic, the fits giving the
//variation of the index of refraction as a function of wavelength is "RefrIndex_CsITl",
//defined in the file f_refricsitl.C.
// The fluorescent characteristics of CsI are taken from :
//     Measurements done by Remi Chipaux from Saclay
//     Measurements done by John E. Eldridge
// Please choose the one you want.
//  A call to dEdx indicates that CsITl is able to emit light when crossed by particles.
//The interpolation "DEdx_MusInCsITlNoL" gives the value of dE/dx [Mev] deposited by a muon
//as a function of its energy [Gev]. There are 50000 photons produced per Mev of deposited
//energy. We do not allow Landau fluctuations.
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
//parametros de los picos:
//TLitSpectrum::AddOneComponent(Double_t m, Double_t s, Double_t A,Double_t td1, Double_t tr1, 
//                              Double_t w1,Double_t td2, Double_t tr2, Double_t w2)
// m : mean of the gaussian component 
// s : sigma of the gaussian component 
// A : weight to be affected to this component. 
// td1 : 1st decay time in ns 
// tr1 : 1st rise time in ns. Put -1.0 if no rise time 
// w1 : weight of this time pair (td1,tr1) component. 
// td2 : 2nd decay time in ns 
// tr2 : 2nd rise time in ns. Put -1.0 if no rise time 
// w2 : weight of this time pair (td2,tr2) component.
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
//  CsITl->dEdx("DEdx_MusInCsITlNoL",1000.0,kFALSE);
  CsITl->dEdx("DEdx_GammaInCsITl",1000.0,kFALSE);// for gammas, it works
//
//
//  Definition of the wrapping "Revcaps", describing the surface of the capsule containing
//the APD, which is in contact with face 5 of the crystal.
  TRevetment *revcaps;
  revcaps  = new TRevetment("Revcaps","Revcaps","Glue",un,"RIndexRev_CMScapsula",
                            "IIndexRev_CMScapsula",un,0.2,85.0);
//
//  Definition of the wrapping "Revtopbot", describing the surface
//    - of the top of the alveola, which is the wrapping of the part of face 5 of the
//       crystal which is not covered with the APD capsule.
//    - of the bottom of the alveola, which is the wrapping of the part of face 4 of the
//crystal which is not covered with the "ferule".
//  The fact that we wrongly refer to "Tyvek" does not matter, since we have put a
//supplementary absorption of 0.99. But we have not put a supplementary absorption of 1.0,
//since we insist that there is a slice of air and that total reflection is possible.
  TRevetment *revtopbot;//"Tyvek" i have to change for vm2000
  revtopbot  = new TRevetment("Revtopbot","Revtopbot","Air",un,"RIndexRev_Tyvek",
                            "IIndexRev_Tyvek",un,0.99,80.0);
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
//  Geometry
//
  const Double_t zero     = 0.0;
  const Double_t crys_dx  = 0.5;   //half length in x of crystal of CsITl
  const Double_t crys_dy  = 0.5;   //half length in y of crystal of CsITl
  const Double_t crys_dz  = 5.0;   //half length in z of crystal of CsITl
  const Double_t laser_dz = 1.0;   //half length in z of TSBRIK with light source
  const Double_t detec_dz = 6.0;   //half length in z of TSBRIK with surface detector
  Double_t laser_z;                //translation in z of TSBRIK with light source
  Double_t detec_z;                //translation in z of TSBRIK with surface detector
  laser_z = - crys_dz - laser_dz;
  detec_z =   crys_dz + detec_dz;
//
//  The crystal is assumed to have the shape of a rectangular parallelipiped ( a TSBRIK )
//with semi-axis crys_dx, crys_dy, crys_dz. The long axis of the crystal is along Oz. The
//shape "crystal" is taken as the reference shape ( because of the code node1->cd(); ).
//So that the local coordinate system of the shape "crystal" is at the same time the world
//coordinate system WCS. The shape "crystal" is made of PbWO4 and is not covered by any
//revetment.
  TSBRIK *crystal;
  crystal    = new TSBRIK("crystal","crystal","CsITl","none",crys_dx,crys_dy,crys_dz);
  crystal->SetLineColor(1);
  crystal->SetLineWidth(2);
//
//  The shape containing the source of light is assumed to be a TSBRIK. It is made of Air,
//and covered with a totally absorbing revetment, except for the face in contact with the
//crystal.
  TSBRIK *laserbrick;
  laserbrick = new TSBRIK("laserbrick","laserbrick","Air","TotAbsorbing",
                           crys_dx,crys_dy,laser_dz);
  laserbrick->SetLineColor(2);
  laserbrick->SetLineWidth(2);
//

//  The shape containing the surface detector is assumed to be a TSBRIK. It is made of Air,
//and covered with a totally absorbing revetment, except for the face in contact with the
//crystal.
  TSBRIK *detecbrick;
  detecbrick = new TSBRIK("detecbrick","Brik with the detector","Air","TotAbsorbing",
                           crys_dx,crys_dy,detec_dz);
  detecbrick->SetLineColor(4);
  detecbrick->SetLineWidth(2);
//declares that the upper face (5) of detecbrick is a perfect detector, with quantum effi-
//ciency of 1 and acceptance angle of 90 degrees. Look at the web documentation about the
//TSBRIK and TSupplShape to understand the numbering of the faces of a TSBRIK, and the
//appearance of the pointer fSuppl here.
  detecbrick->fSuppl->SetSurfDet("perfect","perfect detector",5);

//TSNode gives the position of a shape with respect to the node currently
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
//
//node2 will give the position of the laserbrick shape with respect to the node
//currently selected, the crystal node. A translation vector (0,0,laser_z) will
//put the shape exactly under the crystal. No rotation matrix being given, the laserbrick
//shape will stay with its axis parallel to the Oz axis.
  TSNode *node2;
  node2 = new TSNode("node2","node2",laserbrick,zero,zero,laser_z);
//
//node3 will give the position of the detecbrick shape with respect to the node
//currently selected, the crystal node. A translation vector (0,0,detec_z) will
//put the detecbrick shape exactly above the crystal. No rotation matrix being given, the
//shape will stay with its axis parallel to the Oz axis.
  TSNode *node3;
  node3 = new TSNode("node3","node3",detecbrick,zero,zero,detec_z);
//
//  There is a contact between crystal shape and laserbrick shape. Notice this nice
//property that you do not have to tell which face of crystal is in contact with which
//face of laserbrick: Litrani will find it for you. In fact, it is face 4 of crystal which
//is in contact with face 5 of laserbrick. Look at the web documentation of TSBRIK to
//understand how faces are numbered for a TSBRIK. The face of crystal in contact with the
//face of laserbrick are identical hence "identical".
  TContact *contact12;
  contact12 = new TContact("crystal-laser","crystal-laser","crystal","laserbrick",
                            identical);
//
//  There is a contact between crystal shape and detecbrick shape. Notice this nice
//property that you do not have to tell which face of crystal is in contact with which
//face of laserbrick: Litrani will find it for you. In fact, it is face 5 of crystal which
//is in contact with face 4 of detecbrick.
  TContact *contact13;
  contact13 = new TContact("crystal-detec","crystal-detec","crystal","detecbrick",
                            identical);
// End of geometry
  const Double_t WvlgthStart = 300.0;  //starting value for wavelength
  const Double_t WvlgthStep  = 25.0;   //step in wavelength
  Double_t wavelength = WvlgthStart;
//
//  Declaration of a spontaneous source of light inside the shape "laserbrick", at point
//(0,0,0) in the local coordinate system of the laserbrick, i.e. exactly in the middle
//of the laserbrick. This spontaneous source will emit light at the wavelength "wavelength".
  TSpontan *laser;
  laser = new TSpontan("laser","laser","laserbrick",zero,zero,zero,wavelength);
//
//  DefineCradle() is called here with only the 1st 2 arguments :
//
//    (1) - "laserbrick"  : name of shape in which the cradle of the photons is
//    (2) - point         : kind of cradle : {point, disk, ellipse, parallelogram,
//                                            parallelipiped}
//  The remaining 3->9 arguments have the following default values :
//
//    (3) - a1     = 1.0 : Irrelevant, because of (2).
//    (4) - a2     = 1.0 : Irrelevant, because of (2).
//    (5) - a3     = 1.0 : Irrelevant, because of (2).
//    (6) - theta3 = 0  : angle theta (degree, in LCS) of the Oz axis of the beam
//    (7) - phi3   = 0  : angle phi   (degree, in LCS) of the Oz axis of the beam
//    (8) - theta1 = 90 : angle theta (degree, in LCS) of the Ox axis of the beam
//    (9) - phi1   = 0  : angle phi   (degree, in LCS) of the Ox axis of the beam
//
//  Because the beam cradle is a point, the arguments 3,4,5 of DefineCradle are irrelevant.
//The default values of the arguments 6 and 7 are 0.0 and 0.0, which is exactly what we
//want, since we want a beam parallel to the Oz axis. The beam cradle being a point, the
//complementary axis Ox of the beam coordinate system, is irrelevant and we can leave the
//default values for the arguments (8) and (9). But notice that we have to call
//DefineCradle(), because by default in the constructor of TSpontan, the emission of the
//beam is of the type "on4pi", for which the axis of the beam coordinate system are irre-
//levant, but we want here an emission of the beam of type "flat", for which the direction
//of the beam coordinate system matters ! So this call to DefineCradle is there essentially
//to insure that the direction of the beam is along the Oz axis! Notice that when the
//beam cradle is a point, THE AXIS 
// - {fAxis1, fAxis2, fAxis3 }, coordinate system of the beam cradle shape (disk, ellipse,
//                              parallelogram, parallelipiped} and THE AXIS
// - {fAxisb1,fAxisb2,fAxisb3}, coordinate system of the beam, ARE IDENTICAL.
  laser->DefineCradle("laserbrick",point);
//
//  We want a flat distribution with aperture 0, which means that all photons will be
//emitted parallel to the Oz axis in the local coordinate system of laserbrick.
  laser->DefineBeam(flat,zero);
  for (Int_t i=0;i<21;i++) {
//
//  Generation of 10000 photons of wavelength "wavelength". A run number must be greater
//than 0, hence 100+i and not i !
    laser->Gen(100+i,10000,wavelength);
    wavelength += WvlgthStep;
//Changes the wavelength
    laser->SetWvlgth(wavelength);
  }
//  We book the canvas after the generation of photons is finished !
//  It is a good idea to make a drawing of the setup. Any geometrical mistake will
//appear as an awful drawing! Notice however that ROOT has a very strange and
//questionable way of drawing a setup : the proportion of the 3 axis are not
//respected by ROOT ! ROOT feels free to affect different factor to the 3 axis Ox,
//Oy and Oz ! So do not be surprised by the proportions !
// Booking of the display canvas. The global pointer gTwoPad points towards class
//TwoPadDisplay. Look at it.
  gLit->BookCanvas();
  node1->Draw();
  gTwoPad->ChangePad();
//  Here all runs of generation of photons are finished. It remains to analyze the results.
//For that, you have at hand the 2 classes TResults and TPublication, and all the
//power of ROOT ! Look at the web description of the classes Tresults and
//TPublication where you will find a full description of all types of histograms
//available at the end of a production of Litrani. You will learn there that
//for instance gGp is a pointer towards the summary histograms of Litrani. Naming
//the title of the class TPublication (pointed to by gGp) "wavelength of laser"
//will have for result that all the summary histograms will contain this in their
//title. For instance, the histogram on efficiency that we plot here will have as
//title : "Efficiency versus wavelength of laser ".
  gGp->SetTitle("wavelength of laser");
//Summary() creates all summary histograms
  gGp->Summary();
//Draws one of the available histogram, after having made a fit of a polynom of degree 7
  gGp->fHEfficiency->Fit("pol7","Q");
  gGs->DoStat();
  gLit->CloseFiles();
}

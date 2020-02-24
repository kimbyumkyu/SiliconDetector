#include "WFGUI.h"

WFGUI::WFGUI(const TGWindow *p,UInt_t w,UInt_t h): TGMainFrame(p,w,h), dwpot(Potentials(300,1000,300))
{
	Connect("CloseWindow()", "WFGUI", this, "CloseWindow()");  // connect exit-button of window to CloseWindow()

	SetLayoutManager(new TGHorizontalLayout(this));

	// Tabs
	PotentialTab = new TGTab(this,500,400);

	// Tab "Drift Potential" ///////////////////////////////////////////////////////////////////////
	DriftPTab = PotentialTab->AddTab("Drift Potential");
	DriftPTab->SetLayoutManager(new TGVerticalLayout(DriftPTab));

	driftcanvas = new TRootEmbeddedCanvas("DriftCanvas",DriftPTab,500,400);
	DriftPTab->AddFrame(driftcanvas, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY,1,1,1,1));

	LabelsTabFrame = new TGHorizontalFrame(DriftPTab, 500,20);
	DriftPTab->AddFrame(LabelsTabFrame, new TGLayoutHints(kLHintsExpandX,1,1,1,1));

	driftfcanvas = new TRootEmbeddedCanvas("driftfcanvas",DriftPTab,500,300);
	DriftPTab->AddFrame(driftfcanvas,new TGLayoutHints(kLHintsExpandY | kLHintsExpandX,1,1,1,1));

	//	FieldLabel = new TGLabel(LabelsTabFrame, new TGString("Field Cut :"));
	//	LabelsTabFrame->AddFrame(FieldLabel, new TGLayoutHints(kLHintsLeft,1,1,1,1));

	FieldCutGroup = new TGButtonGroup(LabelsTabFrame, "Field Cut", kHorizontalFrame);
	LabelsTabFrame->AddFrame(FieldCutGroup, new TGLayoutHints(kLHintsLeft,1,1,1,1));

	FieldCutButton[0] = new TGRadioButton(FieldCutGroup, new TGHotString("X cut"));
	FieldCutButton[1] = new TGRadioButton(FieldCutGroup, new TGHotString("Y cut"));    

	FieldCutGroup->AddFrame(FieldCutButton[0], new TGLayoutHints(kLHintsLeft,1,1,1,1));
	FieldCutGroup->AddFrame(FieldCutButton[1], new TGLayoutHints(kLHintsLeft,1,1,1,1));
	FieldCutButton[0]->Connect("Toggled(Bool_t)", "WFGUI", this, "CallFieldCut()");
	FieldCutButton[1]->Connect("Toggled(Bool_t)", "WFGUI", this, "CallFieldCut()");

	WhereCut = new TGNumberEntry(LabelsTabFrame, (Double_t) 0,8,-1,TGNumberFormat::kNESInteger,TGNumberFormat::kNEAAnyNumber,TGNumberFormat::kNELLimitMinMax);
	LabelsTabFrame->AddFrame(WhereCut, new TGLayoutHints(kLHintsLeft,5,1,1,1));

	EtotButton = new TGTextButton(LabelsTabFrame,new TGHotString("|Etot|"));
	LabelsTabFrame->AddFrame(EtotButton, new TGLayoutHints(kLHintsLeft,5,1,1,1));
	EtotButton->SetEnabled(kFALSE);
	ExButton = new TGTextButton(LabelsTabFrame,new TGHotString("|Ex|"));
	LabelsTabFrame->AddFrame(ExButton, new TGLayoutHints(kLHintsLeft,5,1,1,1));
	ExButton->SetEnabled(kFALSE);
	EyButton = new TGTextButton(LabelsTabFrame,new TGHotString("|Ey|"));
	LabelsTabFrame->AddFrame(EyButton, new TGLayoutHints(kLHintsLeft,5,1,1,1));
	EyButton->SetEnabled(kFALSE);

	AddFrame(PotentialTab,new TGLayoutHints(kLHintsExpandX | kLHintsExpandY,2,2,2,2));

	PotentialTab->SetTab(0);

	// Tab "Currents" //////////////////////////////////////////////////////////////////////////////	
	CurrentsTab = PotentialTab->AddTab("Currents");
	CurrentsTab->SetLayoutManager(new TGVerticalLayout(CurrentsTab));

	currentscanvas = new TRootEmbeddedCanvas("CurrentsCanvas",CurrentsTab,500,500);
	CurrentsTab->AddFrame(currentscanvas,new TGLayoutHints(kLHintsExpandY | kLHintsExpandX,1,1,1,1));

	CurrentsInfoFrame = new TGVerticalFrame(CurrentsTab,100,200);
	CurrentsTab->AddFrame(CurrentsInfoFrame,new TGLayoutHints(kLHintsExpandY | kLHintsExpandX,1,1,1,1));

	ChargeCollectionFrame = new TGGroupFrame(CurrentsInfoFrame, "Charge Collection", kHorizontalFrame);

	QETextLabel =  new TGLabel(ChargeCollectionFrame, new TGString("e- Charges (e): "));
	ChargeCollectionFrame->AddFrame(QETextLabel, new TGLayoutHints(kLHintsExpandX,0,0,1,1));
	QELabel = new TGLabel(ChargeCollectionFrame, new TGString("0"));
	ChargeCollectionFrame->AddFrame(QELabel, new TGLayoutHints(kLHintsLeft | kLHintsTop,0,0,1,1));

	QHTextLabel =  new TGLabel(ChargeCollectionFrame, new TGString("h+ Charges (e): "));
	ChargeCollectionFrame->AddFrame(QHTextLabel, new TGLayoutHints(kLHintsExpandX,0,0,1,1));
	QHLabel = new TGLabel(ChargeCollectionFrame, new TGString("0"));
	ChargeCollectionFrame->AddFrame(QHLabel, new TGLayoutHints(kLHintsLeft | kLHintsTop,0,0,1,1));

	QTotTextLabel =  new TGLabel(ChargeCollectionFrame, new TGString("total Charges (e): "));
	ChargeCollectionFrame->AddFrame(QTotTextLabel, new TGLayoutHints(kLHintsExpandX,0,0,1,1));
	QTotLabel = new TGLabel(ChargeCollectionFrame, new TGString("0"));
	ChargeCollectionFrame->AddFrame(QTotLabel, new TGLayoutHints(kLHintsLeft | kLHintsTop,0,0,1,1));

	CurrentsInfoFrame->AddFrame(ChargeCollectionFrame,new TGLayoutHints(kLHintsExpandX,1,1,1,1));

	LorentzInfoFrame = new TGGroupFrame(CurrentsInfoFrame, "Lorentz Drift", kHorizontalFrame);

	LorentzeTextLabel =  new TGLabel(LorentzInfoFrame, new TGString("e- Lorentz Angle (degree): "));
	LorentzInfoFrame->AddFrame(LorentzeTextLabel, new TGLayoutHints(kLHintsExpandX,0,0,1,1));
	LorentzeLabel = new TGLabel(LorentzInfoFrame, new TGString("0"));
	LorentzInfoFrame->AddFrame(LorentzeLabel, new TGLayoutHints(kLHintsLeft | kLHintsTop,0,0,1,1));

	LorentzhTextLabel =  new TGLabel(LorentzInfoFrame, new TGString("h+ Lorentz Angle (degree): "));
	LorentzInfoFrame->AddFrame(LorentzhTextLabel, new TGLayoutHints(kLHintsExpandX,0,0,1,1));
	LorentzhLabel = new TGLabel(LorentzInfoFrame, new TGString("0"));
	LorentzInfoFrame->AddFrame(LorentzhLabel, new TGLayoutHints(kLHintsLeft | kLHintsTop,0,0,1,1));

	CurrentsInfoFrame->AddFrame(LorentzInfoFrame,new TGLayoutHints(kLHintsExpandX,1,1,1,1));


	// Add control frame ///////////////////////////////////////////////////////////////////////////
	SettingsFrame = new TGVerticalFrame(this,900,90);

	// Frame "Detector Properties"
	DetectorPropertiesFrame = new TGGroupFrame(SettingsFrame,"Detector Properties",kVerticalFrame);	

	DimensionsFrame = new TGGroupFrame(DetectorPropertiesFrame,"Dimensions",kHorizontalFrame);
	DimLabelFrame = new TGVerticalFrame(DimensionsFrame ,200,60);	
	DimSetFrame = new TGVerticalFrame(DimensionsFrame ,200,60);

	VoltageFrame  = new TGGroupFrame(DetectorPropertiesFrame,"Voltage",kHorizontalFrame);
	VolLabelFrame = new TGVerticalFrame(VoltageFrame ,50,90);
	VolSetFrame = new TGVerticalFrame(VoltageFrame ,50,90);

	ZcutFrame  = new TGGroupFrame(DetectorPropertiesFrame,"Zcut",kHorizontalFrame);
	ZcutLabelFrame = new TGVerticalFrame(ZcutFrame ,50,90);
	ZcutSetFrame = new TGVerticalFrame(ZcutFrame ,50,90);

	// Fill Dimensions Frame
	XMAXentry = new TGNumberEntry(DimSetFrame, (Double_t)150,8,-1,TGNumberFormat::kNESInteger,TGNumberFormat::kNEANonNegative);
	DimSetFrame->AddFrame(XMAXentry, new TGLayoutHints(kLHintsCenterX | kLHintsTop,2,2,2,2));				
	XMAXLabel = new TGLabel(DimLabelFrame,new TGString("Detector Width (um):"));				
	XMAXLabel->SetMargins(0,0,4,4);									
	DimLabelFrame->AddFrame(XMAXLabel, new TGLayoutHints(kLHintsLeft | kLHintsTop,0,0,1,1));

	YMAXentry = new TGNumberEntry(DimSetFrame, (Double_t) 100,8,-1,TGNumberFormat::kNESInteger,TGNumberFormat::kNEANonNegative);
	DimSetFrame->AddFrame(YMAXentry, new TGLayoutHints(kLHintsCenterX | kLHintsTop,2,2,2,2));				
	YMAXLabel = new TGLabel(DimLabelFrame,new TGString("Detector Height (um):"));					
	YMAXLabel->SetMargins(0,0,4,4);										
	DimLabelFrame->AddFrame(YMAXLabel, new TGLayoutHints(kLHintsLeft | kLHintsTop,0,0,1,1));	

	ZMAXentry = new TGNumberEntry(DimSetFrame, (Double_t) 100,8,-1,TGNumberFormat::kNESInteger,TGNumberFormat::kNEANonNegative);
	DimSetFrame->AddFrame(ZMAXentry, new TGLayoutHints(kLHintsCenterX | kLHintsTop,2,2,2,2));				
	ZMAXLabel = new TGLabel(DimLabelFrame,new TGString("Detector Length (um):"));				
	ZMAXLabel->SetMargins(0,0,4,4);									
	DimLabelFrame->AddFrame(ZMAXLabel, new TGLayoutHints(kLHintsLeft | kLHintsTop,0,0,1,1));

	Pitchentry = new TGNumberEntry(DimSetFrame, (Double_t) 30,8,-1,TGNumberFormat::kNESInteger,TGNumberFormat::kNEANonNegative);
	DimSetFrame->AddFrame(Pitchentry, new TGLayoutHints(kLHintsCenterX | kLHintsTop,2,2,2,2));				
	PitchLabel = new TGLabel(DimLabelFrame,new TGString("Pixel Pitch (um):"));					
	PitchLabel->SetMargins(0,0,4,4);									
	DimLabelFrame->AddFrame(PitchLabel, new TGLayoutHints(kLHintsLeft | kLHintsTop,0,0,1,1));	

	Widthentry = new TGNumberEntry(DimSetFrame, (Double_t) 10,8,-1,TGNumberFormat::kNESInteger,TGNumberFormat::kNEANonNegative);
	DimSetFrame->AddFrame(Widthentry, new TGLayoutHints(kLHintsCenterX | kLHintsTop,2,2,2,2));				
	WidthLabel = new TGLabel(DimLabelFrame,new TGString("Pixel Size (um):"));					
	WidthLabel->SetMargins(0,0,4,4);									
	DimLabelFrame->AddFrame(WidthLabel, new TGLayoutHints(kLHintsLeft | kLHintsTop,0,0,1,1));	

	// Fill VoltageFrame
	Biasentry = new TGNumberEntry(VolSetFrame, (Double_t) 3,8,-1,TGNumberFormat::kNESInteger,TGNumberFormat::kNEAAnyNumber);
	VolSetFrame->AddFrame(Biasentry, new TGLayoutHints(kLHintsCenterX | kLHintsTop,1,1,1,1));				
	BiasLabel = new TGLabel(VolLabelFrame,new TGString("Reverse Bias Voltage (V):"));
	BiasLabel->SetMargins(0,0,4,4);										
	VolLabelFrame->AddFrame(BiasLabel, new TGLayoutHints(kLHintsLeft | kLHintsTop,0,0,1,1));	
	Depletionentry = new TGNumberEntry(VolSetFrame, (Double_t) 1,8,-1,TGNumberFormat::kNESInteger,TGNumberFormat::kNEANonNegative);
	VolSetFrame->AddFrame(Depletionentry, new TGLayoutHints(kLHintsCenterX | kLHintsTop,1,1,1,1));				
	DepletionLabel = new TGLabel(VolLabelFrame,new TGString("Depletion Voltage (V):"));				
	DepletionLabel->SetMargins(0,0,4,4);									
	VolLabelFrame->AddFrame(DepletionLabel, new TGLayoutHints(kLHintsLeft | kLHintsTop,0,0,1,1));	

	// Fill ZcutFrame
	Zcutentry = new TGNumberEntry(ZcutSetFrame, (Double_t) 0,8,-1,TGNumberFormat::kNESInteger,TGNumberFormat::kNEAAnyNumber);
	ZcutSetFrame->AddFrame(Zcutentry, new TGLayoutHints(kLHintsCenterX | kLHintsTop,1,1,1,1));				
	ZcutLabel = new TGLabel(ZcutLabelFrame,new TGString("Zcut (um):"));
	ZcutLabel->SetMargins(0,0,4,4);										
	ZcutLabelFrame->AddFrame(ZcutLabel, new TGLayoutHints(kLHintsLeft | kLHintsTop,0,0,1,1));	

	//	VolLabelFrame->AddFrame(BiasLabel, new TGLayoutHints(kLHintsLeft | kLHintsTop,0,0,1,1));	
	SettingsFrame->AddFrame(DetectorPropertiesFrame, new TGLayoutHints(kLHintsCenterX,0,1,1,1));
	DetectorPropertiesFrame->AddFrame(DimensionsFrame ,new TGLayoutHints(kLHintsCenterY,0,1,1,1));
	DetectorPropertiesFrame->AddFrame(VoltageFrame ,new TGLayoutHints(kLHintsCenterY,0,1,1,1));
	DetectorPropertiesFrame->AddFrame(ZcutFrame ,new TGLayoutHints(kLHintsCenterY,0,1,1,1));
	DimensionsFrame->AddFrame(DimLabelFrame,new TGLayoutHints(kLHintsLeft,1,1,1,1));
	DimensionsFrame->AddFrame(DimSetFrame,new TGLayoutHints(kLHintsRight,1,1,1,1));	
	VoltageFrame->AddFrame(VolLabelFrame,new TGLayoutHints(kLHintsLeft,1,1,1,1));
	VoltageFrame->AddFrame(VolSetFrame,new TGLayoutHints(kLHintsRight,1,1,1,1));
	ZcutFrame->AddFrame(ZcutLabelFrame,new TGLayoutHints(kLHintsLeft,1,1,1,1));
	ZcutFrame->AddFrame(ZcutSetFrame,new TGLayoutHints(kLHintsRight,1,1,1,1));
	// Button "Calculate Potentials" 
	ButtonPotFrame = new TGHorizontalFrame(DetectorPropertiesFrame,50,100);
	CalcPotButton = new TGTextButton(ButtonPotFrame, " Calculate Potentials ");
	CalcPotButton->Connect("Clicked()","WFGUI",this,"ThreadstartPotential()");
	ButtonPotFrame->AddFrame(CalcPotButton, new TGLayoutHints(kLHintsCenterX,1,1,1,1));
	DetectorPropertiesFrame->AddFrame(ButtonPotFrame,new TGLayoutHints(kLHintsCenterX | kLHintsBottom,2,2,2,2));

	// Frame "Currents"
	CurrentsFrame = new TGGroupFrame(SettingsFrame,"Currents",kVerticalFrame);

	CarriersInFrame = new TGGroupFrame(CurrentsFrame,"Incident position of Particle",kHorizontalFrame);
	CarriersInLabelFrame = new TGVerticalFrame(CarriersInFrame,200,60);
	CarriersInSetFrame = new TGVerticalFrame(CarriersInFrame,200,60);

	CarriersInLabel = new TGLabel(CarriersInLabelFrame, new TGString("x-axis (nm): "));
	CarriersInLabel->SetMargins(0,0,4,4);
	CarriersInLabelFrame->AddFrame(CarriersInLabel, new TGLayoutHints(kLHintsLeft,2,2,2,2));
	CarriersInentry = new TGNumberEntry(CarriersInSetFrame, (Double_t) 0,8,-1,TGNumberFormat::kNESInteger,TGNumberFormat::kNEAAnyNumber/*,TGNumberFormat::kNELLimitMinMax*/);
	CarriersInSetFrame->AddFrame(CarriersInentry, new TGLayoutHints(kLHintsLeft|kLHintsExpandX,2,2,2,2));

	CarriersAngleLabel = new TGLabel(CarriersInLabelFrame, new TGString("Angle (degree): "));
	CarriersAngleLabel->SetMargins(0,0,4,4);
	CarriersInLabelFrame->AddFrame(CarriersAngleLabel, new TGLayoutHints(kLHintsLeft,2,2,2,2));
	CarriersAngleentry = new TGNumberEntry(CarriersInSetFrame, (Double_t) 0,8,-1,TGNumberFormat::kNESInteger,TGNumberFormat::kNEAAnyNumber/*,TGNumberFormat::kNELLimitMinMax*/);
	CarriersInSetFrame->AddFrame(CarriersAngleentry, new TGLayoutHints(kLHintsLeft|kLHintsExpandX,2,2,2,2));

	CurrentsFrame->AddFrame(CarriersInFrame, new TGLayoutHints(kLHintsExpandX,1,1,1,1));
	CarriersInFrame->AddFrame(CarriersInLabelFrame, new TGLayoutHints(kLHintsLeft,1,1,1,1));
	CarriersInFrame->AddFrame(CarriersInSetFrame, new TGLayoutHints(kLHintsLeft|kLHintsExpandX,1,1,1,1));

	CurrentsButton[0] = new TGCheckButton(CurrentsFrame,new TGHotString("Switch B-Field on and set to (T):"));
	CurrentsButton[1] = new TGCheckButton(CurrentsFrame,new TGHotString("Diffusion, set Temperature to (K):"));

	BfieldEntry = new TGNumberEntry(CurrentsFrame, (Double_t) 0,8,-1,TGNumberFormat::kNESReal,TGNumberFormat::kNEAAnyNumber);
	BfieldEntry->SetState(kFALSE);
	TempEntry = new TGNumberEntry(CurrentsFrame, (Double_t) 0,8,-1,TGNumberFormat::kNESReal,TGNumberFormat::kNEANonNegative);
	TempEntry->SetState(kFALSE);

	CurrentsButton[1]->Connect("Toggled(Bool_t)","WFGUI",this,"SetDiffusion(Bool_t)");
	CurrentsButton[0]->Connect("Toggled(Bool_t)","WFGUI",this,"SetBField(Bool_t)");

	CurrentsFrame->AddFrame(CurrentsButton[0], new TGLayoutHints(kLHintsLeft,0,40,1,5));
	CurrentsFrame->AddFrame(BfieldEntry, new TGLayoutHints(kLHintsLeft,25,40,1,5));
	CurrentsFrame->AddFrame(CurrentsButton[1], new TGLayoutHints(kLHintsLeft,0,1,1,5));
	CurrentsFrame->AddFrame(TempEntry, new TGLayoutHints(kLHintsLeft,25,40,1,5));


	CalcCurButton = new TGTextButton(CurrentsFrame," Calculate Currents ");
	CalcCurButton->Connect("Clicked()","WFGUI",this,"ThreadstartCurrents()");
	CalcCurButton->SetEnabled(kFALSE);
	CurrentsFrame->AddFrame(CalcCurButton, new TGLayoutHints(kLHintsCenterX,1,1,1,1));

	SettingsFrame->AddFrame(CurrentsFrame, new TGLayoutHints(kLHintsCenterX,1,1,1,1));

	// Button "Exit"
	ExitButton = new TGTextButton(SettingsFrame," Exit ");	
	SettingsFrame->AddFrame(ExitButton,  new TGLayoutHints(kLHintsCenterX | kLHintsBottom,1,1,1,1));
	ExitButton->Associate(this);
	ExitButton->SetCommand("gApplication->Terminate(0)");

	AddFrame(SettingsFrame,new TGLayoutHints(kLHintsRight,2,2,2,2)); 


	////////////////////////////////////////////////////////////////////////////////////////////////	
	canvasp = driftcanvas->GetCanvas();
	canvasp->SetRightMargin(0.1);
	canvasp->SetLeftMargin(0.12);
	canvasp->SetTopMargin(0.08);
	canvasp->SetBottomMargin(0.12);

	canvaspf = driftfcanvas->GetCanvas();
	canvaspf->SetRightMargin(0.03);
	canvaspf->SetLeftMargin(0.12);
	canvaspf->SetTopMargin(0.08);
	canvaspf->SetBottomMargin(0.12);

	curcanvas = currentscanvas->GetCanvas();
	curcanvas->SetRightMargin(0.03);
	curcanvas->SetLeftMargin(0.14);
	curcanvas->SetTopMargin(0.08);
	curcanvas->SetBottomMargin(0.12);

	// Create histograms for potentials and fields
	dhist = new TH2F("dhist"," ; x (um); y (um)",dwpot.GetXMAX(),-dwpot.GetXMAX()*0.5,dwpot.GetXMAX()*0.5,dwpot.GetYMAX(),-dwpot.GetYMAX()*0.5,dwpot.GetYMAX()*0.5);  
	dfhist = new TH2F("dfhist"," ",dwpot.GetXMAX(),-dwpot.GetXMAX()*0.5,dwpot.GetXMAX()*0.5,dwpot.GetYMAX(),-dwpot.GetYMAX()*0.5,dwpot.GetYMAX()*0.5);
	dchist = new TH2F("dchist"," ",dwpot.GetXMAX(),-dwpot.GetXMAX()*0.5,dwpot.GetXMAX()*0.5,dwpot.GetYMAX(),-dwpot.GetYMAX()*0.5,dwpot.GetYMAX()*0.5);

	dhist->SetTitleOffset(1, "Y");
	dhist->SetTitleOffset(1, "X");
	dhist->GetXaxis()->SetTitleSize(0.05);
	dhist->GetYaxis()->SetTitleSize(0.05);
	dhist->GetXaxis()->SetLabelSize(0.05);
	dhist->GetYaxis()->SetLabelSize(0.05);
	dhist->SetStats(0);

	TGaxis::SetMaxDigits(3);

	MapSubwindows();
	SetWindowName("Simulation of a Silicon Pixel Detector");
	Resize(GetDefaultSize());
	MapWindow();
}
/////////////////////////////////////////////////////////////////////////////////////////////
void WFGUI::CallCalculatePotentials() 
{
	CalcCurButton->SetEnabled(kFALSE);

	canvasp->Clear();
	canvasp->Update();

	dhist->Reset();

	dwpot.SetV(Biasentry->GetNumber(),Depletionentry->GetNumber());
	dwpot.SetPitchWidthXY(YMAXentry->GetNumber(),XMAXentry->GetNumber(),ZMAXentry->GetNumber(),Pitchentry->GetNumber(),Widthentry->GetNumber(),Zcutentry->GetNumber());
	dwpot.SetBoundaryConditions();

	dwpot.Multigrid(this); // calculate potentials

	//PopUp();
	//FieldCutButton->SetEnabled(kTRUE);
	CalcCurButton->SetEnabled(kTRUE);
}
////////////////////////////////////////////////////////////////////////////////////////////////////
void WFGUI::PopUp()
{
	pop = new TCanvas("pop","Z cut",100,0,300,900);
	pop->cd();

	TPad *pad1 = new TPad("pad1","",0.02,0.81,0.98,0.99);
	TPad *pad2 = new TPad("pad2","",0.02,0.61,0.98,0.79);
	TPad *pad3 = new TPad("pad3","",0.02,0.41,0.98,0.59);
	TPad *pad4 = new TPad("pad4","",0.02,0.21,0.98,0.39);
	TPad *pad5 = new TPad("pad5","",0.02,0.01,0.98,0.19);

	pad1->Draw();
	pad2->Draw();
	pad3->Draw();
	pad4->Draw();
	pad5->Draw();

	TH2F *z1 = new TH2F("z1"," ; x (um); y (um)",dwpot.GetXMAX(),-dwpot.GetXMAX()*0.5,dwpot.GetXMAX()*0.5,dwpot.GetYMAX(),-dwpot.GetYMAX()*0.5,dwpot.GetYMAX()*0.5);
	TH2F *z2 = new TH2F("z2"," ; x (um); y (um)",dwpot.GetXMAX(),-dwpot.GetXMAX()*0.5,dwpot.GetXMAX()*0.5,dwpot.GetYMAX(),-dwpot.GetYMAX()*0.5,dwpot.GetYMAX()*0.5);
	TH2F *z3 = new TH2F("z3"," ; x (um); y (um)",dwpot.GetXMAX(),-dwpot.GetXMAX()*0.5,dwpot.GetXMAX()*0.5,dwpot.GetYMAX(),-dwpot.GetYMAX()*0.5,dwpot.GetYMAX()*0.5);
	TH2F *z4 = new TH2F("z4"," ; x (um); y (um)",dwpot.GetXMAX(),-dwpot.GetXMAX()*0.5,dwpot.GetXMAX()*0.5,dwpot.GetYMAX(),-dwpot.GetYMAX()*0.5,dwpot.GetYMAX()*0.5);
	TH2F *z5 = new TH2F("z5"," ; x (um); y (um)",dwpot.GetXMAX(),-dwpot.GetXMAX()*0.5,dwpot.GetXMAX()*0.5,dwpot.GetYMAX(),-dwpot.GetYMAX()*0.5,dwpot.GetYMAX()*0.5);

	for(int i=0;i<dwpot.GetXMAX();i++) {        // fill histogram
		for(int j=0;j<dwpot.GetYMAX();j++) {
			z1->TH2F::SetBins((dwpot.GetXMAX()),-(dwpot.GetXMAX())*0.5,(dwpot.GetXMAX())*0.5,(dwpot.GetYMAX()),-(dwpot.GetYMAX())*0.5,(dwpot.GetYMAX())*0.5);
			z2->TH2F::SetBins((dwpot.GetXMAX()),-(dwpot.GetXMAX())*0.5,(dwpot.GetXMAX())*0.5,(dwpot.GetYMAX()),-(dwpot.GetYMAX())*0.5,(dwpot.GetYMAX())*0.5);
			z3->TH2F::SetBins((dwpot.GetXMAX()),-(dwpot.GetXMAX())*0.5,(dwpot.GetXMAX())*0.5,(dwpot.GetYMAX()),-(dwpot.GetYMAX())*0.5,(dwpot.GetYMAX())*0.5);
			z4->TH2F::SetBins((dwpot.GetXMAX()),-(dwpot.GetXMAX())*0.5,(dwpot.GetXMAX())*0.5,(dwpot.GetYMAX()),-(dwpot.GetYMAX())*0.5,(dwpot.GetYMAX())*0.5);
			z5->TH2F::SetBins((dwpot.GetXMAX()),-(dwpot.GetXMAX())*0.5,(dwpot.GetXMAX())*0.5,(dwpot.GetYMAX()),-(dwpot.GetYMAX())*0.5,(dwpot.GetYMAX())*0.5);

			z1->SetBinContent(i+1,j+1,dwpot.Getdpot(j,i,dwpot.GetZfinest()/2));
			z2->SetBinContent(i+1,j+1,dwpot.Getdpot(j,i,dwpot.GetZfinest()/2+Widthentry->GetNumber()/2));
			z3->SetBinContent(i+1,j+1,dwpot.Getdpot(j,i,dwpot.GetZfinest()/2+Pitchentry->GetNumber()/2-1));
			z4->SetBinContent(i+1,j+1,dwpot.Getdpot(j,i,dwpot.GetZfinest()/2+Pitchentry->GetNumber()-Widthentry->GetNumber()/2-2));
			z5->SetBinContent(i+1,j+1,dwpot.Getdpot(j,i,dwpot.GetZfinest()/2+Pitchentry->GetNumber()));
		}
	}

	pad1->cd();
	z1->Draw("COLZ");

	pad2->cd();
	z2->Draw("COLZ");

	pad3->cd();
	z3->Draw("COLZ");

	pad4->cd();
	z4->Draw("COLZ");

	pad5->cd();
	z5->Draw("COLZ");

	gStyle->SetPalette(55.0);
	gStyle->SetNumberContours(99);

	pop->Update();
}
/////////////////////////////////////////////////////////////////////////////////////////////
void WFGUI::DrawHist()
{
	canvasp->Clear();

	dhist->Reset(); //Reset histogram
	dhist->TH2F::SetBins((dwpot.GetXMAX()),-(dwpot.GetXMAX())*0.5,(dwpot.GetXMAX())*0.5,(dwpot.GetYMAX()),-(dwpot.GetYMAX())*0.5,(dwpot.GetYMAX())*0.5);


	for(int i=0;i<dwpot.GetXMAX();i++) {        // fill histogram
		for(int j=0;j<dwpot.GetYMAX();j++) {
			dhist->SetBinContent(i+1,j+1,dwpot.Getdpot(j,i,dwpot.GetZfinest()/2+Zcutentry->GetNumber()));
		}
	}

	canvasp->cd();
	dhist->Draw("COLZ");
}
/////////////////////////////////////////////////////////////////////////////////////////////
void WFGUI::DrawFieldHist() 
{
	int p=10;
	int a=0;
	int h=0;
	int binsx=dwpot.GetXMAX(); // binsx = number of bins on x-axis
	int binsy=dwpot.GetYMAX(); // binsy = number of bins on y-axis


	dfhist->Reset();     //dfhist = histogram of drift field 
	dfhist->TH2F::SetBins((binsx/p+1), -(binsx/p+1)*0.5,(binsx/p+1)*0.5, (binsy/p+1), -(binsy/p+1)*0.5,(binsy/p+1)*0.5);
	canvasp->cd();

	for(int i=0;i<binsx/p+1;i++) {
		for(int j=0;j<binsy/p+1;j++) {
			h=i*p;
			a=j*p;
			dfhist->SetBinContent(i+1,j+1,dwpot.Getdpot(a,h,dwpot.GetZMAX()/2+(Zcutentry->GetNumber()+pow(2,dwpot.Getref())-1)/pow(2,dwpot.Getref())));
		}
	}

	dfhist->SetStats(0);
	dfhist->Draw("SAME" "ARR"); //draw on same canvas as dhist and use arrows  
	canvasp->Update();
}
/////////////////////////////////////////////////////////////////////////////////////////////
void WFGUI::CallCalculateFields() 
{
	df = new Field**[(dwpot.GetYMAX())];
	for (int j=0; j<(dwpot.GetYMAX()); j++){
		df[j] = new Field*[(dwpot.GetXMAX())];
		for (int i=0; i<(dwpot.GetXMAX()); i++){
			df[j][i] = new Field[(dwpot.GetZMAX())];
		}
	}

	wf = new Field**[(dwpot.GetYMAX())];
	for (int j=0; j<(dwpot.GetYMAX()); j++){
		wf[j] = new Field*[(dwpot.GetXMAX())];
		for (int i=0; i<(dwpot.GetXMAX()); i++){
			wf[j][i] = new Field[(dwpot.GetZMAX())];
		}
	}


	CalculateFields(dwpot,df,wf);
	CalculateAbsFields(dwpot,df);
}
/////////////////////////////////////////////////////////////////////////////////////////////
void WFGUI::CallFieldCut() 
{
	if (FieldCutButton[0]->IsOn()) {
		EtotButton->Disconnect();
		ExButton->Disconnect();
		EyButton->Disconnect();		

		EtotButton->SetEnabled(kTRUE);
		ExButton->SetEnabled(kTRUE);
		EyButton->SetEnabled(kTRUE);

		EtotButton->Connect("Clicked()", "WFGUI", this, "DrawFieldsAbs0()");
		ExButton->Connect("Clicked()", "WFGUI", this, "DrawFieldsAbsX0()");
		EyButton->Connect("Clicked()", "WFGUI", this, "DrawFieldsAbsY0()");
	}
	else {
		EtotButton->Disconnect();
		ExButton->Disconnect();
		EyButton->Disconnect();

		EtotButton->SetEnabled(kTRUE);
		ExButton->SetEnabled(kTRUE);
		EyButton->SetEnabled(kTRUE);

		EtotButton->Connect("Clicked()", "WFGUI", this, "DrawFieldsAbs1()");
		ExButton->Connect("Clicked()", "WFGUI", this, "DrawFieldsAbsX1()");
		EyButton->Connect("Clicked()", "WFGUI", this, "DrawFieldsAbsY1()");
	}
}
/////////////////////////////////////////////////////////////////////////////////////////////
void WFGUI::DrawFieldsAbs0() 
{
	CallCalculateFields();

	wherecut = WhereCut->GetNumber();
	wherecut = wherecut+dwpot.GetXMAX()/2;

	double* q;
	q = new double[dwpot.GetYMAX()-1];
	for(int k=0;k<dwpot.GetYMAX()-1;k++) q[k]=k-0.5*dwpot.GetYMAX();//(int)(pow(2,pot.Getref()));

	double *dabs; // Absolute value of drift field
	dabs = new double[dwpot.GetYMAX()]-1;

	for(int i=0; i<dwpot.GetYMAX()-1; i++) {
		dabs[i]=0.0;
	}

	for(int k=0;k<dwpot.GetYMAX()-1;k++) dabs[k] = df[k+1][wherecut][(int)(dwpot.GetZMAX()/2+(Zcutentry->GetNumber()+pow(2,dwpot.Getref())-1)/pow(2,dwpot.Getref()))].Getabs();

	canvaspf->cd();

	dfieldcut = new TGraph(dwpot.GetYMAX()-1,q,dabs);
	dfieldcut->SetLineWidth(4);
	dfieldcut->GetXaxis()->SetTitle("y (um)");
	dfieldcut->GetYaxis()->SetTitle("Drift Field (V/m)");
	dfieldcut->GetXaxis()->SetTitleSize(0.05);
	dfieldcut->GetYaxis()->SetTitleSize(0.05);
	dfieldcut->GetXaxis()->SetLabelSize(0.05);
	dfieldcut->GetYaxis()->SetLabelSize(0.05);
	dfieldcut->SetTitle(" ");
	dfieldcut->Draw("ACP");

	TLegend *legend = new TLegend(0.12,0.8,0.62,0.92);
	legend->SetTextSize(0.05);
	legend->AddEntry(dfieldcut,Form("parallel to y-axis (x=%d um)",(int)WhereCut->GetNumber()),"l");
	legend->Draw();

	canvaspf->Update();
}
/////////////////////////////////////////////////////////////////////////////////////
void WFGUI::DrawFieldsAbsX0() 
{
	CallCalculateFields();
	wherecut = WhereCut->GetNumber();
	wherecut = wherecut+dwpot.GetXMAX()/2;

	double* q;
	q = new double[dwpot.GetYMAX()-1];
	for(int k=0;k<dwpot.GetYMAX()-1;k++) q[k]=k-0.5*dwpot.GetYMAX();

	double *dxabs; // Absolute value of drift field
	dxabs = new double[dwpot.GetYMAX()-1];

	for(int i=0; i<dwpot.GetYMAX()-1; i++) {
		dxabs[i]=0.0;
	}
	for(int k=0;k<dwpot.GetYMAX()-1;k++) dxabs[k] = df[k+1][wherecut][(int)(dwpot.GetZMAX()/2+(Zcutentry->GetNumber()+pow(2,dwpot.Getref())-1)/pow(2,dwpot.Getref()))].GetFieldx();

	canvaspf->cd();

	dfieldcut = new TGraph(dwpot.GetYMAX()-1,q,dxabs);
	dfieldcut->SetLineWidth(4);
	dfieldcut->GetXaxis()->SetTitle("y (um)");
	dfieldcut->GetYaxis()->SetTitle("dEx (V/m)");
	dfieldcut->GetXaxis()->SetTitleSize(0.05);
	dfieldcut->GetYaxis()->SetTitleSize(0.05);
	dfieldcut->GetXaxis()->SetLabelSize(0.05);
	dfieldcut->GetYaxis()->SetLabelSize(0.05);
	dfieldcut->SetTitle(" ");
	dfieldcut->Draw("ACP");

	TLegend *legend = new TLegend(0.12,0.8,0.62,0.92);
	legend->SetTextSize(0.05);
	legend->AddEntry(dfieldcut,Form("parallel to y-axis (x=%d um)",(int)WhereCut->GetNumber()),"l");
	legend->Draw();

	canvaspf->Update();
}
/////////////////////////////////////////////////////////////////////////////////////
void WFGUI::DrawFieldsAbsY0() 
{
	CallCalculateFields();
	wherecut = WhereCut->GetNumber();
	wherecut = wherecut+dwpot.GetXMAX()/2;

	double* q;
	q = new double[dwpot.GetYMAX()-1];
	for(int k=0;k<dwpot.GetYMAX()-1;k++) q[k]=k-0.5*dwpot.GetYMAX();

	double *dyabs; // Absolute value of drift field in y direction
	dyabs = new double[dwpot.GetYMAX()-1];

	for(int i=0; i<dwpot.GetYMAX()-1; i++) { // initialize
		dyabs[i]=0.0;
	}
	for(int k=0;k<dwpot.GetYMAX()-1;k++) dyabs[k] = fabs(df[k+1][wherecut][(int)(dwpot.GetZMAX()/2+(Zcutentry->GetNumber()+pow(2,dwpot.Getref())-1)/pow(2,dwpot.Getref()))].GetFieldy());

	canvaspf->cd();

	dfieldcut = new TGraph(dwpot.GetYMAX()-1,q,dyabs);
	dfieldcut->SetLineWidth(4);
	dfieldcut->GetXaxis()->SetTitle("y (um)");
	dfieldcut->GetYaxis()->SetTitle("dEy (V/m)");
	dfieldcut->GetXaxis()->SetTitleSize(0.05);
	dfieldcut->GetYaxis()->SetTitleSize(0.05);
	dfieldcut->GetXaxis()->SetLabelSize(0.05);
	dfieldcut->GetYaxis()->SetLabelSize(0.05);
	dfieldcut->SetTitle(" ");
	dfieldcut->Draw("ACP");

	TLegend *legend = new TLegend(0.12,0.8,0.62,0.92);
	legend->SetTextSize(0.05);
	legend->AddEntry(dfieldcut,Form("parallel to y-axis (x=%d um)",(int)WhereCut->GetNumber()),"l");
	legend->Draw();

	canvaspf->Update();
}
/////////////////////////////////////////////////////////////////////////////////////
void WFGUI::DrawFieldsAbs1() 
{
	CallCalculateFields();

	wherecut = WhereCut->GetNumber();
	wherecut = wherecut+dwpot.GetYMAX()/2;

	double* q;
	q = new double[dwpot.GetXMAX()];
	for(int k=0;k<dwpot.GetXMAX();k++) q[k]=k-0.5*dwpot.GetXMAX();//(int)(pow(2,pot.Getref()));

	double *dabs; // Absolute value of drift field
	dabs = new double[dwpot.GetXMAX()];

	for(int i=0; i<dwpot.GetXMAX(); i++) {
		dabs[i]=0.0;
	}

	for(int k=0;k<dwpot.GetXMAX();k++) dabs[k] = df[wherecut][k][(int)(dwpot.GetZMAX()/2+(Zcutentry->GetNumber()+pow(2,dwpot.Getref())-1)/pow(2,dwpot.Getref()))].Getabs();

	canvaspf->cd();

	dfieldcut = new TGraph(dwpot.GetXMAX(),q,dabs);
	dfieldcut->SetLineWidth(4);
	dfieldcut->GetXaxis()->SetTitle("x (um)");
	dfieldcut->GetYaxis()->SetTitle("Drift Field (V/m)");
	dfieldcut->GetXaxis()->SetTitleSize(0.05);
	dfieldcut->GetYaxis()->SetTitleSize(0.05);
	dfieldcut->GetXaxis()->SetLabelSize(0.05);
	dfieldcut->GetYaxis()->SetLabelSize(0.05);
	dfieldcut->SetTitle(" ");
	dfieldcut->Draw("ACP");

	TLegend *legend = new TLegend(0.12,0.8,0.62,0.92);
	legend->SetTextSize(0.05);
	legend->AddEntry(dfieldcut,Form("parallel to x-axis (y=%d um)",(int)WhereCut->GetNumber()),"l");
	legend->Draw();

	canvaspf->Update();
}
/////////////////////////////////////////////////////////////////////////////////////
void WFGUI::DrawFieldsAbsX1() 
{
	CallCalculateFields();
	wherecut = WhereCut->GetNumber();
	wherecut = wherecut+dwpot.GetYMAX()/2;

	double* q;
	q = new double[dwpot.GetXMAX()];
	for(int k=0;k<dwpot.GetXMAX();k++) q[k]=k-0.5*dwpot.GetXMAX();

	double *dxabs; // Absolute value of drift field
	dxabs = new double[dwpot.GetXMAX()];

	for(int i=0; i<dwpot.GetXMAX(); i++) {
		dxabs[i]=0.0;
	}
	for(int k=0;k<dwpot.GetXMAX();k++) dxabs[k] = df[wherecut][k][(int)(dwpot.GetZMAX()/2+(Zcutentry->GetNumber()+pow(2,dwpot.Getref())-1)/pow(2,dwpot.Getref()))].GetFieldx();

	canvaspf->cd();

	dfieldcut = new TGraph(dwpot.GetXMAX(),q,dxabs);
	dfieldcut->SetLineWidth(4);
	dfieldcut->GetXaxis()->SetTitle("x (um)");
	dfieldcut->GetYaxis()->SetTitle("dEx (V/m)");
	dfieldcut->GetXaxis()->SetTitleSize(0.05);
	dfieldcut->GetYaxis()->SetTitleSize(0.05);
	dfieldcut->GetXaxis()->SetLabelSize(0.05);
	dfieldcut->GetYaxis()->SetLabelSize(0.05);
	dfieldcut->SetTitle(" ");
	dfieldcut->Draw("ACP");

	TLegend *legend = new TLegend(0.12,0.8,0.62,0.92);
	legend->SetTextSize(0.05);
	legend->AddEntry(dfieldcut,Form("parallel to x-axis (y=%d um)",(int)WhereCut->GetNumber()),"l");
	legend->Draw();

	canvaspf->Update();
}
/////////////////////////////////////////////////////////////////////////////////////
void WFGUI::DrawFieldsAbsY1() 
{
	CallCalculateFields();
	wherecut = WhereCut->GetNumber();
	wherecut = wherecut+dwpot.GetYMAX()/2;

	double* q;
	q = new double[dwpot.GetXMAX()];
	for(int k=0;k<dwpot.GetXMAX();k++) q[k]=k-0.5*dwpot.GetXMAX();

	double *dyabs; // Absolute value of drift field in y direction
	dyabs = new double[dwpot.GetXMAX()];

	for(int i=0; i<dwpot.GetXMAX(); i++) { // initialize
		dyabs[i]=0.0;
	}
	for(int k=0;k<dwpot.GetXMAX();k++) dyabs[k] = fabs(df[wherecut][k][(int)(dwpot.GetZMAX()/2+(Zcutentry->GetNumber()+pow(2,dwpot.Getref())-1)/pow(2,dwpot.Getref()))].GetFieldy());

	canvaspf->cd();

	dfieldcut = new TGraph(dwpot.GetXMAX(),q,dyabs);
	dfieldcut->SetLineWidth(4);
	dfieldcut->GetXaxis()->SetTitle("x (um)");
	dfieldcut->GetYaxis()->SetTitle("dEy (V/m)");
	dfieldcut->GetXaxis()->SetTitleSize(0.05);
	dfieldcut->GetYaxis()->SetTitleSize(0.05);
	dfieldcut->GetXaxis()->SetLabelSize(0.05);
	dfieldcut->GetYaxis()->SetLabelSize(0.05);
	dfieldcut->SetTitle(" ");
	dfieldcut->Draw("ACP");

	TLegend *legend = new TLegend(0.12,0.8,0.62,0.92);
	legend->SetTextSize(0.05);
	legend->AddEntry(dfieldcut,Form("parallel to x-axis (y=%d um)",(int)WhereCut->GetNumber()),"l");
	legend->Draw();

	canvaspf->Update();
}
/////////////////////////////////////////////////////////////////////////////////////
void WFGUI::CallCalculateCurrents()
{
	dhist->Reset();

	if(bfieldon==true) B = BfieldEntry->GetNumber();
	else B=0.0;

	if(diffusionon==true) T = TempEntry->GetNumber();
	else T=0.0;

	carriers = new Carriers[2*75*dwpot.GetYMAX()];

	double pi=4.0*atan(1); 
	carriersangle = CarriersAngleentry->GetNumber();
	carriersin = CarriersInentry->GetNumber();
	carriersin = carriersin+(int)(dwpot.GetXMAX()/2.0-dwpot.GetYMAX()*tan(pi*carriersangle/180.0));	//+1?

	CallCalculateFields();
	CreateCharges(dwpot, carriers, carriersin,carriersangle,this);
	SetLorentz(dwpot,carriers,B);
	CalculateCurrents(dwpot, df, wf, carriers, this);

	curcanvas->Update();
	CurrentsInfoFrame->Layout();
}
////////////////////////////////////////////////////////////////////////////////////////////////////
TH2F* WFGUI::Getdhist() {
	return dhist;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
TH2F* WFGUI::Getdchist() {
	return dchist;
}
/////////////////////////////////////////////////////////////////////////////////////////////
TCanvas* WFGUI::Getcanvasp() {
	return canvasp;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
TCanvas* WFGUI::Getcurcanvas() {
	return curcanvas;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
bool WFGUI::Getdiffusionon() {
	return diffusionon;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
double WFGUI::GetB() {
	return B;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
double WFGUI::GetT() {
	return T;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
void WFGUI::SetQLabel(const char* qetext,const char* qhtext,const char* qtottext) {
	QELabel->SetTitle(qetext);
	QHLabel->SetTitle(qhtext);
	QTotLabel->SetTitle(qtottext);
}
/////////////////////////////////////////////////////////////////////////////////////////////
void WFGUI::SetAngleLabel(const char* ethetatext, const char* hthetatext) {
	LorentzeLabel->SetTitle(ethetatext);
	LorentzhLabel->SetTitle(hthetatext);
}
/////////////////////////////////////////////////////////////////////////////////////////////
void WFGUI::SetBField(Bool_t on) {
	bfieldon=on;
	if (bfieldon) BfieldEntry->SetState(kTRUE);
	else BfieldEntry->SetState(kFALSE);
}
/////////////////////////////////////////////////////////////////////////////////////////////
void WFGUI::SetDiffusion(Bool_t on) {
	diffusionon=on;
	if (diffusionon) TempEntry->SetState(kTRUE);
	else TempEntry->SetState(kFALSE);
}
/////////////////////////////////////////////////////////////////////////////////////////////
void* WFGUI::StartPotentialCalcTh(void* arg)
{	
	WFGUI* gui = (WFGUI*) arg;
	gui->CallCalculatePotentials();
	gui->ThreadstopPotential();
	return NULL;	
}
/////////////////////////////////////////////////////////////////////////////////////////////
Int_t WFGUI::ThreadstartPotential(){
	//if(!PotentialThread){
		//stopped=false;
		WFGUI* arg = this;
		PotentialThread = new TThread("memberfunction",(void(*) (void *))&StartPotentialCalcTh, (void*) arg);
		PotentialThread->Run();	
		return 0;
//	}
//	return 1;
}
/////////////////////////////////////////////////////////////////////////////////////////////
Int_t WFGUI::ThreadstopPotential(){
	DrawHist();
	gStyle->SetPalette(55.0);
	gStyle->SetNumberContours(99);
	canvasp->Update();
	DrawFieldHist();
	if (PotentialThread){
		//stopped=true;
		TThread::Delete(PotentialThread);
		delete PotentialThread;
		PotentialThread=0;
		return 0;
	}
	return 1;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
void* WFGUI::StartCurrentsCalcTh(void* arg)
{
	WFGUI* gui = (WFGUI*) arg;
	gui->CallCalculateCurrents();
	gui->ThreadstopCurrents();
	return NULL;
}
/////////////////////////////////////////////////////////////////////////////////////////////
Int_t WFGUI::ThreadstartCurrents(){
//	if(!CurrentsThread){
		//stopped=false;
		WFGUI* arg = this;
		CurrentsThread= new TThread("memberfunction", (void(*) (void *))&StartCurrentsCalcTh, (void*) arg);
		CurrentsThread->Run();
		return 0;
//	}
//	return 1;
}
/////////////////////////////////////////////////////////////////////////////////////////////
Int_t WFGUI::ThreadstopCurrents(){
	if(CurrentsThread){
		//stopped=true;
		TThread::Delete(CurrentsThread);
		delete CurrentsThread;
		CurrentsThread=0;
		return 0;
	}
	return 1;
}
/////////////////////////////////////////////////////////////////////////////////////////////

#include "TGClient.h"
#include "TGNumberEntry.h"
#include "TGFrame.h"
#include "TGButtonGroup.h"
#include "TGCanvas.h"
#include "TCanvas.h"
#include "TGButton.h"
#include "TGLabel.h"
#include "TGTab.h"
#include "TGObject.h"
#include "TRootEmbeddedCanvas.h"
#include "Riostream.h"
#include "TLatex.h"
#include "TApplication.h"
#include "TH1.h"
#include "TH2.h"
#include "TGaxis.h"
//#include "Potentials.h"
//#include "Field.h"
#include "Carriers.h"
#include "TThread.h"
#include "TRandom3.h"
#include "TMutex.h"
#include "TDatime.h"
#include "TGProgressBar.h"
#include "TStyle.h"
#include "TMath.h"
#include "TLegend.h"
#include <string>

class WFGUI : public TGMainFrame {

	private:

		bool plotupdate;
		bool stopped;
		bool bfieldon;
		bool diffusionon;
		bool currentson;
		bool fieldyes;

		int carriersin;
		int carriersangle;
		double T;					// temperature

		TRootEmbeddedCanvas *driftcanvas;
		TRootEmbeddedCanvas *driftfcanvas;
		TRootEmbeddedCanvas *currentscanvas;
		TGTab *PotentialTab;
		TGCompositeFrame *DriftPTab;
		TGCompositeFrame *CurrentsTab;
		TGVerticalFrame *SettingsFrame;
		TGGroupFrame *DetectorPropertiesFrame;
		TGGroupFrame *DimensionsFrame;
		TGGroupFrame *VoltageFrame;
		TGGroupFrame *ZcutFrame;
		TGGroupFrame *CurrentsFrame;
		TGGroupFrame *CarriersInFrame;
		TGGroupFrame *PlotGroupFrame;
		TGGroupFrame *ChargeCollectionFrame;
		TGGroupFrame *LorentzInfoFrame;

		TGVerticalFrame *DimSetFrame;
		TGVerticalFrame *DimLabelFrame; 
		TGVerticalFrame *VolSetFrame;
		TGVerticalFrame *VolLabelFrame; 
		TGVerticalFrame *ZcutSetFrame;
		TGVerticalFrame *ZcutLabelFrame; 
		TGVerticalFrame *CarriersInLabelFrame; 
		TGVerticalFrame *CarriersInSetFrame;
		TGVerticalFrame *CurrentsInfoFrame;

		TGHorizontalFrame *ButtonPotFrame;
		TGHorizontalFrame *LabelsTabFrame;
        TGHorizontalFrame *DriftPTabFrame;
		TGHorizontalFrame *CurrentsLabelFrame;
//		TGHorizontalFrame *CarriersInFrame;

		TGButtonGroup *FieldCutGroup;
		TGNumberEntry *XMAXentry;
		TGNumberEntry *YMAXentry;
		TGNumberEntry *ZMAXentry;
		TGNumberEntry *Pitchentry;
		TGNumberEntry *Widthentry;
		TGNumberEntry *Biasentry;
		TGNumberEntry *Depletionentry;
		TGNumberEntry *Zcutentry;
		TGNumberEntry *WhereCut;
		TGNumberEntry *BfieldEntry;
		TGNumberEntry *TempEntry;
		TGNumberEntry *CarriersInentry;
		TGNumberEntry *CarriersAngleentry;

		TGCheckButton *CurrentsButton[2];
		TGCheckButton *Eyes;
		TGLabel *XMAXLabel;
		TGLabel *YMAXLabel;
		TGLabel *ZMAXLabel;
		TGLabel *PitchLabel;
		TGLabel *WidthLabel;
		TGLabel *BiasLabel;
		TGLabel *DepletionLabel;
		TGLabel *ZcutLabel;
		TGLabel *DriftCutLabel;
//		TGLabel *FieldLabel;
		TGLabel *CarriersInLabel;
		TGLabel *CarriersAngleLabel;
		TGLabel *ElectronsLabel;
		TGLabel *HolesLabel;
		TGLabel *TotalLabel;
        TGLabel *QELabel;
        TGLabel *QHLabel;
        TGLabel *QTotLabel;
        TGLabel *QHTextLabel;
        TGLabel *QETextLabel;
        TGLabel *QTotTextLabel;
        TGLabel *LorentzeLabel;
        TGLabel *LorentzhLabel;
        TGLabel *LorentzhTextLabel;
        TGLabel *LorentzeTextLabel;

		TGRadioButton *FieldCutButton[2];
		TGTextButton *SetButton;
		TGTextButton *ExitButton;
		TGTextButton *CalcPotButton;
        TGTextButton *CalcCurButton;
		TGTextButton *EtotButton;
		TGTextButton *ExButton;
		TGTextButton *EyButton;
		TGTextButton *DrawCutsUserEntry;


		Potentials dwpot;                       // potentials
		Field ***df;
		Field ***wf;
		Carriers *carriers;

		TCanvas *canvasp;		// canvas for drift potential histogram
        TCanvas *canvaspf;
		TCanvas *curcanvas;
		TCanvas *pop;

		TThread *PotentialThread;	// thread for calculation of potentials
		TThread *CurrentsThread;

		TH2F* dhist;
		TH2F* dfhist;
//		TH2F* dchist;

	public:
		double B;		// magnetic field
		int wherecut;	// draw drift potential cut where? (x coordinate)
			
		//TGraph *driftcut;	// graph for drift potential cut
		TGraph *dfieldcut;	// graph for drift field cut

		WFGUI(const TGWindow *p,UInt_t w,UInt_t h);	// constructor of GUI
		void CallFieldCut();
		void CallCalculatePotentials();			// Method to Calculate Potentials
		void CallCalculateFields();
		void CallCalculateCurrents();
//		void SetPlotFieldHist(Bool_t);
		void DrawFieldsAbs0();
		void DrawFieldsAbsX0();
		void DrawFieldsAbsY0();
        void DrawFieldsAbs1();
        void DrawFieldsAbsX1();
        void DrawFieldsAbsY1();
		void DrawFieldHist();
        TH2F* Getdhist();
        TH2F* Getdchist();
		TCanvas* Getcanvasp();
		TCanvas* Getcurcanvas();
		double GetB();
		double GetT();
		bool Getdiffusionon();
		void SetQLabel(const char*,const char*,const char*);
		void SetAngleLabel(const char*, const char*);		
		void SetBField(Bool_t);
		void SetDiffusion(Bool_t);
		void DrawHist();						// Method to draw histograms of drift and weighting potential
		void PopUp();

		static void* StartPotentialCalcTh(void*);
		virtual Int_t ThreadstartPotential();
		virtual Int_t ThreadstopPotential();
		static void* StartCurrentsCalcTh(void*);
		virtual Int_t ThreadstartCurrents();
		virtual Int_t ThreadstopCurrents();

		TH2F* dchist;

		ClassDef(WFGUI,0);  // muss auskommentiert sein wenn .x ... .cxx+
};

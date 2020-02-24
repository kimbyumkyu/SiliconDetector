#include <iostream>
#include <math.h>
#include <string.h>
#include "TROOT.h"
#include "TH2F.h"
#include "TH1.h"
#include "TStyle.h"
#include "TSystem.h"
#include "TStyle.h"
#include "TColor.h"
#include "TExec.h"
#include "TCanvas.h"
#include "TGButton.h"
#include "TGFrame.h"
#include "TGraph.h"

#define GRIDDIST  0.000001	// grid distance (1um) in m

class WFGUI;

class Potentials {

	private:
		int ref;
		int multig;
		int finest;
		int Zfinest;
		int XMAX;
		int YMAX;
		int ZMAX;
		int pitch;
		int width;
		int Zcut;
		double count;
		double count2;
		double vbias;
		double vdepl;
		double poissonf;
		
		double ***dpot;
		double ***wpot;
		int ***fix;

	public:
		Potentials(int,int,int);				// constructor (YMAX,XMAX,ZMAX)
		virtual ~Potentials();				// destructor
		
		int Getref();
		int GetXMAX();						
		int GetYMAX();
        int GetZMAX();						
        int GetZfinest();						
		int Getpitch();
		int Getwidth();
		double Getvbias();
		double Getvdepl();
		double Getdpot(int,int,int);
		double Getwpot(int,int,int);
		int Getfix(int,int,int);

		void Setdpot(int,int,int,double);
		void Setwpot(int,int,int,double);
		void SetV(double,double);
		void SetPitchWidthXY(int,int,int,int,int,int);
		void SetBoundaryConditions();
		
		void Restrictor();
		int*** FixRestrictor();
		void Iteration(void*);
		void Prolongation();
		void Multigrid(void*);

		ClassDef(Potentials,0); 
};

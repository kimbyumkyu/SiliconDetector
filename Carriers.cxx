#include "WFGUI.h"
////////////////////////////////////////////////////////////////////////////////////////////////////
Carriers::Carriers() {
	x=0;
	y=0;
	z=0;
	inside=1;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
double Carriers::Getx() {
	return x;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
double Carriers::Gety() {
	return y;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
double Carriers::Getz() {
	return z;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
int Carriers::Getinside() {
	return inside;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
double Carriers::Getetheta() {
	return etheta;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
double Carriers::Geththeta() {
	return htheta;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
void Carriers::Setx(double valx) {
	x=valx;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
void Carriers::Sety(double valy) {
	y=valy;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
void Carriers::Setz(double valz) {
	z=valz;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
void Carriers::Setinside(int val) {
	inside=val;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
void Carriers::Setetheta(double angle) {
	etheta=angle;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
void Carriers::Seththeta(double angle) {
	htheta=angle;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
void CreateCharges(Potentials &pot, Carriers *ca, int hit, int angle, void* wfgui)
{
	double pi = 4.0*atan(1);
	double theta=angle*pi/180;	// angle: degree, theta: radian
	int mipcharge=75*pot.GetYMAX()/cos(theta);

	WFGUI* gui = (WFGUI*) wfgui;
	
	gui->Getcanvasp()->cd();
	gui->dchist->TH2F::SetBins((pot.GetXMAX()),-(pot.GetXMAX())*0.5,(pot.GetXMAX())*0.5,(pot.GetYMAX()),-(pot.GetYMAX())*0.5,(pot.GetYMAX())*0.5);

	for (int j=3*75; j<mipcharge; j++) {
		ca[j].Setx(hit+j*sin(theta)/(double)75);
		ca[j].Sety(j*cos(theta)/(double)75);
		ca[j].Setinside(1);
	
		//if (j%750==0) gui->Getdchist()->SetBinContent(ca[j].Getx(), ca[j].Gety(), 75);

		ca[mipcharge+j].Setx(hit+j*sin(theta)/(double)75);
		ca[mipcharge+j].Sety(j*cos(theta)/(double)75);
		ca[mipcharge+j].Setinside(1);
	}
	gui->Getdchist()->Draw();
	gui->Getdchist()->SetStats(0);
	gui->Getcanvasp()->Update();
}
////////////////////////////////////////////////////////////////////////////////////////////////////
void SetLorentz(Potentials &pot, Carriers *carriers, double b) {
	int mipcharge=75*pot.GetYMAX();
    
	for(int i=0; i<mipcharge; i++) {
        carriers[i].Setetheta(atan(MU_E*b)); //1.15
        carriers[i].Seththeta(0.0);

    }
    for(int i=mipcharge; i<2*mipcharge; i++) {
        carriers[i].Setetheta(0.0);
        carriers[i].Seththeta(atan(MU_H*b)); //0.72
    }
}
////////////////////////////////////////////////////////////////////////////////////////////////////
void CalculateCurrents(Potentials &pot, Field ***df, Field ***wf, Carriers *ca, void* wfgui)
{
	WFGUI* gui = (WFGUI*) wfgui;
//	gui->Getcanvasp()->cd();
//	gui->dchist->TH2F::SetBins((pot.GetXMAX()),-(pot.GetXMAX())*0.5,(pot.GetXMAX())*0.5,(pot.GetYMAX()),-(pot.GetYMAX())*0.5,(pot.GetYMAX())*0.5);
//	gui->Getdchist()->SetStats(0);

	double TIMEUNIT=1.0e-10;
	int TMAX=600;
	if (gui->Getdiffusionon() && gui->GetT()!=0.0) {
		//TIMEUNIT=1.0e-12;
		//TMAX=100000;
		TIMEUNIT=TIMEUNIT*1.0e-1;
		TMAX=TMAX*10;
	}
	
	int mipcharge=75*pot.GetYMAX();

	double vx=0,vy=0;
	double eabs=0;
	double mu=0;
	double ynew=0;
	double xnew=0;
	double ex=0,ey=0;
	double qe=0,qh=0,qtot=0;
	double *ie = new double[TMAX];
	double *ih = new double[TMAX];
	double *itot = new double[TMAX];

    TRandom3 *xi;
    TDatime *time;
    time = new TDatime();
    xi = new TRandom3(time->TDatime::GetTime());
    double k = xi->Uniform();
    double rand = xi->Uniform();

    double vabs=0,rx=0,ry=0,vdiffy=0,vdiffx=0;
    double me = 0.26*m0; // mass of electron see: DOI: 10.1109/TNS.2009.2021426
    double mh = 0.36*m0; // mass of holes
    double Temp = gui->GetT();

	for (int t=0; t<TMAX; t++) {
		ie[t]=0.0;
		ih[t]=0.0;
		itot[t]=0.0;
	}

    double theta1=ca[1].Getetheta();
    double theta2=ca[mipcharge+1].Geththeta();
	
    Field*** tempdf1;    // temporary field for electrons
    tempdf1 = new Field**[(pot.GetYMAX())];
    for (int j = 0; j < (pot.GetYMAX()); j++){
		tempdf1[j] = new Field*[(pot.GetXMAX())];
    	for (int i = 0; i < (pot.GetXMAX()); i++){
			tempdf1[j][i] = new Field[(pot.GetZMAX())];
		}
	}
	tempdf1=RotateField(pot,df,theta1);
   
	Field*** tempdf2; // temporary field for holes
    tempdf2 = new Field**[(pot.GetYMAX())];
    for (int j = 0; j < (pot.GetYMAX()); j++) {
		tempdf2[j] = new Field*[(pot.GetXMAX())];
    	for (int i = 0; i < (pot.GetXMAX()); i++) {
			tempdf2[j][i] = new Field[(pot.GetZMAX())];
		}
	}

    tempdf2=RotateField(pot,df,theta2);

	for (int t=0; t<TMAX; t++) {
		for (int j=0; j<mipcharge; j++) {
			if (ca[j].Getinside()!=0) {
				ex=tempdf1[(int)ca[j].Gety()][(int)ca[j].Getx()][(pot.GetZMAX())/2].GetFieldx();
				ey=tempdf1[(int)ca[j].Gety()][(int)ca[j].Getx()][(pot.GetZMAX())/2].GetFieldy();
				eabs=sqrt(ex*ex+ey*ey);
				mu=(MU_E/sqrt(1+SQMUEDIVVESAT*eabs*eabs));
				vx=-mu*ex;
				vy=-mu*ey;
                
				if(gui->Getdiffusionon()) {
                    vabs = sqrt(2*kB*Temp/me*log(fabs(1/(1-rand))));
                    k = xi->Uniform()*2*M_PI;
                    rx = cos(k);
                    ry = sin(k);
                    vdiffx = vabs*rx;
                    vdiffy = vabs*ry;

                    vx=vx+vdiffx;
                    vy=vy+vdiffy;
                }

				// calculate induced currents
				ie[t]+=-ECHARGE*(wf[(int)ca[j].Gety()][(int)ca[j].Getx()][(pot.GetZMAX())/2].GetFieldx()*vx+wf[(int)ca[j].Gety()][(int)ca[j].Getx()][(pot.GetZMAX())/2].GetFieldy()*vy);

				// calculate the new position due to motion in the drift field
				ynew=ca[j].Gety()+vy*TIMEUNIT/GRIDDIST;
				xnew=ca[j].Getx()+vx*TIMEUNIT/GRIDDIST;

				// check if charge is already outside of detector
				if ((xnew<0.0) || (xnew>pot.GetXMAX()) || (ynew<0.0) || (ynew>pot.GetYMAX())) ca[j].Setinside(0);
				else {
				   ca[j].Setx(xnew);
				   ca[j].Sety(ynew);
				}

				//if (t%(int)(TMAX*TIMEUNIT*1e9/100) && j==75*(pot.GetYMAX()-10)) {
//				if (t%(int)(TMAX/100)==0) {
//					gui->Getdchist()->Reset();
//					gui->Getdchist()->SetBinContent(ca[75*(pot.GetYMAX()-10)].Getx(), ca[75*(pot.GetYMAX()-10)].Gety(), 75);
//				}
			}
		}
		
//		gui->Getcanvasp()->cd();
//		gui->Getdchist()->Draw();
//		gui->Getcanvasp()->Update();
			
		for (int j=mipcharge; j<2*mipcharge ; j++)
		{
			if (ca[j].Getinside()!=0)
			{
				ex=tempdf2[(int)ca[j].Gety()][(int)ca[j].Getx()][(pot.GetZMAX())/2].GetFieldx();
				ey=tempdf2[(int)ca[j].Gety()][(int)ca[j].Getx()][(pot.GetZMAX())/2].GetFieldy();
				eabs=sqrt(ex*ex+ey*ey);
				mu=(MU_H/sqrt(1+MUHDIVVHSAT*eabs));
				vx=mu*ex;   // carrier velocity
				vy=mu*ey;

				if(gui->Getdiffusionon()) {
					vabs = sqrt(2*kB*Temp/mh*log(fabs(1/(1-rand))));
					k = xi->Uniform()*2*M_PI;
					rx = cos(k);
					ry = sin(k);
					vdiffx = vabs*rx;
					vdiffy = vabs*ry;
				
					vx=vx+vdiffx;
					vy=vy+vdiffy;
				}

				// calculate induced currents
				ih[t]+=ECHARGE*(wf[(int)ca[j].Gety()][(int)ca[j].Getx()][(pot.GetZMAX())/2].GetFieldx()*vx+wf[(int)ca[j].Gety()][(int)ca[j].Getx()][(pot.GetZMAX())/2].GetFieldy()*vy);

				// calculate the new position due to motion in the drift field
				ynew=ca[j].Gety()+vy*TIMEUNIT/GRIDDIST;
				xnew=ca[j].Getx()+vx*TIMEUNIT/GRIDDIST;

				if ((xnew<0.0) || (xnew>pot.GetXMAX()) || (ynew<0.0) || (ynew>pot.GetYMAX())) ca[j].Setinside(0);
				else {
				   ca[j].Setx(xnew);
				   ca[j].Sety(ynew);
				}
			}
		}
		itot[t]=ie[t]+ih[t];

		qe+=ie[t]*TIMEUNIT;
		qh+=ih[t]*TIMEUNIT;
		qtot+=itot[t]*TIMEUNIT;
	}
	

	double* q;              // x coordinate for graph Current(time)
	q = new double[TMAX];

	//double dt=100.0/((double)TMAX);
	double dt = TIMEUNIT*1e9;
	for(int k=0;k<TMAX;k++) q[k]=dt*(double)k; // set t coordinate

	TGraph *gr;
	gr = new TGraph(TMAX,q,itot);               // Graph for total current
	gr->GetXaxis()->SetTitle("time (ns)");
	gr->GetYaxis()->SetTitle("Current (A)");
	gr->GetYaxis()->SetTitleOffset(1.2);
	gr->GetXaxis()->SetTitleSize(0.05);
	gr->GetYaxis()->SetTitleSize(0.05);
	gr->GetXaxis()->SetLabelSize(0.05);
	gr->GetYaxis()->SetLabelSize(0.05);
	gr->SetLineWidth(3);
	gr->SetLineColor(3);                    // set line color to green
	gr->SetTitle(" ");
	gui->Getcurcanvas()->cd();
	gr->Draw("AL");
	//		gr->GetXaxis()->SetLimits(0,tpath);

	TGraph *gr2;                // Graph for current of electrons
	gr2 = new TGraph(TMAX,q,ie);
	gr2->SetLineWidth(3);
	gr2->SetLineColor(2);       // set line color to red
	gui->Getcurcanvas()->cd();
	gr2->Draw("L");
	gui->Getcurcanvas()->Update();

	TGraph *gr1;                // Graph for current of holes
	gr1 = new TGraph(TMAX,q,ih);
	gr1->SetLineWidth(3);
	gr1->SetLineColor(4);       // set line color to blue
	gui->Getcurcanvas()->cd();
	gr1->Draw("L");

	TLegend *legend = new TLegend(0.5,0.7,0.97,0.92);
	legend->SetTextSize(0.04);
	legend->AddEntry(gr,"Total Current","l");
	legend->AddEntry(gr2,"Current Induced by Electron","l");
	legend->AddEntry(gr1,"Current Induced by Hole","l");
	legend->Draw();

	char * qestring;
	qestring = new char[20];
	sprintf(qestring, "%.0lf", qe/ECHARGE);

	char * qhstring;
	qhstring = new char[20];
	sprintf(qhstring, "%.0lf", qh/ECHARGE);

	char * qtotstring;
	qtotstring = new char[20];
	sprintf(qtotstring, "%.0lf", qtot/ECHARGE);

	char * ethetastring;
	ethetastring = new char[20];
	sprintf(ethetastring, "%.2lf", theta1*180/M_PI);

	char * hthetastring;
	hthetastring = new char[20];
	sprintf(hthetastring, "%.2lf", theta2*180/M_PI);

	gui->SetQLabel(qestring,qhstring,qtotstring);
	gui->SetAngleLabel(ethetastring,hthetastring);
}
////////////////////////////////////////////////////////////////////////////////////////////////////
Carriers::~Carriers() {

}
////////////////////////////////////////////////////////////////////////////////////////////////////

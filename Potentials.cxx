#include "WFGUI.h"

//////////////////////////////////////////////////////////////////////
Potentials::Potentials(int dimy, int dimx, int dimz)	// constructor 
{
	ref=0;
	multig=(int)((log10(dimy*dimx/3)/log(4)))+1;
	finest=dimy;
	Zfinest=dimz;
	XMAX=dimx;
	YMAX=dimy;	
	ZMAX=dimz;

	dpot = new double**[YMAX];
	for (int j=0; j<YMAX; j++) {
		dpot[j] = new double*[XMAX];
		for (int i=0; i<XMAX; i++) {
			dpot[j][i] = new double[ZMAX];
		}
	}

    wpot = new double**[YMAX];
	for (int j=0; j<YMAX; j++) {
		wpot[j] = new double*[XMAX];  
		for (int i=0; i<XMAX; i++) {
                	wpot[j][i] = new double[ZMAX];
              	}
        }

	fix = new int**[YMAX];
	for (int j=0; j<YMAX; j++) {
		fix[j] = new int*[XMAX];
		for (int i=0; i<XMAX; i++) {
			fix[j][i] = new int[ZMAX];
    	}
    }

	
	for (int k=0; k<ZMAX; k++) {
		for (int j=0; j<YMAX; j++) {
			for (int i=0; i<XMAX; i++) {
				dpot[j][i][k]=0.0;
				wpot[j][i][k]=0.0;
				fix[j][i][k]=0;
			}
		}
	} 
}	
/////////////////////////////////////////////////////////////////////
int Potentials::Getref() {
	return ref;
}
/////////////////////////////////////////////////////////////////////
int Potentials::GetXMAX() {
	return XMAX;
}
//////////////////////////////////////////////////////////////////////
int Potentials::GetYMAX() {
	return YMAX;
}
//////////////////////////////////////////////////////////////////////
int Potentials::GetZMAX() {
	return ZMAX;
}
//////////////////////////////////////////////////////////////////////
int Potentials::GetZfinest() {
    return Zfinest;
}
//////////////////////////////////////////////////////////////////////
int Potentials::Getpitch() {
    return pitch;
}
//////////////////////////////////////////////////////////////////////
int Potentials::Getwidth() {
    return width;
}
//////////////////////////////////////////////////////////////////////
double Potentials::Getvbias() {
	return vbias;
}
//////////////////////////////////////////////////////////////////////
double Potentials::Getvdepl() {
	return vdepl;
}
//////////////////////////////////////////////////////////////////////
double Potentials::Getdpot(int posy, int posx, int posz) {
	return dpot[posy][posx][posz];
}
//////////////////////////////////////////////////////////////////////
double Potentials::Getwpot(int posy, int posx, int posz) {
    return wpot[posy][posx][posz];
}
/////////////////////////////////////////////////////////////////////
int Potentials::Getfix(int posy, int posx, int posz) {
	return fix[posy][posx][posz];
}
//////////////////////////////////////////////////////////////////////
void Potentials::Setdpot(int posy, int posx, int posz, double val) {
    dpot[posy][posx][posz]=val;
}
//////////////////////////////////////////////////////////////////////
void Potentials::Setwpot(int posy, int posx, int posz, double val) {
    wpot[posy][posx][posz]=val;
}
//////////////////////////////////////////////////////////////////////
void Potentials::SetV(double bias, double depl) {
    vbias=bias;
    vdepl=depl;
}
//////////////////////////////////////////////////////////////////////
void Potentials::SetPitchWidthXY(int dimy, int dimx, int dimz, int p, int w, int ZCUT)
{
    ref=0;
	multig=(int)((log10(dimy*dimx/3)/log(4)))+1;
	finest=dimy;
	Zfinest=dimz;

	XMAX=dimx;
	YMAX=dimy;
	ZMAX=dimz;
	pitch=p;
	width=w;
	Zcut=ZCUT;
       dpot = new double**[YMAX];
        for (int j=0; j<YMAX; j++) {
                dpot[j] = new double*[XMAX];
                for (int i=0; i<XMAX; i++) {
                        dpot[j][i] = new double[ZMAX];
                }
        }

    wpot = new double**[YMAX];
        for (int j=0; j<YMAX; j++) {
                wpot[j] = new double*[XMAX];
                for (int i=0; i<XMAX; i++) {
                        wpot[j][i] = new double[ZMAX];
                }
        }

        fix = new int**[YMAX];
        for (int j=0; j<YMAX; j++) {
			fix[j] = new int*[XMAX];
			for (int i=0; i<XMAX; i++) { 
				fix[j][i] = new int[ZMAX];
            }
        }


        for (int j=0; j<YMAX; j++) {
        	for (int i=0; i<XMAX; i++) {
        		for (int k=0; k<ZMAX; k++) {
                    dpot[j][i][k]=0.0;
                    wpot[j][i][k]=0.0;
                    fix[j][i][k]=0;
                }
 	        }
		}
}
//////////////////////////////////////////////////////////////////////
void Potentials::SetBoundaryConditions()
{
	for (int i=0; i<XMAX; i++) {

		for (int j=0; j<ZMAX; j++) {		
			dpot[0][i][j]=vbias+vdepl;
			wpot[0][i][j]=1.0;
			fix[0][i][j]=1;             
		}
	}	
		count=XMAX/pitch;
		count2=ZMAX/pitch;

	for (int n=0; n<(count/2); n++)	{
		for (int m=0; m<(count2/2); m++)	{
			for (int i=0; i<width; i++) {
				for (int j=0; j<width; j++) {
	 				if (n+m==0) fix[YMAX-1][XMAX/2-1-width/2+i][ZMAX/2-1-width/2+j]=2;
						/*else if (n==0){
							fix[YMAX-1][XMAX/2-1-width/2+i][ZMAX/2-1+m*pitch-width/2+j]=1;
							fix[YMAX-1][XMAX/2-1-width/2+i][ZMAX/2-1-m*pitch+width/2-j]=1;
						}
						else if (m==0){
							fix[YMAX-1][XMAX/2-1+n*pitch-width/2+i][ZMAX/2-1-width/2+j]=1;
							fix[YMAX-1][XMAX/2-1-n*pitch+width/2+i][ZMAX/2-1+width/2-j]=1;
						}*/
							else{
								fix[YMAX-1][XMAX/2-1+n*pitch-width/2+i][ZMAX/2-1+m*pitch-width/2+j]=1;
								fix[YMAX-1][XMAX/2-1-n*pitch+width/2-i][ZMAX/2-1+m*pitch-width/2+j]=1;
								fix[YMAX-1][XMAX/2-1+n*pitch-width/2+i][ZMAX/2-1-m*pitch+width/2-j]=1;
								fix[YMAX-1][XMAX/2-1-n*pitch+width/2-i][ZMAX/2-1-m*pitch+width/2-j]=1;
					
							}
				}
			}
	
		}
	}
}
//////////////////////////////////////////////////////////////////////
void Potentials::Restrictor()
{
    int z=0,s=0,t=0;    // z: lines, s: columns t:z
    ref=ref+1;
    Potentials newpot((YMAX+1)/2,(XMAX+1)/2,(ZMAX+1)/2);

    for(int k=0; k<(ZMAX+1)/2; k++) {
        z=0;
  	    for(int i=0; i<(YMAX+1)/2; i++) {
      		 s=0;
       		 for(int j=0; j<(XMAX+1)/2; j++) {
           		 newpot.dpot[i][j][k]=dpot[z][s][t];
           		 newpot.wpot[i][j][k]=wpot[z][s][t];
           		 s=s+2;
       		 }
       		 z=z+2;
   		 }
		t=t+2;
	}
   
	XMAX=(XMAX+1)/2;
    YMAX=(YMAX+1)/2;
    ZMAX=(ZMAX+1)/2;

    dpot = new double**[YMAX];
    for (int i=0; i<YMAX; i++){
		dpot[i] = new double*[XMAX];
   		for (int j=0; j<XMAX; j++){
			dpot[i][j] = new double[ZMAX];
		}
	}
    
	wpot = new double**[YMAX];
    for (int i=0; i<YMAX; i++){
		wpot[i] = new double*[XMAX];
    	for (int j=0; j<XMAX; j++){
			wpot[i][j] = new double[ZMAX];
		}
	}

    for(int i=0; i<YMAX; i++) { 
        for(int j=0; j<XMAX; j++) {
       		 for(int k=0; k<ZMAX; k++) {
           	 dpot[i][j][k]=newpot.dpot[i][j][k];
           	 wpot[i][j][k]=newpot.wpot[i][j][k];
       		 }
   		 }
	}
}
//////////////////////////////////////////////////////////////////////
int*** Potentials::FixRestrictor()
{
    int s=0;
    int t=0;
	int ***nfix=0;

	nfix = new int**[YMAX];
	for (int j=0; j<YMAX; j++) {
		nfix[j] = new int*[XMAX];
		for (int i=0; i<XMAX; i++) {
			nfix[j][i] = new int[ZMAX];
		}
	}

		//bottom
	for (int i=0; i<XMAX; i++){
		for (int k=0; k<ZMAX; k++){ 
			nfix[0][i][k]=1;
		
		//middle
			for (int j=1; j<YMAX-1; j++) {
			nfix[j][i][k]=0;
			}
		}	
	}	
		//top
	for (int i=0; i<XMAX; i++){
		t=0;
		for (int k=0; k<ZMAX; k++){ 
			nfix[YMAX-1][i][k]=fix[finest-1][s][t];
			t=t+(int)pow(2,ref);
		}	
		s=s+(int)pow(2,ref);
	}
	return nfix;
}
//////////////////////////////////////////////////////////////////////
void Potentials::Iteration(void *wfgui)
{
	double sum=0.0;
	int it=0;
	double err=(vdepl+vbias)*1.0e-5;
	int N;

    WFGUI* gui= (WFGUI*) wfgui;

/*	if (YMAX>(finest+1)/2) N=1000;
	else if (YMAX>(((finest+1)/2+1)/2)) N=700;
	else N=500;*/

	N=1000;

	int ***tempfix;
	tempfix = new int**[YMAX];
	for (int j=0; j<YMAX; j++) {
		tempfix[j] = new int*[XMAX];
		for (int i=0; i<XMAX; i++) {
			tempfix[j][i] = new int[ZMAX];
		}
	}
	
	tempfix=FixRestrictor();

	poissonf=-2.0*vdepl/pow(finest,2);
	
	while(1)
	{
		if (ref==0) err=(vdepl+vbias)*1.0e-4;

		Potentials newpot(YMAX,XMAX,ZMAX);

		// calculations
		for (int i=0; i<XMAX; i++)	{
			for (int k=0; k<ZMAX; k++) {
			// bottom
				newpot.dpot[0][i][k]=dpot[0][i][k];
				newpot.wpot[0][i][k]=wpot[0][i][k];

			// top
				if (tempfix[YMAX-1][i][k]==0) {
					newpot.dpot[YMAX-1][i][k]=1/6.0*(dpot[YMAX-1][(i-1+XMAX)%(XMAX)][k]+dpot[YMAX-1][(i+1+XMAX)%(XMAX)][k]+dpot[YMAX-2][i][k]+dpot[YMAX-1][i][k]+dpot[YMAX-1][i][(k+1+ZMAX)%(ZMAX)]+dpot[YMAX-1][i][(k-1+ZMAX)%(ZMAX)]-poissonf);
					newpot.wpot[YMAX-1][i][k]=1/6.0*(wpot[YMAX-1][(i-1+XMAX)%(XMAX)][k]+wpot[YMAX-1][(i+1+XMAX)%(XMAX)][k]+wpot[YMAX-2][i][k]+wpot[YMAX-1][i][k]+wpot[YMAX-1][i][(k+1+ZMAX)%(ZMAX)]+wpot[YMAX-1][i][(k-1+ZMAX)%(ZMAX)]);
				}
				else if (tempfix[YMAX-1][i][k]==1) {
					newpot.dpot[YMAX-1][i][k]=dpot[YMAX-1][i][k];
					newpot.wpot[YMAX-1][i][k]=1/6.0*(wpot[YMAX-1][(i-1+XMAX)%(XMAX)][k]+wpot[YMAX-1][(i+1+XMAX)%(XMAX)][k]+wpot[YMAX-2][i][k]+wpot[YMAX-1][i][k]+wpot[YMAX-1][i][(k+1+ZMAX)%(ZMAX)]+wpot[YMAX-1][i][(k-1+ZMAX)%(ZMAX)]);
				}	
				else {
					newpot.dpot[YMAX-1][i][k]=dpot[YMAX-1][i][k];
					newpot.wpot[YMAX-1][i][k]=wpot[YMAX-1][i][k];
				}

            // middle
        	    for (int j=1; j<YMAX-1; j++) {
                	newpot.dpot[j][i][k]=1/6.0*(dpot[j+1][i][k]+dpot[j-1][i][k]+dpot[j][(i+1+XMAX)%(XMAX)][k]+dpot[j][(i-1+XMAX)%(XMAX)][k]+dpot[j][i][(k+1+ZMAX)%(ZMAX)]+dpot[j][i][(k-1+ZMAX)%(ZMAX)]-poissonf);
                   	newpot.wpot[j][i][k]=1/6.0*(wpot[j+1][i][k]+wpot[j-1][i][k]+wpot[j][(i+1+XMAX)%(XMAX)][k]+wpot[j][(i-1+XMAX)%(XMAX)][k]+wpot[j][i][(k+1+ZMAX)%(ZMAX)]+wpot[j][i][(k-1+ZMAX)%(ZMAX)]);
            	}

			}
		}

		if (it%100==0) {
			sum=0.0;
			
			for (int i=0; i<XMAX; i++) {
				for (int j=0; j<YMAX; j++) {
					for (int k=0; k<ZMAX; k++) {
					sum+=fabs(dpot[j][i][k]-newpot.dpot[j][i][k]);
					}
				}
			}

			if (it%N==0) {
				gui->Getdhist()->Reset();
				gui->Getdhist()->TH2F::SetBins((XMAX),-(XMAX)*0.5,(XMAX)*0.5,(YMAX),-(YMAX)*0.5,(YMAX)*0.5);
	
				for(int i=0;i<XMAX;i++) {
					for(int j=0;j<YMAX;j++) {
						gui->Getdhist()->SetBinContent(i+1,j+1,dpot[j][i][(int)(ZMAX/2+(Zcut+pow(2,ref)-1)/pow(2,ref))]);
					}
				}
                
				gui->Getcanvasp()->cd();	
				gui->Getdhist()->Draw("COLZ");
				gStyle->SetPalette(55.0);
				gStyle->SetNumberContours(99);
				gui->Getcanvasp()->Update();
			}
			
			if (sum/((double)(XMAX*YMAX*ZMAX))<err) {
				for (int i=0; i<XMAX; i++) {
					for (int j=0; j<YMAX; j++) {
						for (int k=0; k<ZMAX; k++) {
							dpot[j][i][k]=newpot.dpot[j][i][k];
							wpot[j][i][k]=newpot.wpot[j][i][k];
						}
					}
				}
				break;
			}
		}	

		for (int i=0; i<XMAX; i++) {
			for (int j=0; j<YMAX; j++) {
				for (int k=0; k<ZMAX; k++) {
					dpot[j][i][k]=newpot.dpot[j][i][k];
					wpot[j][i][k]=newpot.wpot[j][i][k];
				}
			}
		}
	
		it++;
	}
}
//////////////////////////////////////////////////////////////////////
void Potentials::Prolongation()
{
	ref=ref-1;
	XMAX=2*XMAX-1;
	YMAX=2*YMAX-1;
    ZMAX=2*ZMAX-1; 
	int z=0,s=0,t=0;
	
	int ***tempfix;
	tempfix = new int**[YMAX];
	for (int j=0; j<YMAX; j++) {
		tempfix[j] = new int*[XMAX];
		for (int i=0; i<XMAX; i++) {
			tempfix[j][i] = new int[ZMAX];
		}
	}	
	tempfix=FixRestrictor();  

	Potentials newpot(YMAX,XMAX,ZMAX);

	for(int i=0;i<YMAX;i=i+2) {
		s=0;
		for(int j=0;j<XMAX;j=j+2) {
			t=0;
			for(int k=0; k<ZMAX;k=k+2) {
				newpot.dpot[i][j][k]=dpot[z][s][t];
				newpot.wpot[i][j][k]=wpot[z][s][t];
				t++;
			}
			s++;
		}
		z++;
	}		

	//bottom
	for (int i=0; i<XMAX; i++) {
		for (int k=0; k<ZMAX; k++) {
			newpot.dpot[0][i][k]=vbias+vdepl;
			newpot.wpot[0][i][k]=1.0;
  	 	}	
	}

	//top
	for (int k=1; k<ZMAX; k=k+2) {
		for (int i=1; i<XMAX; i=i+2) {
			if (tempfix[YMAX-1][i][k]==0) {
				newpot.dpot[YMAX-1][i][k]=0.2*(newpot.dpot[YMAX-1][(i-1+XMAX)%(XMAX)][(k-1+ZMAX)%(ZMAX)]+newpot.dpot[YMAX-2][i][k]+newpot.dpot[YMAX-1][(i+1+XMAX)%(XMAX)][(k-1+ZMAX)%(ZMAX)]+newpot.dpot[YMAX-1][(i+1+XMAX)%(XMAX)][(k+1+ZMAX)%(ZMAX)]+newpot.dpot[YMAX-1][(i-1+XMAX)%(XMAX)][(k+1+ZMAX)%(ZMAX)]);
				newpot.wpot[YMAX-1][i][k]=0.2*(newpot.wpot[YMAX-1][(i-1+XMAX)%(XMAX)][(k-1+ZMAX)%(ZMAX)]+newpot.wpot[YMAX-2][i][k]+newpot.wpot[YMAX-1][(i+1+XMAX)%(XMAX)][(k-1+ZMAX)%(ZMAX)]+newpot.wpot[YMAX-1][(i+1+XMAX)%(XMAX)][(k+1+ZMAX)%(ZMAX)]+newpot.wpot[YMAX-1][(i-1+XMAX)%(XMAX)][(k+1+ZMAX)%(ZMAX)]);
			}
			else if (tempfix[YMAX-1][i][k]==1) {
				newpot.dpot[YMAX-1][i][k]=0.0;
				newpot.wpot[YMAX-1][i][k]=0.2*(newpot.wpot[YMAX-1][(i-1+XMAX)%(XMAX)][(k-1+ZMAX)%(ZMAX)]+newpot.wpot[YMAX-2][i][k]+newpot.wpot[YMAX-1][(i+1+XMAX)%(XMAX)][(k-1+ZMAX)%(ZMAX)]+newpot.wpot[YMAX-1][(i+1+XMAX)%(XMAX)][(k+1+ZMAX)%(ZMAX)]+newpot.wpot[YMAX-1][(i-1+XMAX)%(XMAX)][(k+1+ZMAX)%(ZMAX)]);
			}
			else {
				newpot.dpot[YMAX-1][i][k]=0.0;
				newpot.wpot[YMAX-1][i][k]=0.0;
			}
		}
	}
	for (int i=0; i<XMAX; i++) {
		for (int k=(i+1)%2; k<ZMAX; k=k+2) {
			if (tempfix[YMAX-1][i][k]==0) {
				newpot.dpot[YMAX-1][i][k]=0.2*(newpot.dpot[YMAX-1][(i-1+XMAX)%(XMAX)][k]+newpot.dpot[YMAX-2][i][k]+newpot.dpot[YMAX-1][(i+1+XMAX)%(XMAX)][k]+newpot.dpot[YMAX-1][i][(k-1+ZMAX)%(ZMAX)]+newpot.dpot[YMAX-1][i][(k+1+ZMAX)%(ZMAX)]);
				newpot.wpot[YMAX-1][i][k]=0.2*(newpot.wpot[YMAX-1][(i-1+XMAX)%(XMAX)][k]+newpot.wpot[YMAX-2][i][k]+newpot.wpot[YMAX-1][(i+1+XMAX)%(XMAX)][k]+newpot.wpot[YMAX-1][i][(k-1+ZMAX)%(ZMAX)]+newpot.wpot[YMAX-1][i][(k+1+ZMAX)%(ZMAX)]);
			}
			else if (tempfix[YMAX-1][i][k]==1) {
				newpot.dpot[YMAX-1][i][k]=0.0;
				newpot.wpot[YMAX-1][i][k]=0.2*(newpot.wpot[YMAX-1][(i-1+XMAX)%(XMAX)][k]+newpot.wpot[YMAX-2][i][k]+newpot.wpot[YMAX-1][(i+1+XMAX)%(XMAX)][k]+newpot.wpot[YMAX-1][i][(k-1+ZMAX)%(ZMAX)]+newpot.wpot[YMAX-1][i][(k+1+ZMAX)%(ZMAX)]);
			}
			else {
				newpot.dpot[YMAX-1][i][k]=0.0;
				newpot.wpot[YMAX-1][i][k]=0.0;
			}
		}
	}



	for (int j=2; j<YMAX-1; j=j+2){
		for (int i=1; i<XMAX-1; i=i+2) {        // calculate mean value for grid points without 4 neighbours
        	for (int k=1; k<ZMAX-1; k=k+2) {
            	newpot.dpot[j][i][k]=0.25*(newpot.dpot[j][i-1][k-1]+newpot.dpot[j][i+1][k-1]+newpot.dpot[j][i-1][k+1]+newpot.dpot[j][i+1][k+1]);
            	newpot.wpot[j][i][k]=0.25*(newpot.wpot[j][i-1][k-1]+newpot.wpot[j][i+1][k-1]+newpot.wpot[j][i-1][k+1]+newpot.wpot[j][i+1][k+1]);
        	}
    	}

	    for (int k=1; k<ZMAX-1; k++) {
    	    for (int i=(k+1)%2; i<XMAX; i=i+2) {
        	    newpot.dpot[j][i][k]=0.25*(newpot.dpot[j][(i-1+XMAX)%(XMAX)][k]+newpot.dpot[j][i][(k+1+ZMAX)%(ZMAX)]+newpot.dpot[j][i][(k-1+ZMAX)%(ZMAX)]+newpot.dpot[j][(i+1+XMAX)%(XMAX)][k]);
            	newpot.wpot[j][i][k]=0.25*(newpot.wpot[j][(i-1+XMAX)%(XMAX)][k]+newpot.wpot[j][i][(k+1+ZMAX)%(ZMAX)]+newpot.wpot[j][i][(k-1+ZMAX)%(ZMAX)]+newpot.wpot[j][(i+1+XMAX)%(XMAX)][k]);
        	}
    	}
	}
	for (int j=1; j<YMAX-1; j=j+2){
		for (int i=0; i<XMAX; i++) {        
        	for (int k=0; k<ZMAX; k++) {
            	newpot.dpot[j][i][k]=0.5*(newpot.dpot[j-1][i][k]+newpot.dpot[j+1][i][k]);
            	newpot.wpot[j][i][k]=0.5*(newpot.wpot[j-1][i][k]+newpot.wpot[j+1][i][k]);
        	}
		}
	}

/*	for (int k=1; k<ZMAX-1; k=k+2) {		//bcc
		for ( int i=1; i<XMAX-1; i=i+2) {
         	for (int j=1; j<YMAX-1; j=j+2) {
				newpot.dpot[j][i][k]=0.125*(newpot.dpot[j-1][i-1][k-1]+newpot.dpot[j-1][i+1][k-1]+newpot.dpot[j-1][i+1][k+1]+newpot.dpot[j-1][i-1][k+1]+newpot.dpot[j+1][i-1][k-1]+newpot.dpot[j+1][i-1][k+1]+newpot.dpot[j+1][i+1][k+1]+newpot.dpot[j+1][i+1][k-1]);
				newpot.wpot[j][i][k]=0.125*(newpot.wpot[j-1][i-1][k-1]+newpot.wpot[j-1][i+1][k-1]+newpot.wpot[j-1][i+1][k+1]+newpot.wpot[j-1][i-1][k+1]+newpot.wpot[j+1][i-1][k-1]+newpot.wpot[j+1][i-1][k+1]+newpot.wpot[j+1][i+1][k+1]+newpot.wpot[j+1][i+1][k-1]);
			}
		}
	}

	//bottom
	for (int i=0; i<XMAX; i++) {
		for (int k=0; k<ZMAX; k++) {
			newpot.dpot[0][i][k]=vbias+vdepl;
			newpot.wpot[0][i][k]=1.0;
  	 	}	
	}

	//top
	for (int k=1; k<ZMAX; k=k+2) {
		for (int i=1; i<XMAX; i=i+2) {
			if (tempfix[YMAX-1][i][k]==0) {
				newpot.dpot[YMAX-1][i][k]=0.2*(newpot.dpot[YMAX-1][(i-1+XMAX)%(XMAX)][(k-1+ZMAX)%(ZMAX)]+newpot.dpot[YMAX-2][i][k]+newpot.dpot[YMAX-1][(i+1+XMAX)%(XMAX)][(k-1+ZMAX)%(ZMAX)]+newpot.dpot[YMAX-1][(i+1+XMAX)%(XMAX)][(k+1+ZMAX)%(ZMAX)]+newpot.dpot[YMAX-1][(i-1+XMAX)%(XMAX)][(k+1+ZMAX)%(ZMAX)]);
				newpot.wpot[YMAX-1][i][k]=0.2*(newpot.wpot[YMAX-1][(i-1+XMAX)%(XMAX)][(k-1+ZMAX)%(ZMAX)]+newpot.wpot[YMAX-2][i][k]+newpot.wpot[YMAX-1][(i+1+XMAX)%(XMAX)][(k-1+ZMAX)%(ZMAX)]+newpot.wpot[YMAX-1][(i+1+XMAX)%(XMAX)][(k+1+ZMAX)%(ZMAX)]+newpot.wpot[YMAX-1][(i-1+XMAX)%(XMAX)][(k+1+ZMAX)%(ZMAX)]);
			}
			else if (tempfix[YMAX-1][i][k]==1) {
				newpot.dpot[YMAX-1][i][k]=0.0;
				newpot.wpot[YMAX-1][i][k]=0.2*(newpot.wpot[YMAX-1][(i-1+XMAX)%(XMAX)][(k-1+ZMAX)%(ZMAX)]+newpot.wpot[YMAX-2][i][k]+newpot.wpot[YMAX-1][(i+1+XMAX)%(XMAX)][(k-1+ZMAX)%(ZMAX)]+newpot.wpot[YMAX-1][(i+1+XMAX)%(XMAX)][(k+1+ZMAX)%(ZMAX)]+newpot.wpot[YMAX-1][(i-1+XMAX)%(XMAX)][(k+1+ZMAX)%(ZMAX)]);
			}
			else {
				newpot.dpot[YMAX-1][i][k]=0.0;
				newpot.wpot[YMAX-1][i][k]=0.0;
			}
		}
	}
	for (int i=0; i<XMAX; i++) {
		for (int k=(i+1)%2; k<ZMAX; k=k+2) {
			if (tempfix[YMAX-1][i][k]==0) {
				newpot.dpot[YMAX-1][i][k]=0.2*(newpot.dpot[YMAX-1][(i-1+XMAX)%(XMAX)][k]+newpot.dpot[YMAX-2][i][k]+newpot.dpot[YMAX-1][(i+1+XMAX)%(XMAX)][k]+newpot.dpot[YMAX-1][i][(k-1+ZMAX)%(ZMAX)]+newpot.dpot[YMAX-1][i][(k+1+ZMAX)%(ZMAX)]);
				newpot.wpot[YMAX-1][i][k]=0.2*(newpot.wpot[YMAX-1][(i-1+XMAX)%(XMAX)][k]+newpot.wpot[YMAX-2][i][k]+newpot.wpot[YMAX-1][(i+1+XMAX)%(XMAX)][k]+newpot.wpot[YMAX-1][i][(k-1+ZMAX)%(ZMAX)]+newpot.wpot[YMAX-1][i][(k+1+ZMAX)%(ZMAX)]);
			}
			else if (tempfix[YMAX-1][i][k]==1) {
				newpot.dpot[YMAX-1][i][k]=0.0;
				newpot.wpot[YMAX-1][i][k]=0.2*(newpot.wpot[YMAX-1][(i-1+XMAX)%(XMAX)][k]+newpot.wpot[YMAX-2][i][k]+newpot.wpot[YMAX-1][(i+1+XMAX)%(XMAX)][k]+newpot.wpot[YMAX-1][i][(k-1+ZMAX)%(ZMAX)]+newpot.wpot[YMAX-1][i][(k+1+ZMAX)%(ZMAX)]);
			}
			else {
				newpot.dpot[YMAX-1][i][k]=0.0;
				newpot.wpot[YMAX-1][i][k]=0.0;
			}
		}
	}

	//middle
	for (int j=1; j<YMAX-1; j++) {		
		for (int i=(j+1)%2; i<XMAX; i=i+2) {
			for (int k=(j+1)%2; k<ZMAX; k=k+2) {
				newpot.dpot[j][i][k]=1/6.0*(newpot.dpot[j][(i-1+XMAX)%(XMAX)][(k-1+ZMAX)%(ZMAX)]+newpot.dpot[j+1][i][k]+newpot.dpot[j-1][i][k]+newpot.dpot[j][(i+1+XMAX)%(XMAX)][(k-1+ZMAX)%(ZMAX)]+newpot.dpot[j][(i+1+XMAX)%(XMAX)][(k+1+ZMAX)%(ZMAX)]+newpot.dpot[j][(i-1+XMAX)%(XMAX)][(k+1+ZMAX)%(ZMAX)]);
				newpot.wpot[j][i][k]=1/6.0*(newpot.wpot[j][(i-1+XMAX)%(XMAX)][(k-1+ZMAX)%(ZMAX)]+newpot.wpot[j+1][i][k]+newpot.wpot[j-1][i][k]+newpot.wpot[j][(i+1+XMAX)%(XMAX)][(k-1+ZMAX)%(ZMAX)]+newpot.wpot[j][(i+1+XMAX)%(XMAX)][(k+1+ZMAX)%(ZMAX)]+newpot.wpot[j][(i-1+XMAX)%(XMAX)][(k+1+ZMAX)%(ZMAX)]);
			}
		}
	}
	for (int j=1; j<YMAX-1; j++) {
		for (int i=0; i<XMAX; i++) {
			for (int k=(i+1)%2; k<ZMAX; k=k+2) {
				newpot.dpot[j][i][k]=1/6.0*(newpot.dpot[j][(i-1+XMAX)%(XMAX)][k]+newpot.dpot[j+1][i][k]+newpot.dpot[j-1][i][k]+newpot.dpot[j][(i+1+XMAX)%(XMAX)][k]+newpot.dpot[j][i][(k-1+ZMAX)%(ZMAX)]+newpot.dpot[j][i][(k+1+ZMAX)%(ZMAX)]);
				newpot.wpot[j][i][k]=1/6.0*(newpot.wpot[j][(i-1+XMAX)%(XMAX)][k]+newpot.wpot[j+1][i][k]+newpot.wpot[j-1][i][k]+newpot.wpot[j][(i+1+XMAX)%(XMAX)][k]+newpot.wpot[j][i][(k-1+ZMAX)%(ZMAX)]+newpot.wpot[j][i][(k+1+ZMAX)%(ZMAX)]);
			}
		}
	}
*/
	dpot = new double**[YMAX];
	for (int j=0; j<YMAX; j++){
		dpot[j] = new double*[XMAX];
        for (int i=0; i<XMAX; i++){
			dpot[j][i] = new double[ZMAX];
		}
	}

	wpot = new double**[YMAX];
	for (int j=0; j<YMAX; j++){ 
		wpot[j] = new double*[XMAX];
        for (int i=0; i<XMAX; i++){ 
			wpot[j][i] = new double[ZMAX];
		}
	}

	for (int k=0; k<ZMAX; k++) {
		for (int i=0; i<XMAX; i++) {
			for (int j=0; j<YMAX; j++) {
				dpot[j][i][k]=newpot.dpot[j][i][k];
				wpot[j][i][k]=newpot.wpot[j][i][k];
			}
		}
	}
}
//////////////////////////////////////////////////////////////////////
void Potentials::Multigrid(void *wfgui)
{
	WFGUI* gui= (WFGUI*) wfgui;	
	
	for (int i=0; i<multig; i++) Restrictor();
	
	Iteration(gui);
			
	for (int i=0; i<multig; i++) {
		Prolongation();
		Iteration(gui);
	}
}
///////////////////////////////////////////
Potentials::~Potentials()
{ // destructor
	for (int j=0; j<YMAX; j++) delete [] dpot[j] ;
	delete [] dpot;
	
	for (int j=0; j<YMAX; j++) delete [] wpot[j] ;
	delete [] wpot;

	for (int j=0; j<YMAX; j++) delete [] fix[j] ;
	delete [] fix;
}


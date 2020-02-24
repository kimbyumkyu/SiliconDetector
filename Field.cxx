#include "Field.h"
///////////////////////////////////////////////////////////////////
Field::Field() {
	x=0;
	y=0;
	z=0;
	abs=0;
}
///////////////////////////////////////////////////////////////////
double Field::GetFieldx() {
	return x;
}
///////////////////////////////////////////////////////////////////
double Field::GetFieldy() {
	return y;
}
///////////////////////////////////////////////////////////////////
double Field::GetFieldz() {
	return z;
}
///////////////////////////////////////////////////////////////////
double Field::Getabs() {
    return abs;
}
///////////////////////////////////////////////////////////////////
void Field::SetField(double valx, double valy, double valz) {
	x=valx;
	y=valy;
	z=valz;
}
///////////////////////////////////////////////////////////////////
void Field::Setabs(double val) {
	abs=val;
}
///////////////////////////////////////////////////////////////////
void CalculateFields(Potentials &pot, Field ***df, Field ***wf) {
	for (int k=0; k<pot.GetZMAX(); k++){
		for (int i=0; i<pot.GetXMAX(); i++){
			for (int j=1; j<pot.GetYMAX(); j++){
				df[j][i][k].SetField(-(pot.Getdpot(j,i,k)-pot.Getdpot(j,(i-1+pot.GetXMAX())%(pot.GetXMAX()),k))/GRIDDIST, -(pot.Getdpot(j,i,k)-pot.Getdpot(j-1,i,k))/GRIDDIST, -(pot.Getdpot(j,i,k)-pot.Getdpot(j,i,(k-1+pot.GetZMAX())%(pot.GetZMAX())))/GRIDDIST);
				wf[j][i][k].SetField(-(pot.Getwpot(j,i,k)-pot.Getwpot(j,(i-1+pot.GetXMAX())%(pot.GetXMAX()),k))/GRIDDIST, -(pot.Getwpot(j,i,k)-pot.Getwpot(j-1,i,k))/GRIDDIST, -(pot.Getwpot(j,i,k)-pot.Getwpot(j,i,(k-1+pot.GetZMAX())%(pot.GetZMAX())))/GRIDDIST);
			}

				if (pot.Getfix(0,i,k)){			
					df[0][i][k].SetField(-(pot.Getdpot(0,i,k)-pot.Getdpot(0,(i-1+pot.GetXMAX())%(pot.GetXMAX()),k))/GRIDDIST, 0 ,-(pot.Getdpot(0,i,k)-pot.Getdpot(0,i,(k-1+pot.GetZMAX())%(pot.GetZMAX())))/GRIDDIST);
					wf[0][i][k].SetField(-(pot.Getwpot(0,i,k)-pot.Getwpot(0,(i-1+pot.GetXMAX())%(pot.GetXMAX()),k))/GRIDDIST, -(pot.Getwpot(0,i,k)-pot.Getwpot(0,i,k))/GRIDDIST,-(pot.Getwpot(0,i,k)-pot.Getwpot(0,i,(k-1+pot.GetZMAX())%(pot.GetZMAX())))/GRIDDIST);
				}
		}
	}
}
///////////////////////////////////////////////////////////////////
void CalculateAbsFields(Potentials &pot, Field*** df) {
	for (int i=0; i<pot.GetXMAX(); i++){
		for (int j=0; j<pot.GetYMAX(); j++){
			for (int k=0; k<pot.GetZMAX(); k++){
			df[j][i][k].Setabs(sqrt(pow(df[j][i][k].GetFieldx(),2)+pow(df[j][i][k].GetFieldy(),2)+pow(df[j][i][k].GetFieldz(),2)));
			}
		}
	}
}
///////////////////////////////////////////////////////////////////
Field*** RotateField(Potentials &pot, Field*** df, double angle) {
	Field*** newdf;
	newdf = new Field**[pot.GetYMAX()];
	for (int j=0; j<pot.GetYMAX(); j++){
		newdf[j] = new Field*[pot.GetXMAX()];
		for (int i=0; i<pot.GetXMAX(); i++){
			newdf[j][i] = new Field[pot.GetZMAX()];
		}
	}

	for(int j=0; j<pot.GetYMAX(); j++) {
		for(int i=0; i<pot.GetXMAX(); i++) {
			for(int k=0; k<pot.GetZMAX(); k++) {
				newdf[j][i][k].SetField((cos(angle)*df[j][i][k].GetFieldx()-sin(angle)*df[j][i][k].GetFieldy())/cos(angle), (sin(angle)*df[j][i][k].GetFieldx()+cos(angle)*df[j][i][k].GetFieldy())/cos(angle), df[j][i][k].GetFieldz());
			}
		}
	}

	return newdf;
}
///////////////////////////////////////////////////////////////////
Field::~Field() {

}
///////////////////////////////////////////////////////////////////     

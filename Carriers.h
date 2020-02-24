#include "Field.h"
#include "TMath.h"
#include <stdlib.h>

#define EPSILON			8.854e-12       // vacuum permittivity
#define EPSILONR        11.9            // relative permittivity of silicon
#define ECHARGE     	1.602177e-19    // elementary charge in C

#define kB				1.38065e-23		// Boltzmann constant (J/K)
#define m0          	9.109383e-31    // electron mass (kg)
#define MU_E            0.1350      	// electron mobility in m2/Vs
#define MU_H            0.0480      	// electron mobility in m2/Vs
#define VESAT           1.1e5       	// electron saturation velocity in m/s
#define VHSAT           9.5e4       	// hole saturation velocity in m/s
#define SQMUEDIVVESAT   (MU_E/(VESAT))*(MU_E/(VESAT))
#define MUHDIVVHSAT     (MU_H/(VHSAT))

class Carriers {

    private:
        double x;
        double y;
        double z;
        int inside;
		double etheta;
		double htheta;
//		int mipcharge;

//		TLegend *legend;

    public:
        Carriers();
        ~Carriers();
        double Getx();
        double Gety();
        double Getz();
        int Getinside();
		double Getetheta();
		double Geththeta();
		void Setx(double);
        void Sety(double);
        void Setz(double);
        void Setinside(int);
		void Setetheta(double);
		void Seththeta(double);
};

void CreateCharges(Potentials &,Carriers *,int,int,void*);
void SetLorentz(Potentials&, Carriers*, double);
void CalculateCurrents(Potentials &, Field***, Field***, Carriers *, void*);

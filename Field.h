#include "Potentials.h"

class Field {

	private:
		double x,y,z;
		double abs;

	public:
		Field();
		virtual ~Field();

		double GetFieldx();
		double GetFieldy();
		double GetFieldz();
		double Getabs();

		void Setabs(double);
		void SetField(double,double,double);

		ClassDef(Field,0); 
};

void CalculateFields(Potentials&,Field***,Field***);
void CalculateAbsFields(Potentials&,Field***);
Field*** RotateField(Potentials &,Field***,double);

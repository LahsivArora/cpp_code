#ifndef __FRACTION_H__
#define __FRACTION_H__

#include <stdexcept>
#include <string>

class Fraction {
private:
	int numerator, denominator, sign;
	int LCM(int x, int y); //Lowest Common Multiplier
	int GCD(int x, int y); //Greatest Common Divisor
public:
	Fraction();                                     // default constructor; creates fraction 0/1
	Fraction(int n);                                // creates fraction n/1
	Fraction(int n,int d) throw(std::domain_error); // creates fraction n/d
	std::string getString(void);
	double getValue(void);
	Fraction operator!(void);
	Fraction operator+(Fraction arg);
	Fraction operator*(Fraction arg);
	Fraction operator/(Fraction arg) throw(std::domain_error);
	Fraction& operator+=(Fraction arg);
};

std::ostream& operator<< (std::ostream &ostr, Fraction &f);
#endif
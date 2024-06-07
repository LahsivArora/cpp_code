#include "fraction.h"

int Fraction::LCM(int x, int y) {
	int i = y;
	while(y % x)
		y += i;
	return y;
}

int Fraction::GCD(int x, int y) {
	for(;;)  {
		x %= y;
		if(!x)
			return y;
		y %= x;
		if(!y)
			return x;
	}
}

Fraction::Fraction(){
    numerator=0;
    denominator=1; 
}

Fraction::Fraction(int n){
    numerator=n;
    denominator=1;
}

Fraction::Fraction(int n,int d) throw(std::domain_error){
    sign = (n*d <0?-1:1) ;
    numerator=sign * abs(n);
    denominator=abs(d); 
	if(denominator == 0)
		throw std::domain_error("bad fraction");
}

std::string Fraction::getString(void) {
	std::string str = "[" + std::to_string(numerator) + "/" + std::to_string(denominator) + "]";
	return str;
}

double Fraction::getValue(void) {
	return double(numerator) / double(denominator);
}

Fraction Fraction::operator!(void) {
	int gcd = GCD(numerator, denominator);
	return Fraction(numerator / gcd, denominator / gcd);
}

Fraction Fraction::operator+(Fraction arg) {
	int common_denom = LCM(denominator, arg.denominator);
	int numera = numerator * common_denom / denominator + 
		     arg.numerator * common_denom / arg.denominator;
	Fraction f(numera, common_denom);
	return f;
}

Fraction Fraction::operator*(Fraction arg) {
	int numera = numerator * arg.numerator;
	int denomi = denominator * arg.denominator;
	Fraction f(numera, denomi);
	return !f;
}

Fraction Fraction::operator/(Fraction arg) throw(std::domain_error) {
	if(arg.numerator == 0)
		throw std::domain_error("division by zero");
	int numera = numerator * arg.denominator;
	int denomi = denominator * arg.numerator;
	Fraction f(numera, denomi);
	return !f;
}

Fraction& Fraction::operator+=(Fraction arg) {
	int common_denom = LCM(denominator, arg.denominator);
	int numera = numerator * common_denom / denominator + 
		     arg.numerator * common_denom / arg.denominator;
	numerator   = numera;
	denominator = common_denom;
	return *this;
}

std::ostream& operator<< (std::ostream &ostr, Fraction &f) {
	return ostr << f.getString();
}


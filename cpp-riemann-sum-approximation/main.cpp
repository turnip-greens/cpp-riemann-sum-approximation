#include <iostream>
#include <cstdint>
#include <math.h>
#include <string>

struct monomial
{
	float scalar;
	float exponent;

	float calc(float x)
	{
		return scalar * powf(x, exponent);
	}

	void define_mono(float s, float e)
	{
		scalar = s;
		exponent = e;
	}
};

struct standard_form_3
{
	monomial m[3];
	float constant;

	float calc(float x)
	{
		return m[0].calc(x) + m[1].calc(x) + m[2].calc(x) + constant;
	}
	
	void set_func(float s3, float e3, float s2, float e2, float s1, float e1, float c)
	{
		m[0].define_mono(s3, e3);
		m[1].define_mono(s2, e2);
		m[2].define_mono(s1, e1);
		constant = c;
	}

};

void prompt(std::string s, auto& v)
{
	std::cout << s;
	try 
	{
		std::cin >> v;
	}
	catch (...)
	{
		std::cout << "Invalid Value";
	}
}

int main()
{
	float s3, s2, s1, c;
	float lower_bound, upper_bound;
	uint32_t samples;
	
	prompt("Enter scalar of x^3: ", s3);
	prompt("Enter scalar of x^2: ", s2);
	prompt("Enter scalar of x: ", s1);
	prompt("Enter constant: ", c);

	standard_form_3 function;
	function.set_func(s3, 3, s2, 2, s1, 1, c);

	prompt("Enter lower bound: ", lower_bound);
	prompt("Enter upper bound: ", upper_bound);
	prompt("Enter samples (# of rectanges): ", samples);

	float step_size = (upper_bound - lower_bound) / samples;

	float total_area_insc = 0.f;
	float total_area_circ = 0.f;

	for (int i = 0; i < samples; i++)
	{
		total_area_insc += function.calc(lower_bound + i * step_size) * step_size;
		total_area_circ += function.calc(lower_bound + (i + 1) * step_size) * step_size;
	}

	std::cout << "\intergral " << lower_bound << "<->" << upper_bound << " = " << (total_area_circ + total_area_insc) / 2 << "\n";

	return 0;
}

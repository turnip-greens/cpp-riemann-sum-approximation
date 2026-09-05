#include <iostream>
#include <cstdint>
#include <string>
#include <cmath>

struct monomial
{
	float scalar;
	float exponent;

	float calc(float x)
	{
		return scalar * powf(x, exponent);
	}

	monomial(float s, float e) : scalar(s), exponent(e) {}
};

struct standard_form_3
{
	monomial m[3];
	float constant;

	float calc(float x)
	{
		return m[0].calc(x) + m[1].calc(x) + m[2].calc(x) + constant;
	}
	
	standard_form_3(float s3, float s2, float s1, float c)
		: m{ {s3, 3}, {s2, 2}, {s1, 1} }, constant(c) {}
};

void prompt(std::string s, auto& v)
{
	std::cout << s;
	
	while (true)
	{
		if (!(std::cin >> v))
		{
			std::cout << "Invalid Input; try again: ";
			std::cin.clear();
			std::cin.ignore(10000, '\n');
		}
		else
		{
			break;
		}
	}
}

int main()
{
	float s3, s2, s1, c;
	float lower_bound, upper_bound;
	int samples;
	
	prompt("Enter scalar of x^3: ", s3);
	prompt("Enter scalar of x^2: ", s2);
	prompt("Enter scalar of x: ", s1);
	prompt("Enter constant: ", c);

	standard_form_3 function(s3, s2, s1, c);

	prompt("Enter lower bound: ", lower_bound);
	prompt("Enter upper bound: ", upper_bound);
	prompt("Enter samples (# of rectanges): ", samples);

	float step_size = (upper_bound - lower_bound) / samples;

	float left_sum = 0.f;
	float right_sum = 0.f;

	for (int i = 0; i < samples; i++)
	{
		left_sum += function.calc(lower_bound + i * step_size) * step_size;
		right_sum += function.calc(lower_bound + (i + 1) * step_size) * step_size;
	}

	std::cout << "Intergral " << lower_bound << "<->" << upper_bound << " = " << (left_sum + right_sum) / 2 << "\n";

	return 0;
}

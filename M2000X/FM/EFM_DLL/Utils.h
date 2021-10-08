#pragma once

double round(const double& var, int digits = 1)
{
	// 37.66666 * 100 =3766.66
	// 3766.66 + .5 =3767.16    for rounding off value
	// then type cast to int so value is 3767
	// then divided by 100 so the value converted into 37.67

	int value = static_cast<int>((var * 10 * digits) + .5);
	return (double)value / (10.0 * digits);
}


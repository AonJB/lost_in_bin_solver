#include <stdio.h>
#include <stdlib.h>
#include "eqs.h"

int main(int argc, char** argv)
{
	vars4 ipt = {123, 123, 123, 123};
	vars opt1, opt2, opt3, opt4;
	vars coeff1, coeff2, coeff3, coeff4;

	opt1.v4 = apply_eq1(ipt);
	opt2.v3 = apply_eq2(ipt);
	opt3.v4 = apply_eq3(ipt);
	opt4.v3 = apply_eq4(ipt);

	printf("Results ===============\n");
	printf("eq1 : ");
	print_vars(opt1, VARS4, HEX);
	printf("eq2 : ");
	print_vars(opt2, VARS3, HEX);
	printf("eq3 : ");
	print_vars(opt3, VARS4, HEX);
	printf("eq4 : ");
	print_vars(opt4, VARS3, HEX);

	coeff1 = compute_coeffs(ipt, opt1, VARS4);
	coeff2 = compute_coeffs(ipt, opt2, VARS3);
	coeff3 = compute_coeffs(ipt, opt3, VARS4);
	coeff4 = compute_coeffs(ipt, opt4, VARS3);

	printf("Coeffs ===============\n");
	printf("eq1 : ");
	print_vars(coeff1, VARS4, DEC);
	printf("eq2 : ");
	print_vars(coeff2, VARS3, DEC);
	printf("eq3 : ");
	print_vars(coeff3, VARS4, DEC);
	printf("eq4 : ");
	print_vars(coeff4, VARS3, DEC);

	printf("Equations =============\n");
	printf("eq1 : ");
	print_eq(coeff1, VARS4, eq1_result);
	printf("eq2 : ");
	print_eq(coeff2, VARS3, eq2_result);
	printf("eq3 : ");
	print_eq(coeff3, VARS4, eq3_result);
	printf("eq4 : ");
	print_eq(coeff4, VARS3, eq4_result);

	return 0;
}

#include <stdint.h>
#include <stdlib.h>
#include <inttypes.h>

typedef struct
{
	int64_t a, b, c, d;
} vars4;

typedef struct
{
	int64_t ab, c, d;
} vars3;

typedef union
{
	vars4 v4;
	vars3 v3;
} vars;

typedef enum
{
	VARS4,
	VARS3
} VARS_TYPE;

typedef enum
{
	HEX,
	DEC
} PRINT_TYPE;

int64_t sum_vars(vars vars, VARS_TYPE type)
{
	switch (type)
	{
		case VARS4:
			return vars.v4.a + vars.v4.b + vars.v4.c + vars.v4.d;
		case VARS3:
			return vars.v3.ab + vars.v3.c + vars.v3.d;
	}
}

vars3 reduce_input(vars4 input)
{
	vars3 reduced = {input.a + input.b, input.c, input.d};
	return reduced;
}

vars compute_coeffs(vars4 input, vars eq_output, VARS_TYPE type)
{
	vars coeffs;
	if (type == VARS4)
	{
		coeffs.v4.a = eq_output.v4.a / input.a;
		coeffs.v4.b = eq_output.v4.b / input.b;
		coeffs.v4.c = eq_output.v4.c / input.c;
		coeffs.v4.d = eq_output.v4.d / input.d;
	} else if (type == VARS3)
	{
		vars3 reduced_input = reduce_input(input);
		coeffs.v3.ab = eq_output.v3.ab / reduced_input.ab;
		coeffs.v3.c = eq_output.v3.c / reduced_input.c;
		coeffs.v3.d = eq_output.v3.d / reduced_input.d;
	}
	return coeffs;
}

void print_vars(vars v, VARS_TYPE type, PRINT_TYPE display)
{
	if (type == VARS4)
	{
		switch (display)
		{
			case HEX:
				printf("a: 0x%" PRIx64 "; b: 0x%" PRIx64 "; c: 0x%" PRIx64 "; d: 0x%" PRIx64 ";\n", v.v4.a, v.v4.b, v.v4.c, v.v4.d);
				break;
			case DEC:
				printf("a: %" PRIi64 "; b: %" PRIi64 "; c: %" PRIi64 "; d: %" PRIi64 ";\n", v.v4.a, v.v4.b, v.v4.c, v.v4.d);
				break;
		}
	} else if (type == VARS3)
	{
		switch (display)
		{
			case HEX:
				printf("a+b: 0x%" PRIx64 "; c: 0x%" PRIx64 "; d: 0x%" PRIx64 ";\n", v.v3.ab, v.v3.c, v.v3.d);
				break;
			case DEC:
				printf("a+b: %" PRIi64 "; c: %" PRIi64 "; d: %" PRIi64 ";\n", v.v3.ab, v.v3.c, v.v3.d);
				break;
		}
	}
}

void print_eq(vars coeffs, VARS_TYPE type, int64_t eq_result)
{
	if (type == VARS4)
	{
		printf("%" PRIi64 "*a + %" PRIi64 "*b + %" PRIi64 "*c + %" PRIi64 "*d = %" PRIi64 "\n", coeffs.v4.a, coeffs.v4.b, coeffs.v4.c, coeffs.v4.d, eq_result);
	} else if (type == VARS3)
	{
		printf("%" PRIi64 "*a + %" PRIi64 "*b + %" PRIi64 "*c + %" PRIi64 "*d = %" PRIi64 "\n", coeffs.v3.ab, coeffs.v3.ab, coeffs.v3.c, coeffs.v3.d, eq_result);
	}
}

const int64_t eq1_result = 0xffffffffffffb827;
vars4 apply_eq1(vars4 input)
{
	vars4 output;
	asm (
			// a
			"mov %[a], %%rdx" "\n\t"
			"mov %%rdx, %%rax" "\n\t"
			"shl $2, %%rdx" "\n\t"
			"sub %%rdx, %%rax" "\n\t"
			"shl $3, %%rax" "\n\t"
			"mov %%rax, %[oa]" "\n\t"
			: [oa] "=rm" (output.a)
			: [a] "rm" (input.a)
			: "rax", "rdx"
		);
	asm (
			// b
			"mov %[b], %%rdx" "\n\t"
			"mov $0, %%eax" "\n\t"
			"sub %%rdx, %%rax" "\n\t"
			"shl $3, %%rax" "\n\t"
			"sub %%rdx, %%rax" "\n\t"
			"add %%rax, %%rax" "\n\t"
			"mov %%rax, %[ob]" "\n\t"
			: [ob] "=rm" (output.b)
			: [b] "rm" (input.b)
			: "rax", "rdx"
		);
	asm (
			// c
			"mov %[c], %%rax" "\n\t"
			"mov %%rax, %%rdx" "\n\t"
			"shl $4, %%rax" "\n\t"
			"sub %%rax, %%rdx" "\n\t"
			"mov %%rdx, %%rax" "\n\t"
			"mov %%rax, %[oc]" "\n\t"
			: [oc] "=rm" (output.c)
			: [c] "rm" (input.c)
			: "rax", "rdx"
		);
	asm (
			// d
			"mov %[d], %%rdx" "\n\t"
			"mov %%rdx, %%rax" "\n\t"
			"shl $2, %%rdx" "\n\t"
			"sub %%rdx, %%rax" "\n\t"
			"shl $2, %%rax" "\n\t"
			"mov %%rax, %[od]" "\n\t"
			: [od] "=rm" (output.d)
			: [d] "rm" (input.d)
			: "rax", "rdx"
		);
	return output;
}

const int64_t eq2_result = 0x1143;
vars3 apply_eq2(vars4 input)
{
	vars3 output;
	asm (
			// a+b
			"mov %[a], %%rdx" "\n\t"
			"mov %[b], %%rax" "\n\t"
			"add %%rax, %%rdx" "\n\t"
			"mov %%rdx, %%rax" "\n\t"
			"shl $3, %%rax" "\n\t"
			"add %%rdx, %%rax" "\n\t"
			"add %%rax, %%rax" "\n\t"
			"mov %%rax, %[oab]" "\n\t"
			: [oab] "=rm" (output.ab)
			: [a] "rm" (input.a), [b] "rm" (input.b)
			: "rax", "rdx"
		);
	asm (
			// c
			"mov %[c], %%rdx" "\n\t"
			"mov %%rdx, %%rax" "\n\t"
			"shl $3, %%rax" "\n\t"
			"add %%rdx, %%rax" "\n\t"
			"mov %%rax, %[oc]" "\n\t"
			: [oc] "=rm" (output.c)
			: [c] "rm" (input.c)
			: "rax", "rdx"
		);
	asm (
			// d
			"mov %[d], %%rdx" "\n\t"
			"mov $0, %%eax" "\n\t"
			"sub %%rdx, %%rax" "\n\t"
			"shl $3, %%rax" "\n\t"
			"sub %%rdx, %%rax" "\n\t"
			"mov %%rax, %[od]" "\n\t"
			: [od] "=rm" (output.d)
			: [d] "rm" (input.d)
			: "rax", "rdx"
		);
	return output;
}

const int64_t eq3_result = 0x1c84;
vars4 apply_eq3(vars4 input)
{
	vars4 output;
	asm (
			// a
			"mov %[a], %%rax" "\n\t"
			"shl $4, %%rax" "\n\t"
			"mov %%rax, %[oa]" "\n\t"
			: [oa] "=rm" (output.a)
			: [a] "rm" (input.a)
			: "rax", "rdx"
		);
	asm (
			// b
			"mov %[b], %%rdx" "\n\t"
			"mov %%rdx, %%rax" "\n\t"
			"add %%rax, %%rax" "\n\t"
			"add %%rdx, %%rax" "\n\t"
			"shl $2, %%rax" "\n\t"
			"mov %%rax, %[ob]" "\n\t"
			: [ob] "=rm" (output.b)
			: [b] "rm" (input.b)
			: "rax", "rdx"
		);
	asm (
			// c
			"mov %[c], %%rax" "\n\t"
			"shl $2, %%rax" "\n\t"
			"mov %%rax, %[oc]" "\n\t"
			: [oc] "=rm" (output.c)
			: [c] "rm" (input.c)
			: "rax", "rdx"
		);
	asm (
			// d
			"mov %[d], %%rax" "\n\t"
			"add %%rax, %%rax" "\n\t"
			"mov %%rax, %[od]" "\n\t"
			: [od] "=rm" (output.d)
			: [d] "rm" (input.d)
			: "rax", "rdx"
	   );
	return output;
}

const int64_t eq4_result = 0x0ffffffffffffde5b;
vars3 apply_eq4(vars4 input)
{
	vars3 output;
	asm (
			// a+b
			"mov %[a], %%rdx" "\n\t"
			"mov %[b], %%rax" "\n\t"
			"add %%rax, %%rdx" "\n\t"
			"mov %%rdx, %%rax" "\n\t"
			"shl $2, %%rdx" "\n\t"
			"sub %%rdx, %%rax" "\n\t"
			"add %%rax, %%rax" "\n\t"
			"mov %%rax, %[oab]" "\n\t"
			: [oab] "=rm" (output.ab)
			: [a] "rm" (input.a), [b] "rm" (input.b)
			: "rax", "rdx"
		);
	asm (
			// c
			"mov %[c], %%rax" "\n\t"
			"mov %%rax, %%rdx" "\n\t"
			"shl $2, %%rax" "\n\t"
			"sub %%rax, %%rdx" "\n\t"
			"mov %%rdx, %%rax" "\n\t"
			"mov %%rax, %[oc]" "\n\t"
			: [oc] "=rm" (output.c)
			: [c] "rm" (input.c)
			: "rax", "rdx"
		);
	asm (
			// d
			"mov %[d], %%rax" "\n\t"
			"mov %%rax, %%rdx" "\n\t"
			"lea 0x0(,%%rax, 0x4), %%rcx" "\n\t"
			"sub %%rcx, %%rdx" "\n\t"
			"shl $2, %%rdx" "\n\t"
			"add %%rdx, %%rax" "\n\t"
			"mov %%rax, %[od]" "\n\t"
			: [od] "=rm" (output.d)
			: [d] "rm" (input.d)
			: "rax", "rdx", "rcx"
		);
	return output;
}

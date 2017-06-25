#include <iostream>
#include <fstream>
#include <string>
#include <cstdio>
#include <sys/time.h>
#include <time.h>
#include <cstdlib>
#include <string.h>


using namespace std;

float p_a = 0, q_a = 1, p_b = 1, q_b = 1;
float alpha_num, d_num, denum;

// Comparacion de fraccion
bool menor(float a, float b, float c, float d){
	if(a*d < b*c)
		return true;
	else
		return false;
}

bool less_or_equal(float a, float b, float c, float d){
	if(a*d <= b*c)
		return true;
	else
		return false;
}

float matches(float a, float b){
	if(less_or_equal(a, b, alpha_num-d_num, denum) == true)
		return 0;
	if(menor(a, b, alpha_num + d_num, denum) == true)
		return 1;
	return 0;
}

void find_exact_solution_left(float p_a,float q_a,float p_b,float q_b){
	float k_num = denum*p_b - (alpha_num + d_num) * q_b;
	float k_denum = (alpha_num + d_num) * q_a - denum * p_a;
	float k = (k_num/k_denum) + 1;

	cout<<p_b + k * p_a << " "<<q_b + k*q_a;
}

void find_exact_solution_right(float p_a,float q_a,float p_b,float q_b){
	float k_num = -denum*p_b + (alpha_num - d_num) * q_b;
	float k_denum = -(alpha_num - d_num) * q_a + denum * p_a;
	float k = (k_num/k_denum) + 1;

	cout<<p_b + k * p_a << " "<<q_b + k*q_a;
}


int main(){

	cout<<"alpha_num: ";
	cin>>alpha_num;
	cout<<endl;
	cout<<"d_num: ";
	cin>>d_num;
	cout<<endl;
	cout<<"denum: ";
	cin>>denum;
	cout<<endl;

	int condicion = 0;

	float x, x_num, x_denum, aa, bb, new_p_a, new_p_b, new_q_a, new_q_b;

	while(condicion == 0){
		if(condicion==0){
			x_num = denum * p_b - alpha_num * q_b;
			x_denum = -denum * p_a + alpha_num * q_a;

			x = (x_num+x_denum-1) / x_denum;

			aa = matches(p_b+x*p_a, q_b+x*q_a);
			bb = matches(p_b+(x-1)*p_a, q_b+(x-1)*q_a);

			if(aa || bb){
				find_exact_solution_left(p_a, q_a, p_b, q_b);
				condicion = 1;
			}
		}

		if(condicion == 0){
			new_p_a = p_b + (x-1) * p_a;
			new_q_a = q_b + (x-1) * q_a;

			new_p_b = p_b + x*p_a;
			new_q_b = q_b + x*q_a;

			p_a = new_p_a;
			q_a = new_q_a;
			p_b = new_p_b;
			q_b= new_q_b;

			x_num = alpha_num * q_b - denum * p_b;
			x_denum = -alpha_num * q_a + denum * p_a;

			x = (x_num + x_denum - 1) / x_denum;

			aa = matches(p_b+x*p_a, q_b+x*q_a);
			bb = matches(p_b+(x-1)*p_a, q_b+(x-1)*q_a);

			if(aa || bb){
				find_exact_solution_right(p_a, q_a, p_b, q_b);
				condicion = 1;
			}

			new_p_a = p_b + (x-1) * p_a;
			new_q_a = q_b + (x-1) * q_a;

			new_p_b = p_b + x*p_a;
			new_q_b = q_b + x*q_a;

			p_a = new_p_a;
			q_a = new_q_a;
			p_b = new_p_b;
			q_b= new_q_b;

		}
	}


	return 0;
}
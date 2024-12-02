#include<iostream>
#include<omp.h>
#include<vector>
#include<string>
#include<cstdlib>
#include<ctime>
#include<cmath>
using namespace std;
auto Generate_random_space(int length,int number){
srand(time(0));
vector<string>x;
#pragma omp parallel for
for(int i=0;i<number;i++){
string s;
for (int j=0;j<length;j++){
    s+=rand()%2? '1' : '0';//Ternary Operator

}
x.push_back(s);
}
return x;

}
vector<string> Fitness(vector<string> space, vector<string> space2, int threshold) {
    vector<string> most_fit;

    // First loop: Process "space"
    #pragma omp parallel for
    for (int i = 0; i < space.size(); i++) {
        int coun = 0; // Reset coun for each string
        for (int j = 0; j < space[i].length(); j++) {
            if (space[i][j] == '1') coun++;
        }
        if (coun == threshold || coun == threshold - 1 || coun == threshold - 2 || coun == threshold - 3) {
            #pragma omp critical //Because it shared when adding at space it need to be syncronized
            most_fit.push_back(space[i]);
        }
    }

    // Second loop: Process "space2"
    #pragma omp parallel for
    for (int i = 0; i < space2.size(); i++) {
        int coun = 0; // Reset coun for each string
        for (int j = 0; j < space2[i].length(); j++) {
            if (space2[i][j] == '1') coun++;
        }
        if (coun == threshold || coun == threshold - 1 || coun == threshold - 2 || coun == threshold - 3) {
            #pragma omp critical
            most_fit.push_back(space2[i]);
        }
    }

    return most_fit;
}
vector<string> cross_over(vector<string> fitted_space) {
    vector<string> cross_overd_space;

    int half_size = fitted_space.size() / 2; // Calculate halfway point
    #pragma omp parallel for
    for (int i = 0; i < half_size; i++) {
        // Ensure strings have sufficient length
            string new_string = fitted_space[i].substr(0, 3) + fitted_space[half_size + i].substr(3, 6);
            cross_overd_space.push_back(new_string);

    }

    return cross_overd_space;
}
vector<string> mutate_space(vector<string>space){
    #pragma omp parallel for
    for(int i=0;i<space.size();i++){
        for(int j=0;j<space[i].size();j++){
            if(space[i][j]=='1')space[i][j]='0';
            else if(space[i][j]=='0')space[i][j]='1';

        }
    }
    return space;
}
void print_vector(vector<string>vec,string print){
for(int i=0;i<vec.size();i++){
    cout<<print+" "<<vec[i]<<endl;
}
}
int binary_to_Decimal(string x){
int decimal=0;
int length=x.length();
for(int i=0;i<x.length();i++){
    if(x[length - i - 1]=='0'){
        decimal+=0;
    }
    if(x[length - i - 1]=='1'){
        decimal+=pow(2,i);
    }

}
return decimal;
}
void add_binary_number(vector<string>final_space){
vector<string> soln_x;
vector<string> soln_y;
for(int i=0;i<final_space.size();i++){
    for(int j=0;j<final_space.size()-1;j++){
        if(binary_to_Decimal(final_space[i])+binary_to_Decimal(final_space[j])==20){
                soln_x.push_back(final_space[i]);
                soln_y.push_back(final_space[j]);
    }
    }
}
print_vector(soln_x,"X_solution");
print_vector(soln_y,"Y_solution");
}
int main(){
//X+Y=20
vector x=Generate_random_space(20,500000);
vector y=Generate_random_space(20,500000);
vector fit=Fitness(x,y,4);
vector cross_overed=cross_over(fit);
vector mutated_space=mutate_space(cross_overed);
print_vector(x,"space of variable x");
print_vector(y,"space of variable y");
print_vector(fit,"space of Most Fittness data");
print_vector(cross_overed,"space of cross overd");
print_vector(mutated_space,"space of mutated_space");
add_binary_number(cross_overed);
//cout<<binary_to_Decimal("0010101")<<endl;
return 0;
}

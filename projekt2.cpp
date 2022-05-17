#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

#define MAX_ITER 10000

class Polynomian {
public:
    int degree;
    vector<double> coefficients;
    Polynomian (vector<double> c) {
        int s = c.size();
        for (int i = 0; c[0] == 0 && i < s; i++)
            c.erase(c.begin());
        coefficients = c;
        degree = coefficients.size() - 1;
    }
    double value(double x);
};

double Polynomian::value(double x) {
    double v = 0;
    for (int i = 0, j = degree; i < degree + i && j >= 0; i++, j--)
        v += pow(x, j) * coefficients[i];
    return v;
}

double bisect(Polynomian p, double as, double bs, double ex, int& iter) {
    double a = as, b = bs;
    if (p.value(a) * p.value(b) > 0) return 0;
    double x = (a + b) / 2;
    iter++;
    while (abs(a - x) > ex && iter <= MAX_ITER) {
        if (p.value(a) * p.value(x) < 0) b = x;
        else a = x;
        x = (a + b) / 2;
        iter++;
    }
    return x;
}

double secantMethod(Polynomian p, double a, double b, double ex, int& iter) {
    double x_k1 = a, x_k2 = b;
    double x = x_k1 - (p.value(x_k1) * (x_k1 - x_k2)) / (p.value(x_k1) - p.value(x_k2));
    iter++;
    while (abs(x_k1 - x) > ex && iter <= MAX_ITER) {
        x_k2 = x_k1;
        x_k1 = x;
        x = x_k1 - (p.value(x_k1) * (x_k1 - x_k2)) / (p.value(x_k1) - p.value(x_k2));
        iter++;
    }
    return x;
}

void solve(Polynomian p, double a, double b, double e) {
    int iter = 0;
    if (p.value(a) * p.value(b) < 0) {
        cout << "Metoda polowienia: " << endl;
        cout << "\tWynik: " << bisect(p, a, b, e, iter) << endl;
        cout << "\tIlosc iteracji: " << iter << endl;
    }
    else {
        cout << "Metoda polowienia wymaga aby wartosci na krancach przedzialow mialy rozne znaki" << endl << endl;
    }
    iter = 0;
    cout << "Metoda siecznych: " << endl;
    cout << "\tWynik: " << secantMethod(p, a, b, e, iter) << endl;
    cout << "\tIlosc iteracji: " << iter << endl;
}

int main () {
    Polynomian p = Polynomian({1, 0, -22, 3});
    double e, a, b;
    int c;
    cout << "Podaj krance przedzialu <a, b>: " << endl;
    cin >> a >> b;
    if (cin.fail()) {
        cin.clear();
        cin.ignore(256,'\n');
        cout << "Nieprawidlowa wartosc, podaj jeszcze raz" << endl;
        cin >> a >> b;
    }
    cout << "Podaj dokladnosc(ilosc miejsc po przecinku): " << endl;
    cin >> c;
    if (cin.fail()) {
        cin.clear();
        cin.ignore(256,'\n');
        cout << "Nieprawidlowa wartosc, podaj jeszcze raz" << endl;
        cin >> c;
    }
    e = pow(10, -c);
    solve(p, a, b, e);
    return 0;
}

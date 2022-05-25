#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include <sstream>

using namespace std;

#define MAX_ITER 10000

class Polynomian {
public:
    int degree;
    vector<double> coefficients;
    Polynomian (vector<double> c) {
        int s = c.size();
        if (c.size() > 0)
            for (int i = 0; c[0] == 0 && i < s; i++)
                c.erase(c.begin());
        coefficients = c;
        degree = coefficients.size() - 1;
    }
    Polynomian (): degree{0} {}
    double value(double x);
};

double Polynomian::value(double x) {
    double v = 0;
    for (int i = 0, j = degree; i < degree + 1 && j >= 0; i++, j--)
        v += pow(x, j) * coefficients[i];
    return v;
}

double bisect(Polynomian p, double as, double bs, double ex, int& iter, double& error) {
    double a = as, b = bs;
    if (p.value(a) * p.value(b) > 0) return 0;
    double x = (a + b) / 2;
    iter++;
    while (abs(a - x) > ex && iter <= MAX_ITER) {
        iter++;
        if (p.value(a) * p.value(x) < 0) b = x;
        else if (p.value(x) * p.value(b) < 0) a = x;
        else break;
        x = (a + b) / 2;
    }
    error = abs(bs - as) / pow(2, iter);
    return x;
}

double secantMethod(Polynomian p, double a, double b, double ex, int& iter, double& error) {
    double x_k1 = a, x_k2 = b;
    double x = x_k1 - (p.value(x_k1) * (x_k1 - x_k2)) / (p.value(x_k1) - p.value(x_k2));
    iter++;
    while (abs(x_k1 - x) > ex && iter <= MAX_ITER) {
        x_k2 = x_k1;
        x_k1 = x;
        x = x_k1 - (p.value(x_k1) * (x_k1 - x_k2)) / (p.value(x_k1) - p.value(x_k2));
        iter++;
    //cout << x << " " << x_k1 << " " << x_k2 << " " << abs(x_k1 - x) << " " << p.value(x) << endl;
    }
    error = abs(x_k1 -x);
    return x;
}

void solve(Polynomian p, double a, double b, double e) {
    int iter = 0;
    double error;
    if (p.value(a) * p.value(b) < 0) {
        cout << "Metoda polowienia: " << endl;
        cout << "\tWynik: " << bisect(p, a, b, e, iter, error) << endl;
        cout << "\tIlosc iteracji: " << iter << endl;
        cout << "\tPrzyblizony blad: " << error << endl;
    }
    else {
        cout << "Metoda polowienia wymaga aby wartosci na krancach przedzialow mialy rozne znaki" << endl;
        if (p.coefficients == vector<double>{1, 0, -22, 3})
            cout << "Miejsca zerowe sa w przedzialach <-6;-2>; <-2;2>; <2;6>" << endl;
        cout << endl;
    }
    iter = 0;
    cout << "Metoda siecznych: " << endl;
    cout << "\tWynik: " << secantMethod(p, a, b, e, iter, error) << endl;
    cout << "\tIlosc iteracji: " << iter << endl;
    cout << "\tPrzyblizony blad: " << error << endl;
    cout << "Dokladnosc: " << e << endl;
}

int main () {
    Polynomian p0 = Polynomian({1, 0, -22, 3});
    double e, a, b;
    int c;
    int option;
    while (true) {
        Polynomian p;
        cout << "(1) Wielomian z zadania\n"
        "(2) Dowolny wielomian\n"
        "(3) Wyjscie\n";
        cin >> option;
        if (!cin.fail()) {
            cin.clear();
            cin.ignore(256,'\n');
            switch (option) {
                case 3: {return 0;}
                case 2: {
                    cout << "Podaj wspolczynniki(w jednej linii, oddzielone spacjami): " << endl;
                    vector<double> vc;
                    double c;
                    string line;
                    getline(cin, line);
                    istringstream iss(line);
                    while (iss >> c)
                        if (!iss.fail())
                            vc.push_back(c);
                    p = Polynomian(vc);
                    break;
                }
                case 1: {
                    p = p0;
                }
            }
            cout << "Podaj krance przedzialu <a, b>: " << endl;
            cin >> a >> b;
            while (cin.fail()) {
                cin.clear();
                cin.ignore(256,'\n');
                cout << "Nieprawidlowa wartosc, podaj jeszcze raz" << endl;
                cin >> a >> b;
            }
            cin.clear();
            cin.ignore(256,'\n');
            cout << "Podaj dokladnosc(ilosc miejsc po przecinku): " << endl;
            cin >> c;
            while (cin.fail()) {
                cin.clear();
                cin.ignore(256,'\n');
                cout << "Nieprawidlowa wartosc, podaj jeszcze raz" << endl;
                cin >> c;
            }
            e = pow(10, -c);
            solve(p, a, b, e);
        }
        cin.clear();
        cin.ignore(256,'\n');
    }
    return 0;
}

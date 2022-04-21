#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

int fact(int x) { //funkcja do obliczania silni
    if (x == 0)
        return 1;
    else
        return x * fact(x - 1);
}

vector<double> multiPoly(vector<double>& p1, vector<double>& p2) {
    vector<double> outcome(p1.size() + p2.size() - 1, 0);
    int i = p1.size() - 1;
    for (double& a : p1) {
        int j = p2.size() - 1;
        for (double& b : p2) {
            outcome[outcome.size() - i - j - 1] += a * b;
            j--;
        }
        i--;
    }
    return outcome;
}

vector<double> addPoly(vector<double>& p1, vector<double>& p2) {
    int s = max(p1.size(), p2.size()) - 1;
    vector<double> outcome(s + 1, 0);
    if (p1.size() >= p2.size()) {
        int i;
        for (i = s; i >= s - (int)p2.size() - 1; i--)
            outcome[i] += (p1[i] + p2[i - s + p2.size() - 1]);
        i++;
        for (; i >= 0; i--)
            outcome[i] = p1[i];
    }
    else {
        int i;
        for (i = s; i >= s - (int)p1.size(); i--)
            outcome[i] += (p2[i] + p1[i - s + p1.size() - 1]);
        i++;
        for (; i >= 0; i--)
            outcome[i] = p2[i];
    }
    return outcome;
}

void displayPoly(vector<double>& poly) {
    int i = poly.size() - 1;
    int skipped = 0;
    for (double a : poly) {
        if (a == 0) {
            i--;
            skipped++;
            continue;
        }
        if (i < (int)poly.size() - 1 - skipped) {
            if (a >= 0)
                cout << " + ";
            else {
                cout << " - ";
                a = -a;
            }
        }
        if (i > 1) {
            if(a == 1)
                cout << "x^" << i;
            else if(a == -1)
                cout << "-x^" << i;
            else
                cout << a << "x^" << i;
        }
        else if (i == 1){
            if(a == 1)
                cout << "x";
            else if(a == -1)
                cout << "-x";
            else
                cout << a << "x";
        }
        else
            cout << a;
        i--;
    }
    cout << endl;
}

vector<double> calculatePoly(vector<double>& t, vector<double>& coeff) {
    vector<double> finalPoly{0};
    int i = 0;
    for (double& b : coeff) {
        vector<double> curPoly = {b};
        for (int j = 0; j < i; j++) {
            vector<double> tx {1, -t[j]};
            curPoly = multiPoly(curPoly, tx);
        }
        i++;
        finalPoly = addPoly(finalPoly, curPoly);
    }
    return finalPoly;
}

vector<double> convert(vector<vector<double> >& data) {
    vector<double> output;
    for (vector<double>& x : data)
        output.insert(output.end(), x.begin(), x.end());
    return output;
}

vector<double> build_t(vector<double>& x, vector<vector<double> >& data) {
    vector<double> t;
    for (unsigned int i = 0; i < x.size(); i++) {
        for (auto& j : data[i]) {
            (void)j;
            t.push_back(x[i]);
        }
    }
    return t;
}

vector<double> coeff(vector<double>& x, vector<vector<double> >& data) {
    vector<double> data_list = convert(data); // zmieniamy tablicê 2d z wartoœciami w listê wartoœci
    vector<vector<double> > matrix; // tworzymy strukture do zapamietywania wczesniej policzonych ilorazow
    vector<double> t = build_t(x, data); // tablica przechowuja kolejne t
    for (int i = t.size(); i >= 0 ; i--) { // przydzielamy miejsce na kolejne kolumny
        matrix.push_back(vector<double>(i, 0));
    }
    int i = 0;
    for (vector<double>& x : data) { // przypisujemy kazdemu t f(t)
        for (double& y : x) {
            (void)y;
            matrix[0][i++] = x[0];
        }
    }

    for (unsigned int i = 1; i < matrix.size(); i++) {
        for (unsigned int j = 0; j < matrix[i].size(); j++) { // wypelniamy kolumny wartosciami czyli ilorazami roznicowymi
            if (t[i + j] != t[j])
                matrix[i][j] = (matrix[i - 1][j + 1] - matrix[i - 1][j]) / (t[i + j] - t[j]); // bierzemy to co jest po lewej i odejmujemy to co po lewej
            else {                                                                                                // i jeden powy¿ej i dzielimy przez ró¿nicê jak we wzorze
                int k = j;
                while (t[k] == t[k - 1]) k--;
                if (k == (int)j)            // jesli pierwsze i ostatnie t sa rowne to bierzemy wartosc pochodnej z wczesniej utworzonej listy  i dzielimy przez silnie
                    matrix[i][j] = data_list[k + i] / fact(i);
                else
                    matrix[i][j] = data_list[k + 2 * i - 1] / fact(i);
            }
        }
    }

    vector<double> coeff;
    for (unsigned int i = 0;  i < matrix[0].size(); i++) {                                 //wypelniamy liste wspolczynnikow pierwszymi wartosciami z kazdej kolumny
        coeff.push_back(matrix[i][0]);
    }
    /*for (vector<double>& x : matrix) {
        for (double y : x)
            printf("%4g", y);
        cout << endl;
    }

    for (double b : coeff)
        cout << b << " ";
    cout << endl;
        for (double b : t)
        cout << b << " ";
    cout << endl;*/

    return calculatePoly(t, coeff);
}

int main () {
    vector<double> x {-1, 0, 1, 2}; // wartoœci x
    vector<vector<double> > data // kolejne kolumny to wartoœci dla poszczególnych x
    {
        {3, -12, 30},
        {0, 1, 0},
        {-1, -4, -6},
        {-6, -3, 12}
    };
    vector<double> p = coeff(x, data); // wyliczenie wspó³czynników dla wielomianu Newtona
    cout << "Szukany Wielomian: " << endl;
    displayPoly(p);

    x = {0, 1, 2};
    data = {
        {0, 1},
        {1, 0, 2},
        {2},
    };
    p = coeff(x, data);
    cout << "Szukany Wielomian: " << endl;
    displayPoly(p);

    x = {-1, 0};
    data = {
        {3, -12, 30},
        {0}
    };
    p = coeff(x, data);
    cout << "Szukany Wielomian: " << endl;
    displayPoly(p);

    x = {-2, 2};
    data = {
        {4, -4},
        {4, 4}
    };
    p = coeff(x, data);
    cout << "Szukany Wielomian: " << endl;
    displayPoly(p);

    x = {-2, 2};
    data = {
        {2},
        {-2}
    };
    p = coeff(x, data);
    cout << "Szukany Wielomian: " << endl;
    displayPoly(p);

    x = {-2};
    data = {
        {2}
    };
    p = coeff(x, data);
    cout << "Szukany Wielomian: " << endl;
    displayPoly(p);
    return 0;
}

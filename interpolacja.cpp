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
        for (int i = s; i >= 0; i--)
            outcome[i] += (p1[i] + p2[i - s + p2.size() - 1]);
    }
    else {
        for (int i = s; i >= 0; i--)
            outcome[i] += p2[i] + p1[i - s + p1.size() - 1];
    }
    return outcome;
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
    matrix.push_back(build_t(x, data)); // w pierwszej kolumnie dodajemy poszczegolne t
    for (int i = matrix[0].size(); i >= 0 ; i--) { // przydzielamy miejsce na kolejne kolumny
        matrix.push_back(vector<double>(i, 0));
    }
    int i = 0;
    for (vector<double>& x : data) { // przypisujemy kazdemu t f(t)
        for (double& y : x) {
            (void)y;
            matrix[1][i++] = x[0];
        }
    }

    for (unsigned int i = 2; i < matrix.size(); i++) {
        for (unsigned int j = 0; j < matrix[i].size(); j++) { // wypelniamy kolumny wartosciami czyli ilorazami roznicowymi
            if (matrix[0][i + j - 1] != matrix[0][j])
                matrix[i][j] = (matrix[i - 1][j + 1] - matrix[i - 1][j]) / (matrix[0][i + j - 1] - matrix[0][j]); // bierzemy to co jest po lewej i odejmujemy to co po lewej
            else {                                                                                                // i jeden powy¿ej i dzielimy przez ró¿nicê jak we wzorze
                int k = j;
                while (matrix[0][k] == matrix[0][k - 1]) k--;           // jesli pierwsze i ostatnie t sa rowne to bierzemy wartosc pochodnej z
                matrix[i][j] = data_list[k + 1 + i - 2] / fact(i - 1);  // wczesniej utworzonej listy  i dzielimy przez silnie
            }
        }
    }
    vector<double> coeff(matrix[0].size(), 0);
    for (unsigned int i = 0;  i < coeff.size(); i++) //wypelniamy liste wspolczynnikow pierwszymi wartosciami z kazdej kolumny
        coeff[i] = matrix[i + 1][0];
    for (vector<double>& x : matrix) {
        for (double y : x)
            printf("%4g", y);
        cout << endl;
    }

    //vector<double> finalPoly = calculatePoly(build_t(x, data), coeff);
    //return finalPoly;
    return coeff;
}

int main () {
    vector<double> x {0, 1, 2}; // wartoœci x
    vector<vector<double> > data // kolejne kolumny to wartoœci dla poszczególnych x
    {
        {0, 1},
        {1, 0, 2},
        {2}
    };
    vector<double> c = coeff(x, data); // wyliczenie wspó³czynników dla wielomianu Newtona
    cout << "Wspolczynniki: " << endl;
    for (double& x : c)
        cout << x << " ";
    cout << endl;
    vector<double> p1{32, 0, 1};
    vector<double> p2{1, 2, 1};
    vector<double> d = multiPoly(p1, p2);
    for (double& x : d)
        cout << x << " ";
    cout << endl;
    d = addPoly(p1, p2);
    for (double& x : d)
        cout << x << " ";
    cout << endl;
    return 0;
}

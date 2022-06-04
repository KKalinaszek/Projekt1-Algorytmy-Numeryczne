#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <iomanip>

using namespace std;

template<typename T>
class matrix {
public:
    vector<vector<T> > contents;
    unsigned int n, m;
    matrix(vector<vector<T> >& C) :
        contents{C},
        n{C.size()},
        m{n > 0 ? C[0].size() : 0}
    {}
    matrix() : n{0}, m{0} {}
    void display() {
        for (auto& x_i : contents) {
            for (auto& x_ij : x_i)
                cout << setw(4) << x_ij;
            cout << endl;
        }
        cout << endl;
    }
    void loadFromFile(string fileName);
};

template<typename T>
void matrix<T>::loadFromFile(string fileName) {
    ifstream in;
    in.open(fileName, ios::in);
    if (!in.is_open()) {
        cerr << "Nie udalo sie otworzyc pliku" << endl;
        return;
    }
    contents.clear();
    int i = 0;
    T temp;
    string line;
    while (getline(in, line)) {
        contents.push_back(vector<T>());
        stringstream ss(line);
        while (ss >> temp) {
            if (ss.fail()) break;
            contents[i].push_back(temp);
        }
        i++;
    }
    in.close();
    unsigned int maxRowLength = 0;
    for (auto& x_i : contents)
        maxRowLength = x_i.size() > maxRowLength ? x_i.size() : maxRowLength;
    for (auto& x_i : contents)
        for (unsigned int j = x_i.size() - 1; j < maxRowLength - 1; j++)
            x_i.push_back(0);
    n = contents.size();
    m = n > 0 ? contents[0].size() : 0;
}

template<typename T>
class equationsMatrix : public matrix<T> {
    public:
    vector<T> f;
    vector<T> x;
    equationsMatrix(vector<vector<T> >& C) : matrix<T>(C) {
        this->m--;
        for (auto& x : this->contents)
            f.push_back(x[this->m]);
    }
    equationsMatrix() {}
    void loadFromFile(string fileName) {
        matrix<T>::loadFromFile(fileName);
        this->m--;
        f.clear();
        for (auto& x : this->contents)
            f.push_back(x[this->m]);
    }
    void display() {
        for (auto& x_i : this->contents) {
            unsigned int i = 0;
            for (auto& x_ij : x_i) {
                if (i < this->m && i != 0) {
                    i++;
                    if (x_ij != 0) {
                        if (x_ij > 0)
                            cout << " + " << setw(3) << x_ij << "x_" << i;
                        else
                            cout << " - " << setw(3) << -x_ij << "x_" << i;
                    }
                    else
                        cout << setw(10);
                }
                else if (i == 0) {
                    i++;
                    if (x_ij != 0)
                        cout << setw(5) << x_ij << "x_" << i;
                    else
                        cout << setw(15);
                }
                else
                    cout << " = " << setw(3) << x_ij;
            }
            cout << endl;
        }
    }
    vector<T> solve(int iter) {
        if (this->n != this->m) {
            cerr << "Macierz nie jest kwadratowa" << endl;
            return x;
        }
        x = vector<T>(this->n, 0);
        for (int i = 0; i < iter; i++)
            this->makeIter();
        return x;
    }
private:
    void makeIter();
};

template<typename T>
void equationsMatrix<T>::makeIter() {
    int i = 0;
    for (auto& x_k1 : this->x) {
        T sum1 = 0, sum2 = 0;
        for (int j = 0; j < i; j++)
            sum1 += this->contents[i][j] * x[j];
        for (unsigned int j = i + 1; j < this->n; j++)
            sum2 += this->contents[i][j] * x[j];
        x_k1 = (-sum1 - sum2 + f[i]) / this->contents[i][i];
        i++;
    }
}

template<typename T>
void execute(equationsMatrix<T> testMatrix, int iterations) {
    cout << "Uklad rownan: " << endl;
    testMatrix.display();
    int i = 0;
    cout << "Rozwiazania: " << endl;
    for (auto x : testMatrix.solve(iterations))
        cout << setw(5) << "x" << ++i << " = " << setw(3) << x << endl;
    cout << "Wykonano " << iterations << " iteracji" << endl;
}

int main() {
    equationsMatrix<double> testMatrix;
    testMatrix.loadFromFile("test.txt");
    execute(testMatrix, 10);
    return 0;
}

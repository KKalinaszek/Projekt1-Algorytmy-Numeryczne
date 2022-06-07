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
        fix();
        for (auto& x : this->contents)
            f.push_back(x[this->m]);
    }

    equationsMatrix() {}
    void loadFromFile(string fileName) {
        matrix<T>::loadFromFile(fileName);
        this->m--;
        fix();
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
                            cout << " + " << setw(4) << x_ij << "x_" << i;
                        else
                            cout << " - " << setw(4) << -x_ij << "x_" << i;
                    }
                    else
                        cout << setw(10) << ' ';
                }
                else if (i == 0) {
                    i++;
                    if (x_ij != 0)
                        cout << setw(5) << x_ij << "x_" << i;
                    else
                        cout << setw(8) << ' ';
                }
                else
                    cout << " = " << setw(3) << x_ij;
            }
            cout << endl;
        }
    }

    void displayMatrix() {
        int i = 0;
        cout << endl;
        for (auto x_i : this->contents) {
            cout << "|";
            unsigned int j = 0;
            for (auto x_ij : x_i) {
                if (j == this->m) putchar('|');
                if (j == 0)
                    cout << " " << setw(3) << x_ij;
                else
                    cout << " " << setw(6) << x_ij;
                j++;
            }
            cout << "|\n";
            i++;
        }
        cout << endl;
    }

    void fix() {
        if (this->m != this->n) return;
        for (unsigned int i = 0; i < this->n; i++) {
            if (this->contents[i][i] == 0) {
                unsigned int j = 0;
                while ((this->contents[j][i] == 0 || this->contents[i][j] == 0) && j < this->n - 1) {
                    if (j++ == i) continue;
                }
                swap(this->contents[i], this->contents[j]);
            }
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

    void setDiagonal(int value, int offset) {
        try {
            if (this->m != this->n) {
                throw exception();
            }
        }
        catch (exception) {
            cerr << "Macierz nie jest kwadratowa";
            return;
        }
        for (unsigned int i = 0; i < this->n; i++)
            if (i + offset < this->n && i + offset >= 0)
                this->contents[i][i + offset] = value;
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
void execute(equationsMatrix<T>& testMatrix, int iterations) {
    cout << "\nUklad rownan: " << endl;
    testMatrix.display();
    testMatrix.displayMatrix();
    int i = 0;
    cout << "Rozwiazania: " << endl;
    for (auto x : testMatrix.solve(iterations))
        cout << setw(5) << "x" << ++i << " = " << setw(3) << x << endl;
    cout << "Wykonano " << iterations << " iteracji" << endl;
}

int main() {
    equationsMatrix<double> testMatrix;
    int numberOfIterations;
    do {
        cin.clear();
        cout << "Ustaw ilosc iteracji: \n";
        cin >> numberOfIterations;
        cin.ignore(256, '\n');
    } while (cin.fail());
    testMatrix.loadFromFile("test.txt");
    execute(testMatrix, numberOfIterations);
    int option;
    while (true) {
        cout << "\nCzy policzyc dla nowego parametru nad przekatna? <1, 0>\n";
        cin >> option;
        cin.clear();
        if (option){
            int newDiagValue;
            do {
                cin.clear();
                cout << "Podaj nowa wartosc nad przekatna: \n";
                cin >> newDiagValue;
                cin.ignore(256, '\n');
            } while (cin.fail());
            testMatrix.setDiagonal(newDiagValue, 1);
            execute(testMatrix, numberOfIterations);
        }
        else
            return 0;
    }
    return 0;
}

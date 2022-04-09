#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

int fact(int x) {
    if (x == 0)
        return 1;
    else
        return x * fact(x - 1);
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
        for (auto& j : data[i])
            t.push_back(x[i]);
    }
    return t;
}

vector<double> coeff(vector<double>& x, vector<vector<double> >& data) {
    vector<double> data_list = convert(data);
    vector<vector<double> > matrix;
    matrix.push_back(build_t(x, data));
    for (int i = matrix[0].size(); i >= 0 ; i--) {
        matrix.push_back(vector<double>(i, 0));
    }
    int i = 0;
    for (vector<double>& x : data) {
        for (double& y : x) {
            matrix[1][i++] = x[0];
        }
    }

    for (unsigned int i = 2; i < matrix.size(); i++) {
        for (unsigned int j = 0; j < matrix[i].size(); j++) {
            if (matrix[0][i + j-1] != matrix[0][j])
                matrix[i][j] = (matrix[i - 1][j + 1] - matrix[i - 1][j]) / (matrix[0][i + j - 1] - matrix[0][j]);
            else {
                int k = j;
                while (matrix[0][k] == matrix[0][k - 1]) k--;
                matrix[i][j] = data_list[k + 1 + i - 2] / fact(i - 1);
            }
        }
    }
    vector<double> coeff(matrix[0].size(), 0);
    for (unsigned int i = 0;  i < coeff.size(); i++)
        coeff[i] = matrix[i + 1][0];
    for (vector<double>& x : matrix) {
        for (double y : x)
            printf("%4g", y);
        cout << endl;
    }
    return coeff;
}

int main () {
    vector<double> x {0, 1, 2};
    vector<vector<double> > data
    {
        {0, 1},
        {1, 0, 2},
        {2}
    };
    vector<double> c = coeff(x, data);
    cout << "Wspolczynniki: " << endl;
    for (double& x : c)
        cout << x << " ";
    cout << endl;
    return 0;
}

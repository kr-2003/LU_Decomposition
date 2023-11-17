
#include <bits/stdc++.h>
#include <omp.h>
using namespace std;

const int MAX = 100;

bool validate_answer(vector<vector<float> > &mat, vector<vector<float> > &lower, vector<vector<float> > &upper) {
    int n = lower.size();
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            float ans = 0;
            for(int k = 0; k < n; k++) {
                ans += lower[i][k] * upper[k][j];
            }
            float error = abs(ans - mat[i][j]) / mat[i][j];
            if(error > 1e-3) return false;
        }
    }
    return true;
}

void print_matrices(vector<vector<float> > &mat, vector<vector<float> > &lower, vector<vector<float> > &upper) {
    int n = mat.size();
    cout << "Original matrix is : " << endl;
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
            cout << setw(6) << mat[i][j] << "\t";
        cout << endl;
    }
    cout << endl;
    cout << "Lower Triangular" << endl;
    // Displaying the result :
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
            cout << setw(10) << lower[i][j];
        cout << endl;
    }

    cout << endl;
    cout << "Upper Triangular" << endl;
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
            cout << upper[i][j] << "\t";
        cout << endl;
    }
    cout << endl;
}

void luDecomposition(vector<vector<float> > &mat, int n)
{
    vector<vector<float>> lower(n, vector<float>(n, 0)), upper(n, vector<float>(n, 0));
    omp_lock_t lock;
    omp_init_lock(&lock);

    for (int i = 0; i < n; i++)
    {
        for (int k = i; k < n; k++)
        {
            float sum = 0;
            for (int j = 0; j < i; j++)
                sum += (lower[i][j] * upper[j][k]);
            upper[i][k] = mat[i][k] - sum;
        }

        for (int k = i; k < n; k++)
        {
            if (i == k)
                lower[i][i] = 1;
            else
            {
                float sum = 0;
                for (int j = 0; j < i; j++)
                    sum += (lower[k][j] * upper[j][i]);
                lower[k][i] = (mat[k][i] - sum) / upper[i][i];
            }
        }
    }

    //    print_matrices(mat, lower, upper);
    //    bool flag = validate_answer(mat, lower, upper);
    //    if(flag) {
    //        cout << "Yes!!! L and U matrices are correct!!!" << endl;
    //    } else {
    //        cout << "Oh!! Something's wrong!!!" << endl;
    //    }

}

void random_fill(vector<vector<float>> &matrix, int size)
{
    cout << "Producing random values " << endl;
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            matrix[i][j] = ((rand() % 10) + 1);
        }
    }

    int diagCount = 0;
    float sum = 0;
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            sum += abs(matrix[i][j]);
        }
        sum -= abs(matrix[i][diagCount]);
        matrix[i][diagCount] = sum + ((rand() % 5) + 1);
        ++diagCount;
        sum = 0;
    }
}

int main(int argc, char **argv)
{
    double runtime;
    int numThreads;
    omp_set_num_threads(40);
    srand(1);
    cout << "Enter the size of the matrix: " << endl;
    int n;
    cin >> n;
    vector<vector<float>> mat(n, vector<float>(n, 0));
    random_fill(mat, n);
    runtime = omp_get_wtime();
    luDecomposition(mat, n);
    runtime = omp_get_wtime() - runtime;
    cout << "Runtime: " << runtime << endl;
    return 0;
}
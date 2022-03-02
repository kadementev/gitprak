#include <vector>
#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <string>
#include <iomanip>
#include <stdio.h>
#include <iostream>
#include <math.h>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <algorithm>


const double EPS = 1E-3;


class IndexError {
private:
    std::string ind_error;

public:
    IndexError(std::string error)
        : ind_error(error)
    {
    }

    const char* getError() { return IndexError::ind_error.c_str(); }

};

class MatrixError {
private:
    std::string mtr_error;

public:
    MatrixError(std::string error)
        : mtr_error(error)
    {
    }

    const char* getError() { return MatrixError::mtr_error.c_str(); }

};




class FileError {
private:
    std::string fl_error;

public:
    FileError(std::string error)
        : fl_error(error)
    {
    }

    const char* getError() { return FileError::fl_error.c_str(); }

};








class Matrix {
    protected:
        int strk; // ���������� �����
        int stlb; // ���������� ��������
        std::vector<std::vector<float>> values;

    public:

        // ������������
        Matrix(int a, int b)
        {
            // a - ���������� �����, b - ���������� ��������
            strk = a;
            stlb = b;
            for (int i = 0; i < a; i++) {
                std::vector<float> tmp;
                values.push_back(tmp);
                for (int j = 0; j < b; j++) {
                    values[i].push_back(0);
                }
            }
        };

        Matrix()
        {
            strk = 0;
            stlb = 0;

        };


        Matrix(std::vector<std::vector<float>> vals) {


            for (int i = 0; i < vals.size(); i++) {
                if (vals[0].size() != vals[i].size()) {
                    throw IndexError("Matrix.cell(i, j): wrong vector sizes in Matrix");
                }
            }
            strk = vals.size();
            stlb = vals[0].size();
            values = vals;

        };



        // ���� �������� �������� � ������
        float& cell(int i, int j) { // ������, �������
            if (i >= strk || j >= stlb || i < 0 || j < 0) {
                throw IndexError("Matrix.cell(i, j): no such indexes in Matrix");
            }

            return values[i][j] ;
        };
    
        // ���� �������� �������� � ������ � �������� �� ���������
        float print(int i, int j) const { // ������, �������
            if (i >= strk || j >= stlb || i < 0 || j < 0) {
                throw IndexError("Matrix.cell(i, j): no such indexes in Matrix");
                return -1;
            }

            return values[i][j];
        };



        // ����� ������
        Matrix operator+(const Matrix& tmp) {
            if (this->stlb != tmp.stlb || this->strk != tmp.strk) {
                throw MatrixError("Matrix + Matrix: wrong sizes");
            }

            std::vector<std::vector<float>> vectmp;

            for (int i = 0; i < this->strk; i++) {
                std::vector<float> temp;
                vectmp.push_back(temp);
                for (int j = 0; j < this->stlb; j++) {
                    vectmp[i].push_back(tmp.values[i][j] + this->values[i][j]);
                }
            }

            return Matrix(vectmp);

        }

        // ��������� ������
        Matrix operator-(const Matrix& tmp) {
            if (this->stlb != tmp.stlb || this->strk != tmp.strk) {
                throw MatrixError("Matrix - Matrix: wrong sizes");
            }
            std::vector<std::vector<float>> vectmp;

            for (int i = 0; i < this->strk; i++) {
                std::vector<float> temp;
                vectmp.push_back(temp);
                for (int j = 0; j < this->stlb; j++) {
                    vectmp[i].push_back(tmp.values[i][j] - this->values[i][j]);
                }
            }
            return Matrix(vectmp);

        }

        // ��������� ������
        Matrix operator*(const Matrix& tmp) const{
            if (this->stlb != tmp.strk) {
                throw MatrixError("Matrix * Matrix: wrong sizes");
            }
            std::vector<std::vector<float>> vectmp;

            
            float tmpsum = 0;

            for (int i = 0; i < this->strk; i++) {
                std::vector<float> temp;
                vectmp.push_back(temp);
                for (int j = 0; j < tmp.stlb; j++) {

                    for (int k = 0; k < this->stlb; k++) {
                        if ( fabs(this->print(i, k) * tmp.print(k, j)) < EPS) {
                            tmpsum += 0.;
                        }
                        else {
                            tmpsum += this->print(i, k) * tmp.print(k, j);
                        }
                    }

                    if (fabs(tmpsum) < EPS) {
                        tmpsum = 0;
                    }

                    vectmp[i].push_back(tmpsum);
                    tmpsum = 0;
                }



            }




            return Matrix(vectmp);
        }

        // ��������� �� �����
        Matrix operator*(int num) {
            std::vector<std::vector<float>> tmpvec;

            for (int i = 0; i < this->strk; i++) {
                std::vector<float> temp;
                tmpvec.push_back(temp);
                for (int j = 0; j < this->stlb; j++) {
                    tmpvec[i].push_back(this->values[i][j] * num);
                }


            }

            return Matrix(tmpvec);
        }



        // ����������� ������ ���� �������� ����� �����/��������, �������� ������
        int stroks() const {
            return this->strk;
        }

        int stlbs() const {
            return this->stlb;
        }



        // �������� ����� �� ������� ����������
        bool issquare() const {
            if (this->stroks() == this->stlbs()) {
                return true;
            }
            else {
                return false;
            }
        
        };

        


        // ���� �������
        float trace() const {
            if (this->issquare()) {
                float sum = 0;
                for (int i = 0; i < this->stroks(); i++) {
                    sum += this->print(i, i);
                }
                return sum;
            }
            else {
                throw MatrixError("Matrix must be square");
            }
        }



        //������������ ������� ������� ������

        float det() const {
            if (this->issquare() == false) {
                throw MatrixError("Matrix must be square");
            }
            else {
                Matrix temp(this->values);
                int n = temp.stroks();
                float d = 1;
                for (int i = 0; i < n; ++i) {
                    int k = i;
                    for (int j = i + 1; j < n; ++j)
                        if (abs(temp.print(j,i)) > abs(temp.print(k,i))) // ����� ������������� �������� � �������
                            k = j;
                    if (abs(temp.print(k, i)) < EPS) { // ���� ������������ ������� � ������� ����� ���, �� ������������ 0
                        d = 0;
                        break;
                    }
                    for (int j = 0; j < n; j++) { //  ������������ ������������ ������
                        // � ������������ �� ������ ��������� ������
                        std::swap(temp.cell(i, j), temp.cell(k, j));
                    }

                    if (i != k) // ������ ���� ������ ��� ����� ������������ ������
                        d = -d; // ���� ������������ ������� �������� � ������ �����, ������ �� ������
                    d *= temp.cell(i, i); // �������� det �� ������� �� ������� ���������
                    for (int j = i + 1; j < n; ++j)
                        temp.cell(i,j) /= temp.cell(i, i); // ����� �������� � ������ �� ������������
                    for (int j = 0; j < n; ++j)
                        if (j != i && abs(temp.cell(j, i)) > EPS)
                            for (int k = i + 1; k < n; ++k) {
                                temp.cell(j, k) -= temp.cell(i, k) * temp.cell(j, i);
                            }
                }
                return d;
            }
        }



        float fbnorm() const {
            float norm = 0;
            for (int i = 0; i < this->stlbs(); i++) {
                for (int j = 0; j < this->stroks(); j++) {
                    norm += this->values[i][j] * this->values[i][j];
                }
            }
            return sqrt(norm);
        }










        Matrix T() {
            int strk1 = this->stlb;
            int stlb1 = this->strk;
            std::vector<std::vector<float>> tmp;
            for (int j = 0; j < this->stlb; j++) {
                std::vector<float> temp;
                tmp.push_back(temp);
                for (int i = 0; i < this->strk; i++) {
                    tmp[j].push_back(this->cell(i, j));
                }
            }
            return Matrix(tmp);

        }

        int rank(){

            int i, j, k, l;
            float r;
            std::vector<std::vector<float>> a = this->values;


            i = 0; j = 0;
            while (i < this->strk && j < this->stlb) {
                // ���������: ����� ������� � �������� 0..j-1
                //   ��� �������� � ������������ ����, � ������
                //   � �������� i-1 �������� ��������� ��-�
                //   � ������� � �������, ������� ��� j

                // ���� ������������ ������� � j-� �������,
                // ������� � i-� ������
                r = 0.0;
                for (k = i; k < this->strk; ++k) {
                    if (fabs(a[k][j]) > r) {
                        l = k;      // �������� ����� ������
                        r = fabs(a[k][j]); // � ����. ��-�
                    }
                }
                if (r <= EPS) {
                    // ��� �������� j-�� ������� �� ����������
                    // �������� �� ����������� eps.
                    // ������� �������, ������� � i-� ������
                    for (k = i; k < this->strk; ++k) {
                        a[k][j] = 0.0;
                    }
                    ++j;      // �������� ������ �������
                    continue; // ��������� � ��������� ��������
                }

                if (l != i) {
                    // ������ ������� i-� � l-� ������
                    for (k = j; k < this->stlb; ++k) {
                        r = a[i][k];
                        a[i][k] = a[l][k];
                        a[l][k] = (-r); // ������ ���� ������
                    }
                }

                // �����������: fabs(a[i*n + k]) > eps

                // �������� j-� �������, ������� �� ������ i+1,
                // �������� ����. �������������� ������� ����
                for (k = i + 1; k < this->strk; ++k) {
                    r = (-a[k][j] / a[i][j]);

                    // � k-� ������ ���������� i-�, ���������� �� r
                    a[k][j] = 0.0;
                    for (l = j + 1; l < this->stlb; ++l) {
                        a[k][l] += r * a[i][l];
                    }
                }

                ++i; ++j;   // ��������� � ���������� ������
            }

            return i; // ���������� ����� ��������� �����
        }



        // �������� �������
        Matrix inv()
        {

            if (this->issquare() == false) {
                throw MatrixError("must be square matrix");
            }

            if (this->det() == 0){
                throw MatrixError("Matrix is degenerate (Vurozhdena)");
            }



            std::vector<std::vector<float>> mtr = this->values; // �������� �������� �������
            std::vector<std::vector<float>> tmp; // �������� ��� ��������� �������

            int n = this->values.size();
            float num;
            for (int i = 0; i < n; i++) {
                std::vector<float> temp;
                tmp.push_back(temp);
                for (int j = 0; j < n; j++)
                {
                    tmp[i].push_back(0.0);

                    if (i == j)
                        tmp[i][j] = 1.0;
                }
            }// ��������� ������� ���������




            // ������� ������ �������� �������� � ��������� �������, ��� ���� ��������� �������� �� ������� � ���������
            for (int k = 0; k < n; k++)
            {
                num = mtr[k][k];

                for (int j = 0; j < n; j++)
                {
                    mtr[k][j] /= num;
                    tmp[k][j] /= num;
                }

                for (int i = k + 1; i < n; i++)
                {
                    num = mtr[i][k];

                    for (int j = 0; j < n; j++)
                    {
                        mtr[i][j] -= mtr[k][j] * num;
                        tmp[i][j] -= tmp[k][j] * num;
                        
                    }
                }
            }

            for (int k = n - 1; k > 0; k--)
            {
                for (int i = k - 1; i >= 0; i--)
                {
                    num = mtr[i][k];

                    for (int j = 0; j < n; j++)
                    {
                        mtr[i][j] -= mtr[k][j] * num;
                        tmp[i][j] -= tmp[k][j] * num;
                        


                    }
                }
            }




            for (int i = 0; i < n; i++)
            {
                for (int j = 0; j < n; j++)
                {
                    if (fabs(tmp[i][j]) < EPS) {
                        tmp[i][j] = 0;
                    }
                }
            }


            return Matrix(tmp);
        }


        int write(std::string filename)
        {
            std::ofstream out(filename, std::ios::binary);    //������ ����� "�������� ����"
            if (!out) {
                throw FileError("Error while openning file to write");
            }
            out.write((char*)&this->strk, sizeof(int));       //���������� � ���� ����� ��������
            out.write((char*)&this->stlb, sizeof(int));       //���������� � ���� ����� �����
            for (int i = 0; i < this->strk; i++) {
                for (int j = 0; j < this->stlb; j++) {
                    out.write((char*)&this->values[i][j], sizeof(float));
                }

            }
            out.close();
            return 0;
        }

        


        int read(std::string filename)
        {
            std::ifstream in(filename, std::ios::binary);            //������ ����� "�������� ����"
            if (!in) {
                throw FileError("Error while openning file to read");
            }

            for (int i = 0; i < this->strk; i++) {
                this->values[i].clear();
            }
            this->values.clear(); // ����� �� ������ ������ ���� ������ �������?

            

            
            in.read((char*)&this->strk, sizeof(int));        //
            if (!in) throw FileError("wrong file");
            in.read((char*)&this->stlb, sizeof(int));        //
            if (!in) throw FileError("wrong file");



            for (int i = 0; i < this->strk; i++) {
                std::vector<float> temp;
                this->values.push_back(temp);
                for (int j = 0; j < this->stlb; j++) {
                    float tmp;
                    in.read((char*)&tmp, sizeof(float));
                    if (!in) throw FileError("not enough elements or wrong file");
                    // ���� ������� �� ��� �������� ��� �������� ����������� ��� �������� ���������
                    // ����� ������

                    this->values[i].push_back(tmp);
                }

            }
            in.close();
            return 0;

        
        }












        friend Matrix operator*(int num, const Matrix& tmp);
        friend Matrix adamar(const Matrix& tmp1, const Matrix& tmp2);
        friend std::ostream& operator<< (std::ostream& out, const Matrix& mtr);
        friend std::istream& operator>> (std::istream& in, Matrix& mtr); // ��������
};



Matrix operator*(int num, const Matrix& tmp) {
    std::vector<std::vector<float>> tmpvec;

    for (int i = 0; i < tmp.strk; i++) {
        std::vector<float> temp;
        tmpvec.push_back(temp);
        for (int j = 0; j < tmp.stlb; j++) {
            tmpvec[i].push_back(tmp.values[i][j] * num);
        }
    }


    return Matrix(tmpvec);
}


Matrix adamar(const Matrix& tmp1, const Matrix& tmp2) {
    if (tmp1.stlb != tmp2.stlb || tmp1.strk != tmp2.strk) {
        throw MatrixError("Matrix adamar: wrong sizes");
    }
    std::vector<std::vector<float>> vectmp;

    for (int i = 0; i < tmp1.strk; i++) {
        std::vector<float> temp;
        vectmp.push_back(temp);
        for (int j = 0; j < tmp1.stlb; j++) {
            vectmp[i].push_back(tmp1.values[i][j] * tmp2.values[i][j]);
        }
    }

    return Matrix(vectmp);
}




std::ostream& operator<< (std::ostream& out, const Matrix& mtr)
    {
    int size = 0;
    
    

    size = 7;


    out.setf(std::ios::left);
    out << std::setprecision(5);


    for (int i = 0; i < mtr.stlb - 1; i++) {
        out << std::setfill(' ') << std::setw(size);
        out << mtr.print(0, i) << " ";
    }
    out << mtr.print(0, mtr.stlb -1) << "\n";

    for (int j = 1; j < mtr.strk - 1; j++) {
        for (int i = 0; i < mtr.stlb - 1; i++) {
            out << std::setfill(' ') << std::setw(size);
            out << mtr.print(j, i) << " ";
        }
        out << mtr.print(j, mtr.stlb - 1) << "\n";
    }

    for (int i = 0; i < mtr.stlb - 1; i++) {
        out << std::setfill(' ') << std::setw(size);
        out << mtr.print(mtr.strk - 1, i) << " ";
    }
    out << mtr.print(mtr.strk-1, mtr.stlb - 1) << "\n";

    return out;

    }




std::istream& operator>> (std::istream& in, Matrix& mtr) {

    std::string line;
    for (int i = 0; i < mtr.strk; i++) {
        mtr.values[i].clear();
    }
    mtr.values.clear();
    
    std::vector<std::vector<float>> vec;

    int strk = 0;
    int stlb = 0;
    int tmpstlb = 0;
    std::string tmp;


    while (getline(in, line)) {
        std::replace(line.begin(), line.end(), ',', '.'); // ������ ������� �� �����
        std::stringstream ss;
        ss << line;
        std::vector<float> temp;                          // ���� ������ float � . � , ��������
        vec.push_back(temp);


        while (!ss.eof()) {
            ss >> tmp;
            if (strk == 0) { // ���� ����������� ������ ������ 
                stlb += 1; 
                tmpstlb += 1;
            }
            else {
                tmpstlb += 1;
            }
            vec[strk].push_back(std::stof(tmp));
        }
        if (tmpstlb != stlb) {
            throw FileError{ "file have wrong sizes to be matrix" };
        }

        tmpstlb = 0;

        strk += 1;

    }



    /*
    std::string line;
    std::string tmp;
    while (getline(inf, line)) {
        std::replace(line.begin(), line.end(), ',', '.');
        std::stringstream ss;
        ss << line;
        while (!ss.eof()) {
            ss >> tmp;
            std::cout << std::stof(tmp);
        }

        //std::cout << line;
        tmp = "";
    }
    */


    mtr.stlb = stlb;
    mtr.strk = strk;
    mtr.values = vec;



    return in;


}





















class Vector : public Matrix {
public:

    Vector() {
        stlb = 0;
        strk = 0;
    };

    Vector(std::vector<float> vec) {
        strk = vec.size();
        stlb = 1;

        for (int i = 0; i < vec.size(); i++) {
            std::vector<float> tmp;
            values.push_back(tmp);
            values[i].push_back(vec[i]);
        }

    }

    

    
    float operator*(const Vector& tmp) {
        if (this->strk != tmp.strk) {
            throw MatrixError("vector * vector: wrong sizes");
        }
        
        float sum = 0;

        for (int i = 0; i < this->strk; i++) {
            sum += this->values[i][0] * tmp.values[i][0];
        }

        return sum;

    }


    Matrix operator*(const Matrix& tmp) {
        
        return Matrix(this->values) * tmp;
    }




    float evklidnorm() {
        return sqrt(*this * *this);
    }



    float maxnorm() {
        float max = 0;
        for (int i = 0; i < this->strk; i++) {
            for (int j = 0; j < this->stlb; j++) {
                if (abs(this->values[i][j]) > max) {
                    max = abs(this->values[i][j]);
                }
            }
        }
        return max;
    }



    friend float angle(Vector& v1, Vector& v2);


};

float angle(Vector& v1, Vector& v2) {

    return  acos((v1 * v2) / (v1.evklidnorm() * v2.evklidnorm()));


}




// �������� ��������� ��� ������ ��������� ����� -  
//  ������� � ������ ����������� �������, ������������ �������.


class eye : public Matrix {
public:
    eye(int a) {


        strk = a;
        stlb = a;

        for (int i = 0; i < a; i++) {
            std::vector<float> tmp;
            values.push_back(tmp);
            for (int j = 0; j < a; j++) {
                if (i == j) {
                    values[i].push_back(1);
                }
                else {
                    values[i].push_back(0);
                }
            }
        }
    };

};


class diagonal : public Matrix {
public:
    diagonal(std::vector<float> vec) {

        strk = vec.size();
        stlb = vec.size();

        for (int i = 0; i < strk; i++) {
            std::vector<float> tmp;
            values.push_back(tmp);
            for (int j = 0; j < strk; j++) {
                if (i == j) {
                    values[i].push_back(vec[i]);
                }
                else {
                    values[i].push_back(0);
                }
            }
        }
    };

    diagonal(Matrix mtr) {
        if (mtr.issquare() != true) {
            throw MatrixError("diagonal: matrix must be square");
        }
        for (int i = 0; i < mtr.stroks(); i++) {
            for (int j = 0; j < mtr.stroks(); j++) {
                if (i == j) {
                    values[i].push_back(mtr.cell(i, j));
                }
                else {
                    values[i].push_back(0);
                }
            }
        }
        strk = mtr.stroks();
        stlb = strk;
    }

};


class upper : public Matrix {
public:
    upper(int c) {
        strk = c;
        stlb = c;
        for (int i = 0; i < c; i++) {
            for (int j = 0; j < c; j++) {
                if (j >= i) {
                    float a = (float)(rand() % 10000) / 1000;
                    values[i].push_back(a);
                }
                else {
                    values[i].push_back(0);
                }
            }
        }
    };

    float& cell(int i, int j) {
        if (i > j) {
            throw MatrixError("This value is constant");
        }
        else {
            return Matrix::cell(i, j);
        }
        
    }


};

class lower : public Matrix {
public:
    lower(int c) {
        strk = c;
        stlb = c;
        for (int i = 0; i < c; i++) {
            for (int j = 0; j < c; j++) {
                if (i >= j) {
                    float a = (float)(rand() % 10000) / 1000;
                    values[i].push_back(a);
                }
                else {
                    values[i].push_back(0);
                }
            }
        }
    };

    float& cell(int i, int j) {
        if (j > i) {
            throw MatrixError("This value is constant");
        }
        else {
            return Matrix::cell(i, j);
        }

    }

};





class pair { // ����� ��� ������������ ������� ����������
    // ����� � ������������ ������� �������� ��� �������� ������.
    // ���� ��� �� ����� ����� ������������� ������������ � ������� ������� ��� ����� ���������
public:
    float* a;
    float* b;
    pair(float* tmp1, float* tmp2) {
        a = tmp1;
        b = tmp2;
    }
    

    void operator= (const float num) {
        *a = num;
        *b = num;
    }

};



class simetric : public Matrix {
public:
    simetric(int c) {
        strk = c;
        stlb = c;
        for (int i = 0; i < c; i++) {
            std::vector<float> tmp;
            values.push_back(tmp);
            for (int j = 0; j < c; j++) {
                if (j >= i) {
                    float a = (float)(rand() % 10000) / 1000;
                    values[i].push_back(a);
                }
                else {
                    values[i].push_back(this->print(j, i)); // ������ ��������� ��� ��� ������� ����������� ���������.
                    // �� ���� ���������� ��� ������ ������, ������ ������ ��� ���������� ����� ������ ����� ������� ������ �� ������ ������
                    // � ������� ������ ��� 2 ����� ������� �� ���������� ����� � ��������� ���������� � ��� �����, ������ ������� �� ���������.
                }
            }
        }
    };




    pair cell(int i, int j) {
        return pair(&Matrix::cell(i, j), &Matrix::cell(j, i));
    }



};







/*����������� �������� ��� ��������� � ��������� :
* 
���� ������� (done) trace

������������ �������(������� ������) (done) det

��������� ������������ �������� (done) vector * vector

����� �������(��������� �����, ������������ �����) (done) evkildnorm, maxnorm

����� �������(����� ����������)  (done)
*/












/*��������� ���������� �������, ������������� � ������ ��1 � ��2. ����������� �������� ��� ��������� � ���������:

���� ����� ���������  (done)   angle(v1, v2)

���� ������� (� ������� ��������� ������)   (done) rank()

�������� ������� (���� ����������)   

���������������� ������� (done)  Matrix.T()


*/






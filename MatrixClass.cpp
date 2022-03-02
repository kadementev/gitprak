// MatrixClass.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
#include "MatrixClass.h"


int main()
{
    Matrix A(4,4);
    A.cell(3, 3) = 1;
    A.cell(2, 2) = 1;
    A.cell(0, 1) = 150;

    std::string file1 = "matrix.txt";
    std::string file2 = "data.txt";
    std::string file3 = "loadings.txt";
    std::string file4 = "scores.txt";
    std::string filebin = "matrix.bin";


    std::vector<std::vector<float>> a = {{ 5.522, 2.012, 3 }, {4, 5, 6} , {6, 8, 100}};
    std::vector<float> b = {0, 0, 1 };
    std::vector<float> b1 = {0, 1, 0 };


    try {


        Matrix B(a);
        //Vector v1(b), v2(b1);
        
        //std::cout << "B rank = " << B.rank() << std::endl;
        //std::cout << B * B.inv();


       // std::cout << B;
        //std::cout << B.T();
        //std::cout << angle(v1, v2);
        
        

        B.write(filebin);
        A.read(filebin);  // записывается и читается из файла нормально
        //std::cout << B;
        //std::cout << A;


        
        //
        //std::cout << A.stroks();

        // загружает матрицу из файла
        std::ifstream inf(file3);
        inf >> A;
        std::cout << A;
        inf.close();

        A.cell(0, 0) = 0;

        // записывает матрицу в файл
        std::ofstream outf(file3); // остальные file2,3,4 не ставить, иначе перезапишутся.
        if (!outf) {
            std::cout << "smthng wrong!";
        }
        else {
            outf << A;
        }
        outf.close();





        /*

        std::string line;
        std::string tmp;
        while (getline(inf, line)) {
            std::replace(line.begin(), line.end(), ',', '.');
            std::stringstream ss;
            ss << line;
            while (!ss.eof()) {
                ss >> tmp;
                std::cout << std::stof(tmp) << " ";
            }

            std::cout << "\n";
            tmp = "";
        }


        */



    }
    catch (IndexError& ex) {
        std::cerr << "Exception: " << ex.getError() << std::endl;
        return -1;
    }
    catch (MatrixError& ex) {
        std::cerr << "Exception: " << ex.getError() << std::endl;
        return -1;
    }

    catch (FileError& ex) {
        std::cerr << "Exception: " << ex.getError() << std::endl;
        return -1;
    }




}


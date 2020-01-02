#ifndef __NewMatrix__
#define __NewMatrix__

#include <iostream>
#include <cmath>
#include <string>
#include <vector>


typedef long long yeet;
using namespace std;

template <typename Y>
void deletep(Y&) {}
template <typename Y>
void deletep(Y*& ptr) {
    delete ptr;
    ptr = nullptr;
}
template <typename Y>
void deletea(Y*& ptr) {
    if (ptr != nullptr) {
        delete[] ptr;
        ptr = nullptr;
    }
}
template<typename Y>
class Matrix {
public:
    typedef Y value_type;
    Matrix() : _cols(0), _rows(0), _data(new Y[0]), auto_delete(true) {};
    Matrix(yeet rows, yeet cols, bool auto_del = true);
    //new
    Matrix(yeet, yeet, mutable Y*);
    Matrix(yeet, yeet, vector<Y>);
    Matrix(const std::string&);
    Matrix(const Matrix<Y>&);

    bool exists(yeet row, yeet col) const;
    Y& operator()(yeet row, yeet col);
    Y operator()(yeet row, yeet col) const;
    Matrix<Y> operator=(const Matrix<Y>&);
    virtual ~Matrix();


    /////NOWE
    Matrix<Y> operator+=(const Matrix<Y>&);
    Matrix<Y> operator-=(const Matrix<Y>&);
    Matrix<Y> operator*=(const Matrix<Y>&);
    Matrix<Y> operator*=(const Y&);
    Matrix<Y> operator/=(const Y&);

    //Dodatkowe
    Matrix<Y> operator+(const Matrix<Y>&);
    Matrix<Y> operator-(const Matrix<Y>&);
    Matrix<Y> operator*(const Matrix<Y>&);
    Matrix<Y> operator*(const Y&);
    Matrix<Y> operator/(const Y&);

    Matrix<Y> dot(const Matrix<Y>&);
    Matrix<Y> T();
    Matrix<Y> expa();

    Matrix<Y> sigmoid();
    Matrix<Y> sigmoid_derivative();

    double mean();
    Matrix<Y> square();


    //friend inline Matrix<Y>& sigmoid(const Matrix<Y>& m);
    //friend inline Matrix<Y>& sigmoid_derivative(const Matrix<Y>& m);

    Matrix<Y> print(short round = (short)5);
    void add(std::string);
    std::string getString(bool is_int = false);

    //////////

    yeet size() const { return _rows * _cols; }
    yeet rows() const { return _rows; }
    yeet cols() const { return _cols; }

    Y* getArray() const {
        Y* temp = new Y[size()];
        for (yeet i = 0; i < size(); i++)
            temp[i] = _data[i];
        return temp;
    }
    void print_size() { std::cout << "size : " << _rows << " x " << _cols << std::endl; }
private:
    unsigned power(short inp) {
        unsigned int output = 1;
        for (short i = 0; i < inp; i++)
            output *= 10;
        return output;
    }
    void initMatrix() {
        this->_data = new Y[size()];
        for (yeet i = 0; i < size(); i++) this->_data[i] = 0.0;
    }
    yeet _rows, _cols;
    mutable Y* _data;
    const bool auto_delete;

public:
    void free();
};


template<typename Y>
Matrix<Y> Matrix<Y>::print(short roundness) {
    int pomocnicza = 0;
    roundness = (roundness < 5 ? roundness : 4);
    for (yeet i = 0; i < this->_rows; i++) {
        std::cout << "[";
        for (yeet j = 0; j < this->_cols; j++) {
            if (roundness != 0) {
                pomocnicza = (float)this->_data[i * this->_cols + j] * power(roundness);
                std::cout << " " << ((float)pomocnicza / power(roundness));
            }
            else
                std::cout << " " << round(this->_data[i * this->_cols + j]);
        }
        std::cout << " ]" << std::endl;
    }
    return *this;
}

template <typename Y>
std::string Matrix<Y>::getString(bool is_int) {
    std::string data = "";
    Matrix<Y> temp(*this);
    for (int i = 0; i < this->_rows; i++) {
        data += "[";
        for (int j = 0; j < this->_cols; j++) {
            if (!(is_int)) data += to_string(temp(i, j));
            else data += to_string(int(temp(i, j)));

            if (j < this->_cols - 1) data += ",";
        }
        data += "]";
    }
    return data;
}
////////////////Matematyczne/////////////////////////////////////
template <typename Y>
Matrix<Y> Matrix<Y>::T() {
    Matrix<Y> temp(_cols, _rows);

    for (yeet j = 0; j < _rows; j++)
        for (yeet i = 0; i < _cols; i++)
            temp._data[i * _rows + j] = this->_data[j * _cols + i];
    return temp;
}

template <typename Y>
Matrix<Y> Matrix<Y>::dot(const Matrix& rhs) {
    if (this->_cols != rhs._rows) {
        this->print_size();
        cout << "rhs: " << rhs._rows << " x " << rhs._cols << endl;
        throw "Nie mozna pomnozyc tych macierzy";
    }

    Y w = 0;
    Matrix<Y> temp(this->_rows, rhs._cols);
    for (int i = 0; i < _rows; i++)
        for (int j = 0; j < rhs._cols; j++) {
            for (int h = 0; h < _cols; h++)
                w += _data[i * _cols + h] * rhs._data[h * rhs._cols + j];
            temp._data[i * temp._cols + j] = w;
            w = 0;
        }

    return temp;
}


template <typename Y>
Matrix<Y> Matrix<Y>::expa() {
    Matrix<Y> temp(_rows, _cols);

    for (yeet j = 0; j < this->_rows; j++)
        for (yeet i = 0; i < this->_cols; i++)
            temp._data[j * this->_cols + i] = std::exp((this->_data[j * this->_cols + i]) * -1);
    return temp;
}


template <typename Y>
Matrix<Y> Matrix<Y>::sigmoid() {
    Matrix<Y> temp(this->expa());

    for (yeet j = 0; j < temp._rows; j++)
        for (yeet i = 0; i < temp._cols; i++)
            temp._data[j * temp._cols + i] = 1.0 / (1.0 + temp._data[j * temp._cols + i]);
    return temp;
}


template <typename Y>
Matrix<Y> Matrix<Y>::sigmoid_derivative() {
    Matrix<Y> temp(this->_rows, this->_cols);

    for (yeet j = 0; j < temp._rows; j++)
        for (yeet i = 0; i < temp._cols; i++)
            temp._data[j * temp._cols + i] = this->_data[j * temp._cols + i] * (1 - this->_data[j * temp._cols + i]);
    return temp;
}

template <typename Y>
double Matrix<Y>::mean() {
    double output = 0.0;
    double count = this->size();
    double suma = 0.0;

    double* arr = this->getArray();

    for (yeet i = 0; i < count; i++)
        suma += arr[i];

    output = (suma / (double)count);

    delete[] arr;

    return output;
}

template <typename Y>
Matrix<Y> Matrix<Y>::square() {
    Matrix<double> temp(*this);
    yeet len = size();

    for (yeet i = 0; i < len; i++)
        temp._data[i] *= temp._data[i];

    return temp;
}


/////////////////Operatory//////////////////////////////////////
template<typename Y>
Matrix<Y> Matrix<Y>::operator=(const Matrix<Y>& rhs) {
    this->free();

    this->_rows = rhs._rows;
    this->_cols = rhs._cols;

    this->initMatrix();
    for (yeet i = 0; i < size(); i++) _data[i] = rhs._data[i];

    return *this;
}

template <typename Y>
Matrix<Y> Matrix<Y>::operator+=(const Matrix<Y>& rhs) {
    for (yeet i = 0; i < size(); i++)
        this->_data[i] += rhs._data[i];
    return *this;
}
template <typename Y>
Matrix<Y> Matrix<Y>::operator-=(const Matrix<Y>& rhs) {
    for (yeet i = 0; i < size(); i++)
        this->_data[i] -= rhs._data[i];
    return *this;
}
template <typename Y>
Matrix<Y> Matrix<Y>::operator*=(const Matrix<Y>& rhs) {
    for (yeet i = 0; i < size(); i++)
        this->_data[i] *= rhs._data[i];
    return *this;
}
template <typename Y>
Matrix<Y> Matrix<Y>::operator*=(const Y& rhs) {
    for (yeet i = 0; i < size(); i++)
        this->_data[i] *= rhs;
    return *this;
}
template <typename Y>
Matrix<Y> Matrix<Y>::operator/=(const Y& rhs) {
    for (yeet i = 0; i < size(); i++)
        this->_data[i] /= rhs;
    return *this;
}

/////Reszta operator�w//////////////////////////////////////////
template <typename Y>
Matrix<Y> Matrix<Y>::operator+(const Matrix<Y>& rhs) {
    Matrix<Y> temp(*this);
    for (yeet i = 0; i < size(); i++)
        temp._data[i] += rhs._data[i];
    return temp;
}
template <typename Y>
Matrix<Y> Matrix<Y>::operator-(const Matrix<Y>& rhs) {
    Matrix<Y> temp(*this);
    for (yeet i = 0; i < size(); i++)
        temp._data[i] -= rhs._data[i];
    return temp;
}
template <typename Y>
Matrix<Y> Matrix<Y>::operator*(const Matrix<Y>& rhs) {
    Matrix<Y> temp(*this);
    for (yeet i = 0; i < size(); i++)
        temp._data[i] *= rhs._data[i];
    return temp;
}
template <typename Y>
Matrix<Y> Matrix<Y>::operator*(const Y& rhs) {
    Matrix<Y> temp(*this);
    for (yeet i = 0; i < size(); i++)
        temp._data[i] *= rhs;
    return temp;
}
template <typename Y>
Matrix<Y> Matrix<Y>::operator/(const Y& rhs) {
    Matrix<Y> temp(*this);
    for (yeet i = 0; i < size(); i++)
        temp._data[i] /= rhs;
    return temp;
}


////////////////////////////////////////////////////////////////
template<typename Y>
Matrix<Y>::Matrix(yeet rows, yeet cols, bool auto_del) : _rows(rows), _cols(cols), auto_delete(auto_del) {
    this->initMatrix();
}

template <typename Y>
Matrix<Y>::Matrix(yeet rows, yeet cols, vector<Y> data) : _rows(rows), _cols(cols), auto_delete(1) { //tu b��d
    this->initMatrix();
    for (yeet i = 0; i < data.size(); i++)
        _data[i] = data[i];
}
//nowe/////////////////////////////////////////////////////////
template<typename Y>
Matrix<Y>::Matrix(yeet rows, yeet cols, mutable Y* _data) : _rows(rows), _cols(cols), auto_delete(true) {
    this->initMatrix();
    for (yeet i = 0; i < size(); i++) this->_data[i] = _data[i];
}
template <typename Y>
Matrix<Y>::Matrix(const Matrix<Y>& mat) : auto_delete(true) {
    this->_cols = mat._cols;
    this->_rows = mat._rows;
    this->initMatrix();
    for (yeet i = 0; i < size(); i++)
        this->_data[i] = mat._data[i];
}
template<typename Y>
Matrix<Y>::Matrix(const std::string& macierz) : auto_delete(true) {
    //Podzia� na fragmenty
    yeet len = macierz.length();
    std::string fragment = "";
    vector <std::string> fragmenty;

    for (yeet i = 0; i < len; i++) {
        if (macierz[i] == '[') {
            fragment = "";
            do {
                i++;
                if (macierz[i] == ']') break;

                fragment += macierz[i];
            } while (i < len - 1);
            fragmenty.push_back(fragment);
        }
    }

    this->_rows = fragmenty.size();

    //Podzia� fragment�w na pojedyncze elementy
    vector <double> wartosci;
    short elementy = 0;

    for (yeet i = 0; i < this->_rows; i++) {
        len = fragmenty[i].size(); //d�ugo�� stringa

        for (yeet j = 0; j < len; j++) {
            fragment = "";
            while (fragmenty[i][j] != ',' && j < len) {
                fragment += fragmenty[i][j];
                j++;
            }
            string::size_type sz;
            wartosci.push_back(stod(fragment, &sz));
        }

        if (i == 0) elementy = wartosci.size();
    }

    this->_cols = elementy;

    //Inicjowanie nowej macierzy typu Matrix
    this->initMatrix();

    //Przenoszenie element�w z vector do macierzy typu Matrix
    yeet i = 0;
    for (yeet y = 0; y < this->_rows; y++) {
        for (yeet x = 0; x < this->_cols; x++) {
            this->_data[y * this->_cols + x] = wartosci[i];
            i++;
        }
    }

    //czyszczenie pami�ci
    fragmenty.clear();
    wartosci.clear();
}

template <typename Y>
void Matrix<Y>::add(std::string macierz) {
    //Podzia� na fragmenty
    yeet len = macierz.length();
    std::string fragment = "";
    vector <std::string> fragmenty;

    for (yeet i = 0; i < len; i++) {
        if (macierz[i] == '[') {
            fragment = "";
            do {
                i++;
                if (macierz[i] == ']') break;

                fragment += macierz[i];
            } while (i < len - 1);
            fragmenty.push_back(fragment);
        }
    }

    this->_rows = fragmenty.size();

    //Podzia� fragment�w na pojedyncze elementy
    vector <double> wartosci;
    short elementy = 0;

    for (yeet i = 0; i < this->_rows; i++) {
        len = fragmenty[i].size(); //d�ugo�� stringa

        for (yeet j = 0; j < len; j++) {
            fragment = "";
            while (fragmenty[i][j] != ',' && j < len) {
                fragment += fragmenty[i][j];
                j++;
            }
            string::size_type sz;
            wartosci.push_back(stod(fragment, &sz));
        }

        if (i == 0) elementy = wartosci.size();
    }

    this->_cols = elementy;

    //czyszczenie starej macierzy
    this->free();

    //Inicjowanie nowej macierzy typu Matrix
    this->initMatrix();

    //Przenoszenie element�w z vector do macierzy typu Matrix
    yeet i = 0;
    for (yeet y = 0; y < this->_rows; y++) {
        for (yeet x = 0; x < this->_cols; x++) {
            this->_data[y * this->_cols + x] = wartosci[i];
            i++;
        }
    }

    //czyszczenie pami�ci
    fragmenty.clear();
    wartosci.clear();
}

////////////////////////////////////////////////////////////////////
template<typename Y>
Y& Matrix<Y>::operator()(yeet row, yeet col) {
    return _data[_cols * row + col];
}
template<typename Y>
Y Matrix<Y>::operator()(yeet row, yeet col) const {
    return _data[_cols * row + col];
}

template<typename Y>
bool Matrix<Y>::exists(yeet row, yeet col) const {
    return (row < _rows && col < _cols);
}

template<typename Y>
Matrix<Y>::~Matrix() {
    this->free(); //tutaj
}

template<typename Y>
void Matrix<Y>::free() {
    if (auto_delete) {
        for (yeet i = 0, c = size(); i < c; ++i) {
            //will do nothing if Y isn't a pointer
            deletep(_data[i]);
        }
    }
    //delete[] _data;
    deletea(_data);
}
#endif
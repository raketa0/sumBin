#include <vector>
#include <iostream>
#include <conio.h>
#include <string>
#include <algorithm>
#include <vector>
#include <fstream>

using namespace std;

struct BinaryFloat {
    string integerPart;
    string fractionalPart;

    BinaryFloat(string intPart = "0", string fracPart = "0") :
        integerPart(intPart), fractionalPart(fracPart) {}
};

bool is_binary(const string& s) {
    int cout = 0;
    for (char c : s) {
        if (c != '0' && c != '1' && c!='.') {
            return false;
        }
        if (c == '.') {
            cout = cout++;
            if (cout > 1) {
                return false;
            }
        }
    }
    return true;
}

void normalize(BinaryFloat& num1, BinaryFloat& num2) {
    int maxIntLength = max(num1.integerPart.length(), num2.integerPart.length());
    int maxFracLength = max(num1.fractionalPart.length(), num2.fractionalPart.length());

    num1.integerPart.insert(0, maxIntLength - num1.integerPart.length(), '0');
    num2.integerPart.insert(0, maxIntLength - num2.integerPart.length(), '0');

    num1.fractionalPart.append(maxFracLength - num1.fractionalPart.length(), '0');
    num2.fractionalPart.append(maxFracLength - num2.fractionalPart.length(), '0');
}

BinaryFloat addBinary(const BinaryFloat& num1, const BinaryFloat& num2) {
    BinaryFloat result;
    int carry = 0;

    string fractionalPartResult = "";
    int n = num1.fractionalPart.length();
    for (int i = n - 1; i >= 0; --i) {
        int bit1 = num1.fractionalPart[i] - '0';
        int bit2 = num2.fractionalPart[i] - '0';
        int sum = bit1 + bit2 + carry;
        fractionalPartResult = char((sum % 2) + '0') + fractionalPartResult;
        carry = sum / 2;
    }
    result.fractionalPart = fractionalPartResult;

    string integerPartResult = "";
    int m = num1.integerPart.length();
    for (int i = m - 1; i >= 0; --i) {
        int bit1 = num1.integerPart[i] - '0';
        int bit2 = num2.integerPart[i] - '0';
        int sum = bit1 + bit2 + carry;
        integerPartResult = char((sum % 2) + '0') + integerPartResult;
        carry = sum / 2;
    }
    if (carry) {
        integerPartResult = '1' + integerPartResult;
    }
    result.integerPart = integerPartResult;

    return result;
}

BinaryFloat addBinaryFloat(BinaryFloat num1, BinaryFloat num2) {
    normalize(num1, num2);
    return addBinary(num1, num2);
}

void printBinaryFloat(const BinaryFloat& num) {
    string intPart = num.integerPart;
    string fracPart = num.fractionalPart;

    if (intPart.length() > 1 && intPart.find_first_not_of('0') != string::npos) {
        intPart.erase(0, intPart.find_first_not_of('0'));
    }
    else if (intPart.length() > 1 && intPart.find_first_not_of('0') == string::npos) {
        intPart = "0";
    }

    if (fracPart.length() > 0) {
        fracPart.erase(fracPart.find_last_not_of('0') + 1);
    }


    cout << intPart;
    if (!fracPart.empty()) {
        cout << "." << fracPart;
    }
    cout << endl;
}

BinaryFloat parseBinaryFloat(const string& binaryString) {
    BinaryFloat result;
    size_t dotPos = binaryString.find('.');

    if (dotPos == string::npos) {
        result.integerPart = binaryString;
        result.fractionalPart = "0";
    }
    else {
        result.integerPart = binaryString.substr(0, dotPos);
        result.fractionalPart = binaryString.substr(dotPos + 1);
    }

    if (result.integerPart.empty()) {
        result.integerPart = "0";
    }

    return result;
}

int main() {
    string binaryString1, binaryString2;

    setlocale(LC_ALL, "Russian");
    string x, y, line;
    int k;

    ifstream inputFile("Test.csv");
    if (!inputFile.is_open()) {
        cerr << "Ошибка при открытии файла." << endl;
        return 1;
    }

    ofstream outputFile("result.csv", ios::app);
    if (!outputFile.is_open()) {
        cerr << "Ошибка при открытии файла." << endl;
        return 2;
    }

    while (getline(inputFile, line)) {
        bool okflag = true;
        k = line.find(",");
        if (k != string::npos) {
            x = line.substr(0, k);
            y = line.substr(k + 1);

            if (!is_binary(x)) {
                cerr << "Число " << x << " не является двоичным." << endl;
                okflag = false;
            }
            if (!is_binary(y)) {
                cerr << "Число " << y << " не является двоичным." << endl;
                okflag = false;
            }
            if (x.empty() || y.empty()) {
                cerr << "Число отсутствует" << endl;
                okflag = false;
            }
            if (okflag) {
                BinaryFloat binNumOne = parseBinaryFloat(x);
                BinaryFloat binNumTwo = parseBinaryFloat(y);
                BinaryFloat result = addBinaryFloat(binNumOne, binNumTwo);
                outputFile << "Сумма чисел" << x << ' и ' << y << ',' << "Полученное число равно " << result.integerPart << '.' << result.fractionalPart << endl;
            }
            else
            {
                outputFile << "ERROR" << endl;
            }
        }
    }

    return 0;
}

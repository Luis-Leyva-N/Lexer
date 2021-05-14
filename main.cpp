#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include <vector>
#include <iomanip>

using std::cout;
using std::vector;
using std::ifstream;
using std::string;
using std::pair;
using std::make_pair;
using std::getline;
using std::remove;
using std::endl;
using std::setw;

/* Funcion usada para verificar que el token anterior sea entero, real o variable.
Se agrega a la tabla en casa de que resulte verdadero algun
Evitar repticion de codigo */
void lastToken(int& numType, string& word, vector<pair<string, string>>& output, vector<string> types) {
    if (numType == 0 || numType == 1 || numType == 2 || numType == 8 || numType == 9) {
        output.push_back(make_pair(word, types[numType]));
        word.clear();
    }
    numType = -1;
}

void lexerAritmetico(ifstream& expresiones) {
    int numType; // Se usa esta variable para verificar si es una variable con numeros, un entero o real.
    char prevch; // Caracter previo.
    string line, word; // Linea leida del texto y palabra que se formara al analizar los caracteres.
    vector<pair<string, string>> output; // Vector que almacena la tabla
    vector<string> types{ "Variable", "Entero", "Real", "Asignacion", "Suma", "Resta",
        "Multiplicacion", "Division", "Comentario", "Parentesis inicial", "Parentesis final", "Potencia" };

    //No se cubren todos los errores de sintaxis para no exteneder el codigo mas de lo necesario.
    while (getline(expresiones, line)) {
        line.erase(remove(line.begin(), line.end(), ' '), line.end());
        for (auto ch : line) {
            //Comentarios y divisiones
            if (ch == '/' && prevch != '/') {
                lastToken(numType, word, output, types);
                word.push_back(ch);
                prevch = ch;
            } else if (ch == '/' && prevch == '/') {
                word.push_back(ch);
                numType = 8;
                prevch = ch;
            } else if (word[1] == '/' && word[0] == '/') {
                word.push_back(ch);
                prevch = ch;
            } else if (ch != '/' && prevch == '/') {
                output.push_back(make_pair(word, types[7]));
                word.clear();
            }
            switch (ch) {
                // Signo de asigancion
                case '=':
                    //Si se usa el signo asignacion se entiende que se declaro una variable previamente asi que limpiamos la palabra y la asinamos al vector de output.
                    lastToken(numType, word, output, types);

                    //Se realiza la relacion del simbolo con su tipo
                    output.push_back(make_pair(string(1, ch), types[3]));
                    prevch = ch;
                    break;
                    //Suma
                case '+':
                    //Guardamos el token anterior.
                    lastToken(numType, word, output, types);
                    //Guardamos el token actual
                    output.push_back(make_pair(string(1, ch), types[4]));
                    prevch = ch;
                    break;
                case '*':
                    //Guardamos el token anterior.
                    lastToken(numType, word, output, types);
                    //Guardamos el token actual
                    output.push_back(make_pair(string(1, ch), types[6]));
                    prevch = ch;
                    break;
                case '(':
                    //Guardamos el token anterior.
                    lastToken(numType, word, output, types);
                    //Guardamos el token actual
                    output.push_back(make_pair(string(1, ch), types[9]));
                    prevch = ch;
                    break;
                case ')':
                    //Guardamos el token anterior.
                    lastToken(numType, word, output, types);
                    //Guardamos el token actual
                    output.push_back(make_pair(string(1, ch), types[10]));
                    prevch = ch;
                    break;
                case '^':
                    //Guardamos el token anterior.
                    lastToken(numType, word, output, types);
                    //Guardamos el token actual
                    output.push_back(make_pair(string(1, ch), types[11]));
                    prevch = ch;
                    break;
                default:
                    if ((word.empty() && isalpha(ch)) || (isalpha(word.front()) && ch != '/')) {
                        //Variables
                        numType = 0;
                        word.push_back(ch);
                        prevch = ch;
                    } else if ((isdigit(ch) && word.empty()) || (isdigit(word.front()) && isdigit(ch) && word.find('.') == word.npos) || (word.front() == '-' && isdigit(ch) && word.find('.') == word.npos)) {
                        //Numeros enteros
                        numType = 1;
                        word.push_back(ch);
                        prevch = ch;
                    } else if (ch == '.' && word.empty()) {
                        numType = 2;
                        word.push_back(ch);
                        prevch = ch;
                    } else if (((isdigit(word.front()) && ch != '-') || (word.front() == '-' && ch == '.')) || ((isdigit(ch) || (ch == 'E' || ch == 'e')) && word.find('.') != word.npos)) {
                        //Numeros reales
                        numType = 2;
                        word.push_back(ch);
                        prevch = ch;
                    } else if (word.find('.') != word.npos && (word.find('E') != word.npos || word.find('e') != word.npos) && ch == '-') {
                        //Numeros reales con E
                        word.push_back(ch);
                        prevch = ch;
                    } else if (word.empty() && ch == '-' && !isdigit(prevch) && !isalpha(prevch)) {
                        //Numeros negativos
                        word.push_back(ch);
                        prevch = ch;
                    } else if (ch == '-' && (isdigit(prevch) || isalpha(prevch))) {
                        //Restas
                        lastToken(numType, word, output, types);
                        output.push_back(make_pair(string(1, ch), types[5]));
                        prevch = ch;
                    }
                    break;
            }
        }

        lastToken(numType, word, output, types);
    }

    cout.setf(std::ios_base::adjustfield, std::ios_base::left);
    cout << setw(40) << "Token" << "Type" << endl;
    for (auto out : output) {
        cout << setw(40) << out.first << out.second << endl;
    }
}

int main() {
    ifstream expresiones("expresiones.txt");

    lexerAritmetico(expresiones);
    return 0;
}
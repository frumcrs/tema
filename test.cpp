#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>

using namespace std;

class BigNumber {

private:

  vector<unsigned char> _num;
  unsigned char _base;
  int _sign;


public:

  BigNumber(long long int number, unsigned char base = 10){
    _sign = (number >= 0 ? 1 : -1);
    _base = base;
    if(number < 0) number = -number;
    if(number < 10){
      _num.push_back(number);
      return;
    }
    while(number > 0){
      _num.push_back(number % base);
      number /= base;
    }
  }

  BigNumber(int number = 0, unsigned char base = 10) : BigNumber(static_cast<long long int>(number), base) {}

  BigNumber(string number, unsigned char base = 10) {
    if(*number.begin() == '-') {
      number.erase(0, 1);
      _sign = -1;
    }
    else _sign = 1;
    _base = base;
    vector<unsigned char> aux;
    for(unsigned char s : number) aux.push_back(s);
    for(vector<unsigned char>::iterator it = aux.end(); it != aux.begin(); it--) this->_num.push_back(*it);
  }
  //nu e nevoie de destructor

  int getSign() {
    return this->_sign;
  }

  unsigned char getBase() {
    return this->_base;
  }

  //functioneaza si pe (int big) (big int) (big big) deoarece exista un constructor default pentru o valoare int
  bool operator <(const BigNumber& other) {
    if(this->_sign != other._sign){
      if(this->_sign == -1) return true;
      else return false;
    }
    if(this->_sign == 1){
      if(this->_num.size() > other._num.size()) return false; // daca argumentu are mai multe cifre
      if(this->_num.size() < other._num.size()) return true; // daca argumentu are mai putine cifre
      if(this->_num.size() == other._num.size()) { // daca au nr de cifre egale
        for(int i = 0; i < this->_num.size(); i++) {
          if(this->_num.at(i) == other._num.at(i)) continue; // da skip pana da de 2 cifre care nu sunt egale
          if(this->_num.at(i) < other._num.at(i)) return true; // compara cele 2 cifre decisive
          else return false;
        }
      }
      return false;
    }
    else {
      if(this->_num.size() > other._num.size()) return true;
      if(this->_num.size() > other._num.size()) return false;
      if(this->_num.size() == other._num.size()) {
        for(int i = 0; i < this->_num.size(); i++) {
          if(this->_num.at(i) == other._num.at(i)) continue;
          if(this->_num.at(i) < other._num.at(i)) return false;
          else return true;
        }
      }
      return false;
    }
  }

  bool operator ==(const BigNumber& other) {
    if(this->_sign != other._sign) return false; // daca au semn diferit e fals

    if(this->_num.size() != this->_num.size()) return false; // daca au nr diferit de cifre e fals
    else {
      for(int i = 0; i < this->_num.size(); i++) {
        if(this->_num.at(i) != other._num.at(i)) return false; // daca gaseste o cifra diferita e fals
      }
    }
    return true;
  }

  bool operator !=(const BigNumber& other) {
    return !(*this == other);
  }

  bool operator <=(const BigNumber& other) {
    if(*this < other || *this == other) return true;
    return false;
  }

  bool operator >(const BigNumber& other) {
    if(!(*this < other) && !(*this == other)) return true;
    return false;
  }

  bool operator >=(const BigNumber& other) {
    if(*this > other || *this == other) return true;
    return false;
  }

  BigNumber& operator +=(const BigNumber& other) {
    if(this->_base != other._base) throw invalid_argument("Different bases of provided numbers");

    unsigned char reminder = 0; // cifra pe care o " tinem minte " la adunare
    unsigned char little_sum = 0;
    unsigned char max_size = max(this->_num.size(), other._num.size());
    unsigned char min_size;

    if(this->_sign == other._sign) { // daca au acelasi semn
      if(this->_num.size() <= other._num.size()) min_size = this->_num.size(); //retin minim
      else min_size = other._num.size();

      for(int i = 0; i < max_size || reminder > 0; i++) {
        unsigned char fnl = (i < this->_num.size() ? this->_num.at(i) : 0); // incepe de la ultima cifra
        unsigned char snl = (i < other._num.size() ? other._num.at(i) : 0); // incepe de la ultima cifra
        little_sum = fnl + snl + reminder;
        reminder = little_sum / this->_base;
        little_sum %= this->_base;

        if(i >= min_size) this->_num.push_back(little_sum); // daca am trecut de numarul mai mic, adaugam la sfarsit suplimentar
        else this->_num.at(i) = little_sum; // altfel, inlocuim
      }
    }
    else {
      if(this->_num.size() <= other._num.size()) min_size = this->_num.size();
      else min_size = other._num.size();

      this->_sign = other._sign;
      if((*this > other && this->_sign == 1) || (*this <= other && this->_sign == -1)) {
        for(int i = 0; i < min_size || reminder > 0; i++) {
          unsigned char fnl = this->_num.at(i);
          unsigned char snl = (i < other._num.size() ? other._num.at(i) : 0);
          little_sum = this->_base + fnl - snl - reminder;
          if(little_sum < this->_base) {
            reminder = 1;
          }
          else {
            reminder = 0;
            little_sum -= this->_base;
          }
          this->_num.at(i) = little_sum;
        }
        this->_sign = 1;
      }
      else {
        for(int i = 0; i < min_size || reminder > 0; i++) {
          unsigned char fnl = other._num.at(i);
          unsigned char snl = (i < this->_num.size() ? this->_num.at(i) : 0);
          little_sum = this->_base + fnl - snl - reminder;
          if(little_sum < this->_base) {
            reminder = 1;
          }
          else {
            reminder = 0;
            little_sum -= this->_base;
          }
          if(i < this->_num.size()) this->_num.at(i) = little_sum;
          else this->_num.push_back(little_sum);
        }
        this->_sign = -1;
      }
    }
    return *this;
  }

  BigNumber operator *=(const BigNumber& other){
    BigNumber result = 0;
    for(BigNumber i = 1; i <= other._num.size(); i += 1) result += other; // in loc sa inmultim, folosim adunarea de cate ori e nevoie
    return result;
  }

  BigNumber operator -=(const BigNumber& other){
    BigNumber aux = other;
    aux *= -1;
    *this += aux; // folosim adunarea cu minus
    return *this;
  }

  BigNumber operator /=(const BigNumber& other) {
    BigNumber count = 0;
    BigNumber copy = *this;
    if(*this < 0) copy *= -1;
    while(copy > 0) {
      copy -= other;
      count += 1; // vedem de cate ori am scazut care va fi partea intreaga ramasa
    }
    count._sign = this->_sign; // semnul va fi semnul initial
    *this = count;

    return *this;
  }

  friend BigNumber operator %=(const BigNumber& other) {
    BigNumber copy = *this;
    if(*this < 0) copy *= -1;
    while(copy > 0) copy -= other; // scadem de cate ori putem
    copy._sign = this->_sign; // semnu ramane cel initial
    *this = copy; // inlocuim valoarea initiala cu ce a ramas

    return *this;
  }

  friend BigNumber operator ^=(const BigNumber& other){
    BigNumber result = 1;
    for(BigNumber i = 1; i <= other; i += 1) result *= other;
    return result;
  }

  friend BigNumber operator +(const BigNumber& first, const BigNumber& second){
    BigNumber third = 0;
    third += first;
    third += second;
    return third;
  }

  friend BigNumber operator -(const BigNumber& first, const BigNumber& second){
    BigNumber third = 0;
    third += first;
    third -= second;
    return third;
  }

  friend BigNumber operator *(const BigNumber& first, const BigNumber& second){
    BigNumber third = 1;
    third *= first;
    third *= second;
    return third;
  }

  friend BigNumber operator /(const BigNumber& first, const BigNumber& second){
    BigNumber third = first;
    third /= second;
    return third;
  }

  friend BigNumber operator %(const BigNumber& first, const BigNumber& second){
    BigNumber third = first;
    third %= second;
    return third;
  }

  friend BigNumber operator ^(const BigNumber& first, const BigNumber& second){
    BigNumber third = first;
    third ^= second;
    return third;
  }

  friend BigNumber operator +(const BigNumber& number) {
    return number;
  }

  friend BigNumber operator -(const BigNumber& number) {
    BigNumber num = number;
    num *= -1;
    return num;
  }

  friend ostream& operator <<(ostream& stream, const BigNumber& number) {
    if(number._sign == -1) stream << "-";
    for(int i = number._num.size() - 1; i >= 0; i--){
      stream << to_string(number._num.at(i));
    }
    return stream;
  }
  friend istream& operator >>(istream& stream, BigNumber& number) {
    string s;
    int x;
    stream >> s;
    stream >> x;
    number = BigNumber(s,x);
    return stream;
  }
};

int main(){
  BigNumber a =10 ;
  BigNumber b = 143 ;
    cout<< (a *= b);

  return 0;
}

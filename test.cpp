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
      if(this->_num.size() > this->_num.size()) return false;
      if(this->_num.size() > this->_num.size()) return true;
      if(this->_num.size() == this->_num.size()) {
        for(int i = 0; i < this->_num.size(); i++) {
          if(this->_num.at(i) == other._num.at(i)) continue;
          if(this->_num.at(i) < other._num.at(i)) return true;
          else return false;
        }
      }
      return false;
    }
    else {
      if(this->_num.size() > this->_num.size()) return true;
      if(this->_num.size() > this->_num.size()) return false;
      if(this->_num.size() == this->_num.size()) {
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
    if(this->_sign != other._sign) return false;

    if(this->_num.size() != this->_num.size()) return false;
    else {
      for(int i = 0; i < this->_num.size(); i++) {
        if(this->_num.at(i) != other._num.at(i)) return false;
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
    if(this->_base != other._base) throw invalid_argument("Different bases of numbers provided");

    unsigned char reminder = 0;
    unsigned char little_sum = 0;
    unsigned char max_size = max(this->_num.size(), other._num.size());
    unsigned char min_size;

    if(this->_sign == other._sign) {
      if(this->_num.size() <= other._num.size()) min_size = this->_num.size();
      else min_size = max_size;

      for(int i = 0; i < max_size || reminder > 0; i++) {
        unsigned char fnl = (i < this->_num.size() ? this->_num.at(i) : 0);
        unsigned char snl = (i < other._num.size() ? other._num.at(i) : 0);
        little_sum = fnl + snl + reminder;
        reminder = little_sum / this->_base;
        little_sum %= this->_base;

        if(i >= min_size) this->_num.push_back(little_sum);
        else this->_num.at(i) = little_sum;
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
    for(BigNumber i = 1; i <= other; i += 1) result += other;
    return result;
  }

  BigNumber operator -=(const BigNumber& other){
    BigNumber aux = other;
    aux *= -1;
    *this *= aux;
    return *this;
  }

  BigNumber operator /=(const BigNumber& other) {
    BigNumber count = 0;
    BigNumber copy = *this;
    if(*this < 0) copy *= -1;
    while(copy > 0) {
      copy -= other;
      count += 1;
    }
    count._sign = this->_sign;
    *this = count;

    return *this;
  }

  BigNumber operator %=(const BigNumber& other) {
    BigNumber copy = *this;
    if(*this < 0) copy *= -1;
    while(copy > 0) copy -= other;
    copy._sign = this->_sign;
    *this = copy;

    return *this;
  }

  BigNumber operator ^=(const BigNumber& other){
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
    stream >> s;
    number = BigNumber(s);
    return stream;
  }
};

int main(){
  BigNumber a = 19243;
  BigNumber b = -132415;
  a += b;
  cout << (a + b) << "\n";
  return 0;
}

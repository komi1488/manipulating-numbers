/*********************
 *Автор: Дедушев П.А.*
 *********************/
#include <iostream>
#include <string>
using namespace std;

void removeLeadingZeros(string& num) {
  while (num.length() > 1 && num[0] == '0') {
    num.erase(0, 1);
  }
}

string reverseString(string str) {
  string result = str;
  for (int pos = 0; pos < result.length() / 2; ++pos) {
    swap(result[pos], result[result.length() - 1 - pos]);
  }
  return result;
}

int compareNumbers(string first, string second) {
  if (first.length() != second.length()) {
    return (first.length() > second.length()) ? 1 : -1;
  }
  for (int pos = 0; pos < first.length(); ++pos) {
    if (first[pos] != second[pos]) {
      return (first[pos] > second[pos]) ? 1 : -1;
    }
  }
  return 0;
}

string addNumbers(string first, string second) {
  string firstRev = reverseString(first);
  string secondRev = reverseString(second);
  string result = "";
  int carry = 0;

  for (int pos = 0; pos < max(firstRev.length(), secondRev.length()); ++pos) {
    int digitFirst = (pos < firstRev.length()) ? (firstRev[pos] - '0') : 0;
    int digitSecond = (pos < secondRev.length()) ? (secondRev[pos] - '0') : 0;
    
    int sum = digitFirst + digitSecond + carry;
    result += (sum % 10) + '0';
    carry = sum / 10;
  }

  if (carry > 0) {
    result += '1';
  }

  return reverseString(result);
}

string subtractNumbers(string larger, string smaller) {
  string largerRev = reverseString(larger);
  string smallerRev = reverseString(smaller);
  string result = "";
  int borrow = 0;

  for (int pos = 0; pos < largerRev.length(); ++pos) {
    int digitLarger = (largerRev[pos] - '0') - borrow;
    int digitSmaller = (pos < smallerRev.length()) ? (smallerRev[pos] - '0') : 0;

    if (digitLarger < digitSmaller) {
      digitLarger += 10;
      borrow = 1;
    } else {
      borrow = 0;
    }

    result += (digitLarger - digitSmaller) + '0';
  }

  result = reverseString(result);
  removeLeadingZeros(result);
  return result;
}

string multiplyNumbers(string first, const string& second) {
  if (first == "0" || second == "0") {
    return "0";
  }

  string result(first.length() + second.length(), '0');
  string firstRev = reverseString(first);
  string secondRev = reverseString(second);

  for (int outerIndex = 0; outerIndex < firstRev.length(); ++outerIndex) {
    for (int innerIndex = 0; innerIndex < secondRev.length(); ++innerIndex) {
      int digitProduct = (firstRev[outerIndex] - '0') * (secondRev[innerIndex] - '0');
      int sumWithCarry = digitProduct + (result[outerIndex + innerIndex] - '0');
      
      result[outerIndex + innerIndex] = (sumWithCarry % 10) + '0';
      result[outerIndex + innerIndex + 1] += sumWithCarry / 10;
    }
  }

  for (int pos = 0; pos < result.length(); ++pos) {
    if (result[pos] > '9') {
      if (pos + 1 < result.length()) {
        result[pos + 1] += (result[pos] - '0') / 10;
        result[pos] = ((result[pos] - '0') % 10) + '0';
      }
    }
  }

  result = reverseString(result);
  removeLeadingZeros(result);
  return result;
}

void divideNumbers(string dividend, string divisor, string& quotient, string& remainder) {
  quotient = "0";
  remainder = dividend;

  if (divisor == "0") {
    quotient = "ERROR: division by zero";
    remainder = "0";
    return;
  }

  while (compareNumbers(remainder, divisor) >= 0) {
    string temp = divisor;
    string multiplier = "1";

    while (compareNumbers(addNumbers(temp, divisor), remainder) <= 0) {
      temp = addNumbers(temp, divisor);
      multiplier = addNumbers(multiplier, "1");
    }

    quotient = addNumbers(quotient, multiplier);
    remainder = subtractNumbers(remainder, temp);
  }
}

class BigNumber {
private:
  string digits;
  bool negative;

public:
  BigNumber() : digits("0"), negative(false) {}
  
  BigNumber(string input) {
    negative = false;
    digits = "0";

    for (char ch : input) {
      if (ch == '-') {
        negative = true;
      } else if (isdigit(ch)) {
        digits += ch;
      }
    }

    if (digits == "0") {
      negative = false;
    } else {
      removeLeadingZeros(digits);
    }
  }

  void display() const {
    if (negative && digits != "0") {
      cout << "-";
    }
    cout << digits;
  }

  BigNumber add(BigNumber other) const {
    BigNumber result;

    if (!negative && !other.negative) {
      result.digits = addNumbers(digits, other.digits);
      result.negative = false;
    } else if (negative && other.negative) {
      result.digits = addNumbers(digits, other.digits);
      result.negative = true;
    } else {
      int comp = compareNumbers(digits, other.digits);

      if (comp == 0) {
        result.digits = "0";
        result.negative = false;
      } else if (!negative) {
        if (comp > 0) {
          result.digits = subtractNumbers(digits, other.digits);
          result.negative = false;
        } else {
          result.digits = subtractNumbers(other.digits, digits);
          result.negative = true;
        }
      } else {
        if (comp > 0) {
          result.digits = subtractNumbers(digits, other.digits);
          result.negative = true;
        } else {
          result.digits = subtractNumbers(other.digits, digits);
          result.negative = false;
        }
      }
    }
    return result;
  }

  BigNumber subtract(BigNumber other) const {
    BigNumber negOther = other;
    negOther.negative = !negOther.negative;
    return add(negOther);
  }

  BigNumber multiply(BigNumber other) const {
    BigNumber result;
    result.digits = multiplyNumbers(digits, other.digits);
    result.negative = (negative != other.negative);
    return result;
  }

  BigNumber divide(BigNumber divisor, BigNumber& remainder) const {
    BigNumber quotient;
    string quotStr, remStr;

    divideNumbers(digits, divisor.digits, quotStr, remStr);

    quotient.digits = quotStr;
    quotient.negative = (negative != divisor.negative);

    remainder.digits = remStr;
    remainder.negative = negative;

    return quotient;
  }
};

int main() {
  string num1, num2;

  cout << "Enter first number: ";
  cin >> num1;
  cout << "Enter second number: ";
  cin >> num2;

  BigNumber a(num1);
  BigNumber b(num2);
  BigNumber remainder("0");

  BigNumber sum = a.add(b);
  BigNumber difference = a.subtract(b);
  BigNumber product = a.multiply(b);
  BigNumber quotient = a.divide(b, remainder);

  cout << "Sum: ";
  sum.display();
  cout << endl;

  cout << "Difference: ";
  difference.display();
  cout << endl;

  cout << "Product: ";
  product.display();
  cout << endl;

  cout << "Quotient: ";
  quotient.display();
  cout << endl;

  cout << "Remainder: ";
  remainder.display();
  cout << endl;

  return 0;
}
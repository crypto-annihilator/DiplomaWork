#include <vector>
#include <algorithm>
#include "Parser.h"
#include <limits>
#include <cmath>
#include <iostream>


////////////////////////////////////////////
Parser* Parser::s_parser = nullptr;

////////////////////////////////////////////
Parser* Parser::getParser()
{
    if (s_parser == nullptr) {
        s_parser = new Parser();
    }
    return s_parser;
}

////////////////////////////////////////////
int Parser::findeSign(const std::string& key, const std::string& line)
{
    const auto pos = line.find_first_of(key);
    if (pos == std::string::npos) {
        return std::numeric_limits<int>::max();
    }
    return pos;
}

////////////////////////////////////////////
inline void Parser::min(int& first, int second)
{
    if ( first > second) {
        first = second;
    }
}

////////////////////////////////////////////
int Parser::findeFirstSign(const std::string& str)
{
    int first = findeSign("+", str);
    int second = findeSign("-", str);
    min(first, second);
    second = findeSign("*", str);
    min(first, second);
    second = findeSign("/", str);
    min(first, second);
    second = findeSign("(", str);
    min(first, second);
    second = findeSign(")", str);
    min(first, second);
    second = findeSign("^", str);
    min(first, second);
    return first;
}

////////////////////////////////////////////
int Parser::findeKey(const std::string key, const std::string& line, int& lenght)
{
    const auto pos = line.find(key);
    if (pos == std::string::npos) {
        return std::numeric_limits<int>::max();
    }
    lenght = key.size();
    return pos;
}

////////////////////////////////////////////
inline void Parser::min(int& first, int second, int& lfirst, int lsecond)
{
    if ( first > second) {
        first = second;
        lfirst = lsecond;
    }
}

////////////////////////////////////////////
int Parser::findeFirstKey(const std::string& str, int& lenght)
{
    int lfirst = 0;
    int lsecond = 0;
    int first = findeKey("cos", str, lfirst);
    int second = findeKey("sin", str, lsecond);
    min(first, second, lfirst, lsecond);
    second = findeKey("tg", str, lsecond);
    min(first, second, lfirst, lsecond);
    second = findeKey("ctg", str, lsecond);
    min(first, second, lfirst, lsecond);
    second = findeKey("arccos", str, lfirst);
    min(first, second, lfirst, lsecond);
    second = findeKey("arcsin", str, lsecond);
    min(first, second, lfirst, lsecond);
    second = findeKey("arctg", str, lsecond);
    min(first, second, lfirst, lsecond);
    second = findeKey("arcctg", str, lsecond);
    min(first, second, lfirst, lsecond);
    second = findeKey("ln", str, lsecond);
    min(first, second, lfirst, lsecond);
    second = findeKey("log", str, lsecond);
    min(first, second, lfirst, lsecond);
    second = findeKey("sqrt", str, lsecond);
    min(first, second, lfirst, lsecond);
    second = findeKey("root", str, lsecond);
    min(first, second, lfirst, lsecond);

    lenght = lfirst;
    return first;
}

////////////////////////////////////////////
int Parser::findeFirstKeyOrSign(const std::string& line, int& lenght)
{
    int llenght = 0;
    int first =  findeFirstSign(line);
    int second = findeFirstKey(line, llenght);
    if (first > second) {
        lenght = llenght;
        return second;
    }
    lenght = 1;
    return first;
}


////////////////////////////////////////////
void Parser::ParseLineIntoWords(std::string line, std::list<std::string>& words)
{
    std::string str;
    while(true) {
        if (line.empty()) {
            return;
        }
        int lenght;
        int index = findeFirstKeyOrSign(line, lenght);
        if (index == std::numeric_limits<int>::max()) {
            words.push_back(line);
            return;
        }
        str = line.substr(0, index);
        if (!str.empty()) {
            words.push_back(str);
        }
        words.push_back(line.substr(index, lenght));
        line = line.substr(index + lenght);
    }
    return;
}

////////////////////////////////////////////
bool Parser::isKey(std::string str)
{
    static std::vector<std::string> keys = { "cos",
        "sin", "tg", "ctg", "arccos", "arcsin", "arctg",
        "arcctg", "ln", "log", "sqrt", "root"};
    for(const auto& item : keys) {
        if(item == str) {
            return true;
        }
    }
    return false;
}

////////////////////////////////////////////
bool Parser::isSign(std::string str)
{
    static std::vector<std::string> keys = { "+",
        "-", "*", "/", "^"};
    for(const auto& item : keys) {
        if(item == str) {
            return true;
        }
    }
    return false;
}
////////////////////////////////////////////
bool Parser::isNumber(std::string str)
{
    bool have_point = false;
    for(auto it = str.begin(); it != str.end(); ++it) {
        if((*it < '0' ) || (*it > '9')){
            if ((*it == '.') && (!have_point)) {
                have_point = true;
            } else {
                return false;
            }
        }
    }
    if(str == "."){
        return false;
    }
    return true;
}

////////////////////////////////////////////
bool Parser::haveNumber(std::string str)
{
    try {
        std::stod(str);
    }
    catch(...) {
        return false;
    }
    return true;
}

////////////////////////////////////////////
bool Parser::isLetter(const std::string& str)
{
    if (str.size() != 1) {
        return false;
    }
    if (("z" >= str) && (str >= "a")) {
        return true;
    }
    return false;
}

////////////////////////////////////////////
bool Parser::isVariable(const std::string& str)
{
    if (str.size() == 1) {
        if (("a" <= str) && ("z" >= str) && ("e" != str)) {
            return true;
        }
        return false;
    }
    if (str.size() == 2) {
        return false;
    }

    bool status = true;
    status &= (('a' <= str[0]) && ('z' >= str[0]));
    status &= ('_' == str[1]);
    for (int i = 2; i < str.size(); ++i) {
        status &= isdigit(str[i]);
    }
    return status;
}

////////////////////////////////////////////
bool Parser::isValue(const std::string& str)
{
    if (isNumber(str)) {
        return true;
    }
    if (isVariable(str)) {
        return true;
    }
    return false;
}

////////////////////////////////////////////
void Parser::insert_after(std::list<std::string>& list, std::list<std::string>::iterator& it, std::string value)
{
    if (it != list.end()) {
        ++it;
        list.insert(it, value);
        --it;
    }
}

////////////////////////////////////////////
void Parser::parseStrtoStrandNumber(std::string str, std::string& str1, std::string& number)
{
    str1.clear();
    number.clear();
    int i = 0;
    bool have_point = false;
    while ((isdigit(str[i])) || ((str[i] == '.') && (!have_point))) {
        if (str[i] == '.') {
            have_point = true;
        }
        ++i;
    }
    if (i > 0) {
        str1 = str.substr(i);
        number = str.substr(0, i);
    } else {
        str1 = str;
    }
}

////////////////////////////////////////////
void Parser::parseStrToStrAndVariable(std::string str, std::string& str1, std::string& variable)
{
    variable.clear();
    str1.clear();
    if (str.size() == 1) {
        variable = str;
        return ;
    }
    variable = str.substr(0, 1);
    str1 = str.substr(1);
    int i = 0;
    while (isdigit(str1[i])) {
        ++i;
    }
    if (i > 0) {
        variable += "_";
        if (i == str1.size()) {
            variable += str1;
            str1.clear();
        } else {
            variable += str1.substr(0, i);
            str1 = str1.substr(i);
        }
    }
}

////////////////////////////////////////////
void Parser::parsAfterScop(std::list<std::string>& words, std::list<std::string>::iterator& it)
{
    std::list<std::string>::iterator lit;
    while(true) {
        lit = it;
        if (*it == "(") {
            parsAfterScop(words, ++it);
        } else if (*it == ")") {
            return ;
        } else {
            parseGrups(words, it, true);
        }
        if (lit == it) {
            ++it;
        }
    }
}

////////////////////////////////////////////
void Parser::parseLineVariableNumber (std::list<std::string>& words, std::list<std::string>::iterator& it, bool key)
{
    std::string str;
    bool flag = true;
    if (key) {
        words.insert(it, "(");
    }
    if (haveNumber(*it)) {
        std::string number;
        parseStrtoStrandNumber(*it, str, number);
        *it = number;
    } else {
        std::string variable;
        parseStrToStrAndVariable(*it, str, variable);
        *it = variable;
        if (str.empty()) {
            flag = false;
        }
    }
    ++it;
    if (key) {
        words.insert(it, ")");
    }
    if (flag) {
        words.insert(it, "*");
        words.insert(it, str);
        ----it;
    }
    --it;
}

////////////////////////////////////////////
void Parser::parsePower(std::list<std::string>& words, std::list<std::string>::iterator& it)
{
    if (++it == words.end()) {
        --it;
        return ;
    }
    if (*it != "^") {
        --it;
        return ;
    }
    if (++it == words.end()) {
        --it;
        return ;
    }
    if (*it == "(") {
        parsAfterScop(words, ++it);
        return ;
    }
    if (((*it).size() == 1) || (isNumber(*it))) {
        return ;
    }
    parseLineVariableNumber (words, it);
}

////////////////////////////////////////////
void Parser::parseKey(std::list<std::string>& words, std::list<std::string>::iterator& it)
{
    if (it != words.begin()) {
        --it;
        if (!isSign(*it) && *it != "(") {
            ++it;
            words.insert(it, "*");
        } else {
            ++it;
        }
    }
    if (*(++it) == "(") {
        parsAfterScop(words, ++it);
        parsePower(words, it);
        return ;
    }
    if (isKey(*it)) {
        words.insert(it, "(");
        parseKey(words, it);
        parsePower(words, it);
        insert_after(words, it, ")");
        return ;
    }
    if (((*it).size() == 1) || (isNumber(*it))) {
        words.insert(it, "(");
        parsePower(words, it);
        insert_after(words, it, ")");
        return;
    }
    parseLineVariableNumber (words, it, true);
}

////////////////////////////////////////////
void Parser::addMultiplicationSign(std::list<std::string>& words, std::list<std::string>::iterator& it)
{
    std::string str;
    if (haveNumber(*it)) {
        std::string number;
        parseStrtoStrandNumber(*it, str, number);
        *it = number;
        words.insert(++it, "*");
        words.insert(it, str) ;
        --it;
    }
    while (!isVariable(*it)) {
        std::string variable;
        parseStrToStrAndVariable(*it, str, variable);
        *it = variable;
        if (!str.empty()) {
            words.insert(++it, "*");
            words.insert(it, str);
        } else {
            break;
        }
        --it;
    }
    if (++it != words.end()) {
        if(isKey(*it) || (*it == "(")) {
            words.insert(it, "*");
        }
    }
    --it;
}

////////////////////////////////////////////
void Parser::parseOpenScop(std::list<std::string>& words)
{
    std::list<std::string>::iterator it = std::find(words.begin(), words.end(), "(");
    while (it != words.end()) {
        if (it != words.begin()) {
            if(isNumber(*(--it)) || (*it == ")") || isLetter(*it)) {
                ++it;
                words.insert(it, "*");;
                ++it;
            } else {
                ++++it;
            }
        } else {
            ++it;
        }
        it = std::find(it, words.end(), std::string("("));
    }
}

////////////////////////////////////////////
void Parser::parseCloseScop(std::list<std::string>& words)
{
    std::list<std::string>::iterator it = std::find(words.begin(), words.end(), ")");
    while (it != words.end()) {
        if (++it != words.end()) {
            if (isNumber(*it) || isLetter(*it)) {
                words.insert(it, "*");
                ++it;
            }
        }
        it = std::find(it, words.end(), std::string(")"));
    }

}

////////////////////////////////////////////
void Parser::parseGrups(std::list<std::string>& words, std::list<std::string>::iterator& it, bool isscop)
{
    if ((*it).size() == 1) {
        return ;
    }
    if (isNumber(*it)) {
        return ;
    }
    if (isKey(*it)) {
        parseKey(words, it);
        if(isscop && (*it == ")")) {
            if(++it == words.end())
            {
                --it;
            }
        }
        return ;
    }
    addMultiplicationSign(words, it);
}

////////////////////////////////////////////
void Parser::parse(std::string line, std::list<std::string>& words)
{
    line.erase(std::remove(line.begin(), line.end(), ' '), line.end());
    std::replace(line.begin(), line.end(), ',' , '.');
    ParseLineIntoWords(line, words);

    for (std::list<std::string>::iterator it = words.begin();
            it != words.end(); ++it) {
        parseGrups(words, it);
    }
    parseOpenScop(words);
    parseCloseScop(words);

    if (!checkErrors(words)) {
        std::cout << "error syntax" <<std::endl;
    }
}


/////////////////////////////////////////////////////////////////////////////
//check
/////////////////////////////////////////////////////////////////////////////
bool Parser::checkScop(const std::list<std::string>& list, std::list<std::string>::const_iterator& it)
{
    while (it != list.cend()) {
        if (*it == "(") {
          if (!checkScop(list, ++it)) {
            return false;
          }
          continue;
        }
        if (*it == ")"){
            ++it;
            return true;
        }
        ++it;
    }
    return false;
}
bool Parser::checkValue(const std::list<std::string>& list, std::list<std::string>::const_iterator& it)
{
    if (it == list.cbegin()) {
        ++it;
        if ((it != list.cend()) && (!isSign(*it)) && (*it != ")")) {
            return false;
        }
        --it;
        return true;
    }
    --it;
    if (!isSign(*it) && (*it != "(")) {
        return false;
    }
    ++++it;
    if ((it != list.cend()) && (!isSign(*it)) && (*it != ")")) {
        return false;
    }
    --it;
    return true;
}

////////////////////////////////////////////
bool Parser::checkSign(const std::list<std::string>& list, std::list<std::string>::const_iterator& it)
{
    if (it == list.cbegin()) {
        return false;
    }
    bool flag = (*it == "-");
    --it;
    if ((*it != ")") && (!isValue(*it)) && flag && (*it != "(")) {
        return false;
    }
    ++++it;
    if ((it == list.cend()) || ((*it != "(") && (!isValue(*it)) && (!isKey(*it)) && ("e" != *it))) {
        return false;
    }
    --it;
    return true;
}

////////////////////////////////////////////
bool Parser::checkOScop(const std::list<std::string>& list, std::list<std::string>::const_iterator& it)
{
    if (it == list.cbegin()) {
        ++it;
        if ((it == list.cend()) || (!isValue(*it) && !isKey(*it) && (*it != "(")) && (*it != "-")) {
            return false;
        }
        --it;
        return true;
    }
    --it;
    if ((!isSign(*it)) && (*it != "(") && !isKey(*it)) {
        return false;
    }
    ++++it;
    if ((it == list.cend()) || (!isValue(*it) && !isKey(*it) &&
                (*it != "(")) && (*it != "-") && (*it != "e")) {
        return false;
    }
    --it;
    return true;
}

////////////////////////////////////////////
bool Parser::checkCScop(const std::list<std::string>& list, std::list<std::string>::const_iterator& it)
{
    if (it == list.cbegin()) {
        return false;
    }
    --it;
    if (!isValue(*it) && (*it != ")")) {
        return false;
    }
    ++++it;
    if ((it != list.cend()) && !isSign(*it) && (*it != ")")) {
        return false;
    }
    --it;
    return true;
}


////////////////////////////////////////////
bool Parser::checkKey(const std::list<std::string>& list, std::list<std::string>::const_iterator& it)
{
    if (it == list.cbegin()) {
        ++it;
        if ((it == list.cend()) || (*it != "(")) {
            return false;
        }
        --it;
        return true;
    }
    --it;
    if ((!isSign(*it)) && (*it != "(")) {
        return false;
    }
    ++++it;
    if ((it == list.cend()) || (*it != "(")) {
        return false;
    }
    --it;
    return true;
}

////////////////////////////////////////////
bool Parser::checkE(const std::list<std::string>& list, std::list<std::string>::const_iterator& it)
{
    if (it == list.cbegin()) {
        ++it;
        if ((it == list.cend()) || (*it != "^")) {
            return false;
        }
        --it;
        return true;
    }
    --it;
    if (!isSign(*it) && (*it != "(")) {
        return false;
    }
    ++++it;
    if ((it == list.cend()) || (*it != "^")) {
        return false;
    }
    --it;
    return true;
}

////////////////////////////////////////////
bool Parser::checkErrors(const std::list<std::string>& list)
{
    for(std::list<std::string>::const_iterator it = list.cbegin();
            it != list.cend(); ++it) {
        if (isSign(*it)) {
            if (!checkSign(list, it)) {
                return false;
            }
        } else if (isValue(*it)) {
            if (!checkValue(list, it)) {
                return false;
            }
        } else if (isKey(*it)) {
            if (!checkKey(list, it)) {
                return false;
            }
        } else if (*it == ")") {
            if (!checkCScop(list, it)) {
                return false;
            }
        } else if (*it == "(") {
            if (!checkOScop(list, it)) {
                return false;
            }
        } else if (*it == "e") {
            if (!checkE(list, it)) {
                return false;
            }
        } else {
            return false;
        }
    }
    for(std::list<std::string>::const_iterator it = list.cbegin();
           it != list.cend(); ++it) {
        if (*it == "(") {
            if (!checkScop (list, ++it)) {
                return false;
            }
        }
        if ((it != list.cend()) && (*it == ")")) {
            return false;
        }
        if(it == list.cend()) {
            break;
        }
    }
    return true;
}


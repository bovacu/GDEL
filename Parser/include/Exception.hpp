#ifndef CORE_EXCEPTION_H
#define CORE_EXCEPTION_H

#include <iostream>
#include <sstream>
#include <algorithm>

class CoreException : public std::exception {};

class UnexpectedTokenException : public CoreException {
    public:
        UnexpectedTokenException(const char _char, int _line) {
            std::cerr << "\033[1;31m" << "Error: Unexpected token (" << _char << ") with ascii code ( "<< (int)_char << ") found in line " << _line << "\033[0m\n" << std::endl;
        }
        ~UnexpectedTokenException() { }
};

class UnexpectedEOFException : public CoreException {
    public:
        UnexpectedEOFException(int _line) {
            std::cerr << "\033[1;31m" << "Error: Unexpected EOF found in line " << _line << "\033[0m\n" << std::endl;
        }
        ~UnexpectedEOFException() { }
};

class UnexpectedTypexception : public CoreException {
    public:
        UnexpectedTypexception(const char* _typeExpected, const char* _typeGot, int _line) {
            std::cerr << "\033[1;31m" << "Error: Unexpected type " << _typeGot << ", expected " << _typeExpected << " in line " << _line << "\033[0m\n" << std::endl;
        }
        ~UnexpectedTypexception() { }
};

class IllegalAssigmentExpression : public CoreException {
    public:
        IllegalAssigmentExpression(const char* _assigment, int _line) {
            std::cerr << "\033[1;31m" << "Error: Illegal assigment of var '" << _assigment << "' in line " << _line << "\033[0m\n" << std::endl;
        }
        ~IllegalAssigmentExpression() { }
};

class MalformedStringException : public CoreException {
    public:
        MalformedStringException(const char* _string, int _line)  {
         
            std::stringstream _ss;
            int _length = strlen(_string);
            std::string _strCpy(_string);
            std::replace(_strCpy.begin(), _strCpy.end(), '\n', '\0');
            std::replace(_strCpy.begin(), _strCpy.end(), '\r', '\0');
            _ss << "\033[1;31m" << "Error: string (" << _strCpy.c_str() << ") expected to end with ('), but ended with (" << _string[_length - 1] << ") in line " << _line << "\033[0m\n";
            std::cerr << _ss.str() << std::endl;
            msg = _ss.str().c_str();
        }
        ~MalformedStringException() { }

        virtual const char* what() const throw () {
            return msg;
        }

    private:
        const char* msg;
};

class MalformedFloat : public CoreException {
    public:
        MalformedFloat(const char* _string, int _line)  {
         
            std::stringstream _ss;
            _ss << "\033[1;31m" << "Error: float (" << _string << ") can only have 1 (.) in line " << _line << "\033[0m\n";
            std::cerr << _ss.str() << std::endl;
            msg = _ss.str().c_str();
        }
        ~MalformedFloat() { }

        virtual const char* what() const throw () {
            return msg;
        }

    private:
        const char* msg;
};

#endif // CORE_EXCEPTION_H
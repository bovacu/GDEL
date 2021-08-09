#ifndef __PARSER__
#define __PARSER__

    #include "Parser/include/Defines.h"
    #include "Parser/include/Exception.hpp"

    #include "json.hpp"
    using namespace nlohmann;

    #include <map>

    #include <functional>
    using Func = std::function<json()>;

    #pragma region FUNCTION_HEADERS

        namespace TokenStripper {
            json getTokenType(const char* _code, int& _charPointer, int _linePointer);
            void numberToken(json& _outToken, const char* _code, int& _charPointer, int _linePointer);
            void stringToken(json& _outToken, const char* _code, int& _charPointer, int _linePointer);
            void boolToken(json& _outToken, const char* _code, int& _charPointer, int _linePointer);
            void nullToken(json& _outToken, const char* _code, int& _charPointer, int _linePointer);
            void idToken(json& _outToken, const char* _code, int& _charPointer, int _linePointer);
            std::string checkIfIsKeyword(const std::string& _fullId);
            void symbolToken(json& _outToken, const char* _code, int& _charPointer, int _linePointer);
        }

        json parse(const char* _code);
        json start();
        json statements();
        json eatToken(const char* _tokenType);
        bool hasMoreTokens();
        json getNextToken();

        json getAst();

        json getExpressionStatement();

        json getVariableStatement();
            json getVariableStatement();
            std::vector<json> getDeclarationList(bool _isSpecialDecl = false);
            json getVariableDeclaration(bool _isSpecialDecl = false);
            json getInitializer();

        json getFunctionStatement();
            std::vector<json> getParamListExec();
            std::vector<json> getParamListDecl();

        json getBlockStatement();
        json getIfStatement();
        json getReturnStatement();
        json getLoopStatement();
        json getForLoopStatement();

        json getStructStatement();
            json getSelfExpression();
            json getBaseExpression();
            json getNewExpression();

        json getImportStatement();

        json getExpression();
        json getAssignmentExpression();
        json getLogicalOrExpression();
        json getLogicalAndExpression();
        json getEqualityExpression();
        json getRelationalExpression();
        json getAdditiveExpression();
        json getMultiplicativeExpression();
        json getRangeExpression();
        json getLambdaExpression();
        json getUnaryExpression();
        json getCallExpression();
            std::vector<json> getArguments();
            std::vector<json> getArgumentList();
        json getMemberExpression();
        json getPrimaryExpression();
        json getParenthesisExpression();

        json getLiteral();
            json numberLiteral();
            json stringLiteral();
            json boolLiteral();
            json nullLiteral();
            json idLiteral();
            json symbolLiteral();

        json isAssignement(const char* _tokenType);
        bool isCorrectLeftSideAssigment(json& _left);
        json callee(json& _member);

    #pragma endregion FUNCTION_HEADERS

    #pragma region PARSER

        struct ParserData {
            const char* code = nullptr;
            int codeLength;

            int charPointer;
            int linePointer;

            json lookAhead;

            std::map<std::string, Func> statementFuncs;
        } parserData;

        json parse(const char* _code) {
            parserData.code = _code;
            parserData.codeLength = strlen(_code);
            parserData.linePointer = 0;
            parserData.charPointer = 0;
            parserData.lookAhead = {  };

            parserData.statementFuncs[_LEFT_COLLIBRACE] = getBlockStatement;
            parserData.statementFuncs[_VAR] = getVariableStatement;
            parserData.statementFuncs[_IF] = getIfStatement;
            parserData.statementFuncs[_LOOP] = getLoopStatement;
            parserData.statementFuncs[_FOR] = getForLoopStatement;
            parserData.statementFuncs[_FUNC] = getFunctionStatement;
            parserData.statementFuncs[_RET] = getReturnStatement;
            parserData.statementFuncs[_STRUCT] = getStructStatement;
            parserData.statementFuncs[_IMPORT] = getImportStatement;

            return start();
        }

        json start() {
            json _root {
                {"type", _PROGRAM},
                {"body", statements()["body"].get<json>()}
            };
            
            return _root;
        }

        json statements() {
            std::vector<json> _tokens;
            std::string _currentType;

            parserData.lookAhead = getNextToken();
            while(strcmp((_currentType = parserData.lookAhead["type"].get<std::string>()).c_str(), _EOF) != 0) {
                _tokens.push_back(getAst());
            }

            _tokens.push_back(parserData.lookAhead);

            return json {
                {"body", _tokens}
            };
        }

        json eatToken(const char* _tokenType) {
            const json _currentToken = parserData.lookAhead;
            
            if(strcmp(_currentToken["type"].get<std::string>().c_str(), _ERROR) == 0)
                throw UnexpectedEOFException(parserData.linePointer);
            
            auto _currentTokenType = _currentToken["type"].get<std::string>();
            if(strcmp(_currentTokenType.c_str(), _tokenType) != 0)
                throw UnexpectedTypexception(_tokenType, _currentTokenType.c_str(), _currentToken["value"].get<std::string>().c_str(), parserData.linePointer);
            
            parserData.lookAhead = getNextToken();
            
            return _currentToken;
        }

        bool hasMoreTokens() {
            return parserData.charPointer < parserData.codeLength;
        }

        bool isEof() {
            return parserData.charPointer >= parserData.codeLength;
        }

        json getNextToken() {
            if(!hasMoreTokens())
                return json{ {"type", _EOF}, {"value", _EOF} };

            // Get the type of token
            json _token;
            _token = TokenStripper::getTokenType(parserData.code, parserData.charPointer, parserData.linePointer);  

            // Unexpected token
            if(_token.size() == 0)
                throw UnexpectedTokenException(parserData.code[parserData.charPointer], parserData.linePointer);

            // React depending on the type of token
            auto _tokenType = _token["type"].get<std::string>();
            if(strcmp(_tokenType.c_str(), _IGNORE) == 0) {
                // Line counter for errors
                if(_token.contains("value"))
                    if(strcmp(_token["value"].get<std::string>().c_str(), _N) == 0 || 
                    strcmp(_token["value"].get<std::string>().c_str(), _R) == 0)
                        parserData.linePointer++;

                return getNextToken();
            } else if(strcmp(_tokenType.c_str(), _ERROR) != 0)
                return _token;

            return json {
                {"type", _EOF},
                {"value", _EOF}
            };
        }

        json getAst() {
            auto _tokenType = parserData.lookAhead["type"].get<std::string>();
            if(parserData.statementFuncs.find(_tokenType) != parserData.statementFuncs.end()){
                return parserData.statementFuncs.at(_tokenType)();
            }

            return getExpressionStatement();
        }

    #pragma endregion PARSER

    #pragma region TOKEN_STRIPPER
    namespace TokenStripper {
        json getTokenType(const char* _code, int& _charPointer, int _linePointer) {
            json _outToken;
            numberToken(_outToken, _code, _charPointer, _linePointer);
            stringToken(_outToken, _code, _charPointer, _linePointer);
            boolToken(_outToken, _code, _charPointer, _linePointer);
            nullToken(_outToken, _code, _charPointer, _linePointer);
            idToken(_outToken, _code, _charPointer, _linePointer);
            symbolToken(_outToken, _code, _charPointer, _linePointer);

            return _outToken;
        }

        void numberToken(json& _outToken, const char* _code, int& _charPointer, int _linePointer) {
            if(!_outToken.empty()) return;

            // int _min = 48, _max = 57, _ascii = (int)_code[_charPointer];
            // if(_ascii < _min || _ascii > _max)
            //     return;
            if(!std::isdigit(_code[_charPointer])) return;
            
            std::string _fullNumber;
            int _dots = 0;
            while(std::isdigit(_code[_charPointer]) || _code[_charPointer] == '.') {
                if(_code[_charPointer] == '.')
                    _dots++;

                if(_dots > 1) break;

                _fullNumber.push_back(_code[_charPointer]);
                _charPointer++;
            }

            if(_dots > 3)
                throw MalformedFloat(_fullNumber.c_str(), _linePointer);

            if(_dots > 1) {
                _charPointer--;
                _fullNumber.erase(std::remove(_fullNumber.begin(), _fullNumber.end(), '.'), _fullNumber.end());
                _outToken = json {
                    {"type", _INTEGER},
                    {"value",  _fullNumber.c_str()}  
                };
            } else if(_dots == 1) {
                _outToken = json {
                    {"type", _FLOAT},
                    {"value",  _fullNumber.c_str()}  
                };
            } else {
                _outToken = json {
                    {"type", _INTEGER},
                    {"value",  _fullNumber.c_str()}  
                };
            }
        }

        void stringToken(json& _outToken, const char* _code, int& _charPointer, int _linePointer) {
            if(!_outToken.empty()) return;

            int _quote = 39, _ascii = (int)_code[_charPointer], _backSlash = 92, _at = 64, _leftCollibrace = 123;
            if(_ascii != _quote) return;

            std::vector<json> _formattings;

            std::string _fullString;
            int _codeLength = strlen(_code);
            do {
                if((int)_code[_charPointer] == _backSlash) {
                    _charPointer++;
                    _fullString.push_back(_code[_charPointer++]);
                    continue;
                }

                if((int)_code[_charPointer] == _leftCollibrace) {
                    _fullString.push_back(_code[_charPointer]);
                    auto _startingFormatPos = _charPointer;
                    _charPointer++;
                    parserData.lookAhead = getNextToken();
                    auto _format = getExpression();
                    _fullString.append(std::to_string(_formattings.size()));
                    _fullString.push_back('}');
                    _formattings.emplace_back(json {
                        {"type", _FORMAT_EXPRESSION},
                        {"index", _formattings.size()},
                        {"body", _format}
                    });
                    
                    continue;
                }

                _fullString.push_back(_code[_charPointer++]);
            } while ((int)_code[_charPointer] != 39 && _charPointer != _codeLength);

            // Let's check that the string is closed correctly
            if(_charPointer <= _codeLength && (int)_code[_charPointer] != 39)
                throw MalformedStringException(_fullString.c_str(), _linePointer);

            _charPointer++;

            _outToken = json {
                {"type", _STRING},
                {"formattings", _formattings},
                {"value", _fullString.substr(1).c_str()}  
            };
        }

        void boolToken(json& _outToken, const char* _code, int& _charPointer, int _linePointer) {
            if(!_outToken.empty()) return;

            char _first = _code[_charPointer];
            if(_first != 't' && _first != 'f') return;
            if(_first == 'f' && _charPointer + 4 >= strlen(_code)) return;
            if(_first == 't' && _charPointer + 5 >= strlen(_code)) return;

            std::string _bool = std::string(&_code[_charPointer], &_code[_charPointer] + 4 + (_first == 't' ? 0 : 1));
            if(std::strcmp(_bool.c_str(), "true") != 0 && strcmp(_bool.c_str(), "false") != 0) return;

            std::string _fullBool;
            int _size = _code[_charPointer] == 't' ? 4 : 5;
            for(auto _i = 0; _i < _size; _i++) 
                _fullBool.push_back(_code[_charPointer++]);

            _outToken = json {
                {"type", _BOOL},
                {"value", _fullBool.c_str()}  
            };
        }

        void nullToken(json& _outToken, const char* _code, int& _charPointer, int _linePointer) {
            if(!_outToken.empty()) return;

            char _first = _code[_charPointer];
            if(_first != 'n') return;
            if(_first == 'n' && _charPointer + 4 >= strlen(_code)) return;

            std::string _bool = std::string(&_code[_charPointer], &_code[_charPointer] + 4);
            if(std::strcmp(_bool.c_str(), "null") != 0) return;

            std::string _fullNull;
            int _size = 4;
            for(auto _i = 0; _i < _size; _i++) 
                _fullNull.push_back(_code[_charPointer++]);

            _outToken = json {
                {"type", _NULL},
                {"value", _fullNull.c_str()}  
            };
        }

        void idToken(json& _outToken, const char* _code, int& _charPointer, int _linePointer) {
            int _ascii = (int)_code[_charPointer], _underscore = 95;
            
            if(_ascii != _underscore && !std::isalpha(_code[_charPointer])) return;

            char _currentChar = _code[_charPointer];
            std::string _fullId;
            int _codeLength = strlen(_code);
            while((_currentChar == '_' || std::isalpha(_currentChar) || std::isdigit(_currentChar)) && _charPointer < _codeLength) {
                _fullId.push_back(_currentChar);
                _charPointer++;
                _currentChar = _code[_charPointer];
            }

            auto _keyword = checkIfIsKeyword(_fullId);

            _outToken = json {
                {"type", _keyword.empty() ? _ID : _keyword},
                {"value", _fullId.c_str()}  
            };
        }

        std::string checkIfIsKeyword(const std::string& _fullId) {
            for(const char* _keyword : keywords) {
                if(_keyword != nullptr && strcmp(_fullId.c_str(), _keyword) == 0)
                    return _keyword;
            }
            return "";
        }

        void symbolToken(json& _outToken, const char* _code, int& _charPointer, int _linePointer) {
            if(!_outToken.empty()) return;
            //      /r    to  /t            space to /                :  to  @               [   to   _               {   to    ~
            int _min0 = 9, _max0 = 13, _min1 = 32, _max1 = 47, _min2 = 58, _max2 = 64, _min3 = 91, _max3 = 95, _min4 = 123, _max4 = 126, _ascii = (int)_code[_charPointer];
            if((_ascii < _min0 || _ascii > _max0) && (_ascii < _min1 || _ascii > _max1) && (_ascii < _min2 || _ascii > _max2) && 
            (_ascii < _min3 || _ascii > _max3) && (_ascii < _min4 || _ascii > _max4)) return;

            switch (_ascii) {
                case 9:  _outToken  = json { {"type", _IGNORE}, {"value", "/r"}};           break;
                case 10: _outToken  = json { {"type", _IGNORE}, {"value", "/n"}};           break;
                case 13: _outToken  = json { {"type", _IGNORE}, {"value", "/t"}};           break;
                case 32: _outToken  = json { {"type", _IGNORE}, {"value", " "}};            break;
                // ! and variants
                case 33: {
                        char _nextChar = _code[_charPointer + 1];
                        if(_nextChar == '='){
                            _outToken  = json { {"type", _COM_N_EQ}, {"value", "!="}};
                            _charPointer++;
                        }        
                        else
                            _outToken  = json { {"type", _COM_N}, {"value", "!"}};        
                        break;
                    }
                
                // Comments
                case 35: {
                    auto _length = strlen(_code);
                    while(_code[_charPointer] != '\n' && _code[_charPointer] != '\r' && _charPointer < _length)
                        _charPointer++;

                    _charPointer--;
                    _outToken = json { {"type", _IGNORE}, {"value", "#"}};     break;
                }
                case 37: _outToken  = json { {"type", _MOD}, {"value", "%"}};               break;

                // & and variants
                case 38: {
                        char _nextChar = _code[_charPointer + 1];
                        if(_nextChar == '='){
                            _outToken  = json { {"type", _AEQ}, {"value", "&="}};
                            _charPointer++;
                        } else if(_nextChar == '&'){
                            _outToken  = json { {"type", _COM_AND}, {"value", "&&"}};
                            _charPointer++;
                        } else
                            _outToken  = json { {"type", _EQ}, {"value", "&"}};        
                        break;
                    }
                case 40: _outToken  = json { {"type", _LEFT_PARENTHESIS}, {"value", "("}};  break;
                case 41: _outToken  = json { {"type", _RIGHT_PARENTHESIS}, {"value", ")"}}; break;
                
                // * and variants
                case 42: {
                        char _nextChar = _code[_charPointer + 1];
                        if(_nextChar == '='){
                            _outToken  = json { {"type", _BEQ}, {"value", "*="}};  
                            _charPointer++;
                        } else if(_nextChar == '*'){
                            _outToken  = json { {"type", _MULTR}, {"value", "**"}};  
                            _charPointer++;
                        } else
                            _outToken  = json { {"type", _MUL}, {"value", "*"}};        
                        break;
                    }
                
                // + and variants
                case 43: {
                        char _nextChar = _code[_charPointer + 1];
                        if(_nextChar == '='){
                            _outToken  = json { {"type", _PEQ}, {"value", "+="}};  
                            _charPointer++;
                        } else if(_nextChar == '+') {
                            _outToken  = json { {"type", _INCR}, {"value", "++"}};  
                            _charPointer++;
                        } else
                            _outToken  = json { {"type", _SUM}, {"value", "+"}};        
                        break;
                    }
                case 44: _outToken  = json { {"type", _COMA}, {"value", ","}};              break;
                
                // - and variants
                case 45: {
                        char _nextChar = _code[_charPointer + 1];
                        if(_nextChar == '='){
                            _outToken  = json { {"type", _MEQ}, {"value", "-="}};  
                            _charPointer++;
                        } else if(_nextChar == '-') {
                            _outToken  = json { {"type", _DECR}, {"value", "--"}};  
                            _charPointer++;
                        } else
                            _outToken  = json { {"type", _SUB}, {"value", "-"}};        
                        break;
                    }
                
                // .. and ...
                case 46 : {
                        char _nextChar = _code[_charPointer + 1];
                        char _nextNextChar = _code[_charPointer + 2];
                        if(_nextChar == '.'){
                            _outToken  = json { {"type", _INCL_RANGE}, {"value", ".."}};  
                            _charPointer++;
                            if(_nextNextChar == '.') {
                                _outToken  = json { {"type", _EXCL_RANGE}, {"value", "..."}};  
                                _charPointer++;
                            }
                        } else {
                            _outToken  = json { {"type", _DOT}, {"value", "."}};  
                        }

                        break;
                    }
                // / and variants
                case 47: {
                        char _nextChar = _code[_charPointer + 1];
                        if(_nextChar == '='){
                            _outToken  = json { {"type", _DEQ}, {"value", "/="}};  
                            _charPointer++;
                        }
                        else if(_nextChar == '/'){
                            _outToken  = json { {"type", _DVDR}, {"value", "//"}}; 
                            _charPointer++;
                        } else
                            _outToken  = json { {"type", _DIV}, {"value", "/"}};        
                        break;
                    }
                case 58: _outToken  = json { {"type", _COLON}, {"value", ":"}};         break;
                case 59: _outToken  = json { {"type", _SEMICOLON}, {"value", ";"}};         break;
                
                // < and variants
                case 60: {
                        char _nextChar = _code[_charPointer + 1];
                        if(_nextChar == '='){
                            _outToken  = json { {"type", _COM_LT_EQ}, {"value", "<="}};        
                            _charPointer++;
                        }
                        else
                            _outToken  = json { {"type", _COM_LT}, {"value", "<"}};        
                        break;
                    }

                // = and variants
                case 61: {
                        char _nextChar = _code[_charPointer + 1];
                        if(_nextChar == '='){
                            _outToken  = json { {"type", _COM_EQ}, {"value", "=="}};
                            _charPointer++;
                        }        
                        else
                            _outToken  = json { {"type", _EQ}, {"value", "="}};        
                        break;
                    }
                
                // > and variants
                case 62: {
                        char _nextChar = _code[_charPointer + 1];
                        if(_nextChar == '='){
                            _outToken  = json { {"type", _COM_GT_EQ}, {"value", ">="}};    
                            _charPointer++;
                        }    
                        else
                            _outToken  = json { {"type", _COM_GT}, {"value", ">"}};        
                        break;
                    }
                case 64: _outToken = json { {"type", _AT}, {"value", "@"}};   break;
                case 91: _outToken = json { {"type", _LEFT_BRACKET}, {"value", "["}};   break;
                case 93: _outToken = json { {"type", _RIGHT_BRACKET}, {"value", "]"}};   break;
                case 123: _outToken = json { {"type", _LEFT_COLLIBRACE}, {"value", "{"}};   break;

                // | and variants
                case 124: {
                        char _nextChar = _code[_charPointer + 1];
                        if(_nextChar == '='){
                            _outToken  = json { {"type", _OEQ}, {"value", "|="}};
                            _charPointer++;
                        } else if(_nextChar == '|'){
                            _outToken  = json { {"type", _COM_OR}, {"value", "||"}};
                            _charPointer++;
                        } else
                            _outToken  = json { {"type", _EQ}, {"value", "|"}};        
                        break;
                    }
                case 125: _outToken = json { {"type", _RIGHT_COLLIBRACE}, {"value", "}"}};  break;
                default: {
                        _outToken = {};
                        std::cerr << "Symbol " << _code[_charPointer] << " with ascii " << (int)_code[_charPointer] << " wasn't handled in symbolToken() inside the switch statement, or maybe it wasn't added to the list inside Declarations.h" << std::endl;
                        break;
                    }
            }

            _charPointer++;
        }

    }
    #pragma endregion TOKEN_STRIPPER

    #pragma region VARIABLE_ESTATEMENT

        json getVariableStatement() {
            eatToken(_VAR);
            
            auto _declarations = getDeclarationList();
            
            eatToken(_SEMICOLON);

            return json { 
                {"type", _VARIABLE_STATEMENT},
                {"declarations", _declarations}
            };
        }

        std::vector<json> getDeclarationList(bool _isSpecialDecl) {
            std::vector<json> _declarationList;
            
            _declarationList.push_back(getVariableDeclaration(_isSpecialDecl));

            while(strcmp(parserData.lookAhead["type"].get<std::string>().c_str(), _COMA) == 0) {
                eatToken(_COMA);
                _declarationList.push_back(getVariableDeclaration(_isSpecialDecl));
            }
            
            return _declarationList;
        }

        json getVariableDeclaration(bool _isSpecialDecl) {
            auto _lookAheadType = parserData.lookAhead["type"].get<std::string>();
            if(!_isSpecialDecl) {
                auto _id = eatToken(_ID);
                _lookAheadType = parserData.lookAhead["type"].get<std::string>();
                json _initializer = {};
                _initializer = strcmp(_lookAheadType.c_str(), _SEMICOLON) != 0 &&
                    strcmp(_lookAheadType.c_str(), _COMA) != 0 ? getInitializer() : json {  };
            
                return json {
                    {"type", _VARIABLE_DECLARATION},
                    {"id", _id},
                    {"init", _initializer}
                };
            }else {
                if(strcmp(_lookAheadType.c_str(), _ID) != 0) 
                    throw IllegalFunctionArgument(parserData.lookAhead["value"].get<std::string>().c_str(), parserData.linePointer);
                return getAssignmentExpression();
            }
        }

        json getInitializer() {
            eatToken(_EQ);
            return getAssignmentExpression();
        }

    #pragma endregion VARIABLE_ESTATEMENT
    
    #pragma region FUNCTION_ESTATEMENT

        json getFunctionStatement () {
            eatToken(_FUNC);
            auto _funcName = eatToken(_ID);
            eatToken(_LEFT_PARENTHESIS);
            auto _params = getParamListDecl();
            eatToken(_RIGHT_PARENTHESIS);
            auto _block = getBlockStatement();

            return json {
                {"type", _FUNC_DECL},
                {"name", _funcName},
                {"params", _params},
                {"body", _block}
            };
        }

        std::vector<json> getParamListExec() {
            auto _lookAheadType = parserData.lookAhead["type"].get<std::string>();
            if(strcmp(_lookAheadType.c_str(), _RIGHT_PARENTHESIS) == 0) return std::vector<json>();
            std::vector<json> _args { getExpression() };

            _lookAheadType = parserData.lookAhead["type"].get<std::string>();
            while(strcmp(_lookAheadType.c_str(), _COMA) == 0) {
                eatToken(_COMA);
                _args.push_back(getExpression());
                _lookAheadType = parserData.lookAhead["type"].get<std::string>();
            }

            return _args;
        }

        std::vector<json> getParamListDecl() {
            auto _lookAheadType = parserData.lookAhead["type"].get<std::string>();
            if(strcmp(_lookAheadType.c_str(), _RIGHT_PARENTHESIS) == 0) return std::vector<json>();
            return getDeclarationList(true);
        }

    #pragma endregion FUNCTION_ESTATEMENT

    #pragma region RETURN_ESTATEMENT

        json getReturnStatement () {
            eatToken(_RET);
            auto _lookAheadType = parserData.lookAhead["type"].get<std::string>();
            auto _ret = strcmp(_lookAheadType.c_str(), _SEMICOLON) == 0 ? json {  } : getExpression();
            eatToken(_SEMICOLON);
            
            return json {
                {"type", _RET_STATEMENT},
                {"arguments", _ret}
            };
        }

    #pragma endregion RETURN_ESTATEMENT
    
    #pragma region BLOCK_ESTATEMENT
        
        json getBlockStatement() {
            std::vector<json> _optionalBlockStatement;
            eatToken(_LEFT_COLLIBRACE);
            auto _lookAheadType = parserData.lookAhead["type"].get<std::string>();
            while(strcmp(_lookAheadType.c_str(), _RIGHT_COLLIBRACE) != 0 && !isEof()){
                _optionalBlockStatement.push_back(getAst());
                _lookAheadType = parserData.lookAhead["type"].get<std::string>();
            }

            eatToken(_RIGHT_COLLIBRACE);

            return json { 
                {"type", _BLOCK_STATEMENT },
                {"body", _optionalBlockStatement}
            };
        }

    #pragma endregion BLOCK_ESTATEMENT

    #pragma region STRUCT_ESTATEMENT

        json getStructStatement() {
            eatToken(_STRUCT);
            auto _structName = eatToken(_ID);
            json _inheritance;

            auto _lookAheadType = parserData.lookAhead["type"].get<std::string>();
            if(strcmp(_lookAheadType.c_str(), _EXTENDS) == 0) {
                eatToken(_EXTENDS);
                _inheritance = eatToken(_ID);
            }

            auto _structBody = getBlockStatement();

            return json {
                {"type", _STRUCT_STATEMENT},
                {"name", _structName},
                {"extends", _inheritance},
                {"structBody", _structBody}
            };
        }

        json getSelfExpression() {
            eatToken(_SELF);
            return json {
                {"type", _SELF_EXPRESSION}
            };
        }

        json getBaseExpression() {
            eatToken(_BASE);
            return json {
                {"type", _BASE_EXPRESSION}
            };
        }

        json getNewExpression() {
        eatToken(_NEW);
        auto _memberCallee = getMemberExpression();
        eatToken(_LEFT_PARENTHESIS);
        auto _args = getParamListExec();
        eatToken(_RIGHT_PARENTHESIS);
        return json {
            {"type", _NEW_EXPRESSION},
            {"callee", _memberCallee},
            {"args", _args}
        }; 
    }

    #pragma endregion STRUCT_ESTATEMENT

    #pragma region IF_STATEMENT

        json getIfStatement() {
            eatToken(_IF);
            eatToken(_LEFT_PARENTHESIS);
            auto _condition = getExpression();
            eatToken(_RIGHT_PARENTHESIS);
            auto _consequence = getAst();

            // std::vector<json> _alternatives;
            // if(strcmp(_parser->getCurrentLookAheadType().c_str(), _ELSE_IF) == 0) {
            //     _parser->eatToken(_ELSE_IF);
            //     _parser->eatToken(_LEFT_PARENTHESIS);
            //     auto _alternativeCondition = _expression.getAst(_parser, _parser->getLookAhead());
            //     _parser->eatToken(_RIGHT_PARENTHESIS);
            //     auto _alternativeConsequence = _statement.getAst(_parser, _parser->getLookAhead());
            //     json _elseif =  {
            //         {"type", _ELSE_IF_STATEMENT},
            //         {"condition", _alternativeCondition},
            //         {"consequence", _alternativeConsequence},
            //     };
            //     _alternatives.push_back(_elseif);
            // }

            json _alternative = {};
            if(!parserData.lookAhead.empty() && strcmp(parserData.lookAhead["type"].get<std::string>().c_str(), _ELSE) == 0) {
                eatToken(_ELSE);
                _alternative = getAst();
            }

            return {
                {"type", _IF_STATEMENT},
                {"condition", _condition},
                {"consequence", _consequence},
                {"alternative", _alternative}
            };
        }

    #pragma endregion IF_STATEMENT

    #pragma region LOOP_STATEMENT

        json getLoopStatement() {
            eatToken(_LOOP);
            eatToken(_LEFT_PARENTHESIS);
            auto _condition = getExpression();
            eatToken(_RIGHT_PARENTHESIS);
            auto _loopBody = getAst();

            return {
                {"type", _LOOP_STATEMENT},
                {"condition", _condition},
                {"loopBody", _loopBody}
            };
        }

    #pragma endregion LOOP_STATEMENT

    #pragma region FOR_LOOP_STATEMENT

        json getForLoopStatement() {
            eatToken(_FOR);
            eatToken(_LEFT_PARENTHESIS);

            std::vector<json> _indexVar = getDeclarationList(true);
            eatToken(_IN);
            auto _rangeVar = getExpression();
            eatToken(_RIGHT_PARENTHESIS);
            auto _loopBody = getAst();

            return {
                {"type", _FOR_STATEMENT},
                {"index", _indexVar},
                {"range", _rangeVar},
                {"loopBody", _loopBody}
            };
        }

    #pragma endregion FOR_LOOP_STATEMENT
    
    #pragma region IMPORT_STATEMENT

        json getImportStatement() {
            eatToken(_IMPORT);
            auto _path = eatToken(_STRING);
            eatToken(_SEMICOLON);

            return json {
                {"type", _IMPORT_STATEMENT},
                {"path", _path}
            };
        }

    #pragma endregion IMPORT_STATEMENT

    #pragma region EXPRESSION_STATEMENT

        json getExpressionStatement() {
            auto _ast = getExpression();
            eatToken(_SEMICOLON);
            return json {
                {"type", _EXPRESSION_STATEMENT},
                {"expression", _ast}
            };
        }

        json getExpression() {
            return getAssignmentExpression();
        }

        json getAssignmentExpression() {
            auto _left = getLogicalOrExpression();
            auto _assigment = isAssignement(parserData.lookAhead["type"].get<std::string>().c_str());
            if(_assigment.empty()){
                return _left;
            }
            
            if(isCorrectLeftSideAssigment(_left))
                return json {
                    {"type", _ASSIGMENT_EXPRESSION},
                    {"operator", _assigment},
                    {"left", _left},
                    {"right", getAssignmentExpression()}
                };

            return json {  };
        }

        json isAssignement(const char* _tokenType) {
            for(int _i = 13; _i < 21; _i++)
                if(strcmp(symbols[_i], _tokenType) == 0){
                    auto _token = eatToken(symbols[_i]);
                    return _token;
                }

            return json {};
        }

        bool isCorrectLeftSideAssigment(json& _left) {
            std::string _tokenType = _left["type"].get<std::string>();
            if(strcmp(_tokenType.c_str(), _ID) == 0 || strcmp(_tokenType.c_str(), _MEMBER_EXPRESSION) == 0) return true;
            throw IllegalAssigmentExpression(_tokenType.c_str(), parserData.linePointer);
        }

        json getLogicalOrExpression() {
            auto _left = getLogicalAndExpression();
            
            auto _lookAheadType = parserData.lookAhead["type"].get<std::string>();
            while ( strcmp(_lookAheadType.c_str(), _COM_OR) == 0) {
                auto _op = eatToken(_lookAheadType.c_str());
                auto _right = getLogicalAndExpression();
                _left = {
                    {"type", _LOGICAL_EXPRESSION},
                    {"operator", _op},
                    {"left", _left},
                    {"right", _right}
                };

                _lookAheadType = parserData.lookAhead["type"].get<std::string>();
            }

            return _left;
        }

        json getLogicalAndExpression() {
            auto _left = getEqualityExpression();
            
            auto _lookAheadType = parserData.lookAhead["type"].get<std::string>();
            while ( strcmp(_lookAheadType.c_str(), _COM_AND) == 0) {
                auto _op = eatToken(_lookAheadType.c_str());
                auto _right = getEqualityExpression();
                _left = {
                    {"type", _LOGICAL_EXPRESSION},
                    {"operator", _op},
                    {"left", _left},
                    {"right", _right}
                };

                _lookAheadType = parserData.lookAhead["type"].get<std::string>();
            }

            return _left;
        }

        json getEqualityExpression() {
            auto _left = getRelationalExpression();

            auto _lookAheadType = parserData.lookAhead["type"].get<std::string>();
            while ( strcmp(_lookAheadType.c_str(), _COM_EQ) == 0 || strcmp(_lookAheadType.c_str(), _COM_N_EQ) == 0) {
                auto _op = eatToken(_lookAheadType.c_str());
                auto _right = getRelationalExpression();
                _left = {
                    {"type", _BINARY_OP},
                    {"operator", _op},
                    {"left", _left},
                    {"right", _right}
                };

                _lookAheadType = parserData.lookAhead["type"].get<std::string>();
            }

            return _left;
        }

        json getRelationalExpression() {
            auto _left = getAdditiveExpression();

            auto _lookAheadType = parserData.lookAhead["type"].get<std::string>();
            while ( strcmp(_lookAheadType.c_str(), _COM_GT) == 0 || strcmp(_lookAheadType.c_str(), _COM_GT_EQ) == 0 ||
                    strcmp(_lookAheadType.c_str(), _COM_LT) == 0 || strcmp(_lookAheadType.c_str(), _COM_LT_EQ) == 0) {
                auto _op = eatToken(_lookAheadType.c_str());
                auto _right = getAdditiveExpression();
                _left = {
                    {"type", _BINARY_OP},
                    {"operator", _op},
                    {"left", _left},
                    {"right", _right}
                };

                _lookAheadType = parserData.lookAhead["type"].get<std::string>();
            }

            return _left;
        }

        json getAdditiveExpression() {
            auto _left = getMultiplicativeExpression();
            
            auto _lookAheadType = parserData.lookAhead["type"].get<std::string>();
            while ( strcmp(_lookAheadType.c_str(), _SUM) == 0 ||  strcmp(_lookAheadType.c_str(), _SUB) == 0) {
                auto _op = eatToken(_lookAheadType.c_str());
                auto _right = getMultiplicativeExpression();
                _left = {
                    {"type", _BINARY_OP},
                    {"operator", _op},
                    {"left", _left},
                    {"right", _right}
                };

                _lookAheadType = parserData.lookAhead["type"].get<std::string>();
            }

            return _left;
        }

        json getMultiplicativeExpression() {
            auto _left = getRangeExpression();
            
            auto _lookAheadType = parserData.lookAhead["type"].get<std::string>();
            while ( strcmp(_lookAheadType.c_str(), _MUL) == 0 ||  strcmp(_lookAheadType.c_str(), _DIV) == 0) {
                auto _op = eatToken(_lookAheadType.c_str());
                auto _right = getRangeExpression();
                _left = {
                    {"type", _BINARY_OP},
                    {"operator", _op},
                    {"left", _left},
                    {"right", _right}
                };

                _lookAheadType = parserData.lookAhead["type"].get<std::string>();
            }

            return _left;
        }

        json getRangeExpression() {
            auto _left = getLambdaExpression();
            auto _lookAheadType = parserData.lookAhead["type"].get<std::string>();
            while ( strcmp(_lookAheadType.c_str(), _INCL_RANGE) == 0 || strcmp(_lookAheadType.c_str(), _EXCL_RANGE) == 0) {
                auto _op = eatToken(_lookAheadType.c_str());
                auto _right = getLambdaExpression();
                _left = {
                    {"type", _RANGE},
                    {"operator", _op},
                    {"left", _left},
                    {"right", _right}
                };

                _lookAheadType = parserData.lookAhead["type"].get<std::string>();
            }

            return _left;
        }

        json getLambdaExpression() {
            auto _lookAheadType = parserData.lookAhead["type"].get<std::string>();
            if(strcmp(_lookAheadType.c_str(), _LAMB) == 0) {
                eatToken(_LAMB);
                eatToken(_COLON);

                eatToken(_LEFT_PARENTHESIS);
                auto _args = getParamListDecl();
                eatToken(_RIGHT_PARENTHESIS);
                eatToken(_SUB);
                eatToken(_COM_GT);
                auto _body = getBlockStatement();

                return json {
                    {"type", _LAMB_EXPRESSION},
                    {"args", _args},
                    {"body", _body}
                };
            }
            
            return getUnaryExpression();
        }

        json getUnaryExpression() {
            json _unaryOperator; // ! or - or +
            auto _lookAheadType = parserData.lookAhead["type"].get<std::string>();
            if(strcmp(_lookAheadType.c_str(), _SUM) == 0 || strcmp(_lookAheadType.c_str(), _SUB) == 0 || strcmp(_lookAheadType.c_str(), _COM_N) == 0) {
                _unaryOperator = eatToken(_lookAheadType.c_str());
            }

            if(!_unaryOperator.empty()) {
                return json {
                    {"type", _UNARY_OP},
                    {"operator", _unaryOperator},
                    {"argument", getUnaryExpression()}
                };
            }

            return getCallExpression();
        }

        json getCallExpression() {
            auto _lookAheadType = parserData.lookAhead["type"].get<std::string>();
            if(strcmp(_lookAheadType.c_str(), _BASE) == 0) {
                auto _super = getBaseExpression();
                return callee(_super);
            }

            auto _member = getMemberExpression();
            _lookAheadType = parserData.lookAhead["type"].get<std::string>();

            if(strcmp(_lookAheadType.c_str(), _LEFT_PARENTHESIS) == 0) {
                return callee(_member);
            }

            return _member;
        }

        json callee(json& _member) {

            auto _lookAheadType = parserData.lookAhead["type"].get<std::string>();
            auto _callExpression = json {
                {"type", _CALL_EXPRESSION},
                {"callee", _member},
                {"arguments", getArguments()}
            };

            _lookAheadType = parserData.lookAhead["type"].get<std::string>();

            if(strcmp(_lookAheadType.c_str(), _LEFT_PARENTHESIS) == 0){
                _callExpression = callee(_callExpression);
                _lookAheadType = parserData.lookAhead["type"].get<std::string>();
            }

            return _callExpression;
        }

        std::vector<json> getArguments() {
            eatToken(_LEFT_PARENTHESIS);
            auto _argsList = getArgumentList();
            eatToken(_RIGHT_PARENTHESIS);
            return _argsList;
        }

        std::vector<json> getArgumentList() {
            auto _lookAheadType = parserData.lookAhead["type"].get<std::string>();
            if(strcmp(_lookAheadType.c_str(), _RIGHT_PARENTHESIS) == 0) return std::vector<json>();
            return getParamListExec();
        }

        json getMemberExpression() {
            auto _lookAheadType = parserData.lookAhead["type"].get<std::string>();
            auto _object = getPrimaryExpression();

            _lookAheadType = parserData.lookAhead["type"].get<std::string>();
            while(strcmp(_lookAheadType.c_str(), _DOT) == 0 || strcmp(_lookAheadType.c_str(), _LEFT_BRACKET) == 0) {
                
                if(strcmp(_lookAheadType.c_str(), _DOT) == 0) {
                    eatToken(_DOT);
                    auto _prop = eatToken(_ID);
                    _object = {
                        {"type", _MEMBER_EXPRESSION},
                        {"computed", false},
                        {"object", _object},
                        {"property", _prop}
                    };
                } else {
                    eatToken(_LEFT_BRACKET);
                    auto _prop = getExpression();
                    eatToken(_RIGHT_BRACKET);
                    _object = {
                        {"type", _MEMBER_EXPRESSION},
                        {"computed", true},
                        {"object", _object},
                        {"property", _prop}
                    };
                }

                _lookAheadType = parserData.lookAhead["type"].get<std::string>();
            }

            return _object;
        }

        json getPrimaryExpression() {
            auto _tokenType = parserData.lookAhead["type"].get<std::string>();
            if(strcmp(_tokenType.c_str(), _LEFT_PARENTHESIS) == 0)
                return getParenthesisExpression();
            else if(strcmp(_tokenType.c_str(), _SELF) == 0)
                return getSelfExpression();
            else if(strcmp(_tokenType.c_str(), _NEW) == 0)
                return getNewExpression();
            else
                return getLiteral();
        }

        json getParenthesisExpression() {
            eatToken(_LEFT_PARENTHESIS);
            auto _token = getExpression();
            eatToken(_RIGHT_PARENTHESIS);
            
            return _token;
        }

    #pragma endregion EXPRESSION_STATEMENT

    #pragma region LITERAL

        json getLiteral() {
            auto _tokenType = parserData.lookAhead["type"].get<std::string>();
            json _outToken {};
            if(strcmp(_tokenType.c_str(), _INTEGER) == 0 || strcmp(_tokenType.c_str(), _FLOAT) == 0) {
                _outToken = numberLiteral();
            } else if(strcmp(_tokenType.c_str(), _STRING) == 0) {
                _outToken = stringLiteral();
            } else if(strcmp(_tokenType.c_str(), _BOOL) == 0) {
                _outToken = boolLiteral();
            }else if(strcmp(_tokenType.c_str(), _NULL) == 0) {
                _outToken = nullLiteral();
            } else if(strcmp(_tokenType.c_str(), _ID) == 0) {
                _outToken = idLiteral();
            } else {
                for(const char* _symbol : symbols){
                    if(strcmp(_symbol, _tokenType.c_str()) == 0)
                        _outToken = symbolLiteral();
                }
            }

            return _outToken;
        }

        json numberLiteral() {
            auto _tokenValue = parserData.lookAhead["value"].get<std::string>();
            auto _type = _tokenValue.find('.') != std::string::npos ? _FLOAT : _INTEGER;
            auto _token = eatToken(_type);
            return _token;
        }

        json stringLiteral() {
            return eatToken(_STRING);
        }

        json boolLiteral() {
            return eatToken(_BOOL);
        }

        json nullLiteral() {
            return eatToken(_NULL);
        }

        json idLiteral() {
            return eatToken(_ID);
        }

        json symbolLiteral() {
            auto _tokenType = parserData.lookAhead["type"].get<std::string>();
            for(const char* _symbol : symbols)
                    if(strcmp(_symbol, _tokenType.c_str()) == 0)
                        return eatToken(_tokenType.c_str());
            throw UnexpectedTokenException(parserData.lookAhead["value"].get<std::string>().c_str()[0], parserData.linePointer);
        }

    #pragma endregion LITERAL

#endif // __PARSER__
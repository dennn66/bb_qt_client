#include "token.h"
using namespace L2;
Token::Token(L2GraphicObject *parent) : L2GraphicObject(parent)
{
    //star.load("patterns\\star.bmp");
    nToken = TOKEN_NONE;
    setTimer(20);
}

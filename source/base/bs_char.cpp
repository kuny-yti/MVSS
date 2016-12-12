
#include "bs_string.h"
#include "bs_ucs.h"

namespace bs_char_p {

uchar combining_class(const bs_char &data)
{
    return combiningClass(data.unicode ());
}
bs_char mirrored_char(const bs_char &data)
{
    return (ushort)(mirroredChar(data.unicode ())&0xFFFF);
}
bool has_mirrored(const bs_char &data)
{
    return hasMirrored(data.unicode ());
}
bs_string decomposition(const bs_char &data)
{
    return ::decomposition(data.unicode ());
}
eDecompositions decomposition_tag(const bs_char &data)
{
    return decompositionTag(data.unicode ());
}

eCategorys category(const bs_char &data)
{
    return ::category(data.unicode ());
}
eDirections direction(const bs_char &data)
{
    return ::direction(data.unicode ());
}
eJoiningTypes joining_type(const bs_char &data)
{
    return joiningType(data.unicode ());
}
eScripts script(const bs_char &data)
{
    return ::script(data.unicode ());
}
eUnicodeVersions unicode_version(const bs_char &data)
{
    return unicodeVersion(data.unicode ());
}
}

bs_char::bs_char():
    data(0)
{

}
bs_char::bs_char( const bs_char &c):
    data(c.data)
{
}
bs_char::bs_char( char c):
    data(from_latin1(c).data)
{
}
bs_char::bs_char( wchar_t c):
    data(int(c))
{
}
bs_char::bs_char( ushort c):
    data(c)
{
}

bs_char::~bs_char()
{

}

bs_char::operator char () const
{
    return to_latin1();
}
bs_char::operator wchar_t () const
{
    return wchar_t(data);
}
bs_char::operator ushort &()
{
    return data;
}
bs_char::operator ushort ()const
{
    return data;
}
ushort bs_char::unicode()const
{
    return data;
}
ushort &bs_char::unicode()
{
    return data;
}

bs_char& bs_char::operator=(ushort c)
{
    data = c;
    return *this;
}
bs_char& bs_char::operator=(char c)
{
    data = from_latin1(c).data ;
    return *this;
}
bs_char& bs_char::operator=(wchar_t c)
{
    data = ushort(c);
    return *this;
}
bs_char& bs_char::operator=(const bs_char &c)
{
    data = c.data;
    return *this;
}

bool bs_char::operator==(char c) const
{
    return bs_char(c).data == data;
}
bool bs_char::operator==(wchar_t c)const
{
    return data == bs_char(c).data;
}
bool bs_char::operator==(const bs_char &c)const
{
    return data == c.data;
}

bool bs_char::operator!=(const bs_char& c)const
{
    return !((*this) == c);
}
bool bs_char::operator!=(wchar_t c)const
{
    return !((*this) == c);
}
bool bs_char::operator!=(char c)const
{
    return !((*this) == c);
}

bool bs_char::is_letter_number() const
{
    return isLetterOrNumber(data);
}
bool bs_char::is_title_case() const
{
    return isTitleCase(data);
}
bool bs_char::is_non_char() const
{
    return isNonCharacter(data);
}
bool bs_char::is_high_surrogate() const
{
    return isHighSurrogate(data);
}
bool bs_char::is_low_surrogate() const
{
    return isLowSurrogate(data);
}
bool bs_char::is_surrogate() const
{
    return isSurrogate(data);
}
bool bs_char::is_letter() const
{
    return isLetter(data);
}
bool bs_char::is_unicode() const
{
    return !is_letter();
}
bool bs_char::is_upper() const
{
    return isUpper(data);
}
bool bs_char::is_symbol() const
{
    return isSymbol(data);
}
bool bs_char::is_mark()const
{
    return isMark(data);
}
bool bs_char::is_punct() const
{
    return isPunct(data);
}
bool bs_char::is_print() const
{
    return isPrint(data);
}
bool bs_char::is_space() const
{
    return isSpace(data);
}
bool bs_char::is_digit() const
{
    return isDigit(data);
}
bool bs_char::is_number() const
{
    return isNumber(data);
}
bool bs_char::is_lower() const
{
    return isLower(data);
}
bool bs_char::is_valid()const
{
    return data == 0;
}

void bs_char::lower()
{
    data = toLower(data);
}
void bs_char::upper()
{
    data = toUpper(data);
}
void bs_char::reverse()
{
    if (!is_letter())
        return ;

    if (is_lower())
        upper();
    else
        lower();
}

int bs_char::digit() const
{
    return digitValue(data);
}
char bs_char::to_latin1()const
{
    return data > 0xff ? '\0' : char(data);
}
bs_char bs_char::to_lower() const
{
    return bs_char((ushort)(toLower(data) &0XFFFF));
}
bs_char bs_char::to_upper() const
{
    return (ushort)(toUpper(data)&0xFFFF);
}
bs_char bs_char::to_title_case() const
{
    return (ushort)(toTitleCase(data)&0xFFFF);
}
bs_char bs_char::to_case_folded() const
{
    return (ushort)(toCaseFolded(data)&0xFFFF);
}
bs_char bs_char::from_latin1(char c)
{
    return bs_char(ushort(uchar(c)));
}


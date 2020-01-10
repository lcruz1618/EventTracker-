#include "i18n.h"
#include <iostream>

i18n* i18n::mpI18n = NULL;

i18n::i18n ()
{
}

i18n::i18n(const i18n& other)
{
    mTextDomain = other.mTextDomain;
    mPathDomain = other.mPathDomain;
    //std::cout << "i18n#" << __LINE__ << "#" << this << " " << mTextDomain << " " << mPathDomain << std::endl;
}

i18n&
i18n::operator= (const i18n& other)
{
    mTextDomain = other.mTextDomain;
    mPathDomain = other.mPathDomain;
    //std::cout << "i18n#" << __LINE__ << "#" << this << " " << mTextDomain << " " << mPathDomain << std::endl;

    return *this;
}

i18n*
i18n::Instance ()
{
    if (mpI18n == NULL)
    {
        mpI18n = new i18n ();
    }
    return mpI18n;
}

void
i18n::SetParam (const std::string domain, const std::string path)
{
    mTextDomain = domain;
    mPathDomain = path;
    //std::cout << "i18n#" << __LINE__ << "#" << this << " " << mTextDomain << " " << mPathDomain << std::endl;
}

Glib::ustring
i18n::translate (std::string text)
{
    if (text.size () != 0)
    {
        //std::cout << "i18n#" << __LINE__ << "#" << this << " " << text << std::endl;
        //std::cout << "i18n#" << __LINE__ << "#" << this << " " << mTextDomain << " " << mPathDomain << std::endl;
        // Tell gettext to use current working directory as locale dir for sample
        // if you don't tell "C:\\gnu\\share\\locale" is taken as a default
        bindtextdomain (mTextDomain.c_str (), mPathDomain.c_str ());
        // Tell gettext to use the sample textdomain
        textdomain (mTextDomain.c_str ());
        char* translated = gettext (text.c_str ());
        std::string translatedStr (translated);
        //Glib::ustring translatedGstr (Glib::locale_from_utf8 (translatedStr));
        Glib::ustring translatedGstr (Glib::locale_to_utf8 (translatedStr));
        //Glib::ustring translatedGstr (translatedStr);
        return translatedGstr;
    }
    else
    {
        return Glib::ustring ("");
    }
}

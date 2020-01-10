#ifndef I18N_H__
#define I18N_H__

#include <string>
#include <libintl.h>
#include <glibmm.h>

class i18n
{
public:
    i18n ();

    i18n (const i18n& other);

    i18n& operator= (const i18n& other);

    static i18n* Instance ();

    void SetParam (const std::string domain, const std::string path);

    Glib::ustring translate (std::string text);

private:
    static i18n* mpI18n;

    std::string mTextDomain;

    std::string mPathDomain;
};

#define _(String) i18n::Instance ()->translate (String)

#endif // I18N_H__

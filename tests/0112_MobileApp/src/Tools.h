#ifndef TOOLS_H
#define TOOLS_H

#include <string>
#include <libintl.h>
#include <locale.h>

#define tr(String) gettext (String)

std::string gTranslationDir(std::string aExePath);
std::string gGetProgramName(std::string aExePath);


#endif // TOOLS_H

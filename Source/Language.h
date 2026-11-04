/* EightyOne - A Windows emulator of the Sinclair ZX range of computers.
 *
 * Language.h: the language of the user interface (English or Hungarian)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

//---------------------------------------------------------------------------

#ifndef LanguageH
#define LanguageH

#include <vcl4.h>
#include <Forms.hpp>

//---------------------------------------------------------------------------

// The Language setting
#define LANGUAGE_AUTOMATIC 0
#define LANGUAGE_ENGLISH   1
#define LANGUAGE_HUNGARIAN 2

// A translation: the English text (as in the forms and the code) and the
// Hungarian one, both in the ANSI code pages of the texts (Windows-1252 for
// the English, Windows-1250 for the Hungarian)
struct LanguageEntry
{
        const char *English;
        const char *Hungarian;
};

extern const LanguageEntry LanguageHungarianTable[];

// The setting, and whether the texts are Hungarian with it
extern void LanguageSetSetting(int setting);
extern int LanguageGetSetting(void);
extern bool LanguageIsHungarian(void);

// The text in the current language; English texts not in the table are
// returned unchanged
extern AnsiString Tr(const char *english);
extern AnsiString Tr(const AnsiString &english);

// The same with {0}, {1}, {2} replaced by the arguments
extern AnsiString TrF(const char *english, const AnsiString &a0,
                      const AnsiString &a1 = "", const AnsiString &a2 = "");

// The English of a text shown (itself if it is not a Hungarian text of the
// table), and whether two texts shown are the same in English: for texts
// the code keeps (combo box selections), which may be in either language
extern AnsiString LanguageEnglish(const AnsiString &text);
extern bool LanguageSame(const AnsiString &a, const AnsiString &b);

// A file dialog filter ("Description|*.ext|..."), its descriptions in the
// current language
extern AnsiString LanguageFilter(const AnsiString &filter);

// Translate the texts of a form (captions, hints, menus, lists, file
// dialogs...), or of every form; again after the language changes
extern void LanguageTranslateForm(TForm *form);
extern void LanguageTranslateAll(void);

// Components whose texts are names (of files and folders: the
// documentation and examples menus, the configurations), which the code
// uses: never translated ("Sound" is a folder there, not "Hang").
// LanguageKeepText(component, false) forgets one before it is deleted
extern void LanguageKeepText(TObject *component, bool keep = true);

#endif

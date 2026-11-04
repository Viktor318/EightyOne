/* EightyOne - A Windows emulator of the Sinclair ZX range of computers.
 *
 * Language.cpp: the language of the user interface (English or Hungarian)
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
// The forms are designed in English. When the language is Hungarian, the
// texts of every form are replaced from the table in LanguageHu.cpp (made
// from Translations/hu.tsv), and the texts made by the code go through
// Tr() / TrF(). The forms themselves are not changed, so the English
// version stays as it is.
//
// The language can be changed while running: every text replaced remembers
// its English original. If the code has changed a text since (a caption
// set while running), the new text is taken as the original, turned back
// into English if it is a Hungarian one of the table.
//---------------------------------------------------------------------------

#include <vcl4.h>
#pragma hdrstop

#include <map>
#include <string>
#include <string.h>
#include <TypInfo.hpp>
#include <StdCtrls.hpp>
#include <ExtCtrls.hpp>
#include <ComCtrls.hpp>
#include <Menus.hpp>
#include <Dialogs.hpp>

#include "Language.h"

#pragma package(smart_init)

//---------------------------------------------------------------------------

static int Setting = LANGUAGE_AUTOMATIC;
static bool Hungarian = false;

// English -> Hungarian, and Hungarian -> English; byte-wise keys (the
// AnsiString comparisons of the VCL are locale-aware)
typedef std::map<std::string, std::string> TextMap;
static TextMap ToHungarian;
static TextMap ToEnglish;
static bool MapsBuilt = false;

static void BuildMaps()
{
        int i;

        if (MapsBuilt) return;
        MapsBuilt = true;

        for (i=0; LanguageHungarianTable[i].English; i++)
        {
                std::string en = LanguageHungarianTable[i].English;
                std::string hu = LanguageHungarianTable[i].Hungarian;

                ToHungarian[en] = hu;
                // With more English texts for one Hungarian, the first wins
                if (ToEnglish.find(hu) == ToEnglish.end()) ToEnglish[hu] = en;
        }
}

// The language of the Windows user interface
static bool WindowsIsHungarian()
{
        typedef LANGID (WINAPI *UILanguageFunction)(void);
        UILanguageFunction function;
        LANGID language;
        HMODULE kernel = GetModuleHandle("kernel32.dll");

        // GetUserDefaultUILanguage() is there from Windows 2000 on
        function = kernel ? (UILanguageFunction)
                GetProcAddress(kernel, "GetUserDefaultUILanguage") : NULL;
        language = function ? function() : GetUserDefaultLangID();

        return PRIMARYLANGID(language) == LANG_HUNGARIAN;
}

void LanguageSetSetting(int setting)
{
        if (setting < LANGUAGE_AUTOMATIC || setting > LANGUAGE_HUNGARIAN)
                setting = LANGUAGE_AUTOMATIC;

        Setting = setting;

        if (setting == LANGUAGE_HUNGARIAN) Hungarian = true;
        else if (setting == LANGUAGE_ENGLISH) Hungarian = false;
        else Hungarian = WindowsIsHungarian();
}

int LanguageGetSetting(void)
{
        return Setting;
}

bool LanguageIsHungarian(void)
{
        return Hungarian;
}

//---------------------------------------------------------------------------
// Texts of the code

// A text without its hotkey marks: the VCL puts "&" into the captions of
// menus by itself ("File" becomes "&File"), so the table has to be found
// without them too ("&&" is a "&" itself)
static std::string StripHotkeys(const char *text)
{
        std::string result;

        for (; *text; text++)
        {
                if (*text == '&')
                {
                        if (text[1] != '&') continue;
                        text++;
                }
                result += *text;
        }

        return result;
}

AnsiString Tr(const char *english)
{
        TextMap::const_iterator i;

        if (!english) return "";
        if (!Hungarian) return english;

        BuildMaps();
        i = ToHungarian.find(english);
        if (i == ToHungarian.end() && strchr(english, '&'))
                i = ToHungarian.find(StripHotkeys(english));

        return i != ToHungarian.end() ? AnsiString(i->second.c_str())
                                      : AnsiString(english);
}

AnsiString Tr(const AnsiString &english)
{
        return Tr(english.c_str());
}

static AnsiString ReplaceAll(AnsiString text, const AnsiString &what,
                             const AnsiString &with)
{
        AnsiString result;
        int position;

        while ((position = text.Pos(what)) > 0)
        {
                result += text.SubString(1, position - 1) + with;
                text = text.SubString(position + what.Length(),
                                      text.Length());
        }

        return result + text;
}

AnsiString TrF(const char *english, const AnsiString &a0,
               const AnsiString &a1, const AnsiString &a2)
{
        AnsiString text = Tr(english);

        text = ReplaceAll(text, "{0}", a0);
        text = ReplaceAll(text, "{1}", a1);
        text = ReplaceAll(text, "{2}", a2);

        return text;
}

//---------------------------------------------------------------------------
// Texts of the forms

// The English text for a text on a form: itself, or the English of a
// Hungarian text of the table
static AnsiString EnglishOf(const AnsiString &text)
{
        TextMap::const_iterator i;
        std::string stripped;

        BuildMaps();
        i = ToEnglish.find(text.c_str());
        if (i != ToEnglish.end()) return i->second.c_str();

        if (!text.Pos("&")) return text;

        // With hotkey marks the VCL has put in
        stripped = StripHotkeys(text.c_str());
        i = ToEnglish.find(stripped);
        if (i != ToEnglish.end()) return i->second.c_str();
        if (ToHungarian.find(stripped) != ToHungarian.end())
                return stripped.c_str();

        return text;
}

AnsiString LanguageEnglish(const AnsiString &text)
{
        return EnglishOf(text);
}

bool LanguageSame(const AnsiString &a, const AnsiString &b)
{
        return a == b || EnglishOf(a) == EnglishOf(b);
}

// The text to show for an English one
static AnsiString Shown(const AnsiString &english)
{
        return Tr(english.c_str());
}

// The English original of a property, and the text last put there
struct Original
{
        AnsiString English;
        AnsiString LastSet;
};

typedef std::map<std::pair<TObject *, int>, Original> OriginalMap;
static OriginalMap Originals;

// The properties translated; the number is the key of the original
static const char *Properties[] = { "Caption", "Hint", "Title", "Filter" };
#define PROPERTY_FILTER 3

// A file type description: as a whole, or else its text without the
// patterns in brackets at its end ("Disk Images (*.dsk)")
static AnsiString TranslateDescription(const AnsiString &text, bool toEnglish)
{
        AnsiString result = toEnglish ? EnglishOf(text) : Shown(text);
        AnsiString name, patterns;
        int bracket;

        if (result != text || text.Length() < 3
                || text[text.Length()] != ')') return result;

        bracket = text.LastDelimiter("(");
        if (bracket < 3 || text[bracket - 1] != ' ') return result;

        name = text.SubString(1, bracket - 2);
        patterns = text.SubString(bracket - 1, text.Length());

        return (toEnglish ? EnglishOf(name) : Shown(name)) + patterns;
}

// A file dialog filter: its descriptions are translated, the patterns not
static AnsiString TranslateFilter(const AnsiString &filter, bool toEnglish)
{
        AnsiString result, rest = filter, part;
        int position, index = 0;

        while (true)
        {
                position = rest.Pos("|");
                part = position ? rest.SubString(1, position - 1) : rest;

                if (index % 2 == 0)
                        part = TranslateDescription(part, toEnglish);

                result += part;
                if (!position) break;

                result += "|";
                rest = rest.SubString(position + 1, rest.Length());
                index++;
        }

        return result;
}

static void TranslateProperty(TObject *object, int property)
{
        PPropInfo info;
        AnsiString current, english, text;
        OriginalMap::iterator i;
        std::pair<TObject *, int> key(object, property);

        info = GetPropInfo((PTypeInfo)object->ClassInfo(),
                           Properties[property]);
        if (!info || !info->PropType || !*info->PropType) return;
        if ((*info->PropType)->Kind != tkLString
                && (*info->PropType)->Kind != tkString) return;

        current = GetStrProp(object, info);

        i = Originals.find(key);
        if (i == Originals.end()
                || StripHotkeys(i->second.LastSet.c_str())
                   != StripHotkeys(current.c_str()))
        {
                // First time here, or the code has set a new text since
                english = property == PROPERTY_FILTER ?
                          TranslateFilter(current, true) : EnglishOf(current);
        }
        else
        {
                english = i->second.English;
        }

        text = property == PROPERTY_FILTER ?
               TranslateFilter(english, false) : Shown(english);

        if (text != current) SetStrProp(object, info, text);

        Originals[key].English = english;
        Originals[key].LastSet = text;
}

// List items (combo boxes, list boxes, radio groups): no originals are
// kept, as the code often fills them; the items of the table are turned
// back into English first
static void TranslateList(TStrings *items)
{
        int i;
        AnsiString text;

        if (!items) return;

        items->BeginUpdate();
        for (i=0; i<items->Count; i++)
        {
                text = Shown(EnglishOf(items->Strings[i]));
                if (text != items->Strings[i]) items->Strings[i] = text;
        }
        items->EndUpdate();
}

static void TranslateComponent(TComponent *component)
{
        int i, property;

        for (property=0; property<4; property++)
                TranslateProperty(component, property);

        if (TComboBox *combo = dynamic_cast<TComboBox *>(component))
        {
                int index = combo->ItemIndex;
                TranslateList(combo->Items);
                combo->ItemIndex = index;
        }
        else if (TListBox *list = dynamic_cast<TListBox *>(component))
        {
                int index = list->ItemIndex;
                TranslateList(list->Items);
                list->ItemIndex = index;
        }
        else if (TRadioGroup *group = dynamic_cast<TRadioGroup *>(component))
        {
                int index = group->ItemIndex;
                TranslateList(group->Items);
                group->ItemIndex = index;
        }
        else if (TStatusBar *status = dynamic_cast<TStatusBar *>(component))
        {
                for (i=0; i<status->Panels->Count; i++)
                {
                        TStatusPanel *panel = status->Panels->Items[i];
                        AnsiString text = Shown(EnglishOf(panel->Text));
                        if (text != panel->Text) panel->Text = text;
                }
        }
        else if (TListView *view = dynamic_cast<TListView *>(component))
        {
                for (i=0; i<view->Columns->Count; i++)
                        TranslateProperty(view->Columns->Items[i], 0);
        }
}

void LanguageTranslateForm(TForm *form)
{
        int i;
        TMainMenu *menu;

        if (!form) return;

        TranslateProperty(form, 0);
        for (i=0; i<form->ComponentCount; i++)
                TranslateComponent(form->Components[i]);

        // The menu bar of the form does not show the new captions of its
        // top items until the menu is built again
        menu = form->Menu;
        if (menu && form->HandleAllocated())
        {
                form->Menu = NULL;
                form->Menu = menu;
                DrawMenuBar(form->Handle);
        }
}

void LanguageTranslateAll(void)
{
        int i;

        for (i=0; i<Screen->FormCount; i++)
                LanguageTranslateForm(Screen->Forms[i]);
}

AnsiString LanguageFilter(const AnsiString &filter)
{
        return TranslateFilter(filter, false);
}

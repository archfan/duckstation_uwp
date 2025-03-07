// SPDX-FileCopyrightText: 2019-2022 Connor McLaughlin <stenzek@gmail.com>
// SPDX-License-Identifier: (GPL-3.0 OR CC-BY-NC-ND-4.0)

#include "qtutils.h"
#include "common/string_util.h"
#include "fmt/format.h"
#include "util/input_manager.h"
#include <QtGui/QKeyEvent>

struct KeyCodeName
{
  int code;
  const char* name;
};

static constexpr KeyCodeName s_qt_key_names[] = {{Qt::Key_Escape, "Escape"},
                                                 {Qt::Key_Tab, "Tab"},
                                                 {Qt::Key_Backtab, "Backtab"},
                                                 {Qt::Key_Backspace, "Backspace"},
                                                 {Qt::Key_Return, "Return"},
                                                 {Qt::Key_Enter, "Enter"},
                                                 {Qt::Key_Insert, "Insert"},
                                                 {Qt::Key_Delete, "Delete"},
                                                 {Qt::Key_Pause, "Pause"},
                                                 {Qt::Key_Print, "Print"},
                                                 {Qt::Key_SysReq, "SysReq"},
                                                 {Qt::Key_Clear, "Clear"},
                                                 {Qt::Key_Home, "Home"},
                                                 {Qt::Key_End, "End"},
                                                 {Qt::Key_Left, "Left"},
                                                 {Qt::Key_Up, "Up"},
                                                 {Qt::Key_Right, "Right"},
                                                 {Qt::Key_Down, "Down"},
                                                 {Qt::Key_PageUp, "PageUp"},
                                                 {Qt::Key_PageDown, "PageDown"},
                                                 {Qt::Key_Shift, "Shift"},
                                                 {Qt::Key_Control, "Control"},
                                                 {Qt::Key_Meta, "Meta"},
                                                 {Qt::Key_Alt, "Alt"},
                                                 {Qt::Key_CapsLock, "CapsLock"},
                                                 {Qt::Key_NumLock, "NumLock"},
                                                 {Qt::Key_ScrollLock, "ScrollLock"},
                                                 {Qt::Key_F1, "F1"},
                                                 {Qt::Key_F2, "F2"},
                                                 {Qt::Key_F3, "F3"},
                                                 {Qt::Key_F4, "F4"},
                                                 {Qt::Key_F5, "F5"},
                                                 {Qt::Key_F6, "F6"},
                                                 {Qt::Key_F7, "F7"},
                                                 {Qt::Key_F8, "F8"},
                                                 {Qt::Key_F9, "F9"},
                                                 {Qt::Key_F10, "F10"},
                                                 {Qt::Key_F11, "F11"},
                                                 {Qt::Key_F12, "F12"},
                                                 {Qt::Key_F13, "F13"},
                                                 {Qt::Key_F14, "F14"},
                                                 {Qt::Key_F15, "F15"},
                                                 {Qt::Key_F16, "F16"},
                                                 {Qt::Key_F17, "F17"},
                                                 {Qt::Key_F18, "F18"},
                                                 {Qt::Key_F19, "F19"},
                                                 {Qt::Key_F20, "F20"},
                                                 {Qt::Key_F21, "F21"},
                                                 {Qt::Key_F22, "F22"},
                                                 {Qt::Key_F23, "F23"},
                                                 {Qt::Key_F24, "F24"},
                                                 {Qt::Key_F25, "F25"},
                                                 {Qt::Key_F26, "F26"},
                                                 {Qt::Key_F27, "F27"},
                                                 {Qt::Key_F28, "F28"},
                                                 {Qt::Key_F29, "F29"},
                                                 {Qt::Key_F30, "F30"},
                                                 {Qt::Key_F31, "F31"},
                                                 {Qt::Key_F32, "F32"},
                                                 {Qt::Key_F33, "F33"},
                                                 {Qt::Key_F34, "F34"},
                                                 {Qt::Key_F35, "F35"},
                                                 {Qt::Key_Super_L, "Super_L"},
                                                 {Qt::Key_Super_R, "Super_R"},
                                                 {Qt::Key_Menu, "Menu"},
                                                 {Qt::Key_Hyper_L, "Hyper_L"},
                                                 {Qt::Key_Hyper_R, "Hyper_R"},
                                                 {Qt::Key_Help, "Help"},
                                                 {Qt::Key_Direction_L, "Direction_L"},
                                                 {Qt::Key_Direction_R, "Direction_R"},
                                                 {Qt::Key_Space, "Space"},
                                                 {Qt::Key_Any, "Any"},
                                                 {Qt::Key_Exclam, "Exclam"},
                                                 {Qt::Key_QuoteDbl, "QuoteDbl"},
                                                 {Qt::Key_NumberSign, "NumberSign"},
                                                 {Qt::Key_Dollar, "Dollar"},
                                                 {Qt::Key_Percent, "Percent"},
                                                 {Qt::Key_Ampersand, "Ampersand"},
                                                 {Qt::Key_Apostrophe, "Apostrophe"},
                                                 {Qt::Key_ParenLeft, "ParenLeft"},
                                                 {Qt::Key_ParenRight, "ParenRight"},
                                                 {Qt::Key_Asterisk, "Asterisk"},
                                                 {Qt::Key_Plus, "Plus"},
                                                 {Qt::Key_Comma, "Comma"},
                                                 {Qt::Key_Minus, "Minus"},
                                                 {Qt::Key_Period, "Period"},
                                                 {Qt::Key_Slash, "Slash"},
                                                 {Qt::Key_0, "0"},
                                                 {Qt::Key_1, "1"},
                                                 {Qt::Key_2, "2"},
                                                 {Qt::Key_3, "3"},
                                                 {Qt::Key_4, "4"},
                                                 {Qt::Key_5, "5"},
                                                 {Qt::Key_6, "6"},
                                                 {Qt::Key_7, "7"},
                                                 {Qt::Key_8, "8"},
                                                 {Qt::Key_9, "9"},
                                                 {Qt::Key_Colon, "Colon"},
                                                 {Qt::Key_Semicolon, "Semicolon"},
                                                 {Qt::Key_Less, "Less"},
                                                 {Qt::Key_Equal, "Equal"},
                                                 {Qt::Key_Greater, "Greater"},
                                                 {Qt::Key_Question, "Question"},
                                                 {Qt::Key_At, "At"},
                                                 {Qt::Key_A, "A"},
                                                 {Qt::Key_B, "B"},
                                                 {Qt::Key_C, "C"},
                                                 {Qt::Key_D, "D"},
                                                 {Qt::Key_E, "E"},
                                                 {Qt::Key_F, "F"},
                                                 {Qt::Key_G, "G"},
                                                 {Qt::Key_H, "H"},
                                                 {Qt::Key_I, "I"},
                                                 {Qt::Key_J, "J"},
                                                 {Qt::Key_K, "K"},
                                                 {Qt::Key_L, "L"},
                                                 {Qt::Key_M, "M"},
                                                 {Qt::Key_N, "N"},
                                                 {Qt::Key_O, "O"},
                                                 {Qt::Key_P, "P"},
                                                 {Qt::Key_Q, "Q"},
                                                 {Qt::Key_R, "R"},
                                                 {Qt::Key_S, "S"},
                                                 {Qt::Key_T, "T"},
                                                 {Qt::Key_U, "U"},
                                                 {Qt::Key_V, "V"},
                                                 {Qt::Key_W, "W"},
                                                 {Qt::Key_X, "X"},
                                                 {Qt::Key_Y, "Y"},
                                                 {Qt::Key_Z, "Z"},
                                                 {Qt::Key_BracketLeft, "BracketLeft"},
                                                 {Qt::Key_Backslash, "Backslash"},
                                                 {Qt::Key_BracketRight, "BracketRight"},
                                                 {Qt::Key_AsciiCircum, "AsciiCircum"},
                                                 {Qt::Key_Underscore, "Underscore"},
                                                 {Qt::Key_QuoteLeft, "QuoteLeft"},
                                                 {Qt::Key_BraceLeft, "BraceLeft"},
                                                 {Qt::Key_Bar, "Bar"},
                                                 {Qt::Key_BraceRight, "BraceRight"},
                                                 {Qt::Key_AsciiTilde, "AsciiTilde"},
                                                 {Qt::Key_nobreakspace, "nobreakspace"},
                                                 {Qt::Key_exclamdown, "exclamdown"},
                                                 {Qt::Key_cent, "cent"},
                                                 {Qt::Key_sterling, "sterling"},
                                                 {Qt::Key_currency, "currency"},
                                                 {Qt::Key_yen, "yen"},
                                                 {Qt::Key_brokenbar, "brokenbar"},
                                                 {Qt::Key_section, "section"},
                                                 {Qt::Key_diaeresis, "diaeresis"},
                                                 {Qt::Key_copyright, "copyright"},
                                                 {Qt::Key_ordfeminine, "ordfeminine"},
                                                 {Qt::Key_guillemotleft, "guillemotleft"},
                                                 {Qt::Key_notsign, "notsign"},
                                                 {Qt::Key_hyphen, "hyphen"},
                                                 {Qt::Key_registered, "registered"},
                                                 {Qt::Key_macron, "macron"},
                                                 {Qt::Key_degree, "degree"},
                                                 {Qt::Key_plusminus, "plusminus"},
                                                 {Qt::Key_twosuperior, "twosuperior"},
                                                 {Qt::Key_threesuperior, "threesuperior"},
                                                 {Qt::Key_acute, "acute"},
                                                 {Qt::Key_mu, "mu"},
                                                 {Qt::Key_paragraph, "paragraph"},
                                                 {Qt::Key_periodcentered, "periodcentered"},
                                                 {Qt::Key_cedilla, "cedilla"},
                                                 {Qt::Key_onesuperior, "onesuperior"},
                                                 {Qt::Key_masculine, "masculine"},
                                                 {Qt::Key_guillemotright, "guillemotright"},
                                                 {Qt::Key_onequarter, "onequarter"},
                                                 {Qt::Key_onehalf, "onehalf"},
                                                 {Qt::Key_threequarters, "threequarters"},
                                                 {Qt::Key_questiondown, "questiondown"},
                                                 {Qt::Key_Agrave, "Agrave"},
                                                 {Qt::Key_Aacute, "Aacute"},
                                                 {Qt::Key_Acircumflex, "Acircumflex"},
                                                 {Qt::Key_Atilde, "Atilde"},
                                                 {Qt::Key_Adiaeresis, "Adiaeresis"},
                                                 {Qt::Key_Aring, "Aring"},
                                                 {Qt::Key_AE, "AE"},
                                                 {Qt::Key_Ccedilla, "Ccedilla"},
                                                 {Qt::Key_Egrave, "Egrave"},
                                                 {Qt::Key_Eacute, "Eacute"},
                                                 {Qt::Key_Ecircumflex, "Ecircumflex"},
                                                 {Qt::Key_Ediaeresis, "Ediaeresis"},
                                                 {Qt::Key_Igrave, "Igrave"},
                                                 {Qt::Key_Iacute, "Iacute"},
                                                 {Qt::Key_Icircumflex, "Icircumflex"},
                                                 {Qt::Key_Idiaeresis, "Idiaeresis"},
                                                 {Qt::Key_ETH, "ETH"},
                                                 {Qt::Key_Ntilde, "Ntilde"},
                                                 {Qt::Key_Ograve, "Ograve"},
                                                 {Qt::Key_Oacute, "Oacute"},
                                                 {Qt::Key_Ocircumflex, "Ocircumflex"},
                                                 {Qt::Key_Otilde, "Otilde"},
                                                 {Qt::Key_Odiaeresis, "Odiaeresis"},
                                                 {Qt::Key_multiply, "multiply"},
                                                 {Qt::Key_Ooblique, "Ooblique"},
                                                 {Qt::Key_Ugrave, "Ugrave"},
                                                 {Qt::Key_Uacute, "Uacute"},
                                                 {Qt::Key_Ucircumflex, "Ucircumflex"},
                                                 {Qt::Key_Udiaeresis, "Udiaeresis"},
                                                 {Qt::Key_Yacute, "Yacute"},
                                                 {Qt::Key_THORN, "THORN"},
                                                 {Qt::Key_ssharp, "ssharp"},
                                                 {Qt::Key_division, "division"},
                                                 {Qt::Key_ydiaeresis, "ydiaeresis"},
                                                 {Qt::Key_AltGr, "AltGr"},
                                                 {Qt::Key_Multi_key, "Multi_key"},
                                                 {Qt::Key_Codeinput, "Codeinput"},
                                                 {Qt::Key_SingleCandidate, "SingleCandidate"},
                                                 {Qt::Key_MultipleCandidate, "MultipleCandidate"},
                                                 {Qt::Key_PreviousCandidate, "PreviousCandidate"},
                                                 {Qt::Key_Mode_switch, "Mode_switch"},
                                                 {Qt::Key_Kanji, "Kanji"},
                                                 {Qt::Key_Muhenkan, "Muhenkan"},
                                                 {Qt::Key_Henkan, "Henkan"},
                                                 {Qt::Key_Romaji, "Romaji"},
                                                 {Qt::Key_Hiragana, "Hiragana"},
                                                 {Qt::Key_Katakana, "Katakana"},
                                                 {Qt::Key_Hiragana_Katakana, "Hiragana_Katakana"},
                                                 {Qt::Key_Zenkaku, "Zenkaku"},
                                                 {Qt::Key_Hankaku, "Hankaku"},
                                                 {Qt::Key_Zenkaku_Hankaku, "Zenkaku_Hankaku"},
                                                 {Qt::Key_Touroku, "Touroku"},
                                                 {Qt::Key_Massyo, "Massyo"},
                                                 {Qt::Key_Kana_Lock, "Kana_Lock"},
                                                 {Qt::Key_Kana_Shift, "Kana_Shift"},
                                                 {Qt::Key_Eisu_Shift, "Eisu_Shift"},
                                                 {Qt::Key_Eisu_toggle, "Eisu_toggle"},
                                                 {Qt::Key_Hangul, "Hangul"},
                                                 {Qt::Key_Hangul_Start, "Hangul_Start"},
                                                 {Qt::Key_Hangul_End, "Hangul_End"},
                                                 {Qt::Key_Hangul_Hanja, "Hangul_Hanja"},
                                                 {Qt::Key_Hangul_Jamo, "Hangul_Jamo"},
                                                 {Qt::Key_Hangul_Romaja, "Hangul_Romaja"},
                                                 {Qt::Key_Hangul_Jeonja, "Hangul_Jeonja"},
                                                 {Qt::Key_Hangul_Banja, "Hangul_Banja"},
                                                 {Qt::Key_Hangul_PreHanja, "Hangul_PreHanja"},
                                                 {Qt::Key_Hangul_PostHanja, "Hangul_PostHanja"},
                                                 {Qt::Key_Hangul_Special, "Hangul_Special"},
                                                 {Qt::Key_Dead_Grave, "Dead_Grave"},
                                                 {Qt::Key_Dead_Acute, "Dead_Acute"},
                                                 {Qt::Key_Dead_Circumflex, "Dead_Circumflex"},
                                                 {Qt::Key_Dead_Tilde, "Dead_Tilde"},
                                                 {Qt::Key_Dead_Macron, "Dead_Macron"},
                                                 {Qt::Key_Dead_Breve, "Dead_Breve"},
                                                 {Qt::Key_Dead_Abovedot, "Dead_Abovedot"},
                                                 {Qt::Key_Dead_Diaeresis, "Dead_Diaeresis"},
                                                 {Qt::Key_Dead_Abovering, "Dead_Abovering"},
                                                 {Qt::Key_Dead_Doubleacute, "Dead_Doubleacute"},
                                                 {Qt::Key_Dead_Caron, "Dead_Caron"},
                                                 {Qt::Key_Dead_Cedilla, "Dead_Cedilla"},
                                                 {Qt::Key_Dead_Ogonek, "Dead_Ogonek"},
                                                 {Qt::Key_Dead_Iota, "Dead_Iota"},
                                                 {Qt::Key_Dead_Voiced_Sound, "Dead_Voiced_Sound"},
                                                 {Qt::Key_Dead_Semivoiced_Sound, "Dead_Semivoiced_Sound"},
                                                 {Qt::Key_Dead_Belowdot, "Dead_Belowdot"},
                                                 {Qt::Key_Dead_Hook, "Dead_Hook"},
                                                 {Qt::Key_Dead_Horn, "Dead_Horn"},
                                                 {Qt::Key_Back, "Back"},
                                                 {Qt::Key_Forward, "Forward"},
                                                 {Qt::Key_Stop, "Stop"},
                                                 {Qt::Key_Refresh, "Refresh"},
                                                 {Qt::Key_VolumeDown, "VolumeDown"},
                                                 {Qt::Key_VolumeMute, "VolumeMute"},
                                                 {Qt::Key_VolumeUp, "VolumeUp"},
                                                 {Qt::Key_BassBoost, "BassBoost"},
                                                 {Qt::Key_BassUp, "BassUp"},
                                                 {Qt::Key_BassDown, "BassDown"},
                                                 {Qt::Key_TrebleUp, "TrebleUp"},
                                                 {Qt::Key_TrebleDown, "TrebleDown"},
                                                 {Qt::Key_MediaPlay, "MediaPlay"},
                                                 {Qt::Key_MediaStop, "MediaStop"},
                                                 {Qt::Key_MediaPrevious, "MediaPrevious"},
                                                 {Qt::Key_MediaNext, "MediaNext"},
                                                 {Qt::Key_MediaRecord, "MediaRecord"},
                                                 {Qt::Key_MediaPause, "MediaPause"},
                                                 {Qt::Key_MediaTogglePlayPause, "MediaTogglePlayPause"},
                                                 {Qt::Key_HomePage, "HomePage"},
                                                 {Qt::Key_Favorites, "Favorites"},
                                                 {Qt::Key_Search, "Search"},
                                                 {Qt::Key_Standby, "Standby"},
                                                 {Qt::Key_OpenUrl, "OpenUrl"},
                                                 {Qt::Key_LaunchMail, "LaunchMail"},
                                                 {Qt::Key_LaunchMedia, "LaunchMedia"},
                                                 {Qt::Key_Launch0, "Launch0"},
                                                 {Qt::Key_Launch1, "Launch1"},
                                                 {Qt::Key_Launch2, "Launch2"},
                                                 {Qt::Key_Launch3, "Launch3"},
                                                 {Qt::Key_Launch4, "Launch4"},
                                                 {Qt::Key_Launch5, "Launch5"},
                                                 {Qt::Key_Launch6, "Launch6"},
                                                 {Qt::Key_Launch7, "Launch7"},
                                                 {Qt::Key_Launch8, "Launch8"},
                                                 {Qt::Key_Launch9, "Launch9"},
                                                 {Qt::Key_LaunchA, "LaunchA"},
                                                 {Qt::Key_LaunchB, "LaunchB"},
                                                 {Qt::Key_LaunchC, "LaunchC"},
                                                 {Qt::Key_LaunchD, "LaunchD"},
                                                 {Qt::Key_LaunchE, "LaunchE"},
                                                 {Qt::Key_LaunchF, "LaunchF"},
                                                 {Qt::Key_MonBrightnessUp, "MonBrightnessUp"},
                                                 {Qt::Key_MonBrightnessDown, "MonBrightnessDown"},
                                                 {Qt::Key_KeyboardLightOnOff, "KeyboardLightOnOff"},
                                                 {Qt::Key_KeyboardBrightnessUp, "KeyboardBrightnessUp"},
                                                 {Qt::Key_KeyboardBrightnessDown, "KeyboardBrightnessDown"},
                                                 {Qt::Key_PowerOff, "PowerOff"},
                                                 {Qt::Key_WakeUp, "WakeUp"},
                                                 {Qt::Key_Eject, "Eject"},
                                                 {Qt::Key_ScreenSaver, "ScreenSaver"},
                                                 {Qt::Key_WWW, "WWW"},
                                                 {Qt::Key_Memo, "Memo"},
                                                 {Qt::Key_LightBulb, "LightBulb"},
                                                 {Qt::Key_Shop, "Shop"},
                                                 {Qt::Key_History, "History"},
                                                 {Qt::Key_AddFavorite, "AddFavorite"},
                                                 {Qt::Key_HotLinks, "HotLinks"},
                                                 {Qt::Key_BrightnessAdjust, "BrightnessAdjust"},
                                                 {Qt::Key_Finance, "Finance"},
                                                 {Qt::Key_Community, "Community"},
                                                 {Qt::Key_AudioRewind, "AudioRewind"},
                                                 {Qt::Key_BackForward, "BackForward"},
                                                 {Qt::Key_ApplicationLeft, "ApplicationLeft"},
                                                 {Qt::Key_ApplicationRight, "ApplicationRight"},
                                                 {Qt::Key_Book, "Book"},
                                                 {Qt::Key_CD, "CD"},
                                                 {Qt::Key_Calculator, "Calculator"},
                                                 {Qt::Key_ToDoList, "ToDoList"},
                                                 {Qt::Key_ClearGrab, "ClearGrab"},
                                                 {Qt::Key_Close, "Close"},
                                                 {Qt::Key_Copy, "Copy"},
                                                 {Qt::Key_Cut, "Cut"},
                                                 {Qt::Key_Display, "Display"},
                                                 {Qt::Key_DOS, "DOS"},
                                                 {Qt::Key_Documents, "Documents"},
                                                 {Qt::Key_Excel, "Excel"},
                                                 {Qt::Key_Explorer, "Explorer"},
                                                 {Qt::Key_Game, "Game"},
                                                 {Qt::Key_Go, "Go"},
                                                 {Qt::Key_iTouch, "iTouch"},
                                                 {Qt::Key_LogOff, "LogOff"},
                                                 {Qt::Key_Market, "Market"},
                                                 {Qt::Key_Meeting, "Meeting"},
                                                 {Qt::Key_MenuKB, "MenuKB"},
                                                 {Qt::Key_MenuPB, "MenuPB"},
                                                 {Qt::Key_MySites, "MySites"},
                                                 {Qt::Key_News, "News"},
                                                 {Qt::Key_OfficeHome, "OfficeHome"},
                                                 {Qt::Key_Option, "Option"},
                                                 {Qt::Key_Paste, "Paste"},
                                                 {Qt::Key_Phone, "Phone"},
                                                 {Qt::Key_Calendar, "Calendar"},
                                                 {Qt::Key_Reply, "Reply"},
                                                 {Qt::Key_Reload, "Reload"},
                                                 {Qt::Key_RotateWindows, "RotateWindows"},
                                                 {Qt::Key_RotationPB, "RotationPB"},
                                                 {Qt::Key_RotationKB, "RotationKB"},
                                                 {Qt::Key_Save, "Save"},
                                                 {Qt::Key_Send, "Send"},
                                                 {Qt::Key_Spell, "Spell"},
                                                 {Qt::Key_SplitScreen, "SplitScreen"},
                                                 {Qt::Key_Support, "Support"},
                                                 {Qt::Key_TaskPane, "TaskPane"},
                                                 {Qt::Key_Terminal, "Terminal"},
                                                 {Qt::Key_Tools, "Tools"},
                                                 {Qt::Key_Travel, "Travel"},
                                                 {Qt::Key_Video, "Video"},
                                                 {Qt::Key_Word, "Word"},
                                                 {Qt::Key_Xfer, "Xfer"},
                                                 {Qt::Key_ZoomIn, "ZoomIn"},
                                                 {Qt::Key_ZoomOut, "ZoomOut"},
                                                 {Qt::Key_Away, "Away"},
                                                 {Qt::Key_Messenger, "Messenger"},
                                                 {Qt::Key_WebCam, "WebCam"},
                                                 {Qt::Key_MailForward, "MailForward"},
                                                 {Qt::Key_Pictures, "Pictures"},
                                                 {Qt::Key_Music, "Music"},
                                                 {Qt::Key_Battery, "Battery"},
                                                 {Qt::Key_Bluetooth, "Bluetooth"},
                                                 {Qt::Key_WLAN, "WLAN"},
                                                 {Qt::Key_UWB, "UWB"},
                                                 {Qt::Key_AudioForward, "AudioForward"},
                                                 {Qt::Key_AudioRepeat, "AudioRepeat"},
                                                 {Qt::Key_AudioRandomPlay, "AudioRandomPlay"},
                                                 {Qt::Key_Subtitle, "Subtitle"},
                                                 {Qt::Key_AudioCycleTrack, "AudioCycleTrack"},
                                                 {Qt::Key_Time, "Time"},
                                                 {Qt::Key_Hibernate, "Hibernate"},
                                                 {Qt::Key_View, "View"},
                                                 {Qt::Key_TopMenu, "TopMenu"},
                                                 {Qt::Key_PowerDown, "PowerDown"},
                                                 {Qt::Key_Suspend, "Suspend"},
                                                 {Qt::Key_ContrastAdjust, "ContrastAdjust"},
                                                 {Qt::Key_LaunchG, "LaunchG"},
                                                 {Qt::Key_LaunchH, "LaunchH"},
                                                 {Qt::Key_TouchpadToggle, "TouchpadToggle"},
                                                 {Qt::Key_TouchpadOn, "TouchpadOn"},
                                                 {Qt::Key_TouchpadOff, "TouchpadOff"},
                                                 {Qt::Key_MicMute, "MicMute"},
                                                 {Qt::Key_Red, "Red"},
                                                 {Qt::Key_Green, "Green"},
                                                 {Qt::Key_Yellow, "Yellow"},
                                                 {Qt::Key_Blue, "Blue"},
                                                 {Qt::Key_ChannelUp, "ChannelUp"},
                                                 {Qt::Key_ChannelDown, "ChannelDown"},
                                                 {Qt::Key_Guide, "Guide"},
                                                 {Qt::Key_Info, "Info"},
                                                 {Qt::Key_Settings, "Settings"},
                                                 {Qt::Key_MicVolumeUp, "MicVolumeUp"},
                                                 {Qt::Key_MicVolumeDown, "MicVolumeDown"},
                                                 {Qt::Key_New, "New"},
                                                 {Qt::Key_Open, "Open"},
                                                 {Qt::Key_Find, "Find"},
                                                 {Qt::Key_Undo, "Undo"},
                                                 {Qt::Key_Redo, "Redo"},
                                                 {Qt::Key_MediaLast, "MediaLast"},
                                                 {Qt::Key_Select, "Select"},
                                                 {Qt::Key_Yes, "Yes"},
                                                 {Qt::Key_No, "No"},
                                                 {Qt::Key_Cancel, "Cancel"},
                                                 {Qt::Key_Printer, "Printer"},
                                                 {Qt::Key_Execute, "Execute"},
                                                 {Qt::Key_Sleep, "Sleep"},
                                                 {Qt::Key_Play, "Play"},
                                                 {Qt::Key_Zoom, "Zoom"},
                                                 {Qt::Key_Exit, "Exit"},
                                                 {Qt::Key_Context1, "Context1"},
                                                 {Qt::Key_Context2, "Context2"},
                                                 {Qt::Key_Context3, "Context3"},
                                                 {Qt::Key_Context4, "Context4"},
                                                 {Qt::Key_Call, "Call"},
                                                 {Qt::Key_Hangup, "Hangup"},
                                                 {Qt::Key_Flip, "Flip"},
                                                 {Qt::Key_ToggleCallHangup, "ToggleCallHangup"},
                                                 {Qt::Key_VoiceDial, "VoiceDial"},
                                                 {Qt::Key_LastNumberRedial, "LastNumberRedial"},
                                                 {Qt::Key_Camera, "Camera"},
                                                 {Qt::Key_CameraFocus, "CameraFocus"}};

std::optional<u32> InputManager::ConvertHostKeyboardStringToCode(const std::string_view& str)
{
  std::string compare_name(str);
  u32 modifier_bits = 0;
  if (StringUtil::StartsWith(compare_name, "Numpad"))
  {
    compare_name = compare_name.substr(6);
    modifier_bits |= Qt::KeypadModifier;
  }

  for (const KeyCodeName& name : s_qt_key_names)
  {
    if (compare_name == name.name)
      return static_cast<u32>(name.code) | modifier_bits;
  }

  return std::nullopt;
}

std::optional<std::string> InputManager::ConvertHostKeyboardCodeToString(u32 code)
{
  std::optional<std::string> ret;

  const u32 modifier_bits = (code & Qt::KeyboardModifierMask);
  const u32 masked_code = (code & ~Qt::KeyboardModifierMask);

  for (const KeyCodeName& name : s_qt_key_names)
  {
    if (static_cast<int>(masked_code) == name.code)
    {
      if (modifier_bits & Qt::KeypadModifier)
        ret = fmt::format("Numpad{}", name.name);
      else
        ret = std::string(name.name);

      break;
    }
  }

  return ret;
}

u32 QtUtils::KeyEventToCode(const QKeyEvent* ev)
{
  return static_cast<u32>(ev->key()) | (static_cast<u32>(ev->modifiers()) & static_cast<u32>(Qt::KeypadModifier));
}

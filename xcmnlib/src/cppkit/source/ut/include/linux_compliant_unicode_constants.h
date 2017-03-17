#ifndef __linux_compliant_unicode_constants_H__
#define __linux_compliant_unicode_constants_H__

static const wchar_t CHINESE_UNICODE_STRING_1[] = L"好久不见";
static const size_t CHINESE_UNICODE_STRING_1_SIZE = 12;
static const wchar_t CHINESE_UNICODE_STRING_1_END[] = L"不见";
static const wchar_t CHINESE_UNICODE_STRING_1_MIDDLE[] = L"不";
static const wchar_t CHINESE_UNICODE_STRING_1_BEGIN[] = L"好久不";
static const wchar_t CHINESE_UNICODE_STRING_1_REORDERED[] = L"久见不好";

static const wchar_t CHINESE_UNICODE_STRING_2[] = L"三人成虎";
static const size_t CHINESE_UNICODE_STRING_2_SIZE = 12;

static const wchar_t LONG_CHINESE_UNICODE_STRING[] = L"说曹操,曹操到";
static const wchar_t LONG_CHINESE_UNICODE_STRING_BEGINING[] = L"说曹操";
static const wchar_t LONG_CHINESE_UNICODE_STRING_ENDING[] = L"曹操到";



const wchar_t CHINESE_UNICODE_STRING_3[] = L"围魏救赵";

static const wchar_t JAPANESE_UNICODE_STRING_1[] = L"あなたは猿です";


static const wchar_t CHINESE_UNICODE_WITH_LEFT_SAPCE[] = L" 关门捉贼";
static const wchar_t CHINESE_UNICODE_WITH_RIGHT_SAPCE[] = L"关门捉贼 ";
static const wchar_t CHINESE_UNICODE_WITH_SAPCE_AT_ENDS[] = L" 关门捉贼 ";
static const wchar_t CHINESE_UNICODE_WITH_NO_SAPCE[] = L"关门捉贼";


static const wchar_t ARABIC_UNICODE_TO_BE_STRIPPED[] = L" الصفحة الرئيسية ";
static const wchar_t ARABIC_UNICODE_STRIPPED[] = L"الصفحة الرئيسية";

static const wchar_t RUSSIAN_UNICODE_TO_BE_SEARCHED[] = L"Охота за \"Красным октябрем\"";
static const wchar_t RUSSIAN_UNICODE_TO_BE_FOUND[] = L"Красным";


static const wchar_t UNICODE_STRING_TO_SPLIT[] = L"是故百戰百勝，非善之善者也；不戰而屈人之兵，善之善者也。";
static const wchar_t UNICODE_STRING_SPLIT_BY_CHARACTER_PIECE_0[] = L"是故百戰百勝，非";
static const wchar_t UNICODE_STRING_SPLIT_BY_CHARACTER_PIECE_1[] = L"之";
static const wchar_t UNICODE_STRING_SPLIT_BY_CHARACTER_PIECE_2[] = L"者也；不戰而屈人之兵，";
static const wchar_t UNICODE_STRING_SPLIT_BY_CHARACTER_PIECE_3[] = L"之";
static const wchar_t UNICODE_STRING_SPLIT_BY_CHARACTER_PIECE_4[] = L"者也。";
static wchar_t UNICODE_CHARACTER_TO_SPLIT_ON = L'善';
static const wchar_t UNICODE_STRING_SPLIT_BY_STRING_PIECE_0[] = L"是故百戰百勝，非";
static const wchar_t UNICODE_STRING_SPLIT_BY_STRING_PIECE_1[] = L"者也；不戰而屈人之兵，";
static const wchar_t UNICODE_STRING_SPLIT_BY_STRING_PIECE_2[] = L"者也。";
static wchar_t UNICODE_STRING_TO_SPLIT_ON[] = L"善之善";

static const wchar_t UNICODE_CHARACTER_TO_BE_REPLACED = L'好';
static const wchar_t UNICODE_CHARACTER_TO_REPLACE_WITH = L'不';
static const wchar_t UNICODE_STRING_BEFORE_REPLACING[] = L"好我好我好";
static const wchar_t UNICODE_STRING_AFTER_REPLACING[] = L"不我不我不";


static const wchar_t UNICODE_CHARACTER_TO_BE_ERASED = L'好';
static const wchar_t UNICODE_STRING_TO_ERASE_FROM[] = L"认识你很好，我不感觉不太好";
static const wchar_t UNICODE_STRING_AFTER_ERASE[] = L"认识你很，我不感觉不太";

static const wchar_t UNICODE_STRING_FOR_FINDING_VALUES_IN[] = L"是故百戰百勝，非善之善者也；不戰而屈人之兵，善之善者也。";
static const wchar_t UNICODE_STRING_FOR_FINDING[] = L"百";
static const wchar_t UNICODE_CHARACTER_FOR_FINDING = L'百';

#endif

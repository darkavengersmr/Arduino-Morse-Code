//================================================================================================================
// Настройки кнопок.
//================================================================================================================

// подключение

// 0
// 1
// 2 экран nokia 84x48 PIN_SCLK
// 3 экран nokia 84x48 PIN_SDIN
// 4 экран nokia 84x48 PIN_DC
// 5 экран nokia 84x48 PIN_SCE 
// 6 экран nokia 84x48 PIN_RESET
// 7 кнопка - точка
// 8 кнопка - тире
// 9 светодиод B
// 10 светодиод G
// 11 светодиод R
// 12 динамик
// 13

// всего введено текста
int ALLTEXT = 0;

// текущий распознанный символ
char CURRSYM;
char* SCREENSYM;

// Кнопка-точка будет слева.
#define BUTTON_DOT   7
// Кнопка-тире - справа.
#define BUTTON_TIRE  8

int buttons, buttonsPrev; // Здесь будем в виде битовых масок хранить нынешнее и предыдущее состояние клавиш.
#define BUTTON_DOT_MASK   1
#define BUTTON_TIRE_MASK  2

// С помощью этих переменных мы засекаем, когда в последний раз была нажата и отпущена кнопка. Любая. По этому
//  времени мы определяем, завершен ли ввод текущего символа и не пора ли переходить в спящий режим.
unsigned long int timeRelease, timePress;

// Смену состояния кнопки на время менее 0.03с будем считать дребезгом и игнорировать.
#define DEBOUNCING_TIME  30
// В эти переменные будем сохранять время для отфильтровывания дребезга контактов.
unsigned long int timeDotDebouncing;
unsigned long int timeTireDebouncing;

// Максимальная пауза между точками и тире в букве - 0.4 секунды.
//  Если пауза больше - считаем ввод буквы завершенным и переходим к вводу следующей буквы.
#define DELIMITER_TIME  500
// Если кнопка не нажималась более минуты - переходим в спящий режим.
#define SLEEP_TIME  60000
// Выйти из спящего режима можно, удерживая нажатой любую кнопку в течение 1 секунды.
#define WAKEUP_TIME  2000

// Для переключения раскладки на кириллицу нажимаем кнопку-точку и, не отпуская ее, нажимаем кнопку-тире. 
// Для переключения раскладки на латынь нажимаем правую кнопку-тире и, не отпуская ее, нажимаем кнопку-точку. 

//================================================================================================================
// Настройки RGB-светодиода.
//================================================================================================================

// Для обратной связи будем использовать RGB-светодиод:
#define LED_R  11
#define LED_G  10
#define LED_B  9

// Цвета диода будем задавать в виде числа-битовой маски: 00000RGB, вспомним цвета в старых добрых EGA и Yamaha MSX.
//  Семи цветов (черный не в счет) нам более, чем хватит.
#define COLOR_BLACK    0
#define COLOR_BLUE     1
#define COLOR_GREEN    2
#define COLOR_CYAN     3
#define COLOR_RED      4
#define COLOR_MAGENTA  5
#define COLOR_YELLOW   6
#define COLOR_WHITE    7

// Кириллица - зеленый, латынь - желтый, спящий режм - мигание фиолетовым.
#define COLOR_CYRILLIC_LAYOUT  COLOR_GREEN
#define COLOR_LATIN_LAYOUT     COLOR_YELLOW
#define COLOR_SLEEP_MODE       COLOR_MAGENTA

// Яркость мигания для режима печати и спящего режима. Не забывваем, что у нас логика инвертирована
//  и 0 означает максимальную яркость, а 255 - погашенный светодиод.

#define BRIGHTNESS_TYPING_LOW   (1)
#define BRIGHTNESS_TYPING_DOT   (7)
#define BRIGHTNESS_TYPING_TIRE  (15)
#define BRIGHTNESS_SLEEP_LOW    (0)
#define BRIGHTNESS_SLEEP_HIGH   (1)
/*
#define BRIGHTNESS_TYPING_LOW   (255-7)
#define BRIGHTNESS_TYPING_DOT   (255-128)
#define BRIGHTNESS_TYPING_TIRE  (255-255)
#define BRIGHTNESS_SLEEP_LOW    (255-8)
#define BRIGHTNESS_SLEEP_HIGH   (255-128)
*/
//================================================================================================================
// Настройки пьезо-динамика.
//================================================================================================================

#define PIEZO  12
byte piezoData;
unsigned long int piezoShutUpTime;

//================================================================================================================
// Азбука Морзе.
//================================================================================================================

// Этими символами мы будем обозначать точки и тире.
#define MORSE_DOT     '*'
#define MORSE_TIRE    '-'
// Точка или тире пока не введены. 
#define MORSE_EMPTY   0
// Это - для блокировки ввода точек/тире при смене раскладки или выходе из спящего режима.
#define MORSE_LOCKED  '!'

// Максимальная длина символа азбуки Морзе (в точках и тире)
#define MAX_MORSE_SYMBOL_LENGTH  8
// Буфер для записи морзе-символа.
byte morseSymbol[MAX_MORSE_SYMBOL_LENGTH];
unsigned int morseSymbolLen;
byte newMorseSignal; // Новый введенный сигнал - точка или тире.

// Таблица кодов Морзе. N-ный элемент кода соответствует n-ному символу раскладки.
char* code[] = {
  "*-","-***","*--","--*","-**","*","***-","--**","**","*---",
  "-*-","*-**","--","-*","---","*--*","*-*","***","-","**-",
  "**-*","****","-*-*","---*","----","--*-","-*--","-**-","**-**","**--",
  "*-*-",
  "*----","**---","***--","****-","*****","-****","--***","---**","----*","-----",
  "......","*-*-*-","---***","-*-*-","-*--*-","*----*","*-**-*","-****-","-**-*","**--**","--**--",
  "-***-","********","*--*-*","**-*-",
  ""
};

// Кириллическая раскладка.
char* layoutCyrillic[] = {
  "A","B","V","G","D","E","J","Z","I","I",
  "K","L","M","N","O","P","R","S","T","U",
  "F","H","C","^","W","X","Y","Q","]","[",
  "@",
  "1","2","3","4","5","6","7","8","9","0",
  ".",",",":",";","(","\'","\"","-","/","?","!",
  " *DELIMITER* "," *ERR* ","@"," *END* ",
  ""
  };
// Латинская раскладка.
char* layoutLatin[] = {
  "a","b","w","g","d","e","v","z","i","j",
  "k","l","m","n","o","p","r","s","t","u",
  "f","h","c","ö","ch","q","y","x","é","ü",
  "ä",
  "1","2","3","4","5","6","7","8","9","0",
  ".",",",":",";","(","\'","\"","-","/","?","!",
  " *DELIMITER* "," *ERR* ","@"," *END* ",
  ""
};

char** currentLayout;
char** newLayout;

//================================================================================================================
// Режимы работы.
//================================================================================================================

#define TYPING_MODE  0
#define SLEEP_MODE   1
int mode;
boolean flagWakeUp; // Этот флаг будем использовать для выхода из спящего режима.
byte ledLevelSleepCounter; // Переключатель яркости для мигания диода в спящеь режиме.

//================================================================================================================

#define PIN_SCE   5
#define PIN_RESET 6
#define PIN_DC    4
#define PIN_SDIN  3
#define PIN_SCLK  2

#define LCD_C     LOW
#define LCD_D     HIGH

#define LCD_X     84
#define LCD_Y     48

#define LANG_ENG 0
#define LANG_RUS 1
/*
static const byte ASCII_ENG[][5] =
{
 {0x00, 0x00, 0x00, 0x00, 0x00} // 20  
,{0x00, 0x00, 0x5f, 0x00, 0x00} // 21 !
,{0x00, 0x07, 0x00, 0x07, 0x00} // 22 "
,{0x14, 0x7f, 0x14, 0x7f, 0x14} // 23 #
,{0x24, 0x2a, 0x7f, 0x2a, 0x12} // 24 $
,{0x23, 0x13, 0x08, 0x64, 0x62} // 25 %
,{0x36, 0x49, 0x55, 0x22, 0x50} // 26 &
,{0x00, 0x05, 0x03, 0x00, 0x00} // 27 '
,{0x00, 0x1c, 0x22, 0x41, 0x00} // 28 (
,{0x00, 0x41, 0x22, 0x1c, 0x00} // 29 )
,{0x14, 0x08, 0x3e, 0x08, 0x14} // 2a *
,{0x08, 0x08, 0x3e, 0x08, 0x08} // 2b +
,{0x00, 0x50, 0x30, 0x00, 0x00} // 2c ,
,{0x08, 0x08, 0x08, 0x08, 0x08} // 2d -
,{0x00, 0x60, 0x60, 0x00, 0x00} // 2e .
,{0x20, 0x10, 0x08, 0x04, 0x02} // 2f /
,{0x3e, 0x51, 0x49, 0x45, 0x3e} // 30 0
,{0x00, 0x42, 0x7f, 0x40, 0x00} // 31 1
,{0x42, 0x61, 0x51, 0x49, 0x46} // 32 2
,{0x21, 0x41, 0x45, 0x4b, 0x31} // 33 3
,{0x18, 0x14, 0x12, 0x7f, 0x10} // 34 4
,{0x27, 0x45, 0x45, 0x45, 0x39} // 35 5
,{0x3c, 0x4a, 0x49, 0x49, 0x30} // 36 6
,{0x01, 0x71, 0x09, 0x05, 0x03} // 37 7
,{0x36, 0x49, 0x49, 0x49, 0x36} // 38 8
,{0x06, 0x49, 0x49, 0x29, 0x1e} // 39 9
,{0x00, 0x36, 0x36, 0x00, 0x00} // 3a :
,{0x00, 0x56, 0x36, 0x00, 0x00} // 3b ;
,{0x08, 0x14, 0x22, 0x41, 0x00} // 3c <
,{0x14, 0x14, 0x14, 0x14, 0x14} // 3d =
,{0x00, 0x41, 0x22, 0x14, 0x08} // 3e >
,{0x02, 0x01, 0x51, 0x09, 0x06} // 3f ?
,{0x32, 0x49, 0x79, 0x41, 0x3e} // 40 @
,{0x7e, 0x11, 0x11, 0x11, 0x7e} // 41 A
,{0x7f, 0x49, 0x49, 0x49, 0x36} // 42 B
,{0x3e, 0x41, 0x41, 0x41, 0x22} // 43 C
,{0x7f, 0x41, 0x41, 0x22, 0x1c} // 44 D
,{0x7f, 0x49, 0x49, 0x49, 0x41} // 45 E
,{0x7f, 0x09, 0x09, 0x09, 0x01} // 46 F
,{0x3e, 0x41, 0x49, 0x49, 0x7a} // 47 G
,{0x7f, 0x08, 0x08, 0x08, 0x7f} // 48 H
,{0x00, 0x41, 0x7f, 0x41, 0x00} // 49 I
,{0x20, 0x40, 0x41, 0x3f, 0x01} // 4a J
,{0x7f, 0x08, 0x14, 0x22, 0x41} // 4b K
,{0x7f, 0x40, 0x40, 0x40, 0x40} // 4c L
,{0x7f, 0x02, 0x0c, 0x02, 0x7f} // 4d M
,{0x7f, 0x04, 0x08, 0x10, 0x7f} // 4e N
,{0x3e, 0x41, 0x41, 0x41, 0x3e} // 4f O
,{0x7f, 0x09, 0x09, 0x09, 0x06} // 50 P
,{0x3e, 0x41, 0x51, 0x21, 0x5e} // 51 Q
,{0x7f, 0x09, 0x19, 0x29, 0x46} // 52 R
,{0x46, 0x49, 0x49, 0x49, 0x31} // 53 S
,{0x01, 0x01, 0x7f, 0x01, 0x01} // 54 T
,{0x3f, 0x40, 0x40, 0x40, 0x3f} // 55 U
,{0x1f, 0x20, 0x40, 0x20, 0x1f} // 56 V
,{0x3f, 0x40, 0x38, 0x40, 0x3f} // 57 W
,{0x63, 0x14, 0x08, 0x14, 0x63} // 58 X
,{0x07, 0x08, 0x70, 0x08, 0x07} // 59 Y
,{0x61, 0x51, 0x49, 0x45, 0x43} // 5a Z
,{0x00, 0x7f, 0x41, 0x41, 0x00} // 5b [
,{0x02, 0x04, 0x08, 0x10, 0x20} // 5c ?
,{0x00, 0x41, 0x41, 0x7f, 0x00} // 5d ]
,{0x04, 0x02, 0x01, 0x02, 0x04} // 5e ^
,{0x40, 0x40, 0x40, 0x40, 0x40} // 5f _
,{0x00, 0x01, 0x02, 0x04, 0x00} // 60 `
,{0x20, 0x54, 0x54, 0x54, 0x78} // 61 a
,{0x7f, 0x48, 0x44, 0x44, 0x38} // 62 b
,{0x38, 0x44, 0x44, 0x44, 0x20} // 63 c
,{0x38, 0x44, 0x44, 0x48, 0x7f} // 64 d
,{0x38, 0x54, 0x54, 0x54, 0x18} // 65 e
,{0x08, 0x7e, 0x09, 0x01, 0x02} // 66 f
,{0x0c, 0x52, 0x52, 0x52, 0x3e} // 67 g
,{0x7f, 0x08, 0x04, 0x04, 0x78} // 68 h
,{0x00, 0x44, 0x7d, 0x40, 0x00} // 69 i
,{0x20, 0x40, 0x44, 0x3d, 0x00} // 6a j 
,{0x7f, 0x10, 0x28, 0x44, 0x00} // 6b k
,{0x00, 0x41, 0x7f, 0x40, 0x00} // 6c l
,{0x7c, 0x04, 0x18, 0x04, 0x78} // 6d m
,{0x7c, 0x08, 0x04, 0x04, 0x78} // 6e n
,{0x38, 0x44, 0x44, 0x44, 0x38} // 6f o
,{0x7c, 0x14, 0x14, 0x14, 0x08} // 70 p
,{0x08, 0x14, 0x14, 0x18, 0x7c} // 71 q
,{0x7c, 0x08, 0x04, 0x04, 0x08} // 72 r
,{0x48, 0x54, 0x54, 0x54, 0x20} // 73 s
,{0x04, 0x3f, 0x44, 0x40, 0x20} // 74 t
,{0x3c, 0x40, 0x40, 0x20, 0x7c} // 75 u
,{0x1c, 0x20, 0x40, 0x20, 0x1c} // 76 v
,{0x3c, 0x40, 0x30, 0x40, 0x3c} // 77 w
,{0x44, 0x28, 0x10, 0x28, 0x44} // 78 x
,{0x0c, 0x50, 0x50, 0x50, 0x3c} // 79 y
,{0x44, 0x64, 0x54, 0x4c, 0x44} // 7a z
,{0x00, 0x08, 0x36, 0x41, 0x00} // 7b {
,{0x00, 0x00, 0x7f, 0x00, 0x00} // 7c |
,{0x00, 0x41, 0x36, 0x08, 0x00} // 7d }
,{0x10, 0x08, 0x08, 0x10, 0x08} // 7e <
,{0x78, 0x46, 0x41, 0x46, 0x78} // 7f >
};
*/
static const byte ASCII_RUS[][5] =
{
 {0x00, 0x00, 0x00, 0x00, 0x00} // 20  
,{0x00, 0x00, 0x5f, 0x00, 0x00} // 21 !
,{0x00, 0x07, 0x00, 0x07, 0x00} // 22 "
,{0x14, 0x7f, 0x14, 0x7f, 0x14} // 23 #
,{0x24, 0x2a, 0x7f, 0x2a, 0x12} // 24 $
,{0x23, 0x13, 0x08, 0x64, 0x62} // 25 %
,{0x36, 0x49, 0x55, 0x22, 0x50} // 26 &
,{0x00, 0x05, 0x03, 0x00, 0x00} // 27 '
,{0x00, 0x1c, 0x22, 0x41, 0x00} // 28 (
,{0x00, 0x41, 0x22, 0x1c, 0x00} // 29 )
,{0x14, 0x08, 0x3e, 0x08, 0x14} // 2a *
,{0x08, 0x08, 0x3e, 0x08, 0x08} // 2b +
,{0x00, 0x50, 0x30, 0x00, 0x00} // 2c ,
,{0x08, 0x08, 0x08, 0x08, 0x08} // 2d -
,{0x00, 0x60, 0x60, 0x00, 0x00} // 2e .
,{0x20, 0x10, 0x08, 0x04, 0x02} // 2f /
,{0x3e, 0x51, 0x49, 0x45, 0x3e} // 30 0
,{0x00, 0x42, 0x7f, 0x40, 0x00} // 31 1
,{0x42, 0x61, 0x51, 0x49, 0x46} // 32 2
,{0x21, 0x41, 0x45, 0x4b, 0x31} // 33 3
,{0x18, 0x14, 0x12, 0x7f, 0x10} // 34 4
,{0x27, 0x45, 0x45, 0x45, 0x39} // 35 5
,{0x3c, 0x4a, 0x49, 0x49, 0x30} // 36 6
,{0x01, 0x71, 0x09, 0x05, 0x03} // 37 7
,{0x36, 0x49, 0x49, 0x49, 0x36} // 38 8
,{0x06, 0x49, 0x49, 0x29, 0x1e} // 39 9
,{0x00, 0x36, 0x36, 0x00, 0x00} // 3a :
,{0x00, 0x56, 0x36, 0x00, 0x00} // 3b ;
,{0x08, 0x14, 0x22, 0x41, 0x00} // 3c <
,{0x14, 0x14, 0x14, 0x14, 0x14} // 3d =
,{0x00, 0x41, 0x22, 0x14, 0x08} // 3e >

//,{0x02, 0x01, 0x51, 0x09, 0x06} // 3f ?
,{0x7f, 0x8, 0x3e, 0x41, 0x3e} // 5c ? = Ю

,{0x46, 0x29, 0x19, 0x9, 0x7f} // 40 @ = Я
,{0x7e, 0x11, 0x11, 0x11, 0x7e} // 41 A  
,{0x7f, 0x49, 0x49, 0x49, 0x31} // 42 B = Б
,{0x7f, 0x40, 0x40, 0x7f, 0xc0} // 43 C = Ц
,{0xc0, 0x7f, 0x41, 0x7f, 0xc0} // 44 D = Д
,{0x7f, 0x49, 0x49, 0x49, 0x41} // 45 E 
,{0xe, 0x11, 0x7f, 0x11, 0xe} // 46 F = Ф
,{0x7f, 0x1, 0x1, 0x1, 0x1} // 47 G = Г
,{0x63, 0x14, 0x08, 0x14, 0x63} // 48 H = Х
,{0x7f, 0x10, 0xc, 0x2, 0x7f} // 49 I = И
,{0x73, 0xc, 0x7f, 0xc, 0x73} // 4a J = Ж
,{0x7f, 0x08, 0x14, 0x22, 0x41} // 4b K 
,{0x40, 0x3e, 0x1, 0x1, 0x7f} // 4c L = Л
,{0x7f, 0x02, 0x0c, 0x02, 0x7f} // 4d M
,{0x7f, 0x08, 0x08, 0x08, 0x7f} // 4e N = Н
,{0x3e, 0x41, 0x41, 0x41, 0x3e} // 4f O
,{0x7f, 0x1, 0x1, 0x1, 0x7f} // 50 P = П
,{0x7f, 0x48, 0x48, 0x30, 0x0} // 51 Q = Ь
,{0x7f, 0x09, 0x09, 0x09, 0x06} // 52 R = Р
,{0x3e, 0x41, 0x41, 0x41, 0x22} // 53 S = С
,{0x01, 0x01, 0x7f, 0x01, 0x01} // 54 T
,{0x43, 0x4c, 0x30, 0xc, 0x3} // 55 U = У
,{0x7f, 0x49, 0x49, 0x49, 0x36} // 56 V = В
,{0x7f, 0x40, 0x7f, 0x40, 0x7f} // 57 W = Ш
,{0x7f, 0x40, 0x7f, 0x40, 0xff} // 58 X = Щ
,{0x7f, 0x48, 0x30, 0x0, 0x7f} // 59 Y = Ы
,{0x41, 0x49, 0x49, 0x36, 0x0} // 5a Z = З
,{0x1, 0x7f, 0x48, 0x48, 0x30} // 5b [ = Ъ
,{0x7f, 0x8, 0x3e, 0x41, 0x3e} // 5c ? = Ю
,{0x49, 0x49, 0x49, 0x3e, 0x0} // 5d ] = Э
,{0x7, 0x8, 0x8, 0x7f, 0x0} // 5e ^ = Ч
,{0x40, 0x40, 0x40, 0x40, 0x40} // 5f _
,{0x00, 0x01, 0x02, 0x04, 0x00} // 60 `
,{0x20, 0x54, 0x54, 0x54, 0x78} // 61 a
,{0x3c, 0x4a, 0x4b, 0x49, 0x30} // 62 b = б
,{0x7c, 0x40, 0x40, 0x7c, 0xc0} // 63 c = ц
,{0xc0, 0x7c, 0x44, 0x7c, 0xc0} // 64 d = д
,{0x38, 0x54, 0x54, 0x54, 0x18} // 65 e
,{0x18, 0x24, 0xfc, 0x24, 0x18} // 66 f = ф
,{0x7c, 0x4, 0x4, 0x4, 0x0} // 67 g = г
,{0x44, 0x28, 0x10, 0x28, 0x44} // 68 h = х
,{0x7c, 0x20, 0x10, 0x8, 0x7c} // 69 i = и
,{0x44, 0x28, 0x7c, 0x28, 0x44} // 6a j = ж
,{0x7c, 0x10, 0x28, 0x44, 0x0} // 6b k = к
,{0x40, 0x3c, 0x4, 0x4, 0x7c} // 6c l = л
,{0x7c, 0x8, 0x10, 0x8, 0x7c} // 6d m = м
,{0x7c, 0x10, 0x10, 0x7c, 0x0} // 6e n = н
,{0x38, 0x44, 0x44, 0x44, 0x38} // 6f o
,{0x7c, 0x4, 0x4, 0x7c, 0x0} // 70 p = п
,{0x7c, 0x50, 0x50, 0x20, 0x0} // 71 q = ь
,{0x7c, 0x14, 0x14, 0x14, 0x08} // 72 r = р
,{0x38, 0x44, 0x44, 0x44, 0x20} // 73 s = с
,{0x4, 0x4, 0x7c, 0x4, 0x4} // 74 t = т
,{0x84, 0x98, 0x60, 0x18, 0x4} // 75 u = у
,{0x7c, 0x54, 0x54, 0x28, 0x0} // 76 v = в
,{0x7c, 0x40, 0x7c, 0x40, 0x7c} // 77 w = ш
,{0x7c, 0x40, 0x7c, 0x40, 0xfc} // 78 x = щ
,{0x7c, 0x50, 0x70, 0x0, 0x7c} // 79 y = ы
,{0x44, 0x54, 0x54, 0x28, 0x0} // 7a z = з
,{0x4, 0x7c, 0x50, 0x50, 0x20} // 7b { = ъ
,{0x54, 0x54, 0x54, 0x38, 0x0} // 7c | = э
,{0x7c, 0x10, 0x38, 0x44, 0x38} // 7d } = ю
,{0x48, 0x34, 0x14, 0x7c, 0x0} // 7e < = я
,{0xc, 0x10, 0x10, 0x7c, 0x0} // 7f > = ч
};

void LcdCharacter(char character, byte lang)
{
  //LcdWrite(LCD_D, 0x00);
  for (int index = 0; index < 5; index++)
  {
    /*
    if (lang == LANG_ENG) {
      LcdWrite(LCD_D, ASCII_ENG[character - 0x20][index]); 
    } else if (lang == LANG_RUS) {
      LcdWrite(LCD_D, ASCII_RUS[character - 0x20][index]);     
    }
    */
  LcdWrite(LCD_D, ASCII_RUS[character - 0x20][index]);   
  }
  LcdWrite(LCD_D, 0x00);
}

void LcdString(char *characters, byte lang)
{
  while (*characters)
  {
    LcdCharacter(*characters++, lang);
  }
}
void LcdClear(void)
{
  for (int index = 0; index < LCD_X * LCD_Y / 8; index++)
  {
    LcdWrite(LCD_D, 0x00);
  }
}

void LcdInitialise(void)
{
  pinMode(PIN_SCE, OUTPUT);
  pinMode(PIN_RESET, OUTPUT);
  pinMode(PIN_DC, OUTPUT);
  pinMode(PIN_SDIN, OUTPUT);
  pinMode(PIN_SCLK, OUTPUT);
  digitalWrite(PIN_RESET, LOW);
  digitalWrite(PIN_RESET, HIGH);
  LcdWrite(LCD_C, 0x20);
  LcdWrite(LCD_C, 0x0C);
}

void LcdWrite(byte dc, byte data)
{
  digitalWrite(PIN_DC, dc);
  digitalWrite(PIN_SCE, LOW);
  shiftOut(PIN_SDIN, PIN_SCLK, MSBFIRST, data);
  digitalWrite(PIN_SCE, HIGH);
}

void LcdGotoXY(int x, int y) //moving cursor to desired row/column
{
  LcdWrite( 0, 0x80 | x);  // Column.
  LcdWrite( 0, 0x40 | y);  // Row.  
}




void setup() {
  Serial.begin(9600);
  pinMode(LED_R, OUTPUT);
  pinMode(LED_G, OUTPUT);
  pinMode(LED_B, OUTPUT);
  // И кнопки, и светодиод у нас работают с инвертированной логикой: кнопка нажата = LOW, отпущена = HIGH,
  //  светодиод горит на полную яркость = LOW, погашен = HIGH. Погасим светодиоды:
  analogWrite(LED_R, 0);
  analogWrite(LED_G, 0);
  analogWrite(LED_B, 0);

  pinMode(PIEZO, OUTPUT);
  digitalWrite(PIEZO, LOW);

  pinMode(BUTTON_DOT, INPUT);
  pinMode(BUTTON_TIRE, INPUT);
  buttons = 0;
  buttonsPrev = 0;
  
  mode = TYPING_MODE;
  flagWakeUp = false;

  morseSymbolLen = 0;

  currentLayout = layoutCyrillic;
//  currentLayout = layoutLatin;  
  newLayout = 0;
  newMorseSignal = MORSE_EMPTY;
  ledLevelSleepCounter = 0;
  
  LcdInitialise();
  LcdClear();
  LcdGotoXY (0,0);  

  //LcdString("MORSE", LANG_ENG); 
}

//================================================================================================================
// Зажжем светодиод нужными цветом и яркостью. Не забываем, что у нас инвертирована логика и 0 - это самый яркий
//  свет, а 255 - погашенный светодиод.
void setLed(int ledColor, int ledBrightness) {
  if (ledColor & COLOR_RED) {
    analogWrite(LED_R, ledBrightness);
  } else {
    analogWrite(LED_R, 0);
  }
  if (ledColor & COLOR_GREEN) {
    analogWrite(LED_G, ledBrightness);
  } else {
    analogWrite(LED_G, 0);
  }
  if (ledColor & COLOR_BLUE) {
    analogWrite(LED_B, ledBrightness);
  } else {
    analogWrite(LED_B, 0);
  }
}

//================================================================================================================
// Работа с пьезо-динамиком
void doPiezo(unsigned long int currentTime) {
  if (currentTime >= piezoShutUpTime) {
    if (piezoShutUpTime > 0) {
      piezoShutUpTime = 0;
      digitalWrite(PIEZO, LOW);
    }
    return;
  }
  piezoData = (piezoData == LOW) ? HIGH : LOW;
  digitalWrite(PIEZO, piezoData);
}

void playPiezo(unsigned long int t, unsigned long int currentTime) {
  piezoShutUpTime = currentTime + t;
}

//================================================================================================================
// Считывание состояния кнопки с учетом возможного дребезга контактов.
int getButtonState(int btnPrevState, int BUTTON_PIN, unsigned long int* timeDebouncing, unsigned long int currentTime) {
  int btnState = digitalRead(BUTTON_PIN);

// инвертируем логику кнопок  
//  btnState = !btnState;
  
  if (btnState == HIGH) {
    if (btnPrevState == LOW) {
      if (*timeDebouncing == 0) {
        // Засечем время, которое кнопка будет нажата - чтобы не спутать дребезг контактов с нажатием.
        *timeDebouncing = currentTime;
        // Пока не воспринимаем нажатие, считая его дребезгом контактов.
        btnState = LOW;
      } else {
        if ((currentTime - *timeDebouncing) < DEBOUNCING_TIME) {
          // Пока не воспринимаем нажатие, считая его дребезгом контактов.
          btnState = LOW;
        } else {
          // Это не дребезг контактов, это реальное нажатие кнопки.
          btnState = HIGH;
          *timeDebouncing = 0;
        }
      }
    } else {
      *timeDebouncing = 0;
    }
  } else {
    if (btnPrevState == HIGH) {
      if (*timeDebouncing == 0) {
        // Засечем время, которое кнопка будет нажата - чтобы не спутать дребезг контактов с нажатием.
        *timeDebouncing = currentTime;
        // Пока не воспринимаем отпускание, считая его дребезгом контактов.
        btnState = HIGH;
      } else {
        if ((currentTime - *timeDebouncing) < DEBOUNCING_TIME) {
          // Пока не воспринимаем отпускание, считая его дребезгом контактов.
          btnState = HIGH;
        } else {
          // Это не дребезг контактов, это реальное отпукание кнопки.
          btnState = LOW;
          *timeDebouncing = 0;
        }
      }
    } else {
      *timeDebouncing = 0;
    }
  }
  
  return btnState;
}

//================================================================================================================
// Отправим на компьютер введенный символ.
void sendMorseSymbol() {
  int i, j;
  if (morseSymbolLen < 1) {
    return;
  }
  playPiezo(50, millis());
  for (i = 0; code[i][0] != '\0'; i++) {
    // Сравним введенный символ с символами из таблицы кодов Морзе.
    for (j = 0; (j < morseSymbolLen) && (code[i][j] != '\0'); j++) {
      if (code[i][j] != morseSymbol[j]) {
        j = -1;
        break;
      }
    }
    if ((j != -1) && (j == morseSymbolLen) && (code[i][j]=='\0')) {
      // Символ из таблицы кодов Морзе соответствует введенному символу.
      //  Отправим символ на компьютер.
      Serial.print(currentLayout[i]);
      
      

// вывод на экран =====================

  CURRSYM = *currentLayout[i];

  if (CURRSYM == '[') CURRSYM = '?';


  *SCREENSYM = CURRSYM;

  if (currentLayout == layoutCyrillic) {
    LcdString(SCREENSYM, LANG_RUS);
  } else {
    LcdString(SCREENSYM, LANG_ENG);
  }
/*
  ALLTEXT++;
  
  if (ALLTEXT == 14) LcdGotoXY (0,1);
  if (ALLTEXT == 28) LcdGotoXY (0,2);
  if (ALLTEXT == 42) LcdGotoXY (0,3); 
  if (ALLTEXT == 56) LcdGotoXY (0,4); 
  if (ALLTEXT == 70) LcdGotoXY (0,5); 
  if (ALLTEXT >= 84) {
    LcdClear();    
    LcdGotoXY (0,0);   
    }
*/      
// вывод на экран =====================

      morseSymbolLen = 0;
      return;
    }
  }
  // Символ в таблице не найден. Напечатаем нераспознанный символ.
  Serial.print(" [");
  for (i = 0; i < morseSymbolLen; i++) {
    Serial.print(morseSymbol[i]);
  }
  Serial.print("] ");
  morseSymbolLen = 0;
  

}

//================================================================================================================
// Режим печати

void typingLoop() {
  unsigned long int t, dt; // Эти пременные будем использовать для замеров времени.
  int btnDotState, btnTireState;  // В эти переменные считаем состояния кнопок. В принципе, их можно было бы сразу
                                  //  занести в переменную buttons, но так код будет понятнее.
  int ledLevel; // Яркость диода
  int ledColor; // Цвет диода, битовая маска - 00000RGB.

//  analogWrite(PIEZO, 0);

  t = millis();
  // Не забываем, что у нас логика инвертирована, и нажатая кнопка - это LOW.
  btnDotState = getButtonState((buttonsPrev & BUTTON_DOT_MASK) ? LOW : HIGH, BUTTON_DOT, &timeDotDebouncing, t);
  btnTireState = getButtonState((buttonsPrev & BUTTON_TIRE_MASK) ? LOW : HIGH, BUTTON_TIRE, &timeTireDebouncing, t);
  buttons = ((btnDotState == LOW) ? BUTTON_DOT_MASK : 0) | ((btnTireState == LOW) ? BUTTON_TIRE_MASK : 0);
  
  if (buttons == 0) {
    // Обе кнопки отпущены, можно добавить введенную точку, тире или переключить раскладку.
    // Если пауза дольше SLEEP_TIME - перейдем в спящий режим.
    // Если пауза дольше DELIMITER_TIME - отправим символ.
    if (buttonsPrev != 0) {
      timeRelease = t;
    }
    if (newLayout) {
      //currentLayout = newLayout;
        newLayout = 0;
        LcdClear();
        LcdGotoXY (0,0); 
    } else switch (newMorseSignal) {
    case MORSE_DOT:
    case MORSE_TIRE:
      morseSymbol[morseSymbolLen++] = newMorseSignal;
      break; // MORSE_DOT, MORSE_TIRE
    }
    newMorseSignal = MORSE_EMPTY;
    dt = t - timeRelease;
    if ((morseSymbolLen > 0) && (dt > DELIMITER_TIME)) {
      sendMorseSymbol();
    } else if (dt > SLEEP_TIME) {
      mode = SLEEP_MODE;
Serial.println("\nSleep mode\n");
    }
  } else if (newMorseSignal != MORSE_LOCKED) {
    switch (buttons) {
    case BUTTON_DOT_MASK:
      if (newMorseSignal == MORSE_EMPTY) {
        // Нажата "точка".
        newMorseSignal = MORSE_DOT;
        timePress = t;
      }
      break; // BUTTON_DOT_MASK
    case BUTTON_TIRE_MASK:
      if (newMorseSignal == MORSE_EMPTY) {
        // Нажато "тире".
        newMorseSignal = MORSE_TIRE;
        timePress = t;
      }
      break; // BUTTON_DOT_MASK
    case BUTTON_DOT_MASK | BUTTON_TIRE_MASK:
      // Нажаты обе кнопки. Сменим раскладку.
      switch (buttonsPrev ) {
      case 0: // Маловероятно, что обе кнопки нажаты одновременно, но в этом случае переключимся на кириллицу.
      case BUTTON_DOT_MASK:
        if (newLayout == 0) {
          sendMorseSymbol();
          newLayout = layoutCyrillic;
Serial.println("\nLayout: cyrillic\n");
        }
        break; // 0, BUTTON_DOT_MASK
      case BUTTON_TIRE_MASK:
        if (newLayout == 0) {
          sendMorseSymbol();
          newLayout = layoutLatin;
Serial.println("\nLayout: latin\n");
        }
        break; // BUTTON_TIRE_MASK
      }
      timePress = t;
      newMorseSignal = MORSE_LOCKED;
      break; // BUTTON_DOT_MASK | BUTTON_TIRE_MASK
    }
  }
  
  // Займемся светодиодом.
  if (currentLayout == layoutCyrillic) {
    ledColor = COLOR_CYRILLIC_LAYOUT;
  } else {
    ledColor = COLOR_LATIN_LAYOUT;
  }
  setLed(ledColor, (buttons == 0) ? BRIGHTNESS_TYPING_LOW : ((buttons == BUTTON_DOT_MASK) ? BRIGHTNESS_TYPING_DOT : BRIGHTNESS_TYPING_TIRE));
  
  doPiezo(t);
  
  buttonsPrev = buttons;
  delay(10);
}

//================================================================================================================
// Спящий режим
void sleepLoop() {
  unsigned long int t, dt; // Эти пременные будем использовать для замеров времени.
  int btnDotState, btnTireState;  // В эти переменные считаем состояния кнопок. В принципе, их можно было бы сразу
                                  //  занести в переменную buttons, но так код будет понятнее.
  int ledLevel; // Яркость диода
  int ledColor; // Цвет диода, битовая маска - 00000RGB.

  // Мы же  спим - поэтому будем проверять статус кнопок редко - раз в 0.3 с.
  delay(300);

  t = millis();
  // Не забываем, что у нас логика инвертирована, и нажатая кнопка - это LOW.
  btnDotState = getButtonState((buttonsPrev & BUTTON_DOT_MASK) ? LOW : HIGH, BUTTON_DOT, &timeDotDebouncing, t);
  btnTireState = getButtonState((buttonsPrev & BUTTON_TIRE_MASK) ? LOW : HIGH, BUTTON_TIRE, &timeTireDebouncing, t);
  buttons = ((btnDotState == LOW) ? BUTTON_DOT_MASK : 0) | ((btnTireState == LOW) ? BUTTON_TIRE_MASK : 0);

  if (buttons != 0) {
    if (buttonsPrev == 0) {
      timePress = t;
    }
    // Определим, достаточно ли долго была нажата кнопка для выхода из спячки.
    if (!flagWakeUp && ((t - timePress) >= WAKEUP_TIME)) {
      flagWakeUp = true;
    }
  } else {
    if (buttonsPrev != 0) {
      timeRelease = t;
    }
    if (flagWakeUp) {
      // Просыпаемся.
      flagWakeUp = false;
      mode = TYPING_MODE;
Serial.println("\nTYPING_MODE\n");
      return;
    }
  }

  // Помигаем светодиодом.
  if (flagWakeUp) {
    // Зажжем цвет, соответствующий текущей раскладке.
    if (currentLayout == layoutCyrillic) {
      ledColor = COLOR_CYRILLIC_LAYOUT;
    } else {
      ledColor = COLOR_LATIN_LAYOUT;
    }
    ledLevel = BRIGHTNESS_TYPING_TIRE;
  } else {
    ledColor = COLOR_SLEEP_MODE;
    ledLevel = (ledLevelSleepCounter == 0) ? BRIGHTNESS_SLEEP_LOW : BRIGHTNESS_SLEEP_HIGH;
    ledLevelSleepCounter = 1-ledLevelSleepCounter;
  }
  setLed(ledColor, ledLevel);

  buttonsPrev = buttons;
}

//================================================================================================================
// Главный цикл.
void loop() {
  switch(mode) {
    case TYPING_MODE:
      typingLoop();
      break;
    case SLEEP_MODE:
      sleepLoop();
      break;
  }
}

#include <iostream>
#include <Windows.h>
#include <cwchar>

int main() {
    
    HANDLE hConsole, hConsoleInput, hConsoleOutput;
    wchar_t buffer[255];
    INPUT_RECORD input_record[4000];
    DWORD eventRead;

    wchar_t *teks = L" aku suka matcha" ;
    int len = wcslen(teks);

    int row = 0;
    int cursorCol = 0;
    int cursorColBef = 0;
    int colHistory[50] = {};
    // CHAR_INFO (ARRAY OF STRUCT)
    CHAR_INFO charInfo[80 * 50] = {};
    COORD topLeft = {0,0};
    COORD sizeConsole = {80,50};
    
   


    // for(int i = 0; i < len; i++){
    //     charInfo[i].Char.UnicodeChar = teks[i];
    //     charInfo[i].Attributes = FOREGROUND_BLUE | BACKGROUND_GREEN;
    // }

    hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE,
                                        FILE_SHARE_READ | FILE_SHARE_WRITE,
                                        NULL, 
                                        CONSOLE_TEXTMODE_BUFFER, 
                                        NULL);


    hConsoleInput = GetStdHandle(STD_INPUT_HANDLE);
    hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleActiveScreenBuffer(hConsole);



    
    bool running = false;

    while(!running){
        ReadConsoleInput(hConsoleInput, input_record, 32, &eventRead); // input // blocking

        for(DWORD i = 0; i < eventRead; i++){
            if(input_record[i].EventType == KEY_EVENT){


                COORD cursorPos = {cursorCol, row};
                SetConsoleCursorPosition(hConsole, cursorPos);

                KEY_EVENT_RECORD key = input_record[i].Event.KeyEvent;


                if(key.bKeyDown){
                    if(key.wVirtualKeyCode == VK_ESCAPE){
                        running = true;
                    }

                    if(key.wVirtualKeyCode == VK_RETURN){
                        colHistory[row] = cursorCol;
                        row++;
                        cursorCol = 0;
                    }
                
                    // HANDLE KEYBOARD INPUT
                    if(key.uChar.UnicodeChar >= 32){
                        int idx = row * 80 + cursorCol;
                        charInfo[idx].Char.UnicodeChar = key.uChar.UnicodeChar;
                        charInfo[idx].Attributes = FOREGROUND_RED;
                        cursorCol++;
                    }  
                    
                    if(key.wVirtualKeyCode == VK_TAB){
                        int totalCol = cursorCol + 4;
                        for(int i = 0; i < totalCol; i++){
                            charInfo[i].Attributes = BACKGROUND_RED;
                        }
                      
                    }


                    // HANDLE BACKSPACE
                    if(key.wVirtualKeyCode == VK_BACK){

                    if(row >= 1 && cursorCol == 0){
                        row--;
                        cursorCol = colHistory[row];
                    } else if(cursorCol >= 1){
              
                        int idx = row * 80 + cursorCol--;
                        charInfo[idx].Char.UnicodeChar= ' ';
                        charInfo[idx].Attributes = FOREGROUND_RED;

                    }
                }

    
                        SMALL_RECT wRegion = {0,0,79,row};
                        WriteConsoleOutput(hConsole,charInfo,sizeConsole,topLeft,&wRegion); // output
                }
            }
        }
    }



}
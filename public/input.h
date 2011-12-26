#ifndef INPUT_H
#define INPUT_H

/*
TODO: Make this completely cross platform 
*/

/*
NOTENOTE: This code uses a little hack. Since some special keys in Qt, the windowing library
that also is used to catch the input, have an additional bit set, the key codes end up with 
numbers >= 255. In these cases, an attempt to remove this bit is made and their value is tested again.
This might mean that two keys might share the same internal key code, but this has not been tested
*/

#include "game_systems.h"

class CInput : public CGameSystem
{
    CInput();
public:
    
    static CInput* GetInstance()
    {
        static CInput input;
        return &input;
    }

    bool Init() { return true; }
    void Destroy() {}

    void Update() {}
    void PostRender();

    void SetKeyUp(int keycode);
    void SetKeyDown(int keycode);

    bool KeyUp(int keycode);
    bool KeyPressed(int keycode);
    bool KeyHeld(int keycode);
    bool KeyReleased(int keycode);

protected:
private:

    char *m_pCurrentState;
    char *m_pPreviousState;

    char m_szKeyBuffer1[255];
    char m_szKeyBuffer2[255];
};

extern inline CInput* Input()
{
    return CInput::GetInstance();
}

#define VK_0 Qt::Key_0 // 0 key
#define VK_1 Qt::Key_1 // 1 key
#define VK_2 Qt::Key_2 // 2 key
#define VK_3 Qt::Key_3 // 3 key
#define VK_4 Qt::Key_4 // 4 key
#define VK_5 Qt::Key_5 // 5 key
#define VK_6 Qt::Key_6 // 6 key
#define VK_7 Qt::Key_7 // 7 key
#define VK_8 Qt::Key_8 // 8 key
#define VK_9 Qt::Key_9 // 9 key
#define VK_A Qt::Key_A // A key
#define VK_B Qt::Key_B // B key
#define VK_C Qt::Key_C // C key
#define VK_D Qt::Key_D // D key
#define VK_E Qt::Key_E // E key
#define VK_F Qt::Key_F // F key
#define VK_G Qt::Key_G // G key
#define VK_H Qt::Key_H // H key
#define VK_I Qt::Key_I // I key
#define VK_J Qt::Key_J // J key
#define VK_K Qt::Key_K // K key
#define VK_L Qt::Key_L // L key
#define VK_M Qt::Key_M // M key
#define VK_N Qt::Key_N // N key
#define VK_O Qt::Key_O // O key
#define VK_P Qt::Key_P // P key
#define VK_Q Qt::Key_Q // Q key
#define VK_R Qt::Key_R // R key
#define VK_S Qt::Key_S // S key
#define VK_T Qt::Key_T // T key
#define VK_U Qt::Key_U // U key
#define VK_V Qt::Key_V // V key
#define VK_W Qt::Key_W // W key
#define VK_X Qt::Key_X // X key
#define VK_Y Qt::Key_Y // Y key
#define VK_Z Qt::Key_Z // Z key

#define VK_Tab Qt::Key_Tab
#define VK_Esc Qt::Key_Escape
#define VK_Alt Qt::Key_Alt
#define VK_Return Qt::Key_Return
#define VK_Enter Qt::Key_Enter

#define VK_Minus Qt::Key_Minus
#define VK_Plus Qt::Key_Plus

#define VK_f5 Qt::Key_F5

/*

#define VK_0 0x30 // 0 key
#define VK_1 0x31 // 1 key
#define VK_2 0x32 // 2 key
#define VK_3 0x33 // 3 key
#define VK_4 0x34 // 4 key
#define VK_5 0x35 // 5 key
#define VK_6 0x36 // 6 key
#define VK_7 0x37 // 7 key
#define VK_8 0x38 // 8 key
#define VK_9 0x39 // 9 key
#define VK_A 0x41 // A key
#define VK_B 0x42 // B key
#define VK_C 0x43 // C key
#define VK_D 0x44 // D key
#define VK_E 0x45 // E key
#define VK_F 0x46 // F key
#define VK_G 0x47 // G key
#define VK_H 0x48 // H key
#define VK_I 0x49 // I key
#define VK_J 0x4A // J key
#define VK_K 0x4B // K key
#define VK_L 0x4C // L key
#define VK_M 0x4D // M key
#define VK_N 0x4E // N key
#define VK_O 0x4F // O key
#define VK_P 0x50 // P key
#define VK_Q 0x51 // Q key
#define VK_R 0x52 // R key
#define VK_S 0x53 // S key
#define VK_T 0x54 // T key
#define VK_U 0x55 // U key
#define VK_V 0x56 // V key
#define VK_W 0x57 // W key
#define VK_X 0x58 // X key
#define VK_Y 0x59 // Y key
#define VK_Z 0x5A // Z key

*/
#endif // INPUT_H

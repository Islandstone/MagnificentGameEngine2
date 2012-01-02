#ifndef INPUT_H
#define INPUT_H

/*
TODO: Make this completely cross platform 
*/

/*
NOTENOTE: This code uses a little hack. Since some special keys in Qt, the windowing library
that also is used to catch the input, have an additional bit set, the key codes end up with 
numbers >= 255. In these cases, an attempt to remove this bit is made and their value is tested again.
This might mean that two keys might share the same internal key code, but this has not been tested.
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

#define KEY_0 Qt::Key_0 // 0 key
#define KEY_1 Qt::Key_1 // 1 key
#define KEY_2 Qt::Key_2 // 2 key
#define KEY_3 Qt::Key_3 // 3 key
#define KEY_4 Qt::Key_4 // 4 key
#define KEY_5 Qt::Key_5 // 5 key
#define KEY_6 Qt::Key_6 // 6 key
#define KEY_7 Qt::Key_7 // 7 key
#define KEY_8 Qt::Key_8 // 8 key
#define KEY_9 Qt::Key_9 // 9 key
#define KEY_A Qt::Key_A // A key
#define KEY_B Qt::Key_B // B key
#define KEY_C Qt::Key_C // C key
#define KEY_D Qt::Key_D // D key
#define KEY_E Qt::Key_E // E key
#define KEY_F Qt::Key_F // F key
#define KEY_G Qt::Key_G // G key
#define KEY_H Qt::Key_H // H key
#define KEY_I Qt::Key_I // I key
#define KEY_J Qt::Key_J // J key
#define KEY_K Qt::Key_K // K key
#define KEY_L Qt::Key_L // L key
#define KEY_M Qt::Key_M // M key
#define KEY_N Qt::Key_N // N key
#define KEY_O Qt::Key_O // O key
#define KEY_P Qt::Key_P // P key
#define KEY_Q Qt::Key_Q // Q key
#define KEY_R Qt::Key_R // R key
#define KEY_S Qt::Key_S // S key
#define KEY_T Qt::Key_T // T key
#define KEY_U Qt::Key_U // U key
#define KEY_V Qt::Key_V // V key
#define KEY_W Qt::Key_W // W key
#define KEY_X Qt::Key_X // X key
#define KEY_Y Qt::Key_Y // Y key
#define KEY_Z Qt::Key_Z // Z key

#define KEY_TAB Qt::Key_Tab
#define KEY_ESC Qt::Key_Escape
#define KEY_ALT Qt::Key_Alt
#define KEY_RETURN Qt::Key_Return
#define KEY_ENTER Qt::Key_Enter

#define KEY_PAGEUP Qt::Key_PageUp
#define KEY_PAGEDOWN Qt::Key_PageDown
#define KEY_HOME	Qt::Key_Home

#define KEY_MINUS Qt::Key_Minus
#define KEY_PLUS Qt::Key_Plus

#define KEY_UP Qt::Key_Up
#define KEY_DOWN Qt::Key_Down
#define KEY_LEFT Qt::Key_Left
#define KEY_RIGHT Qt::Key_Right

#define KEY_F5 Qt::Key_F5

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

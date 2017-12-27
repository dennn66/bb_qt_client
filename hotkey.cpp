#include "hotkey.h"

HotKey::HotKey(int id,  DWORD modifierCode, DWORD activationKeyCode, QObject *parent) : QObject(parent)
{
    vkID = id;

    bModifier = (modifierCode == 0);
    vkModifierCode = modifierCode;
    vkActivationKeyCode = activationKeyCode;
}

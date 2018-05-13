#include "hotkey.h"

HotKey::HotKey(int id, bool modifier, DWORD modifierCode, DWORD activationKeyCode, QObject *parent) : QObject(parent)
{
    vkID = id;
    bModifier = modifier;
    vkModifierCode = modifierCode;
    vkActivationKeyCode = activationKeyCode;
}

/******************************************************************************
  Target Script for Nuvoton NUC121/NUC125

  THIS FILE IS PROVIDED AS IS WITH NO WARRANTY OF ANY KIND, INCLUDING THE
  WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 ******************************************************************************/

function Reset()
{
  var impl = TargetInterface.implementation ? TargetInterface.implementation() : "";
  if (impl == "j-link")
    TargetInterface.resetAndStop(100);
  else
    {
      TargetInterface.pokeWord(0xE000EDFC, 0x00000001);
      TargetInterface.pokeWord(0xE000ED0C, 0x05FA0004);
      TargetInterface.pokeWord(0xE000EDF0, 0xA05F0001);
      TargetInterface.delay(100);
      TargetInterface.resetDebugInterface();
      TargetInterface.pokeWord(0xE000EDF0, 0xA05F0003);
      TargetInterface.waitForDebugState(100);
    }
}

/******************************************************************************
  Target Script for Cypress PSoC4xxx

  THIS FILE IS PROVIDED AS IS WITH NO WARRANTY OF ANY KIND, INCLUDING THE
  WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 ******************************************************************************/

function Reset()
{
  if (TargetInterface.implementation() != "crossworks_simulator")
    TargetInterface.pokeWord(0x40030014, 0x80000000);
  if (TargetInterface.implementation() == "j-link")
  {
    TargetInterface.resetAndStop(400);
  }
  else
  {
    TargetInterface.pokeWord(0xE000ED0C, 0x05FA0001);
    TargetInterface.resetDebugInterface();
    TargetInterface.stop();
  }
}

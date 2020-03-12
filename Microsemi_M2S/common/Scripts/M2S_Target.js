/*****************************************************************************
 * Microsemi SmartFusion2 target script                                      *
 *                                                                           *
 * THIS FILE IS PROVIDED AS IS WITH NO WARRANTY OF ANY KIND, INCLUDING THE   *
 * WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. *
 *                                                                           *
 *****************************************************************************/

function SF2_goto_CM3_JTAG()
{
/*
J-Link Commander script:
rtap
wjraw 5,06,00
wjraw 8,80,0A // write IR=0x0A
wjraw 6,0E,00
wjraw 8,80,00 // write DR=0x00
wjraw 4,6,0
wjraw 8,00,00
wjraw 15,1683,01E0
wjraw 32,80000000,00000000 // read back ARM IDCODE
wjraw 23,216836,001A00
*/
  // go to JTAG 'test logic reset'
  TargetInterface.setTMS(1);
  TargetInterface.cycleTCK(5);

  // transition from JTAG 'test logic reset' to 'shift IR'
  TargetInterface.setTMS(0);
  TargetInterface.cycleTCK(1);
  TargetInterface.setTMS(1);
  TargetInterface.cycleTCK(2);
  TargetInterface.setTMS(0);
  TargetInterface.cycleTCK(2);

  // shift command (0x0A) into IR
  TargetInterface.setTDI(0);
  TargetInterface.cycleTCK(1);
  TargetInterface.setTDI(1);
  TargetInterface.cycleTCK(1);
  TargetInterface.setTDI(0);
  TargetInterface.cycleTCK(1);
  TargetInterface.setTDI(1);
  TargetInterface.cycleTCK(1);
  TargetInterface.setTDI(0);
  TargetInterface.cycleTCK(3);
  TargetInterface.setTMS(1);
  TargetInterface.cycleTCK(1);

  // go to Shift-DR
  TargetInterface.setTMS(0);
  TargetInterface.cycleTCK(1);
  TargetInterface.setTMS(1);
  TargetInterface.cycleTCK(3);
  TargetInterface.setTMS(0);
  TargetInterface.cycleTCK(2);

  // shift data (0x00) into DR
  TargetInterface.setTDI(0);
  TargetInterface.cycleTCK(7);
  TargetInterface.setTMS(1);
  TargetInterface.cycleTCK(1);

  // go to 'run test idle'
  TargetInterface.setTMS(0);
  TargetInterface.cycleTCK(1);
  TargetInterface.setTMS(1);
  TargetInterface.cycleTCK(2);
  TargetInterface.setTMS(0);
  TargetInterface.cycleTCK(1);
  TargetInterface.cycleTCK(8); /* not strictly needed, but observed in SoftConsole */
}

function Reset()
{
/*
J-Link Commander works with:
w4 0xE000EDF0, 0xA05F0001
w4 0xE000EDFC, 0x00000001
w4 0xE000ED0C, 0x05FA0004
halt
*/
  TargetInterface.pokeWord(0xE000EDF0, 0xA05F0001); // set C_DEBUGEN in DHCSR      
  TargetInterface.pokeWord(0xE000EDFC, 0x00000001); // set VC_CORERESET in DEMCR

  TargetInterface.pokeWord(0x40038098, TargetInterface.peekWord(0x40038098) & 0xFFC00103); // restore MSS PLL to reset state

  if (TargetInterface.implementation() == "j-link")
  {
    TargetInterface.stopAndReset(1000);
  }
  else
  {
    TargetInterface.pokeWord(0xE000ED0C, 0x05FA0004); // set SYSRESETREQ in AIRCR
    TargetInterface.stop();
  }

  TargetInterface.pokeWord(0x4003806C, 0); // disable watchdog
  TargetInterface.pokeWord(0x40038018, 0); // disable cache
  TargetInterface.pokeWord(0x40038048, 0x07FFFFFF); // invoke soft_reset all peripherals
  TargetInterface.pokeWord(0x40038048, 0x00000000); // release soft_reset of all peripherals
}

function Connect()
{
  TargetInterface.setDebugInterfaceProperty("reset_debug_interface_enabled", false);
  SF2_goto_CM3_JTAG();
}

function SRAMReset()
{
  Reset();
}

function FLASHReset()
{
  Reset();
}

function GetPartName()
{
  var device_id = TargetInterface.peekWord(0x4003814C) & 0xFFFF;

  switch (device_id)
  {
  case 0xF802:
    return "M2S050";
  case 0xF803:
    return "M2S010";
  case 0xF804:
    return "M2S025";
  case 0xF805:
    return "M2S005";
  case 0xF806:
    return "M2S150";
  case 0xF807:
    return "M2S090";
  case 0xF808:
    return "M2S060";
  }

  return "unknown 0x" + (device_id).toString(16);
}

function MatchPartName(name)
{
  var partName = GetPartName();
   return partName.indexOf(name) != -1;
}

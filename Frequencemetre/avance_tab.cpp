#include "avance_tab.h"


S_AVANCE s_Avance[128];


unsigned int AVANCE_TAB_GetSize(void)
{
  return 128;
}

S_AVANCE * AVANCE_TAB_GetTab(void)
{
  return s_Avance;
}


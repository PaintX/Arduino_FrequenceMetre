#ifndef _AVANCE_TAB_H_
#define _AVANCE_TAB_H_

typedef struct
{
  float   speed;
  float   avance;
}S_AVANCE;

unsigned int AVANCE_TAB_GetSize(void);
S_AVANCE * AVANCE_TAB_GetTab(void);

#endif

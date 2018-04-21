/**************************************************************************/
/* LabWindows/CVI User Interface Resource (UIR) Include File              */
/*                                                                        */
/* WARNING: Do not add to, delete from, or otherwise modify the contents  */
/*          of this include file.                                         */
/**************************************************************************/

#include <userint.h>

#ifdef __cplusplus
    extern "C" {
#endif

     /* Panels and Controls: */

#define  PANEL                            1
#define  PANEL_QUITBUTTON                 2       /* control type: command, callback function: QuitCallback */
#define  PANEL_DECORATION_4               3       /* control type: deco, callback function: (none) */
#define  PANEL_DECORATION_3               4       /* control type: deco, callback function: (none) */
#define  PANEL_DECORATION_5               5       /* control type: deco, callback function: (none) */
#define  PANEL_DECORATION_2               6       /* control type: deco, callback function: (none) */
#define  PANEL_DECORATION                 7       /* control type: deco, callback function: (none) */
#define  PANEL_Info_2                     8       /* control type: textMsg, callback function: (none) */
#define  PANEL_Info                       9       /* control type: textMsg, callback function: (none) */
#define  PANEL_Portinfo                   10      /* control type: textMsg, callback function: (none) */
#define  PANEL_Test_CON                   11      /* control type: textMsg, callback function: (none) */
#define  PANEL_TEXTMSG                    12      /* control type: textMsg, callback function: (none) */
#define  PANEL_iLBComPortList             13      /* control type: listBox, callback function: (none) */
#define  PANEL_COMMANDBUTTON_2            14      /* control type: command, callback function: OnCloseComPortPressed */
#define  PANEL_bPaintHelp                 15      /* control type: command, callback function: bPaintHelpPressed */
#define  PANEL_bPaintClearMatrix          16      /* control type: command, callback function: bPaintClearMatrixPressed */
#define  PANEL_bPaintRandom               17      /* control type: command, callback function: bPaintRandomPressed */
#define  PANEL_bPaintColorPicker          18      /* control type: command, callback function: bPaintColorPicker */
#define  PANEL_bPaintGreen                19      /* control type: command, callback function: bPaintGreenPressed */
#define  PANEL_bPaintBlue                 20      /* control type: command, callback function: bPaintBluePressed */
#define  PANEL_bPaintRed                  21      /* control type: command, callback function: bPaintRedPressed */
#define  PANEL_COMMANDBUTTON              22      /* control type: command, callback function: OnOpenComPortPressed */
#define  PANEL_sCODE                      23      /* control type: textBox, callback function: (none) */
#define  PANEL_sTBComPortStatus           24      /* control type: textBox, callback function: (none) */
#define  PANEL_iBLED                      25      /* control type: textButton, callback function: OnLedPressed */
#define  PANEL_bSendLedColorAndPlace      26      /* control type: command, callback function: bSendLedColorAndPlace */
#define  PANEL_LEDNumber                  27      /* control type: numeric, callback function: (none) */
#define  PANEL_LedColorList               28      /* control type: listBox, callback function: (none) */
#define  PANEL_TABLE                      29      /* control type: table, callback function: bTABLEPressedCell */
#define  PANEL_bSENDTEXT                  30      /* control type: command, callback function: bSENDTEXT */
#define  PANEL_bCOPYCODE                  31      /* control type: command, callback function: bCopyCodePressed */
#define  PANEL_bGETCODE                   32      /* control type: command, callback function: bGetCodePressed */
#define  PANEL_bSENDMATRIX                33      /* control type: command, callback function: bSENDMATRIX */
#define  PANEL_STRING                     34      /* control type: string, callback function: (none) */
#define  PANEL_bClearAll                  35      /* control type: command, callback function: bClearAllPressed */
#define  PANEL_CanvasColorPicker          36      /* control type: canvas, callback function: (none) */

#define  PANEL_HELP                       2
#define  PANEL_HELP_COMMANDBUTTON         2       /* control type: command, callback function: (none) */
#define  PANEL_HELP_Help                  3       /* control type: textMsg, callback function: (none) */


     /* Control Arrays: */

#define  CTRLARRAY                        1

     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK bClearAllPressed(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK bCopyCodePressed(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK bGetCodePressed(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK bPaintBluePressed(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK bPaintClearMatrixPressed(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK bPaintColorPicker(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK bPaintGreenPressed(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK bPaintHelpPressed(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK bPaintRandomPressed(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK bPaintRedPressed(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK bSendLedColorAndPlace(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK bSENDMATRIX(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK bSENDTEXT(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK bTABLEPressedCell(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnCloseComPortPressed(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnLedPressed(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnOpenComPortPressed(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK QuitCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
